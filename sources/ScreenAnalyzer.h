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
class ScreenAnalyzer : public QThread
{
public:
	QMap<QString,QChar> letters;
	ScreenAnalyzer( QObject *parent, VariablesClass* var);
	~ScreenAnalyzer();
	void run();
	bool enableScreenAnalyzer = true;
	QString getNameOfLastTakenScreenShot();

	//analyze
	void TMP_save(QImage* img);//////////////kurwa skasuj to Opie
	void TMP();
	void TMP2();
	void changeGreyPixelsToBlack(QImage* img, int min, int max);
	void deleteScreenShotFolder();
	void imgToBlackAndWhiteOneColor(QImage* img, int threshold);
	void imgToBlackAndWhiteAllColors(QImage* img, int threshold);
	QString imgWithStrToStr(QImage* img);
	QString TMP_letterImgToString(QImage* tt);
	void initQmap();
	void cutBlackBordersOfImg(QImage* img);
	void cutImgWithLettersToSingleLettersImgList(QImage* img, QList<QImage>* list);
private:
	VariablesClass* var;
	void mainLoop();
	QString pathToScreenFolder = "C:\\Users\\ADMIN\\AppData\\Local\\Tibia\\packages\\Tibia\\screenshots";//TODO
};
