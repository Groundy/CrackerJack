#pragma once
#include <QThread>
#include <qimage.h>
#include <qdatetime.h>

#include "ImgEditor.h"
#include "VariablesClass.h"
#include "Utilities.h"
#include "Profile.h"
#include "JsonParser.h"
class ManaHealthStateAnalyzer : public QThread
{
	Q_OBJECT
public:
	ManaHealthStateAnalyzer(QObject *parent, Profile* profile, std::shared_ptr<VariablesClass> var);
	~ManaHealthStateAnalyzer();

	void run();

signals:
	void demandReCalibration();
	void sendValueToMainThread(double, double, double);
	void sendInfoAboutPotAmountsToGUI(QStringList);

private:
	struct FoundFlags {
		bool health, mana, combined, shield;
	};
	struct ImageValues {
		QImage health, mana, manaShield, combined;
	};
	struct ValuesStrs {
		QString health, mana, manaShield, combined;
	};
	struct ValuesInts {
		int health, maxHealth;
		int mana, maxMana;
		int shield, maxShield;
	};
	struct ValuesDoubles {
		ValuesDoubles(double health, double mana, double manaShield) :
			health(health), mana(mana), manaShield(manaShield) {}
		ValuesDoubles() :
			health(NULL), mana(NULL), manaShield(NULL) {}
		double health, mana, manaShield;
	};


	//QVector<RestorationMethode> healthRestorationsMethode, manaRestorationsMethode;
	//QVector<int> healthThresholds, manaThresholds;
	//QList<Utilities::RestoreMethode> healthMethodes;
	//QList<Item> manaMethodes;

	void mainLoop();
	bool checkIfEverythingIsCorrectToProcess();
	void setupRestorationMethodes(QStringList listOfRestorationMethode_Health, QStringList listOfRestorationMethode_Mana);
	bool getKeysForConnectorClass(Key& healthKey, Key& manaKey);
	void sleepAppropirateTimeToNextAnalyze();
	void getAmountsOfPotions();
//	void readManaBreak();


	std::shared_ptr<VariablesClass> var;
	QMap<int, RestorationMethode> healthMap, manaMap;	
	int extraTimeToWaitBetweenManaPotUse = 1400;
	const int SLEEP_TIME = 500;
	LONG64 lastTimeAnalyzed = 0;

	ValuesDoubles ChangeToPercentages(ValuesInts currentValues) {
		bool healthValuesOk = 
			currentValues.health <= currentValues.maxHealth && 
			currentValues.health >= 0 && 
			currentValues.maxHealth > 0;

		double healthPercentage;
		if (healthValuesOk)
			healthPercentage = 100.0 * currentValues.health / currentValues.maxHealth;
		else
			healthPercentage = NULL;

		bool manaValuesOk = 
			currentValues.mana <= currentValues.maxMana &&
			currentValues.mana >= 0 &&
			currentValues.maxMana > 0;
			
		double manaPercentage;
		if (manaValuesOk)
			manaPercentage = 100.0 * currentValues.mana / currentValues.maxMana;
		else
			manaPercentage = NULL;

		bool manaShieldValusOk = currentValues.shield <= currentValues.maxShield;

		double manaShieldPercentage;
		if (manaShieldValusOk && currentValues.maxShield > 0)
			manaShieldPercentage = 100.0 * currentValues.shield / currentValues.maxShield;
		else
			manaShieldPercentage = NULL;
		ValuesDoubles toRet(healthPercentage, manaShieldValusOk, manaShieldPercentage);
		return toRet;
	}
	QVector<int> GetThresholdsOfRestorationMethodes(const QVector<RestorationMethode>& restorationMethodes) {
		QVector<int> thresholds;
		for each (auto var in restorationMethodes)
			thresholds.push_back(var.getThreshold());
		return thresholds;
	}
	bool PopulateHealthManaMaps(const Profile* profile) {
		try{
			auto healthRestorationsMethode = profile->getRestMethodesHealth();
			auto healthThresholds = GetThresholdsOfRestorationMethodes(healthRestorationsMethode);
			if (healthThresholds.size() != healthRestorationsMethode.size())
				throw std::exception("wrong sizes of health restoration methodes passed to ManaHealthAnalyzer!");

			for (size_t i = 0; i < healthThresholds.size(); i++)
				healthMap.insert(healthThresholds[i], healthRestorationsMethode[i]);
			
			auto manaRestorationsMethode = profile->getRestMethodesMana();
			auto manaThresholds = GetThresholdsOfRestorationMethodes(manaRestorationsMethode);
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
	ValuesInts ValuesToInts(FoundFlags foundFlags, ValuesStrs valueStrs) {
		ValuesInts valuesInts;
		if (foundFlags.health)
			getValuesFromStringRegularCase(valueStrs.health, valuesInts.health, valuesInts.maxHealth);
		if (foundFlags.combined)
			getValuesFromStringOfCombinedBox(valueStrs.combined, valuesInts.mana, valuesInts.maxMana, valuesInts.shield, valuesInts.maxShield);
		if (foundFlags.shield)
			getValuesFromStringRegularCase(valueStrs.manaShield, valuesInts.shield, valuesInts.maxShield);
		if (foundFlags.mana)
			getValuesFromStringRegularCase(valueStrs.mana, valuesInts.mana, valuesInts.maxMana);
		return valuesInts;
	};
	ValuesStrs ImagesToStrs(FoundFlags foundFlags, ImageValues imgVals) {
		try {
			ValuesStrs strVals;
			if (foundFlags.health)
				strVals.health = ImgEditor::imgWithStrToStr(imgVals.health).remove("\0");
			else
				strVals.health = QString();

			if (foundFlags.mana)
				strVals.mana = ImgEditor::imgWithStrToStr(imgVals.mana).remove("\0");
			else
				strVals.mana = QString();


			if (foundFlags.shield)
				strVals.manaShield = ImgEditor::imgWithStrToStr(imgVals.manaShield).remove("\0");
			else
				strVals.manaShield = QString();

			if (foundFlags.combined)
				strVals.combined = ImgEditor::imgWithStrToStr(imgVals.combined).remove("\0");
			else
				strVals.combined = QString();

			const int MIN_LENGTH_FOR_CORR_STR = 3;
			if (strVals.health.length() < MIN_LENGTH_FOR_CORR_STR)
				throw std::exception("Error in converting ims with health bar to str form");
			if (strVals.mana.length() < MIN_LENGTH_FOR_CORR_STR)
				throw std::exception("Error in converting ims with mana bar to str form");

			return strVals;
		}
		catch (const std::exception& e) {
			qDebug() << e.what();
			return ValuesStrs();
		}
	}
	void sendDataToGui(ValuesDoubles currentValues) {
		emit sendValueToMainThread(currentValues.health, currentValues.mana, currentValues.manaShield);
	}
	void writeDataToVariableClass(ValuesDoubles values){
		var->setCurrentHealthPercentage(values.health);
		var->setCurrentManaPercentage(values.mana);
		var->setCurrentMSPercentage(values.manaShield);
	}
	ImageValues getImages() {
		ImageValues toRet;
		bool clearImgs = true;
		var->getImageHealth(toRet.health, clearImgs);
		var->getImageMana(toRet.mana, clearImgs);
		var->getImageMS(toRet.manaShield, clearImgs);
		var->getImageCombined(toRet.combined, clearImgs);
		return toRet;
	}
	FoundFlags getFoundFlags() {
		FoundFlags flags;
		flags.health = var->getHealthArea().isEmpty();
		flags.mana = var->getManaArea().isEmpty();
		flags.combined = var->getCombinedArea().isEmpty();
		flags.shield = var->getMSArea().isEmpty();
		return flags;
	}
	bool getValuesFromStringRegularCase(QString in, int& current, int& max){
	try{
		const QStringList partOfStr = in.split("/");//wanted form of input currentVal/maxVal
		if (partOfStr.size() != 2)
			throw std::exception(QString("ManaHealthAnalyzer recived wrong input, input = " + in).toStdString().c_str());
		const int currentVal = partOfStr[0].toInt();
		const int maxVal = partOfStr[1].toInt();
		const int MAX_POSSIBLE_VALUE = 100000;
		const bool wrongValues = currentVal > MAX_POSSIBLE_VALUE || currentVal > maxVal || currentVal < 0 || maxVal < 0;
		if (wrongValues)
			throw std::exception("Wrong int values in splittling str currentVal/maxVal");
		current = currentVal;
		max = maxVal;
		return true;
	}
	catch (const std::exception& e){
		qDebug() << e.what();
		return false;
	}
}
	bool getValuesFromStringOfCombinedBox(QString in, int& currentMana, int& maxMana, int& currentManaShield, int& maxManaShield) {
		//wanted form of input manaMinVal/maxVal(minShieldValue/maxShieldValue)
		try {
			bool inputOk = in.count("/") == 3 && in.count("(") == 1 && in.count(")") == 1;
			if (!inputOk) {
				QString msg = "Wrong input in splitting str with values of mana combined with magic shield, input = " + in;
				throw std::exception(msg.toStdString().c_str());
			}

			in.remove(")");
			QStringList parts = in.split("(");
			if (parts.size() != 2)
				throw std::exception("error in splitting  str with values of mana combined with magic shield");

			QString manaStr = parts[0];
			QString shieldStr = parts[1];
			int currentManaTMP, maxManaTMP, currentManaShieldTMP, maxManaShieldTMP;
			bool splittingOk1 = getValuesFromStringRegularCase(manaStr, currentManaTMP, maxManaTMP);
			bool splittingOk2 = getValuesFromStringRegularCase(shieldStr, currentManaShieldTMP, maxManaShieldTMP);
			bool okResult = splittingOk1 && splittingOk2 && maxManaTMP > 0;
			if (!okResult)
				throw std::exception("error in splitting  str with values of mana combined with magic shield");

			currentMana = currentManaTMP;
			maxMana = maxManaTMP;
			currentManaShield = currentManaShieldTMP;
			maxManaShield = maxManaShieldTMP;
			return true;
		}
		catch (const std::exception& e) {
			qDebug() << e.what();
			return false;
		}
	}
	int findNearestThresholdIndex(int currentValue, const QVector<int>& thresholds) {
		try {
			bool wrongInput = currentValue < 0 || currentValue > 100 || thresholds.size() > 5 || thresholds.size() < 0;
			if (wrongInput)
				throw std::exception("Wrong input passed to fun looking for neareast threshold");

			if (thresholds.size() == 0)
				return -1;

			for (int i = 0; i < thresholds.size(); i++) {
				if (currentValue > thresholds[i])
					return (i == 0) ? 0 : i - 1;
			}
			return -1;//not sure
		}
		catch (const std::exception& e) {
			qDebug() << e.what();
			return -1;
		}
	}
};
