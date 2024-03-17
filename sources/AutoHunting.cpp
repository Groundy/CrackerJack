#include "AutoHunting.h"

AutoHunting::AutoHunting(QObject* parent, QSharedPointer<VariablesClass> var, QSharedPointer<GameConnecter> gameConnector, Route route)
    : QThread(parent), var(var), route(route), gameConnector(gameConnector) {
  auto data            = var->getProf()->getAutoHuntData();
  attackMethodes       = data.getAttacks();
  minEnemiesToStop     = data.getMinMonToStop();
  minEnemiesToContinue = data.getMinMonToContinue();
}

AutoHunting::~AutoHunting() = default;

void AutoHunting::run() {
  while (true) {
    msleep(SLEEP_TIME);
    if (!var->getSettings().getKeepHuntingAutomaticly() || !var->getSettings().getKeepAnalyzeMiniMap()) {
      msleep(SLEEP_TIME * 30);
      continue;
    }
    playerFoundOnBattleList();
    if (playerIsFighting()) continue;
    if (!updatePlayerCurrentPos()) continue;
    bool isInNextNode = checkIfPlayerIsInDesiredNode();
    if (isInNextNode) {
      bool endOfRoute  = lastAchivedPoint + 2 == route.size();
      lastAchivedPoint = endOfRoute ? -1 : lastAchivedPoint + 1;
    }
    if (playerIsMoving()) continue;
    moveToNextNode();
    // clickOnMiddleOfCompass();
  }
}
QPoint AutoHunting::getDistFromOnePtToAnother(QPoint start, QPoint end) {
  return QPoint(end.x() - start.x(), end.y() - start.y());
}
QPoint AutoHunting::addTwoPoints(QPoint start, QPoint end) {
  return QPoint(end.x() + start.x(), end.y() + start.y());
}
void AutoHunting::addNewPosition(QPoint pt) {
  if (lastPositions.size() < 3)
    lastPositions.push_back(pt);
  else {
    lastPositions.push_back(pt);
    lastPositions.pop_front();
  }
}
bool AutoHunting::playerIsMoving() {
  if (lastPositions.size() < 2) return false;

  bool playerIsMoving = false;
  for (int i = 1; i < lastPositions.size(); i++) {
    if (lastPositions[i] != lastPositions[0]) {
      playerIsMoving = true;
      break;
    }
  }
  return playerIsMoving;
}
bool AutoHunting::checkIfPlayerIsInDesiredNode() {
  return route.checkIfPositionIsOnListOnIndex(currentPos, lastAchivedPoint + 1);
}
void AutoHunting::moveToNextNode() {
  qint64 nowTime = now();
  if (nowTime <= lastTimeMovedToNextNode + minPeriodBetweenMovingToNodes) return;
  lastTimeMovedToNextNode                = nowTime;
  QPoint  nextNodePosOnWholeMap          = route.getPoint(lastAchivedPoint + 1).getPosition().getXY();
  QPoint  fromPlayerToTargetOnWholeMap   = getDistFromOnePtToAnother(currentPos.getXY(), nextNodePosOnWholeMap);
  QPoint  miniMapFrameStartOnWholeScreen = var->getMiniMap().getFrameMiniMap().topLeft();
  QPoint  playerPosOnWholeScreen         = addTwoPoints(miniMapFrameStartOnWholeScreen, QPoint(53, 54));
  QPoint  whereToClick                   = addTwoPoints(playerPosOnWholeScreen, fromPlayerToTargetOnWholeMap);
  QString msgToDisplayToUser             = QString("Zmierzam do : %1").arg(QString::number(lastAchivedPoint + 1));
  QString msgToConsole =
      QString("%1, x=%2, y=%3 ")
          .arg(msgToDisplayToUser, QString::number(fromPlayerToTargetOnWholeMap.x()), QString::number(fromPlayerToTargetOnWholeMap.y()));
  logger.log(msgToConsole, false, true, true);
  emit updateHeadingPointInGUI(msgToDisplayToUser);
  gameConnector->clickLeft(whereToClick);
}
bool AutoHunting::updatePlayerCurrentPos() {
  var->getPosition().clear();
  int     triesLeft = 50;
  Point3D currentPosTmp;
  do {
    msleep(SLEEP_TIME);
    currentPosTmp = var->getPosition().getPlayerPos();
    triesLeft--;
  } while (!currentPosTmp.isValid() && triesLeft > 0);
  if (!currentPosTmp.isValid()) return false;
  currentPos = currentPosTmp;
  addNewPosition(currentPos.getXY());
  return true;
}
void AutoHunting::useAttackMethode(const AttackMethode& methode) {
  qint64 nowTime = now();
  attacksMethodesTimers.insert(methode.getName(), nowTime);
  lastTimeSpecialAttackUsed = nowTime;
  // var->log("Used " + methode.getName(), true, true, true);
  msleep(50);
  gameConnector->sendKeyStrokeToProcess(methode.getKey());
}
bool AutoHunting::canUseMethode(const AttackMethode& methode) {
  qint64    nowTime        = now();
  const int ADD_TIME       = 333;
  qint64    minGeneralTime = (methode.getCdGroup() * 1000) + lastTimeSpecialAttackUsed + ADD_TIME;
  if (nowTime < minGeneralTime) return false;

  qint64 lastTimeThisMethodeUsed = attacksMethodesTimers.value(methode.getName(), 0);
  qint64 minSpecificTime         = (methode.getCd() * 1000) + lastTimeThisMethodeUsed + ADD_TIME;
  if (nowTime < minSpecificTime) return false;

  return true;
}
void AutoHunting::useSpecialAttackIfNeeded() {
  for each (auto methode in attackMethodes) {
    if (!canUseMethode(methode)) continue;
    useAttackMethode(methode);
  }
}
bool AutoHunting::playerIsFighting() {
  int  enemiesOnScreen = var->getBattleList().getEnemisAmout();
  emit updateEnemiesAmountInGUI(enemiesOnScreen);
  bool isFighting = enemiesOnScreen >= minEnemiesToStop;
  if (isFighting) {
    if (!atLastLoopPlayerWasFighting) {  // stop and fight
      gameConnector->sendKeyStrokeToProcess(VK_ESCAPE, 1);
      msleep(20);
      lastPositions.empty();
      atLastLoopPlayerWasFighting = true;
      QString msgToDisplayToUser  = "W stanie walki";
      emit    updateHeadingPointInGUI(msgToDisplayToUser);
    }
    keepAtackingTargetOnBattleList();
    useSpecialAttackIfNeeded();
  } else {  // start moving
    if (atLastLoopPlayerWasFighting) {
      atLastLoopPlayerWasFighting = false;
      gameConnector->autoLootAroundPlayer();
    }
  }
  return isFighting;
}
qint64 AutoHunting::now() {
  return QDateTime::currentMSecsSinceEpoch();
}
void AutoHunting::keepAtackingTargetOnBattleList() {
  qint64 nowTime = now();
  if (var->getBattleList().firstEnemieIsInRedFrame()) {
    return;
  }
  if (nowTime < minPeriodBetweenAttackingMob + lastTimePressedAttack) {
    return;
  }
  gameConnector->sendKeyStrokeToProcess(VK_ESCAPE);
  msleep(50);
  gameConnector->sendKeyStrokeToProcess(var->getProf()->getAutoAttackKey());
  lastTimePressedAttack = nowTime;
}
bool AutoHunting::playerFoundOnBattleList() {
  QStringList enemiesNamesOnBattleList = var->getBattleList().getUniqueMonstersNames();
  bool        playerFound              = false;
  for each (QString name in enemiesNamesOnBattleList) {
    bool nameIsAllowed = alloweNamesOnBattleList.contains(name);
    if (nameIsAllowed) {
      continue;
    }
    playerFound = true;
    break;
  }
  return playerFound;
}

void AutoHunting::clickOnMiddleOfCompass() {
  // doing it prevents losing focus after click on minimap!
  QPoint pt = var->getMiniMap().getCompasMiddlePoint();
  if (pt.isNull()) {
    return;
  }
  gameConnector->clickLeft(pt);
}
