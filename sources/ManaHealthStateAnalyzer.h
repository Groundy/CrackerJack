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
		bool isValid() {
			if (!health)
				return false;
			if (!mana && !combined)
				return false;
			return true;
		}
	};
	struct ImageValues {
		QImage health, mana, manaShield, combined;
		bool isValid() {
			if (health.isNull())
				return false;
			if (mana.isNull() && combined.isNull())
				return false;
			return true;
		}
	};
	struct ValuesStrs {
		QString health, mana, manaShield, combined;
		bool isValid() {
			if (health.isEmpty())
				return false;
			if (mana.isEmpty() && combined.isEmpty())
				return false;
			return true;
		}
	};
	struct ValuesInts {
		int health, maxHealth;
		int mana, maxMana;
		int shield, maxShield;
		bool isValid() {
			if (maxHealth <= 0)
				return false;
			if (maxMana <= 0)
				return false;
			if (health > maxHealth)
				return false;
			if (mana > maxMana)
				return false;
			return true;
		}
	};
	struct ValuesDoubles {
		ValuesDoubles(double health, double mana, double manaShield) :
			health(health), mana(mana), manaShield(manaShield) {}
		ValuesDoubles() :
			health(-1), mana(-1), manaShield(-1) {}
		bool isValid() {
			bool isValid = health >= 0.0 && health <= 100.0 && mana >= 0.0 && mana <= 100.0 && manaShield >= 0.0 && manaShield <= 100.0;
			return isValid;
		}
		double health, mana, manaShield;
	};


	std::shared_ptr<VariablesClass> var;
	QMap<int, RestorationMethode> healthMap, manaMap;	
	//int extraTimeToWaitBetweenManaPotUse = 1400;
	const int SLEEP_TIME = 50;

	ValuesDoubles toDoubles(ValuesInts currentValues) {
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
		ValuesDoubles toRet(healthPercentage, manaPercentage, manaShieldPercentage);
		return toRet;
	}
	bool PopulateHealthManaMaps(const Profile* profile) {
		try{
			auto healthRestorationsMethode = profile->getRestMethodesHealth();
			QVector<int> healthThresholds;
			for each (auto var in healthRestorationsMethode)
				healthThresholds.push_back(var.getThreshold());
			if (healthThresholds.size() != healthRestorationsMethode.size())
				throw std::exception("wrong sizes of health restoration methodes passed to ManaHealthAnalyzer!");
			for (size_t i = 0; i < healthThresholds.size(); i++)
				healthMap.insert(healthThresholds[i], healthRestorationsMethode[i]);
			
			auto manaRestorationsMethode = profile->getRestMethodesMana();
			QVector<int> manaThresholds;
			for each (auto var in manaRestorationsMethode)
				manaThresholds.push_back(var.getThreshold());
			if (manaThresholds.size() != manaRestorationsMethode.size())
				throw std::exception("wrong sizes of health restoration methodes passed to ManaHealthAnalyzer!");
			for (size_t i = 0; i < manaThresholds.size(); i++)
				manaMap.insert(manaThresholds[i], manaRestorationsMethode[i]);

			return true;
		}
		catch (const std::exception& e ){
			qDebug() << e.what();
			return false;
		}
	}
	ValuesInts toIntsValues(FoundFlags foundFlags, ValuesStrs valueStrs) {
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
	ValuesStrs toStrsValues(FoundFlags foundFlags, ImageValues imgVals) {
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
			if (strVals.mana.length() < MIN_LENGTH_FOR_CORR_STR && strVals.combined.length() < MIN_LENGTH_FOR_CORR_STR)
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
		var->setCurrentPercentage(values.health, values.mana, values.manaShield);
	}
	void writeDataToVariableClass(ValuesInts values) {
		var->setCurrentRawValues(values.health, values.mana ,values.shield);
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
		flags.health = !var->getHealthArea().isEmpty();
		flags.mana = !var->getManaArea().isEmpty();
		flags.combined = !var->getCombinedArea().isEmpty();
		flags.shield = !var->getMSArea().isEmpty();
		return flags;
	}
	bool getValuesFromStringRegularCase(QString in, int& current, int& max){
	try{
		const QStringList partOfStr = in.split("\\");//wanted form of input currentVal/maxVal
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
			bool inputOk = in.count("\\") == 2 && in.count("(") == 1 && in.count(")") == 1;
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
	QVector<RestorationMethode> findRestorationToUse(double currentValue, const QMap<int, RestorationMethode>& methodes) {
		QVector<RestorationMethode> toRet = {};
		if (methodes.size() == 0)
			return toRet;

		try {
			bool wrongInput = currentValue < 0.0 || currentValue > 100.0 || methodes.size() > 5;
			if (wrongInput)
				throw std::exception("Wrong input passed to fun looking for neareast threshold");

			auto thresholds = methodes.keys();
			for (int i = 0; i < thresholds.size(); i++) {
				if (currentValue > thresholds[i])
					continue;

				auto currentMethode = methodes[thresholds[i]];
				if (toRet.isEmpty())
					toRet.push_back(currentMethode);
				else{
					auto alreadyAssignedType = toRet[0].getType();
					if(currentMethode.getType() != alreadyAssignedType)
						toRet.push_back(currentMethode);
				}

				if (toRet.size() == 2)
					break;
			}
  			return toRet;
		}
		catch (const std::exception& e) {
			qDebug() << e.what();
			return toRet;
		}
	}
	ValuesDoubles getCurrentPercentage() {
		if (!var->checkRestoringState())
			return ValuesDoubles();
		ImageValues imgs = getImages();
		if (!imgs.isValid())
			return ValuesDoubles();
		FoundFlags flags = getFoundFlags();
		if (!flags.isValid())
			return ValuesDoubles();
		ValuesStrs strsValues = toStrsValues(flags, imgs);
		if (!strsValues.isValid())
			return ValuesDoubles();
		ValuesInts intsValues = toIntsValues(flags, strsValues);
		if (!intsValues.isValid())
			return ValuesDoubles();
		ValuesDoubles percentages = toDoubles(intsValues);
		if (!percentages.isValid())
			return ValuesDoubles();

		writeDataToVariableClass(percentages);
		writeDataToVariableClass(intsValues);

		return percentages;
	}
};
