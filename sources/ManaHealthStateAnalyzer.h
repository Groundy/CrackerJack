#pragma once
#include <qdatetime.h>
#include <qimage.h>

#include <QChar>
#include <QThread>

#include "GameConnecter.h"
#include "ImgEditor.h"
#include "JsonParser.h"
#include "Profile.h"
#include "VariablesClass.hpp"
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
  //void sendInfoAboutPotAmountsToGUI(QStringList);

 private:
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

  QSharedPointer<VariablesClass> var;
  QSharedPointer<GameConnecter>  gameConnector;
  ImgEditor*                     imgEditor;
  QMap<int, RestorationMethode>  healthMap, manaMap;
  const int                      SLEEP_TIME = 50;
  ///Max possible value of Health or Mana in game
  const int MAX_POSSIBLE_VALUE = 100000;
  //bool usedRestorationMethodeOnLastLoop = false;

  //qint64 lastTimeAnalyzed = now();
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
  int                         calcTimeBetweenManaPots(int currentManaPercentage) {
    return 1200 * currentManaPercentage / 100.0;
  }
  void handleStates() {
    Settings& settings = var->getSettings();
    if (!settings.getKeepHasted()) return;

    auto states = var->getEquipment().getCurrentStates(true);
    if (states.isEmpty()) return;

    handleHaste(settings.getKeepHasted(), states);
    handleUpgrade(settings.getKeepUpraded(), states);
  }
  void handleHaste(bool keepHasting, QVector<Equipment::STATES>& states) {
    if (!keepHasting) return;
    if (states.contains(Equipment::HASTE)) return;
    qint64 currentTime = now();
    bool   canHaste    = currentTime >= lastTimeHasted + 1333;
    if (!canHaste) return;
    gameConnector->sendKeyStrokeToProcess(VK_F12);  //todo
    var->log("Hasted!", false, true, true);
    lastTimeHasted = currentTime;
  }
  void handleUpgrade(bool keepUpgraded, QVector<Equipment::STATES>& states) {
    if (!keepUpgraded) return;
    if (states.contains(Equipment::UPGRADED)) return;
    qint64 currentTime = now();
    bool   canUpgrade  = currentTime >= lastTimeUpgraded + 20000;
    if (!canUpgrade) return;
    gameConnector->sendKeyStrokeToProcess(VK_F11);  //todo
    var->log("Upgraded!", false, true, true);
    lastTimeUpgraded = currentTime;
  }

  qint64 now() {
    return QDateTime::currentMSecsSinceEpoch();
  }
  /*
	bool populareMapsWithBottomBarsLetters(QMap<QString, int>& lightMap, QMap<QString, int>& darkMap) {
		try {
			QString path = PathResource::getPathToFileWithBottomsBarDigitsCodes();
			QJsonObject obj;
			bool readCorrectly = JsonParser::openJsonFile(obj, path);
			if (!readCorrectly)
				throw std::exception("Error, can't find bottomBarsDigits.json file");

			QJsonValue value = obj["darkNumbers"];
			if (value.isUndefined() || !value.isArray())
				throw std::exception("No darkNumbers field in bottomBar json file!");
			QJsonArray array = value.toArray();
			for each (auto singleValue in array) {
				QJsonObject singleObject = singleValue.toObject();
				int intVal = singleObject["name"].toString().toInt();
				QString pixValues = singleObject["value"].toString();
				darkMap.insert(pixValues, intVal);
			}


			value = obj["lightNumbers"];
			if (value.isUndefined() || !value.isArray())
				throw std::exception("No lightNumbers field in bottomBar json file!");
			array = value.toArray();
			for each (auto singleValue in array) {
				QJsonObject singleObject = singleValue.toObject();
				int intVal = singleObject["name"].toInt();
				QString pixValues = singleObject["value"].toString();
				lightMap.insert(pixValues, intVal);
			}
			bool toRet = lightMap.size() == 10 && darkMap.size() == 10;
			return toRet;
		}
		catch (const std::exception& e) {
			var->log(e.what(), true, false, false);
			return false;
		}
	};
	*/
  /*
	int ImgEditor::getNumberFromBottomBar(QImage& imgToShearchWithin){
		QMap<QString, int> lightMap, darkMap;
		Utilities::getMapWithNumbersFromBottomBar(lightMap, darkMap);
		QStringList lightCodes = lightMap.keys();
		QStringList darkCodes = darkMap.keys();

		QMap<int, int> anotherMap; // <positionX, value>

		for (size_t i = 0; i < lightCodes.size(); i++){
			QList<QImage*> listWithLightAndDarkLetterImg;
			QImage darkLetter = ImgEditor::getImageFromAdvancedCode(darkCodes[i]);
			QImage lightLetter = ImgEditor::getImageFromAdvancedCode(lightCodes[i]);
			listWithLightAndDarkLetterImg.push_back(&darkLetter);
			listWithLightAndDarkLetterImg.push_back(&lightLetter);

			int digit = lightMap[lightCodes[i]];
			QList<QPoint> listOfStartingPoints = ImgEditor::findStartPositionInImg_mulitpeImgs(listWithLightAndDarkLetterImg, imgToShearchWithin);// (listWithLightAndDarkLetterImg, imgToShearchWithin);
			for each (QPoint var in listOfStartingPoints)
				anotherMap.insert(var.x(), digit);
		}

		QString strToRe;
		for each (auto key in anotherMap.keys())
			strToRe.push_back(QString::number(anotherMap[key]));
    
		return strToRe.toInt();
}
	*/
  /*
	void getAmountsOfPotions() {
		QMap<QString,QRect> map_copy = var->potionName_rectPosOnScreen_map;
		QList<int> amountOfPots;
		QStringList namesOfPots;
		QImage wholeImg;
		var->getCopyOfCurrentFullImg(wholeImg);
		for each (QString nameOfPot in map_copy.keys()) {
			QRect rect = map_copy[nameOfPot];
			if (rect.isEmpty())
				continue;
			QImage img = wholeImg.copy(rect);
			int amount = ImgEditor::getNumberFromBottomBar(img);
			amountOfPots.push_back(amount);
			namesOfPots.push_back(nameOfPot);
		}

		QStringList infoToSendToMainThread;
		for (size_t i = 0; i < namesOfPots.size(); i++){
			QString toAdd = namesOfPots[i] + ": " + QString::number(amountOfPots[i]);
			infoToSendToMainThread.push_back(toAdd);
		}
		sendInfoAboutPotAmountsToGUI(infoToSendToMainThread);
}
	*/
};
