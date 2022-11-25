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
	//void sendInfoAboutPotAmountsToGUI(QStringList);

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
			if (shield > maxShield)
				return false;
			return true;
		}
		bool isHealthOk() const { return health <= maxHealth && health >= 0 && maxHealth > 0;	}
		bool isManaOk() const { return mana <= maxMana && mana >= 0 && maxMana > 0;	}
		bool isShieldOk() const { return (shield <= maxShield) && (shield > 0) && (maxShield == 0); }
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
	const int SLEEP_TIME = 50;

	ValuesDoubles toDoubles(ValuesInts currentValues);
	bool PopulateHealthManaMaps(const Profile* profile);
	ValuesInts toIntsValues(FoundFlags foundFlags, ValuesStrs valueStrs);
	ValuesStrs toStrsValues(FoundFlags foundFlags, ImageValues imgVals);
	void sendDataToGui(ValuesDoubles currentValues);
	void writeDataToVariableClass(ValuesDoubles values);
	void writeDataToVariableClass(ValuesInts values);
	ImageValues getImages();
	FoundFlags getFoundFlags();
	bool getValuesFromStringRegularCase(QString in, int& current, int& max);
	bool getValuesFromStringOfCombinedBox(QString in, int& currentMana, int& maxMana, int& currentManaShield, int& maxManaShield);
	QVector<RestorationMethode> findRestorationToUse(double currentValue, const QMap<int, RestorationMethode>& methodes);
	ValuesDoubles getCurrentPercentage();
};