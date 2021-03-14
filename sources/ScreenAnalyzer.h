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
	QMap<QString, QChar> charCodesMap;
	ScreenAnalyzer( QObject *parent, VariablesClass* var);
	~ScreenAnalyzer();
	void run();
	bool enableScreenAnalyzer = true;
	QString getNameOfLastTakenScreenShot();

	//analyze
	void TMP_save(QImage* img);//////////////kurwa skasuj to Opie
	void TMP();
	void changeGreyPixelsToBlack(QImage* img, int min, int max);
	void deleteScreenShotFolder();
	QChar getCharFromStrMadeFromImg(QString imgEncoded);//todo
	void imgToBlackAndWhite(QImage* img, int threshold);
	QString TMP_letterImgToString(QImage* tt);
private:
	VariablesClass* var;
	void mainLoop();
	QString pathToScreenFolder = "C:\\Users\\ADMIN\\AppData\\Local\\Tibia\\packages\\Tibia\\screenshots";
};
