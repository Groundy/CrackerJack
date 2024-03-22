#pragma once
#include <QChar.h>
#include <QThread.h>
#include <qdatetime.h>
#include <qimage.h>

#include "GameConnecter.h"
#include "JsonParser.h"
#include "Profile.h"
#include "VariablesClass.hpp"
struct FoundFlags {
  bool health, mana, combined, shield;
  bool isValid() {
    if (!health) {
      return false;
    }
    if (!mana && !combined) {
      return false;
    }
    return true;
  }
};
struct ImageValues {
  CJ_Image health, mana, manaShield, combined;
  bool     isValid() const {
    if (health.isNull()) {
      return false;
    }
    if (mana.isNull() && combined.isNull()) {
      return false;
    }
    return true;
  }
};
struct ValuesStrs {
  QString health, mana, manaShield, combined;
  bool    isValid() {
    if (health.isEmpty()) {
      return false;
    }
    if (mana.isEmpty() && combined.isEmpty()) {
      return false;
    }
    return true;
  }
};
struct ValuesInts {
  int  health = 0, maxHealth = 0;
  int  mana = 0, maxMana = 0;
  int  shield = 0, maxShield = 0;
  bool isValid() const {
    if (maxHealth <= 0 || maxMana <= 0 || health > maxHealth || mana > maxMana || shield > maxShield) {
      return false;
    }
    return true;
  }
  bool isHealthOk() const {
    return health <= maxHealth && health >= 0 && maxHealth > 0;
  }
  bool isManaOk() const {
    return mana <= maxMana && mana >= 0 && maxMana > 0;
  }
  bool isShieldOk() const {
    return (shield <= maxShield) && (shield > 0) && (maxShield == 0);
  }
};
struct ValuesDoubles {
  ValuesDoubles(double health, double mana, double manaShield) : health(health), mana(mana), manaShield(manaShield) {}
  ValuesDoubles() : health(-1), mana(-1), manaShield(-1) {}
  bool isValid() const {
    bool isValid = health >= 0.0 && health <= 100.0 && mana >= 0.0 && mana <= 100.0 && manaShield >= 0.0 && manaShield <= 100.0;
    return isValid;
  }
  double health, mana, manaShield;
};

class VitalityAnalyzer : public QThread {
  Q_OBJECT
 public:
  VitalityAnalyzer(QObject* parent, QSharedPointer<VariablesClass> var, QSharedPointer<GameConnecter> gameConnector);
  ~VitalityAnalyzer();

  void run(){};

 signals:
  void demandReCalibration();
  void sendValueToMainThread(double, double, double);

 public slots:
  void execute();

 private:
  QSharedPointer<Vitallity> vitality_;
  QSharedPointer<Settings>  settings_;
  QSharedPointer<Timers>    timers_;
  QSharedPointer<Equipment> equipment_;

  QSharedPointer<GameConnecter> gameConnector;
  QMap<int, RestorationMethode> healthMap, manaMap;

  const int  MAX_POSSIBLE_VALUE = 100000;
  const uint ONE_SEC            = 1000;

  qint64 lastTimeHasted   = now();
  qint64 lastTimeUpgraded = now();

  ValuesDoubles toDoubles(ValuesInts currentValues);
  bool          populateHealthManaMaps(const Profile* profile);
  ValuesInts    toIntsValues(FoundFlags foundFlags, ValuesStrs valueStrs);
  ValuesStrs    toStrsValues(FoundFlags foundFlags, ImageValues imgVals);
  void          sendDataToGui(ValuesDoubles currentValues);
  void          writeDataToVariableClass(ValuesDoubles values);
  void          writeDataToVariableClass(ValuesInts values);
  ImageValues   getImages();
  FoundFlags    getFoundFlags();
  bool          getValuesFromStringRegularCase(QString in, int& current, int& max);
  bool          getValuesFromStringOfCombinedBox(QString in, int& currentMana, int& maxMana, int& currentManaShield, int& maxManaShield);
  QVector<RestorationMethode> findRestorationToUse(double currentValue, const QMap<int, RestorationMethode>& methodes);
  ValuesDoubles               getCurrentPercentage();
  bool                        restMethodeCanBeUsed(const RestorationMethode& restMethode);
  int                         calcTimeBetweenManaPots(int currentManaPercentage);
  void                        handleStates();
  void                        handleHaste(bool keepHasting, QVector<Equipment::STATES>& states);
  void                        handleUpgrade(bool keepUpgraded, QVector<Equipment::STATES>& states);
  static qint64               now();
  Logger&                     logger = Logger::instance();
};
