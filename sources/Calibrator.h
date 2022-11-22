#pragma once
#include <QObject>
#include <QMap>
#include <qrect.h>
#include <qimage.h>
#include <qmap.h>

#include "Utilities.h"
#include "ImgEditor.h"
#include "Profile.h"

class Calibrator{
public:
	Calibrator(QImage& fullScreen, std::shared_ptr<VariablesClass> var, Profile* profileToSet);
	~Calibrator();	
	static QList<QPoint> findStartPositionInImg_mulitpeImgs(QList<QImage*> imgsToFind, QImage& imgToShareWithin);
	static QList<QPoint> findStartPositionInImg(const QImage& imgToFind, const QImage& imgToShareWithin);
	bool calibrateManaAndHealthBar();

private:
	bool fillRectWithPotsInVarClass(QImage& fullscreen, QStringList nameOfPotionsToLookFor);
	bool findPotionsOnBottomBar(QStringList namesOfPotionsToFind, QStringList& namesOfPotionosFound, QList<QRect>& rectsWithFoundPots, QImage& bottomBarImg);
	void getMapWithPotionsImgCodes(QMap<QString, QString>& name_code_light, QMap<QString, QString>& name_code_dark);
	bool findIndexesOfRectangleThatContainsSlashes(QImage& fullScreen, QList<QRect> importantFrames, QList<int>& indexesOfFramesWithSlashesVert, QList<int>& indexesOfFramesWithSlashesHor, int& indexOfFrameCombined);
	bool setPositionHealthImgs(QImage& fullscreen, QList<QRect> listOfImportantRectangles, int& indexOfHealth, int& indexOfMana, int& indexOfManaShield, int& indexOfManaAndManaShieldCombined, int& howTheyShouldBeRotated);
	bool categorizeWindows(QImage& fullscreen, QList<QRect>& importantRectangles);
	bool findWindowsOnScreen(QImage& fullScreen, QList<QRect>& importantRectangles);
	void TEST_setPositionHealthImhs(QString pathToFolderWithDiffrentPositionsStylesScreen, QString pathToOutPutFolder);
	bool getRectsFromProfile(QList<QRect>& importRectsFromProf);
	QImage* fullScreen;
	std::shared_ptr<VariablesClass> var;
	Profile* profile;

	void sortByXY(QList<QPoint>& points, QList<QPoint>& sortedByX, QList<QPoint>& sortedByY) {
		QMap<int, QPoint> mapX;
		QMap<int, QPoint> mapY;
		for each (QPoint point in points) {
			mapX.insert(point.x(), point);
			mapY.insert(point.y(), point);
		}
		sortedByX = mapX.values();
		sortedByY = mapY.values();
	}
	void sortByXY(QList<QRect>& inputRects, QList<QRect>& sortedByX, QList<QRect>& sortedByY) {
		QMap<int, QRect> mapX;
		QMap<int, QRect> mapY;
		for each (QRect rect in inputRects) {
			mapX.insert(rect.x(), rect);
			mapY.insert(rect.y(), rect);
		}
		sortedByX = mapX.values();
		sortedByY = mapY.values();
	}

};
