#pragma once
#include <QThread>
#include "VariablesClass.h";
#include "qimage.h"

class ManaHealthStateAnalyzer : public QThread
{
	Q_OBJECT
//QImage* tt = new QImage(img->copy(386, 11, 78, 14));
public:
	ManaHealthStateAnalyzer(QObject *parent, VariablesClass* varClass);
	~ManaHealthStateAnalyzer();
	int miliSecBetweenCheckingForNewValuesImg = 100;
	void run();
private:
	QImage* healthImg;
	QImage* manaImg;
	QImage* stateImg;
	VariablesClass* var;
	void mainLoop();
	float getPercentagesOfHealth();
};
