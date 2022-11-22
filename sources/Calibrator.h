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
	QImage* fullScreen;
	std::shared_ptr<VariablesClass> var;
	Profile* profile;

	bool fillRectWithPotsInVarClass(QImage& fullscreen, QStringList nameOfPotionsToLookFor);
	bool findPotionsOnBottomBar(QStringList namesOfPotionsToFind, QStringList& namesOfPotionosFound, QList<QRect>& rectsWithFoundPots, QImage& bottomBarImg);
	void getMapWithPotionsImgCodes(QMap<QString, QString>& name_code_light, QMap<QString, QString>& name_code_dark);
	bool findIndexesOfRectangleThatContainsSlashes(QImage& fullScreen, QList<QRect> importantFrames, QList<int>& indexesOfFramesWithSlashesVert, QList<int>& indexesOfFramesWithSlashesHor, int& indexOfFrameCombined);
	bool setPositionHealthImgs(QImage& fullscreen, QList<QRect> listOfImportantRectangles, int& indexOfHealth, int& indexOfMana, int& indexOfManaShield, int& indexOfManaAndManaShieldCombined, int& howTheyShouldBeRotated);
	bool categorizeWindows(QImage& fullscreen, QList<QRect>& importantRectangles);
	bool findWindowsOnScreen(QImage& fullScreen, QList<QRect>& importantRectangles);
	void TEST_setPositionHealthImhs(QString pathToFolderWithDiffrentPositionsStylesScreen, QString pathToOutPutFolder);
	bool getRectsFromProfile(QList<QRect>& importRectsFromProf);
	void sortByXY(QList<QPoint>& points, QList<QPoint>& sortedByX, QList<QPoint>& sortedByY);
	void sortByXY(QList<QRect>& inputRects, QList<QRect>& sortedByX, QList<QRect>& sortedByY);
};
