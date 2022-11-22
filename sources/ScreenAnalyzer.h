#pragma once
#include <QThread>
#include <memory>
#include <qdir.h>
#include <qimage.h>
#include <qmap.h>
#include <qdebug.h>
#include <qdatetime.h>
#include <qdebug.h>

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
	ScreenAnalyzer( QObject *parent, std::shared_ptr<VariablesClass> var, Profile* prof);
	~ScreenAnalyzer();
	void run();

	bool enableScreenAnalyzer = true;
	bool isManaHealthClassEnabledToAnalyzeImgs = false;
	bool loadScreen(QImage& img);
	void mainLoop();
	int cutImportantImgsFromWholeScreenAndSendThemToVarClass(const QImage& fullscreen);
	QString getNameOfLastTakenScreenShot();

private:
	const int SLEEP_TIME = 50;
	std::shared_ptr<VariablesClass> var;
	QDir screenShotFolder;
	Profile* profile;

	void deleteScreenShotFolder();
	QDir setUpScreenFolder();
};
