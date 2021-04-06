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
		ERROR_IN_SETTING_POSITION_OF_INTERFACE = 512
	};
	struct Frames {
		QRect gameFrame;
		QRect miniMapFrame;
		QRect healthFrame;
		QRect manaFrame;
		QRect manaShieldFrame;
		bool manaShieldIsIncludedInManaBar = false;
	};
	ScreenAnalyzer( QObject *parent, VariablesClass* var);
	~ScreenAnalyzer();
	void run();
	bool enableScreenAnalyzer = true;

public slots:
	void reCalibrate();
signals:
	void sendAllowenceToAnalyze(bool state);
private:
	VariablesClass* var;
	int timeBetweenNextCheckingsOfScrennShotFolder = 100;
	bool stateOfAnalyzer = false;

	//Advanced shearch
	//QPoint healthSlash;
	//QPoint manaSlash;
	//QPoint manaShieldSlash;
	QRect healthRectImg;
	QRect manaRectImg;
	QRect manaShieldRectImg;
	//const int HEIGHT_OF_HEALING_SLASH = 11;
	//const int WIDTH_FROM_SLASH = 75;

	void mainLoop();
	int calibrate();
	//int calibrate_ManaHealthManaShield(QImage* fullScreenBlackAndWhite);
	int categorizeWindows(QImage fullscreen, QList<QRect> importantRectangles, Frames* frames);
	//int setImgRectsForHealthAnalyzerClass(QImage* full);
	//int determineManaHealthManaShieldPosHor(QImage* fullImg);
	//int determineManaHealthManaShieldPosVer(QImage* fullImg);
	//int splitToPieces(QImage* fullImg);
	void sortByXAndYPoints(QList<QPoint>* points, QList<QPoint>* pointsSortedByX, QList<QPoint>* pointsSortedByY);
	int sortByXAndYRects(QList<QRect> inputRects, QList<QRect>* rectsSortedByPosX, QList<QRect>* rectsSortedByPosY);
	int findWindowsOnScreen(QImage fullScreen, QList<QRect>* importantRectangles);
	int setPositionHealthImgs(QImage fullscreen, QList<QRect> listOfImportantRectangles, bool* manaAndManashieldAreToghere, int* indexOfHealth, int* indexOfMana, int* indexOfManaShield, int* indexOfManaAndManaShieldCombined, int* howTheyShouldBeRotated);
	int loadScreen(QImage* img);
	void deleteScreenShotFolder();
	QString getNameOfLastTakenScreenShot();
	int getNameOfLastTakenScreenShotForSure(QString& toRet, int maxTries);
	QString pathToScreenFolder = "C:\\Users\\ADMIN\\AppData\\Local\\Tibia\\packages\\Tibia\\screenshots";//TODO

	void TEST_setPositionHealthImhs(QString pathToFolderWithDiffrentPositionsStylesScreen, QString pathToOutPutFolder);
};
