#pragma once
#include <QObject>
#include <qrect.h>
#include <qimage.h>
#include <qmap.h>
#include "Utilities.h"
#include "ProfileDataBaseManager.h"
class Calibrator{
public:
	Calibrator(QImage& fullScreenImg, VariablesClass& varClass, Profile* profileToSet);
	~Calibrator();	
	enum ERR {
		OK = 0,
		UNDEFINED_ERR = 2,
		NOT_ALL_POTS_FOUND = 4,
		NO_SLASHES_FOUND_IN_GAME_SCREEN = 8,
		ERROR_IN_SETTING_POSITION_OF_INTERFACE = 16,
		NO_ENOUGH_FRAMES_FOUND = 32,
		NO_FRAMES_FOUND = 64
	};
	static QList<QPoint> findStartPositionInImg_mulitpeImgs(QList<QImage*> imgsToFind, QImage& imgToShareWithin);
	static QList<QPoint> findStartPositionInImg(const QImage& imgToFind, const QImage& imgToShareWithin);
	int calibrateManaAndHealthBar();

private:
	int fillRectWithPotsInVarClass(QImage& fullscreen, QStringList nameOfPotionsToLookFor);
	bool findPotionsOnBottomBar(QStringList namesOfPotionsToFind, QStringList& namesOfPotionosFound, QList<QRect>& rectsWithFoundPots, QImage& bottomBarImg);
	void getMapWithPotionsImgCodes(QMap<QString, QString>& name_code_light, QMap<QString, QString>& name_code_dark);
	int findIndexesOfRectangleThatContainsSlashes(QImage& fullScreen, QList<QRect> importantFrames, QList<int>& indexesOfFramesWithSlashesVert, QList<int>& indexesOfFramesWithSlashesHor, int& indexOfFrameCombined);
	int setPositionHealthImgs(QImage& fullscreen, QList<QRect> listOfImportantRectangles, int& indexOfHealth, int& indexOfMana, int& indexOfManaShield, int& indexOfManaAndManaShieldCombined, int& howTheyShouldBeRotated);
	void sortByXAndYPoints(QList<QPoint>& points, QList<QPoint>& pointsSortedByX, QList<QPoint>& pointsSortedByY);
	int sortByXAndYRects(QList<QRect>& inputRects, QList<QRect>& rectsSortedByPosX, QList<QRect>& rectsSortedByPosY);
	int categorizeWindows(QImage& fullscreen, QList<QRect>& importantRectangles);
	int findWindowsOnScreen(QImage& fullScreen, QList<QRect>& importantRectangles);
	void TEST_setPositionHealthImhs(QString pathToFolderWithDiffrentPositionsStylesScreen, QString pathToOutPutFolder);
	bool getRectsFromProfile(QList<QRect>& importRectsFromProf);
	QImage* fullScreen;
	VariablesClass* var;
	Profile* profile;
};
