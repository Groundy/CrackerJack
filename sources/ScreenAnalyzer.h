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
	//enum style {DEFAULT, PARALLEL, LARGE, COMPACT};
public:
	ScreenAnalyzer( QObject *parent, VariablesClass* var);
	~ScreenAnalyzer();
	void run();
	bool enableScreenAnalyzer = true;
private:
	void deleteScreenShotFolder();
	QString getNameOfLastTakenScreenShot();
	VariablesClass* var;
	void mainLoop();
	void calibrate(QImage fullScreenImg);
	const int heightOfHealthImgStr = 11;
	const int widthFromSlashChar = 75;
	QPoint healthSlash;
	QPoint manaSlash;
	QPoint manaShieldSlash;
	bool setManaHealthManaShieldPosHor(QImage* fullImg);
	bool setManaHealthManaShieldPosVer(QImage* fullImg);
	QString pathToScreenFolder = "C:\\Users\\ADMIN\\AppData\\Local\\Tibia\\packages\\Tibia\\screenshots";//TODO
};
