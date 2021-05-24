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
#include "Calibrator.h"
#include "Profile.h"
#include "Utilities.h"
#include "ProfileDataBaseManager.h"
class ScreenAnalyzer : public QThread
{
	Q_OBJECT

public:
	enum ERROR_CODE {
		OK = 0,
		UNDEFINED_ERROR = 2,
		CANT_LOAD_SCREEN_FROM_SCREENSHOT_FOLDER = 4,
		NO_ENOUGH_FRAMES_FOUND = 8
	};

	ScreenAnalyzer( QObject *parent, VariablesClass* var, Profile* prof);
	~ScreenAnalyzer();
	void run();
	bool enableScreenAnalyzer = true;

signals:
	void sendAllowenceToAnalyze(bool state);
private:
	VariablesClass* var;
	Profile* profile;
	int timeBetweenNextCheckingsOfScrennShotFolder = 100;
	bool isManaHealthClassEnabledToAnalyzeImgs = false;


	void mainLoop();
	int cutImportantImgsFromWholeScreenAndSendThemToVarClass(QImage& fullscreen);
	int loadScreen(QImage& img);
	void deleteScreenShotFolder();
	QString getNameOfLastTakenScreenShot();
	int getNameOfLastTakenScreenShotForSure(QString& toRet, int maxTries);
	QString pathToScreenFolder = "C:\\Users\\ADMIN\\AppData\\Local\\Tibia\\packages\\Tibia\\screenshots";//TODO

	void notifyOtherProcessOfStateOfAnalyzer(bool worksGood);
};
