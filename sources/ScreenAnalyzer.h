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
#include "ErrorManager.h"
class ScreenAnalyzer : public QThread
{
	Q_OBJECT
	enum style {DEFAULT, PARALLEL, LARGE, COMPACT};
	enum position { LEFT, RIGHT, TOP, DOWN };
public:
	enum ERROR_CODE {
		OK = 0,
		UNDEFINED_ERROR = 2,
		CANT_GET_SCREEN_NAME_FROM_SCREENSHOT_FOLDER = 4,
		CANT_LOAD_SCREEN_FROM_SCREENSHOT_FOLDER = 8,
		NO_SLASHES_FOUND_IN_GAME_SCREEN = 16,
		NO_POSTION_ASSIGNED_TO_SLASHES = 32,
	};
	ScreenAnalyzer( QObject *parent, VariablesClass* var);
	~ScreenAnalyzer();
	void run();
	bool enableScreenAnalyzer = true;

public slots:
	void reCalibrate();

private:
	VariablesClass* var;
	int timeBetweenNextCheckingsOfScrennShotFolder = 100;

	//simple shearch
	void SIMPLE_mainLoop();
	void SIMPLE_calibration();
	QRect manaBar;
	QRect healthBar;

	//Advanced shearch
	QPoint healthSlash;
	QPoint manaSlash;
	QPoint manaShieldSlash;
	QRect healthRectImg;
	QRect manaRectImg;
	QRect manaShieldRectImg;
	const int heightOfHealthImgStr = 11;
	const int widthFromSlashChar = 75;
	void mainLoop();
	int calibrate();
	int setImgRectsForHealthAnalyzerClass(QImage* full);
	int determineManaHealthManaShieldPosHor(QImage* fullImg);
	int determineManaHealthManaShieldPosVer(QImage* fullImg);
	int splitToPieces(QImage* fullImg);
	void sortByXAndY(QList<QPoint>* points, QList<QPoint>* pointsSortedByX, QList<QPoint>* pointsSortedByY);
	int howTheyShouldBeRotatedInRight; //-1 one time to left, 1 one time to right
	
	int loadScreen(QImage* img);
	void deleteScreenShotFolder();
	QString getNameOfLastTakenScreenShot();
	int getNameOfLastTakenScreenShotForSure(QString& toRet, int maxTries);
	QString pathToScreenFolder = "C:\\Users\\ADMIN\\AppData\\Local\\Tibia\\packages\\Tibia\\screenshots";//TODO
};
