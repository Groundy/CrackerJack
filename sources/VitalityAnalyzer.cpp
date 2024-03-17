#include "VitalityAnalyzer.h"

VitalityAnalyzer::VitalityAnalyzer(QObject* parent, QSharedPointer<VariablesClass> var, QSharedPointer<GameConnecter> gameConnector)
    : QThread(parent), var(var), gameConnector(gameConnector) {
  populateHealthManaMaps(var->getProf().get());
}
VitalityAnalyzer::~VitalityAnalyzer() {
  this->terminate();
}

void VitalityAnalyzer::execute() {
  ValuesDoubles percentages = getCurrentPercentage();
  if (!percentages.isValid()) {
    return;
  }
  sendDataToGui(percentages);

  auto healthMetodes = findRestorationToUse(percentages.health, healthMap);
  foreach (auto methode, healthMetodes) {
    gameConnector->useRestorationMethode(methode);
  }

  auto manahMetodes = findRestorationToUse(percentages.mana, manaMap);
  foreach (auto methode, manahMetodes) {
    int additionalManaBreak = calcTimeBetweenManaPots(percentages.mana);
    gameConnector->useRestorationMethode(methode, additionalManaBreak);
  }

  handleStates();
}

ValuesDoubles VitalityAnalyzer::toDoubles(ValuesInts currentValues) {
  double healthPercentage;
  if (currentValues.isHealthOk())
    healthPercentage = 100.0 * currentValues.health / currentValues.maxHealth;
  else
    healthPercentage = NULL;

  double manaPercentage;
  if (currentValues.isManaOk())
    manaPercentage = 100.0 * currentValues.mana / currentValues.maxMana;
  else
    manaPercentage = NULL;

  double manaShieldPercentage;
  if (currentValues.isShieldOk())
    manaShieldPercentage = 100.0 * currentValues.shield / currentValues.maxShield;
  else
    manaShieldPercentage = NULL;
  ValuesDoubles toRet(healthPercentage, manaPercentage, manaShieldPercentage);
  return toRet;
}
bool VitalityAnalyzer::populateHealthManaMaps(const Profile* profile) {
  auto         healthRestorationsMethode = profile->getRestMethodesHealth();
  QVector<int> healthThresholds;
  for each (auto var in healthRestorationsMethode) healthThresholds.push_back(var.getThreshold());
  if (healthThresholds.size() != healthRestorationsMethode.size()) {
    qWarning() << "wrong sizes of health restoration methodes passed to ManaHealthAnalyzer!";
    return false;
  }
  for (size_t i = 0; i < healthThresholds.size(); i++) {
    healthMap.insert(healthThresholds[i], healthRestorationsMethode[i]);
  }

  auto         manaRestorationsMethode = profile->getRestMethodesMana();
  QVector<int> manaThresholds;
  for each (auto var in manaRestorationsMethode) {
    manaThresholds.push_back(var.getThreshold());
  }
  if (manaThresholds.size() != manaRestorationsMethode.size()) {
    qWarning() << "wrong sizes of health restoration methodes passed to ManaHealthAnalyzer!";
    return false;
  }
  for (size_t i = 0; i < manaThresholds.size(); i++) {
    manaMap.insert(manaThresholds[i], manaRestorationsMethode[i]);
  }

  return true;
}
ValuesStrs VitalityAnalyzer::toStrsValues(FoundFlags foundFlags, ImageValues imgVals) {
  ValuesStrs strVals;

  strVals.health     = foundFlags.health ? imgVals.health.toString().remove("\0") : QString();
  strVals.mana       = foundFlags.mana ? imgVals.mana.toString().remove("\0") : QString();
  strVals.manaShield = foundFlags.shield ? imgVals.manaShield.toString().remove("\0") : QString();
  strVals.combined   = foundFlags.combined ? imgVals.combined.toString().remove("\0") : QString();

  constexpr int MIN_LENGTH_FOR_CORR_STR = 3;
  if (strVals.health.length() < MIN_LENGTH_FOR_CORR_STR) {
    qWarning() << "Error in converting ims with health bar to str form";
    return ValuesStrs();
  }
  if (strVals.mana.length() < MIN_LENGTH_FOR_CORR_STR && strVals.combined.length() < MIN_LENGTH_FOR_CORR_STR) {
    qWarning() << "Error in converting ims with mana bar to str form";
    return ValuesStrs();
  }
  return strVals;
}

ValuesInts VitalityAnalyzer::toIntsValues(FoundFlags foundFlags, ValuesStrs valueStrs) {
  ValuesInts valuesInts;
  if (foundFlags.health) {
    getValuesFromStringRegularCase(valueStrs.health, valuesInts.health, valuesInts.maxHealth);
  }
  if (foundFlags.combined) {
    getValuesFromStringOfCombinedBox(valueStrs.combined, valuesInts.mana, valuesInts.maxMana, valuesInts.shield, valuesInts.maxShield);
  }
  if (foundFlags.shield) {
    getValuesFromStringRegularCase(valueStrs.manaShield, valuesInts.shield, valuesInts.maxShield);
  }
  if (foundFlags.mana) {
    getValuesFromStringRegularCase(valueStrs.mana, valuesInts.mana, valuesInts.maxMana);
  }
  return valuesInts;
};
FoundFlags VitalityAnalyzer::getFoundFlags() {
  FoundFlags flags;
  flags.health   = !var->getVitalitty().getHealthArea().isEmpty();
  flags.mana     = !var->getVitalitty().getManaArea().isEmpty();
  flags.combined = !var->getVitalitty().getCombinedArea().isEmpty();
  flags.shield   = !var->getVitalitty().getMSArea().isEmpty();
  return flags;
}
bool VitalityAnalyzer::getValuesFromStringRegularCase(QString in, int& current, int& max) {
  const QStringList partOfStr = in.split("\\");  //wanted form of input currentVal/maxVal
  if (partOfStr.size() != 2) {
    qWarning() << "ManaHealthAnalyzer recived wrong input, input = " + in;
    return false;
  }
  const int currentVal = partOfStr[0].toInt();
  const int maxVal     = partOfStr[1].toInt();

  if (currentVal > MAX_POSSIBLE_VALUE || currentVal > maxVal || currentVal < 0 || maxVal < 0) {
    qWarning() << "Wrong int values in splittling str currentVal/maxVal";
    return false;
  }

  current = currentVal;
  max     = maxVal;
  return true;
}
bool VitalityAnalyzer::getValuesFromStringOfCombinedBox(QString in, int& currentMana, int& maxMana, int& currentManaShield,
                                                        int& maxManaShield) {
  //wanted form of input manaMinVal/maxVal(minShieldValue/maxShieldValue)
  bool inputOk = in.count("\\") == 2 && in.count("(") == 1 && in.count(")") == 1;
  if (!inputOk) {
    qWarning() << "Wrong input in splitting str with values of mana combined with magic shield, input = " + in;
    return false;
  }

  in.remove(")");
  QStringList parts = in.split("(");
  if (parts.size() != 2) {
    qWarning() << "error in splitting  str with values of mana combined with magic shield";
    return false;
  }

  QString manaStr   = parts[0];
  QString shieldStr = parts[1];
  int     currentManaTMP, maxManaTMP, currentManaShieldTMP, maxManaShieldTMP;
  bool    splittingOk1 = getValuesFromStringRegularCase(manaStr, currentManaTMP, maxManaTMP);
  bool    splittingOk2 = getValuesFromStringRegularCase(shieldStr, currentManaShieldTMP, maxManaShieldTMP);
  bool    okResult     = splittingOk1 && splittingOk2 && maxManaTMP > 0;
  if (!okResult) {
    qWarning() << "error in splitting  str with values of mana combined with magic shield";
    return false;
  }

  currentMana       = currentManaTMP;
  maxMana           = maxManaTMP;
  currentManaShield = currentManaShieldTMP;
  maxManaShield     = maxManaShieldTMP;
  return true;
}
QVector<RestorationMethode> VitalityAnalyzer::findRestorationToUse(double currentValue, const QMap<int, RestorationMethode>& methodes) {
  QVector<RestorationMethode> toRet = {};
  if (methodes.size() == 0) {
    return toRet;
  }

  if (currentValue < 0.0 || currentValue > 100.0 || methodes.size() > 5) {
    qWarning() << "Wrong input passed to fun looking for neareast threshold";
    return toRet;
  }

  auto thresholds = methodes.keys();
  for (int i = 0; i < thresholds.size(); i++) {
    if (currentValue > thresholds[i]) {
      continue;
    }
    if (toRet.size() == 2) {
      break;
    }
    auto currentMethode          = methodes[thresholds[i]];
    bool suchTypeIsAlreadyOnList = false;
    for each (auto var in toRet) {
      if (var.isType(currentMethode.getType())) {
        suchTypeIsAlreadyOnList = true;
        break;
      }
    }
    if (suchTypeIsAlreadyOnList || !restMethodeCanBeUsed(currentMethode)) {
      continue;
    }
    toRet.push_back(currentMethode);
  }
  return toRet;
}
ValuesDoubles VitalityAnalyzer::getCurrentPercentage() {
  if (!var->getSettings().getRestoringState()) return ValuesDoubles();
  ImageValues imgs = getImages();
  if (!imgs.isValid()) return ValuesDoubles();
  FoundFlags flags = getFoundFlags();
  if (!flags.isValid()) return ValuesDoubles();
  ValuesStrs strsValues = toStrsValues(flags, imgs);
  if (!strsValues.isValid()) return ValuesDoubles();
  ValuesInts intsValues = toIntsValues(flags, strsValues);
  if (!intsValues.isValid()) return ValuesDoubles();
  ValuesDoubles percentages = toDoubles(intsValues);
  if (!percentages.isValid()) return ValuesDoubles();

  writeDataToVariableClass(percentages);
  writeDataToVariableClass(intsValues);

  return percentages;
}
void VitalityAnalyzer::sendDataToGui(ValuesDoubles currentValues) {
  emit sendValueToMainThread(currentValues.health, currentValues.mana, currentValues.manaShield);
}
void VitalityAnalyzer::writeDataToVariableClass(ValuesDoubles values) {
  var->getVitalitty().setCurrentPercentage(values.health, values.mana, values.manaShield);
}
void VitalityAnalyzer::writeDataToVariableClass(ValuesInts values) {
  var->getVitalitty().setCurrentRawValues(values.health, values.mana, values.shield);
}
ImageValues VitalityAnalyzer::getImages() {
  ImageValues toRet;
  bool        clearImgs = var->getSettings().getClearVitalityImgs();

  var->getVitalitty().getImageHealth(toRet.health, clearImgs);
  var->getVitalitty().getImageMana(toRet.mana, clearImgs);
  var->getVitalitty().getImageMS(toRet.manaShield, clearImgs);
  var->getVitalitty().getImageCombined(toRet.combined, clearImgs);
  return toRet;
}
bool VitalityAnalyzer::restMethodeCanBeUsed(const RestorationMethode& restMethode) {
  const qint64 now    = QDateTime::currentMSecsSinceEpoch();
  Timers&      timers = var->getTimers();
  const auto   type   = restMethode.getType();

  if (type == RestorationMethode::Type::POTION) {
    if (now < timers.getTimeLastItemUsageGeneral() + (ONE_SEC * restMethode.getCdGroup())) {
      return false;
    }
    if (now < timers.getTimeLastItemUsage(restMethode.getName()) + (ONE_SEC * restMethode.getCd())) {
      return false;
    }
    return true;  // todo later should be added checking if char has proper pot!
  } else if (type == RestorationMethode::Type::SPELL) {
    if (var->getVitalitty().getCurrentRawManaVal() < restMethode.getMana()) {
      return false;
    }
    if (now < timers.getTimeLastSpellUsageHealing() + (ONE_SEC * restMethode.getCdGroup())) {
      return false;
    }
    if (now < timers.getTimeLastSpellUsed(restMethode.getName()) + (ONE_SEC * restMethode.getCd())) {
      return false;
    }
    return true;
  } else {
    return false;
  }
}
int VitalityAnalyzer::calcTimeBetweenManaPots(int currentManaPercentage) {
  return 1200 * currentManaPercentage / 100.0;
}
void VitalityAnalyzer::handleStates() {
  Settings& settings = var->getSettings();
  if (!settings.getKeepHasted()) {
    return;
  }
  auto states = var->getEquipment().getCurrentStates(true);
  if (states.isEmpty()) {
    return;
  }
  handleHaste(settings.getKeepHasted(), states);
  handleUpgrade(settings.getKeepUpraded(), states);
}
void VitalityAnalyzer::handleHaste(bool keepHasting, QVector<Equipment::STATES>& states) {
  if (!keepHasting) {
    return;
  }
  if (states.contains(Equipment::HASTE)) {
    return;
  }
  qint64 currentTime = now();
  bool   canHaste    = currentTime >= lastTimeHasted + (1.3 * ONE_SEC);
  if (!canHaste) {
    return;
  }
  gameConnector->sendKeyStrokeToProcess(VK_F12);  //todo
  logger.log("Hasted!", false, true, true);
  lastTimeHasted = currentTime;
}
void VitalityAnalyzer::handleUpgrade(bool keepUpgraded, QVector<Equipment::STATES>& states) {
  if (!keepUpgraded) {
    return;
  }
  if (states.contains(Equipment::UPGRADED)) {
    return;
  }
  qint64 currentTime = now();
  bool   canUpgrade  = currentTime >= lastTimeUpgraded + (20 * ONE_SEC);
  if (!canUpgrade) {
    return;
  }
  gameConnector->sendKeyStrokeToProcess(VK_F11);  //todo
  logger.log("Upgraded!", false, true, true);
  lastTimeUpgraded = currentTime;
}
qint64 VitalityAnalyzer::now() {
  return QDateTime::currentMSecsSinceEpoch();
}