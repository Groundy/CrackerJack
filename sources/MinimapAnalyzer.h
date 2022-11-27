#pragma once
#include <QObject>
#include <qdir.h>
#include <qimage.h>
#include <qstring.h>
#include <qthread.h>
#include <qpoint.h>
#include <qrandom.h>
#include <qdatetime.h>

#include "Utilities.h"	
#include "Calibrator.h"
#include "VariablesClass.h"
#include "Utilities.h"

class MinimapAnalyzer : public QThread {
	Q_OBJECT
public:
	MinimapAnalyzer(QObject* parent, std::shared_ptr<VariablesClass> var) : QThread(parent), var(var) {
	}
	~MinimapAnalyzer() {};
	void run(){
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
		}
	}

signals:
	void sendPostitionsToGUI(QString, QString, QString);

private:
	std::shared_ptr<VariablesClass> var;
	const int SLEEP_TIME_BETWEEN_LOOPS = 20, SLEEP_TIME_NO_ACTIVE = 1000;
	const int MIN_LAYER = -7, MAX_LAYER = 8;
	const QImage LAYER_SLIDER_IMG = setSliderImg();
	const int MIN_X_MAP_INDEX = 124, MAX_X_MAP_INDEX = 134, MIN_Y_MAP_INDEX = 121, MAX_Y_MAP_INDEX = 129;

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
	QString getNameOfMapFileToLoad(int floor, bool regularMapType = true);
	QString setPathToFolderMap();

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
	const QPoint POSITION_OF_PLAYER_ON_MINIMAP =QPoint(53,54);
	const uint RED_COLOR_ON_MAP = qRgb(255, 51, 0);
	
*/