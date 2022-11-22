#pragma once
#include <QThread>
#include "VariablesClass.h"
#include "qimage.h"
#include "Utilities.h"
#include "qdatetime.h"
#include "Profile.h"
#include "JsonParser.h"
class ManaHealthStateAnalyzer : public QThread
{
	Q_OBJECT
public:
	ManaHealthStateAnalyzer(QObject *parent, Profile* profile, std::shared_ptr<VariablesClass> var);
	~ManaHealthStateAnalyzer();

	int miliSecBetweenCheckingForNewValuesImg = 80;
	bool shouldThisThreadBeActive = true;
	void run();

public slots:
	void setThreadEnabilityToRun(bool stateToSet);

signals:
	void demandReCalibration();
	void sendValueToMainThread(double, double, double);
	void sendInfoAboutPotAmountsToGUI(QStringList);

private:
	QMap<int, RestorationMethode> healthMap, manaMap;

	QImage healthImg, manaImg, manaShieldImg, combinedImg;
	bool healthFound, manaFound, manaShieldFound, combinedFound;
	QString healthValueStr, manaValueStr, manaShieldValueStr, combinedValueStr;
	int mana, maxMana;
	int manaShield, maxManaShield;
	int health, maxHealth;
	float healthPercentage, manaPercentage, manaShieldPercentage;

	//QVector<RestorationMethode> healthRestorationsMethode, manaRestorationsMethode;
	//QVector<int> healthThresholds, manaThresholds;
	//QList<Utilities::RestoreMethode> healthMethodes;
	//QList<Item> manaMethodes;
	LONG64 lastTimeAnalyzed = 0;

	int extraTimeToWaitBetweenManaPotUse = 1400;
	void getInfoFromVarClass();
	void mainLoop();
	bool changeImgsToStrings();
	void getValuesFromStringsToGlobablVariables();
	void PreapareAndSendInfoToGuiInMainThread();
	int getValuesFromStringRegularCase(QString in, int& min, int& max);
	int getValuesFromStringOfCombinedBox(QString in, int* minMana, int* maxMana, int* minManaShield, int* maxManaShield);	
	int findNearestThresholdIndex(int value, const QVector<int>& thresholds);
	bool checkIfEverythingIsCorrectToProcess();
	void writeDataToVariableClass();
	void setupRestorationMethodes(QStringList listOfRestorationMethode_Health, QStringList listOfRestorationMethode_Mana);
	bool getKeysForConnectorClass(Key& healthKey, Key& manaKey);
	void sleepAppropirateTimeToNextAnalyze();
	void getAmountsOfPotions();
//	void readManaBreak();
	std::shared_ptr<VariablesClass> var;

	QVector<int> getThresholdsOfRestorationMethodes(const QVector<RestorationMethode>& restorationMethodes) {
		QVector<int> thresholds;
		for each (auto var in restorationMethodes)
			thresholds.push_back(var.getThreshold());
		return thresholds;
	}
	bool populateHealthManaMaps(const Profile* profile) {
		try{
			auto healthRestorationsMethode = profile->getRestMethodesHealth();
			auto healthThresholds = getThresholdsOfRestorationMethodes(healthRestorationsMethode);
			if (healthThresholds.size() != healthRestorationsMethode.size())
				throw std::exception("wrong sizes of health restoration methodes passed to ManaHealthAnalyzer!");

			for (size_t i = 0; i < healthThresholds.size(); i++)
				healthMap.insert(healthThresholds[i], healthRestorationsMethode[i]);
			
			auto manaRestorationsMethode = profile->getRestMethodesMana();
			auto manaThresholds = getThresholdsOfRestorationMethodes(manaRestorationsMethode);
			if (manaThresholds.size() != manaRestorationsMethode.size())
				throw std::exception("wrong sizes of health restoration methodes passed to ManaHealthAnalyzer!");

			for (size_t i = 0; i < manaThresholds.size(); i++)
				healthMap.insert(manaThresholds[i], manaRestorationsMethode[i]);

			return true;
		}
		catch (const std::exception& e ){
			qDebug() << e.what();
			return false;
		}
	}
};
