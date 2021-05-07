#pragma once

#include <QThread>
#include "VariablesClass.h";
#include "qimage.h"
#include "Utilities.h"
#include "qdatetime.h"
#include "Profile.h"
class ManaHealthStateAnalyzer : public QThread
{
	Q_OBJECT
public:
	ManaHealthStateAnalyzer(QObject *parent, Profile* profile, VariablesClass* varClass);
	~ManaHealthStateAnalyzer();
	enum ERROR_CODES {
		OK = 0,
		OTHER_ERROR = 2,
		WRONG_STR_OF_VALUES = 4,
		ZERO_AS_MAX_VALUE = 8,
		WRONG_INPUT_PARAMETERS = 16,
		NOT_THRESHOLDS_ON_THE_LIST = 32

	};
	int miliSecBetweenCheckingForNewValuesImg = 80;
	bool shouldThisThreadBeActive = false;
	void run();
public slots:
	void setThreadEnabilityToRun(bool stateToSet);
signals:
	void demandReCalibration();
	void sendValueToMainThread(QString, QString, QString);
private:
	QImage healthImg, manaImg, manaShieldImg, combinedImg;
	bool healthFound, manaFound, manaShieldFound, combinedFound;
	QString healthValueStr, manaValueStr, manaShieldValueStr, combinedValueStr;
	int mana, maxMana;
	int manaShield, maxManaShield;
	int health, maxHealth;
	/*
	struct RestoreMethode{
		enum Type {Potion, Spell};
		QString name, inc;
		Type type;
		int mana, cd, groupCd, threshold, key;
	};
	*/
	//info from prof
	QList<int> lifeThreshholds;
	QList<int> manaThreshholds;
	QList<Key> healthKeys;
	QList<Key> manaKeys;
	QList<QString> namesOfHealthRestoreMethodes;
	QList<QString> namesOfManaRestoreMethodes;

	QList<QObject> healthMethodes;
	QList<Utilities::Item> manaMethodes;

	bool getInfoFromVarClass();
	void mainLoop();
	int changeImgsToStrings();
	void getValuesFromStringsToGlobablVariables();
	void PreapareAndSendInfoToGuiInMainThread();
	int getValuesFromStringRegularCase(QString in, int* min, int* max);
	int getValuesFromStringOfCombinedBox(QString in, int* minMana, int* maxMana, int* minManaShield, int* maxManaShield);
	int makeStringsForSignalToSend(QString* health, QString* mana, QString* manaShield);
	int findNearestThresholdIndex(int value, QList<int> thresholds, int* out_index);
	bool checkIfEverythingIsCorrectToProcess();
	void writeDataToVariableClass();
	void fillListsWithMethodesOfRestoring(QList<QObject>* listOfHealthRestoration, QList<Utilities::Item>* listOfManaRestore, QList<QString> namesOfHealthMethodes, QList<QString> namesOfManaMethodes);
	void findKeysThatShouldBePassedToKeySender();
	VariablesClass* var;
};
