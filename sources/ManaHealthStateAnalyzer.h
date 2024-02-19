#pragma once
#include <QChar.h>
#include <QThread.h>
#include <qdatetime.h>
#include <qimage.h>

#include "GameConnecter.h"
#include "ImgEditor.h"
#include "JsonParser.h"
#include "Profile.h"
#include "VariablesClass.hpp"
struct FoundFlags {
  bool health, mana, combined, shield;
  bool isValid() {
    if (!health) return false;
    if (!mana && !combined) return false;
    return true;
  }
};
struct ImageValues {
  QImage health, mana, manaShield, combined;
  bool   isValid() {
    if (health.isNull()) return false;
    if (mana.isNull() && combined.isNull()) return false;
    return true;
  }
};
struct ValuesStrs {
  QString health, mana, manaShield, combined;
  bool    isValid() {
    if (health.isEmpty()) return false;
    if (mana.isEmpty() && combined.isEmpty()) return false;
    return true;
  }
};
struct ValuesInts {
  int  health = 0, maxHealth = 0;
  int  mana = 0, maxMana = 0;
  int  shield = 0, maxShield = 0;
  bool isValid() {
    if (maxHealth <= 0) return false;
    if (maxMana <= 0) return false;
    if (health > maxHealth) return false;
    if (mana > maxMana) return false;
    if (shield > maxShield) return false;
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
  bool isValid() {
    bool isValid = health >= 0.0 && health <= 100.0 && mana >= 0.0 && mana <= 100.0 && manaShield >= 0.0 && manaShield <= 100.0;
    return isValid;
  }
  double health, mana, manaShield;
};

class ManaHealthStateAnalyzer : public QThread {
  Q_OBJECT
 public:
  ManaHealthStateAnalyzer(QObject* parent, Profile* profile, QSharedPointer<VariablesClass> var,
                          QSharedPointer<GameConnecter> gameConnector);
  ~ManaHealthStateAnalyzer();

  void run();

 signals:
  void demandReCalibration();
  void sendValueToMainThread(double, double, double);

 private:
  QSharedPointer<VariablesClass> var;
  QSharedPointer<GameConnecter>  gameConnector;
  ImgEditor*                     imgEditor;
  QMap<int, RestorationMethode>  healthMap, manaMap;
  const int                      SLEEP_TIME = 50;

  const int MAX_POSSIBLE_VALUE = 100000;

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

  //bool usedRestorationMethodeOnLastLoop = false;
  ///Max possible value of Health or Mana in game
  //qint64 lastTimeAnalyzed = now();
  //
  //void sendInfoAboutPotAmountsToGUI(QStringList);  //SIGNAL
  //bool populareMapsWithBottomBarsLetters(QMap<QString, int>& lightMap, QMap<QString, int>& darkMap);
  //int getNumberFromBottomBar(QImage& imgToShearchWithin);
  //void getAmountsOfPotions();
};
