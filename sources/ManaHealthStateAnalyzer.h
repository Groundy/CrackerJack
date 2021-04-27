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
	enum ERROR_CODES {
		OK = 0,
		OTHER_ERROR = 2,
		WRONG_STR_OF_VALUES = 4,
		ZERO_AS_MAX_VALUE = 8
	};
	int miliSecBetweenCheckingForNewValuesImg = 100;
	bool shouldThisThreadBeActive = false;
	void run();
public slots:
	void setThreadEnabilityToRun(bool stateToSet);
signals:
	void demandReCalibration();
	void sendValueToMainThread(QString, QString, QString);
private:
	//advanced
	QImage healthImg;
	QImage manaImg;
	QImage manaShieldImg;
	QImage combinedImg;

	bool healthFound;
	bool manaFound;
	bool manaShieldFound;
	bool combinedFound;

	QString healthValueStr;
	QString manaValueStr;
	QString manaShieldValueStr;
	QString combinedValueStr;
	int mana, maxMana;
	int manaShield, maxManaShield;
	int health, maxHealth;
	bool getInfoFromVarClass();
	void mainLoop();
	int changeImgsToStrings();
	void setHealthManaValues();
	int getValuesFromStringRegularCase(QString in, int* min, int* max);
	int getValuesFromStringOfCombinedBox(QString in, int* minMana, int* maxMana, int* minManaShield, int* maxManaShield);
	int makeStringsForSignalToSend(QString* health, QString* mana, QString* manaShield);
	void writeDataToVariableClass();
	VariablesClass* var;
};
