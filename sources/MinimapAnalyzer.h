#pragma once
#include <QObject>
#include <qdir.h>
#include <qimage.h>
#include <qstring.h>
#include <qthread.h>
#include <qpoint.h>
#include <qrandom.h>
#include <qdatetime.h>
#include <qsize.h>

#include "Utilities.h"	
#include "Calibrator.h"
#include "VariablesClass.h"
#include "Utilities.h"
class ImgShearcher : public QThread {
	Q_OBJECT
	public:
		ImgShearcher(QObject* parent, const QImage& imgSmall, const QImage& imgBig, QList<QPoint>& foundStartPostitons, QRect frameToLookFor)
			: QThread(parent), imgSmall(imgSmall), imgBig(imgBig), foundStartPostitons(foundStartPostitons), frameToLookFor(frameToLookFor){};
		const QImage& imgSmall, & imgBig;
		QList<QPoint>& foundStartPostitons;
		QRect frameToLookFor;
		void run() {
			foundStartPostitons = ImgEditor::findStartPositionInImg(imgSmall, imgBig, frameToLookFor);
		}
};
class MinimapAnalyzer : public QThread {
	Q_OBJECT
public:
	MinimapAnalyzer(QObject* parent, std::shared_ptr<VariablesClass> var) : QThread(parent), var(var) {
	}
	~MinimapAnalyzer() {};
	void run(){	
		QMap<int, QImage*> floorsMaps;
		while (true){
			msleep(SLEEP_TIME_BETWEEN_LOOPS);
			if (!var->getSettingKeepAnalyzeMiniMap()) {
				msleep(SLEEP_TIME_BETWEEN_LOOPS);
				continue;
			}
			QImage miniMap, miniMapLayer;
			var->getImgMiniMap(miniMap);
			var->getImgMiniMapLayer(miniMapLayer);
			if (miniMap.isNull() || miniMapLayer.isNull())
				continue;

			int currentLayer = getCurrentLayer(miniMapLayer);
			if(!floorsMaps.contains(currentLayer))
				floorsMaps.insert(currentLayer, new QImage(getMapFilePath(currentLayer, MapType::REGULAR)));
			auto currentPosition = findPlayerPosition(miniMap, floorsMaps[currentLayer]);
			previousPosition = currentPosition;

			if(!currentPosition.isNull())
				sendPostitionsToGUI(QString::number(currentPosition.x()), QString::number(currentPosition.y()), QString::number(currentLayer));
			else
				sendPostitionsToGUI("--", "--", QString::number(currentLayer));
		}
	}

signals:
	void sendPostitionsToGUI(QString, QString, QString);

private:
	enum class MapType { REGULAR, WAYPOINT };

	std::shared_ptr<VariablesClass> var;
	const int SLEEP_TIME_BETWEEN_LOOPS = 20, SLEEP_TIME_NO_ACTIVE = 1000;
	const int MIN_LAYER = -8, MAX_LAYER = 7;
	const QImage LAYER_SLIDER_IMG = setSliderImg();
	const int MIN_X_MAP_INDEX = 124, MAX_X_MAP_INDEX = 134, MIN_Y_MAP_INDEX = 121, MAX_Y_MAP_INDEX = 129;
	const QVector<QRect> MINI_MAP_PARTS_POSITIONS = getMiniMapPartsPostions();
	const QSize PROPER_MINIMAP_IMG_SIZE = QSize(106, 109);
	QPoint previousPosition;

	QImage setSliderImg(){
		QString path = "C:\\Moje\\pliki\\repos\\CrackerJackClient\\ResourcesUsing\\mapLayer.png";
		QImage img;
		img.load(path);
		int a = 3;
		return img;
	}
	int getCurrentLayer(const QImage& layerImg) {
		QList<QPoint> startPostions = ImgEditor::findStartPositionInImg(LAYER_SLIDER_IMG, layerImg);
		if (startPostions.size() != 1)
			return -100;

		const int HIGHEST_LAYER_Y_POS = 2;
		const int HEIGHT_DIFF_FOR_ONE_LAYER = 4;
		int yPosition = startPostions[0].y() - HIGHEST_LAYER_Y_POS;
		int layerDiffToHighestLayer = yPosition / HEIGHT_DIFF_FOR_ONE_LAYER;
		int layer = MAX_LAYER - layerDiffToHighestLayer;

		if(layer >= MIN_LAYER && layer <= MAX_LAYER)
			return layer;
		else
			return -100;
	}
	QString getMapFilePath(int floor, MapType mapType) {
		const QString typeStr = mapType == MapType::REGULAR ? "Color" : "WaypointCost";
		const QString PATH_TO_FOLDER = "C:\\Moje\\pliki\\repos\\CrackerJackClient\\ResourcesUsing\\maps";
		return QString("%1\\%2_%3.png").arg(PATH_TO_FOLDER, QString::number(floor), typeStr);
	}
	QVector<QRect> getMiniMapPartsPostions() {
		//5 is player mark
		QVector<QRect> imgParts;
		imgParts.push_back(QRect(0, 0, 53, 54));//1
		imgParts.push_back(QRect(53, 0, 2, 52));//2
		imgParts.push_back(QRect(55, 0, 51, 54));//3
		imgParts.push_back(QRect(0, 54, 51, 2));//4
		imgParts.push_back(QRect(57, 54, 49, 2));//6
		imgParts.push_back(QRect(0, 56, 53, 53));//7
		imgParts.push_back(QRect(53, 58, 2, 51));//8
		imgParts.push_back(QRect(55, 56, 51, 53));//9
		
		return imgParts;
	}
	QList<QImage> splitMiniMap(const QImage& wholeMiniMap) {
		if (wholeMiniMap.size() != PROPER_MINIMAP_IMG_SIZE)
			return QList<QImage>();

		QList<QImage> toRet = {};
		for each (QRect rect in MINI_MAP_PARTS_POSITIONS){
			toRet.push_back(wholeMiniMap.copy(rect));
		}
		return toRet;
	}
	QPoint findPlayerPosition(const QImage& miniMap, const QImage* wholeMap) {
		const QList<QImage> miniMapParts = splitMiniMap(miniMap);
		QList<QList<QPoint>> pointsFromThreads;
		for (size_t i = 0; i < miniMapParts.size(); i++)
			pointsFromThreads.push_back(QList<QPoint>());

		QList<ImgShearcher*> threads;
		QRect frame = getFrameToLookByPreviousPos();
		for (size_t i = 0; i < miniMapParts.size(); i++){
			auto thread = new ImgShearcher(this, miniMapParts[i], *wholeMap, pointsFromThreads[i], frame);
			threads.push_back(thread);
			thread->start();
		}

		bool keepWaiting = false;
		do{	
			keepWaiting = false;
			msleep(50);
			for (size_t i = 0; i < threads.size(); i++){
				if (threads[i]->isRunning()) 
					keepWaiting = true;
				else {
					bool foundPoint = pointsFromThreads[i].size() == 1;
					if (foundPoint) {
						keepWaiting = false;
						break;
					}
				}
			}
		} while (keepWaiting);

		for (size_t i = 0; i < threads.size(); i++) {
			threads[i]->terminate();
			delete threads[i];
		}

		
		for (size_t i = 0; i < pointsFromThreads.size(); i++){
			auto currentList = &pointsFromThreads[i];
			if (currentList->size() != 1)
				continue;

			QSize vectorToPlayerPos = getVectorToPlayerFromImgPieceIndex(i);
			QPoint playerPos(vectorToPlayerPos.width() + currentList[0].first().x(), vectorToPlayerPos.height() + currentList[0].first().y());
			return playerPos;
		}
		return QPoint();
	}
	QList<QPoint> findStartPosOfImgMap(const QImage& imgToFind, const QImage& imgToSearchWithin, QRect frameInBigWindow) {
		try {
			const int WIDTH_SMALL_PIC = imgToFind.width();
			const int HEIGHT_SMALL_PIC = imgToFind.height();
			const int WIDTH_BIG_PIC = imgToSearchWithin.width();
			const int HEIGHT_BIG_PIC = imgToSearchWithin.height();

			if (imgToFind.isNull() || imgToSearchWithin.isNull())
				throw std::exception("Cant find postion, one of imgs is null");
			if (WIDTH_SMALL_PIC > WIDTH_BIG_PIC)
				throw std::exception("Cant find postion, Wrong imgs size");
			if (HEIGHT_SMALL_PIC > HEIGHT_BIG_PIC)
				throw std::exception("Cant find postion, Wrong imgs size");
			if (imgToFind.format() != imgToSearchWithin.format())
				throw std::exception("Cant find postion, wrong formats");
			if (frameInBigWindow.right() > WIDTH_BIG_PIC)
				throw std::exception("Cant find postion, Wrong frame size");
			if (frameInBigWindow.bottom() > HEIGHT_BIG_PIC)
				throw std::exception("Cant find postion, Wrong frame size");


			const int maxIndexToCheckX = frameInBigWindow.isEmpty() ? WIDTH_BIG_PIC - WIDTH_SMALL_PIC : frameInBigWindow.right();
			const int maxIndexToCheckY = frameInBigWindow.isEmpty() ? HEIGHT_BIG_PIC - HEIGHT_SMALL_PIC : frameInBigWindow.bottom();
			const int minIndexToCheckX = frameInBigWindow.isEmpty() ? 0 : frameInBigWindow.left();
			const int minIndexToCheckY = frameInBigWindow.isEmpty() ? 0 : frameInBigWindow.top();

			QList<QPoint> startPointsListToRet;
			for (int x = minIndexToCheckX; x <= maxIndexToCheckX; x++) {
				for (int y = minIndexToCheckY; y <= maxIndexToCheckY; y++) {
					uint pixSmallImg = imgToFind.pixel(0, 0);
					uint pixBigImg = imgToSearchWithin.pixel(x, y);
					if (pixSmallImg != pixBigImg)
						continue;

					bool foundPosition = true;
					for (int offsetX = 1; offsetX < WIDTH_SMALL_PIC; offsetX++) {
						for (int offsetY = 1; offsetY < HEIGHT_SMALL_PIC; offsetY++) {
							pixSmallImg = imgToFind.pixel(offsetX, offsetY);
							pixBigImg = imgToSearchWithin.pixel(x + offsetX, y + offsetY);
							if (pixBigImg == pixSmallImg)
								continue;
							//break both loops
							offsetX = WIDTH_SMALL_PIC;
							offsetY = HEIGHT_SMALL_PIC;
							foundPosition = false;
						}
					}
					if (foundPosition)
						startPointsListToRet.push_back(QPoint(x, y));

				}
			}
			return startPointsListToRet;
		}
		catch (const std::exception& e) {
			Logger::staticLog(e.what());
			return QList<QPoint>();
		}
	}
	QRect getFrameToLookByPreviousPos() {
		if (previousPosition.isNull())
			return QRect();

		const int SIZE = 220 / 2;
		QRect frameToLookWithin = QRect(previousPosition.x() - SIZE, previousPosition.y() - SIZE, 2 * SIZE, 2 * SIZE);
		return frameToLookWithin;
	}
	QSize getVectorToPlayerFromImgPieceIndex(int index) {
		QSize vectorToPlayerPos;
		switch (index)
		{
		case 0: vectorToPlayerPos = QSize(53, 54); break;//topLeft
		case 1: vectorToPlayerPos = QSize(0, 54); break;//top
		case 2: vectorToPlayerPos = QSize(-55, 54); break;//topRight
		case 3: vectorToPlayerPos = QSize(+53, 0); break;//left
		case 4: vectorToPlayerPos = QSize(-4, 0); break;//right
		case 5: vectorToPlayerPos = QSize(+53, -2); break;//downLeft
		case 6: vectorToPlayerPos = QSize(0, -4); break;//down
		case 7: vectorToPlayerPos = QSize(-2, -2); break;//downRight
		}
		return vectorToPlayerPos;
	}
};



/*
class ShearchArea {
	public:
	QMap<int, QList<QRect>> areas;
	ShearchArea() {};
	ShearchArea(QString structStr) {
		//expected struct [#lvlNumber[x,y,w,h;x,y,w,h;]#lvlNumber[x,y,w,h;x,y,w,h]]

		bool wholeStructRead = structStr.left(1) == "[" && structStr.right(1) == "]";
		if (!wholeStructRead) {
			//Logger::logPotenialBug("error in creating ShearchArea struct", "ShearchArea(QString)", "ShearchArea");
			return;
		}
		structStr = structStr.mid(1, structStr.size() - 2);
		QStringList levels = structStr.split("#", Qt::SkipEmptyParts);
		for each (QString lvlStr in levels){
			QString possibleLevelNumber = lvlStr.left(2);
			bool ok;
			int levelNumber = possibleLevelNumber.toInt(&ok);
			bool somethingWrong = !ok || levelNumber < 0 || levelNumber > 15;
			if (somethingWrong) {
				//Logger::logPotenialBug("error in creating ShearchArea struct", "ShearchArea(QString)", "ShearchArea");
				return;
			}
			lvlStr = lvlStr.right(lvlStr.size() - 2);
			somethingWrong = lvlStr.left(1) != "[" || lvlStr.right(1) != "]";
			if (somethingWrong) {
				//Logger::logPotenialBug("error in creating ShearchArea struct", "ShearchArea(QString)", "ShearchArea");
				return;
			}
			lvlStr = lvlStr.mid(1, lvlStr.size() - 2);
			QStringList rectanglesParametersList = lvlStr.split(";", Qt::SkipEmptyParts);
			QList<QRect> rectsToSet;
			for each (QString rectParametersStr in rectanglesParametersList){
				QStringList rectParameters = rectParametersStr.split(",", Qt::SkipEmptyParts);
				if (rectParameters.size() != 4) {
					//Logger::logPotenialBug("error in creating ShearchArea struct", "ShearchArea(QString)", "ShearchArea");
					return;
				}
				int x = rectParameters[0].toInt();
				int y = rectParameters[1].toInt();
				int w = rectParameters[2].toInt();
				int h = rectParameters[3].toInt();
				QRect toAdd(x,y,w,h);
				rectsToSet.append(toAdd);
			}
			this->areas.insert(levelNumber, rectsToSet);
		}

	}
	QString toString() {
		//struct to obtain [#lvlNumber[x,y,w,h;x,y,w,h;]#lvlNumber[x,y,w,h;x,y,w,h]]
		QString toRet = "[";
		for each (int level in areas.keys()){
			QString toAdd;
			QString lvlNumberStr = QString::number(level);
			if (lvlNumberStr.size() == 1)
				lvlNumberStr.push_back(QString::number(0));
			toAdd.push_back("#" + lvlNumberStr + "[");
			QList<QRect> listOfAreasInThatLvl = areas.value(level);
			for each (QRect area in listOfAreasInThatLvl){
				QString xStart = QString::number(area.x());
				QString yStart = QString::number(area.y());
				QString width = QString::number(area.width());
				QString height = QString::number(area.height());
				QString toAppend = xStart + "," + yStart + "," + width + "," + height + ";";
				toAdd.append(toAppend);
			}
			toAdd.append("]");
			toRet.push_back(toAdd);
		}
		toRet.push_back("]");

		return toRet;
	}
	void addArea(int level, QList<QRect> areasOnThatLvl) {
		bool structHasNoThatLvl = this->areas.value(level).isEmpty();
		if (structHasNoThatLvl)
			this->areas.insert(level, areasOnThatLvl);
		else {
			QList<QRect> newRectsList = this->areas.value(level);
			newRectsList.append(areasOnThatLvl);
			this->areas.remove(level);
			this->areas.insert(level, newRectsList);
		};
	}
	void setAllMaps() {
		this->areas.clear();
		QList<QRect> listToAdd;	
		listToAdd.push_back(QRect(0, 0, 2560, 2048));
		for (size_t i = 0; i < 16; i++)
			this->areas.insert(i, listToAdd);
	};
};
*/









/*
	public:
	MinimapAnalyzer();

	void test();
private:
	QList<uint> test_getAllPossibleMapColors();
	QList<QRect> test_fillListWithRectsPosOfMiniMapParts();
	QRect test_cutImgToShearchFromDarkAndWater(QImage& img);
	void test_TEST_PERFORMANCE();
	QPair<QImage, QPoint> test_cutRandomAreaFromMap(QImage& map);
	QList<QImage> test_splitMiniMapScreenToListWithoutCross(QImage& miniMapScreen);
	QPoint test_findPlayerPositionByParts(QImage& snipet, QImage& map);
	QList<QPoint> test_findPlayerOnMap(QImage& snipet, QImage& map);
	void test_showImg(QImage img);
	QList<QPoint> test_fillForbiddenPixPositions();

	QString pathToMapFolder;
	QImage minimapWithSlider;

	const QList<QPoint> forbiddenPixPositions = test_fillForbiddenPixPositions();
	const QList<QRect> test_miniMapParts = test_fillListWithRectsPosOfMiniMapParts();
	const QString PATH_TO_FOLDER_WITH_MAPS = setPathToFolderMap();
	const QList<uint> test_allPosibleColorsOnTheMap = test_getAllPossibleMapColors();
	const int WIDTH_TO_WHOLE_PASSED_IMG = 160;
	const int WIDTH_OF_MAP_ONLY_AREA = 106;
	const int HEIGHT_OF_WHOLE_PASSED_IMG = 109;
	const QSize MAP_SIZE = QSize(2560,2048);
	const QSize MAP_START_CORINATES = QSize(31744, 30976);
	const QPoint POSITION_OF_PLAYER_ON_MINIMAP = QPoint(53,54);
	const uint RED_COLOR_ON_MAP = qRgb(255, 51, 0);
	
*/