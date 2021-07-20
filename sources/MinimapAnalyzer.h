#pragma once
#include <QObject>
#include <qdir.h>
#include <qimage.h>
#include <qstring.h>
#include <qpoint.h>
#include "Logger.h"
#include "Utilities.h"
#include "Calibrator.h"

class ShearchArea {
public:
	QMap<int, QList<QRect>> areas;
	ShearchArea() {};
	ShearchArea(QString structStr) {
		//expected struct [#lvlNumber[x,y,w,h;x,y,w,h;]#lvlNumber[x,y,w,h;x,y,w,h]]

		bool wholeStructRead = structStr.left(1) == "[" && structStr.right(1) == "]";
		if (!wholeStructRead) {
			Logger::logPotenialBug("error in creating ShearchArea struct", "ShearchArea(QString)", "ShearchArea");
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
				Logger::logPotenialBug("error in creating ShearchArea struct", "ShearchArea(QString)", "ShearchArea");
				return;
			}
			lvlStr = lvlStr.right(lvlStr.size() - 2);
			somethingWrong = lvlStr.left(1) != "[" || lvlStr.right(1) != "]";
			if (somethingWrong) {
				Logger::logPotenialBug("error in creating ShearchArea struct", "ShearchArea(QString)", "ShearchArea");
				return;
			}
			lvlStr = lvlStr.mid(1, lvlStr.size() - 2);
			QStringList rectanglesParametersList = lvlStr.split(";", Qt::SkipEmptyParts);
			QList<QRect> rectsToSet;
			for each (QString rectParametersStr in rectanglesParametersList){
				QStringList rectParameters = rectParametersStr.split(",", Qt::SkipEmptyParts);
				if (rectParameters.size() != 4) {
					Logger::logPotenialBug("error in creating ShearchArea struct", "ShearchArea(QString)", "ShearchArea");
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

struct Position3D{
	int x, y, floor;
};

class MinimapAnalyzer : public QObject
{
	Q_OBJECT

public:
	MinimapAnalyzer();
	~MinimapAnalyzer();
	QList<Position3D> findPlayerPositionOnMap(QImage& miniMapSnippet, ShearchArea shearchArea);
	QList<Position3D> findPlayerPositionOnMap(QImage& miniMapSnippet, int floor);
	QList<Position3D> findPlayerPositionOnMap(QImage& miniMapSnippet);

	QList<QPoint> findPlayerPosition(QImage& miniMapSnippet, QImage& ImgToShareWithIn, int maxPixThatCanBeDiffrent);
private:
	QString getNameOfMapFileToLoad(int floor, bool regularMapType = true);
	QString setPathToFolderMap();
	QImage setSliderImg();
	uint getFloorNumber();
	QList<QPoint> fillFobiddenPixList();

	QString pathToMapFolder;
	QImage minimapWithSlider;
	const QImage maps[16];
	
	const QList<QPoint> forbiddenPixels = fillFobiddenPixList();
	const QString PATH_TO_FOLDER_WITH_MAPS = setPathToFolderMap();
	const QImage SLIDER_IMG = setSliderImg();
	const int WIDTH_TO_WHOLE_PASSED_IMG = 160;
	const int WIDTH_OF_MAP_ONLY_AREA = 106;
	const int HEIGHT_OF_WHOLE_PASSED_IMG = 109;
	const QSize MAP_SIZE = QSize(2560,2048);
	const QSize MAP_START_CORINATES = QSize(31744, 30976);
};
