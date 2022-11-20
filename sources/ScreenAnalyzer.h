#pragma once
#include <QThread>
#include <memory>
#include <qdir.h>
#include <qimage.h>
#include <qmap.h>
#include <qdebug.h>
#include <qdatetime.h>

#include "Calibrator.h"
#include "Profile.h"
#include "Utilities.h"
#include "RGBstruct.h"
#include "ScreenAnalyzer.h"
#include "VariablesClass.h"
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

	ScreenAnalyzer( QObject *parent, std::shared_ptr<VariablesClass> var, Profile* prof);
	~ScreenAnalyzer();
	void run();
	bool enableScreenAnalyzer = true;
	int loadScreen(QImage& img);
	void deleteScreenShotFolder();
signals:
	void sendAllowenceToAnalyze(bool state);
private:
	std::shared_ptr<VariablesClass> var;
	Profile* profile;
	int timeBetweenNextCheckingsOfScrennShotFolder = 100;
	bool isManaHealthClassEnabledToAnalyzeImgs = false;


	void mainLoop();
	int cutImportantImgsFromWholeScreenAndSendThemToVarClass(QImage& fullscreen);
	QString getNameOfLastTakenScreenShot();
	int getNameOfLastTakenScreenshotWithTries(QString& toRet, const int maxTries);
	QDir screenShotFolder;
	QDir setUpScreenFolder();
	void notifyOtherProcessOfStateOfAnalyzer(bool worksGood);
};
