#pragma once
#include <QThread>
#include "VariablesClass.h";
#include "qimage.h"
#include "Utilities.h"
#include "qdatetime.h"
class ManaHealthStateAnalyzer : public QThread
{
	Q_OBJECT

public:
	ManaHealthStateAnalyzer(QObject *parent, VariablesClass* varClass);
	~ManaHealthStateAnalyzer();
	int miliSecBetweenCheckingForNewValuesImg = 100;
	bool shouldThisThreadBeActive = true;
	void run();
signals:
	void demandReCalibration();
	void sendValueToMainThread(QString, QString, QString);
private:
	//simple
	void SIMPLE_mainloop();
	//advanced
	QImage healthImg;
	QImage manaImg;
	QImage manaShieldImg;
	QList<QImage*> listOfImg;
	QString healthValueStr;
	QString manaValueStr;
	QString manaShieldValueStr;
	int mana;
	int manaShield;
	int health;
	int rotationNeeded;
	bool isManaShieldActive;
	void imgsToBlackAndWhite();
	void getInfoFromVarClass();
	void setImagesToAnalyze();
	void rotateImgsInfNeeded();
	void mainLoop();
	bool changeImgsToStrings();
	void cutBorders();
	bool checkIfImgWereLoadedCorrectly();
	void setHealthManaValues();
	void deletePartOfStrSideOfBracket_Outside(QString* strToEdit);
	void deletePartOfStrOutSideOfBracket_Inside(QString* strToEdit);
	void changeStrWithValueToValue(QString in, int* outValue);
	void makeStringsForSignalToSend(QString* health, QString* mana, QString* manaShield);
	
	VariablesClass* var;
};
