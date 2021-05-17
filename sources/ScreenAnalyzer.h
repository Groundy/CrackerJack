#pragma once
#include <QThread>
#include "ScreenAnalyzer.h"
#include "VariablesClass.h"
#include "qdir.h"
#include "qimage.h"
#include "RGBstruct.h"
#include "qdatetime.h"
#include "qmap.h"
#include "qdebug.h"
#include "Profile.h"
#include "Utilities.h"
class ScreenAnalyzer : public QThread
{
	Q_OBJECT

public:
	enum ERROR_CODE {
		OK = 0,
		UNDEFINED_ERROR = 2,
		CANT_GET_SCREEN_NAME_FROM_SCREENSHOT_FOLDER = 4,
		CANT_LOAD_SCREEN_FROM_SCREENSHOT_FOLDER = 8,
		NO_SLASHES_FOUND_IN_GAME_SCREEN = 16,
		NO_POSTION_ASSIGNED_TO_SLASHES = 32,
		NO_RECTANGLE_COPYING_WHOLE_SCREEN_TO_VAR = 64,
		NO_FRAMES_FOUND = 128,
		NO_ENOUGH_FRAMES_FOUND = 256,
		ERROR_IN_SETTING_POSITION_OF_INTERFACE = 512,
		CALIBRATION_FAILED = 1024,
		NOT_ALL_POTS_FOUND = 2048,
	};
	struct Frames {
		QRect gameFrame;
		QRect miniMapFrame;
		QRect healthFrame;
		QRect manaFrame;
		QRect manaShieldFrame;
		QRect combinedFrame;
		int howTheyShouldBeRotated;
	};

	ScreenAnalyzer( QObject *parent, VariablesClass* var, Profile* prof);
	~ScreenAnalyzer();
	void run();
	bool enableScreenAnalyzer = true;
	typedef ERROR_CODE ERR;

public slots:
	int reCalibrate();
signals:
	void sendAllowenceToAnalyze(bool state);
private:
	VariablesClass* var;
	int timeBetweenNextCheckingsOfScrennShotFolder = 100;
	bool isManaHealthClassEnabledToAnalyzeImgs = false;
	Frames frames;
	QStringList listOfPotionNamesToLookFor;


	void mainLoop();
	int calibrate();
	int cutImportantImgsFromWholeScreenAndSendThemToVarClass(QImage& fullscreen);
	int categorizeWindows(QImage& fullscreen, QList<QRect>& importantRectangles, Frames& frames);
	void sortByXAndYPoints(QList<QPoint>& points, QList<QPoint>& pointsSortedByX, QList<QPoint>& pointsSortedByY);
	int sortByXAndYRects(QList<QRect>& inputRects, QList<QRect>& rectsSortedByPosX, QList<QRect>& rectsSortedByPosY);
	int findWindowsOnScreen(QImage& fullScreen, QList<QRect>& importantRectangles);
	int setPositionHealthImgs(QImage& fullscreen, QList<QRect> listOfImportantRectangles, int& indexOfHealth, int& indexOfMana, int& indexOfManaShield, int& indexOfManaAndManaShieldCombined, int& howTheyShouldBeRotated);
	int loadScreen(QImage& img);
	void deleteScreenShotFolder();
	QString getNameOfLastTakenScreenShot();
	int findRectanglesWithPotionsPos(QImage& fullscreen);
	int getNameOfLastTakenScreenShotForSure(QString& toRet, int maxTries);
	QString pathToScreenFolder = "C:\\Users\\ADMIN\\AppData\\Local\\Tibia\\packages\\Tibia\\screenshots";//TODO
	int findIndexesOfRectangleThatContainsSlashes(QImage& fullScreen, QList<QRect> importantFrames, QList<int>& indexesOfFramesWithSlashesVert, QList<int>& indexesOfFramesWithSlashesHor, int& indexOfFrameCombined);
	void TEST_setPositionHealthImhs(QString pathToFolderWithDiffrentPositionsStylesScreen, QString pathToOutPutFolder);

	void notifyOtherProcessOfStateOfAnalyzer(bool worksGood);
};
