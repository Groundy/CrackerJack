#pragma once
#include <QThread>
#include <qrect.h>

#include <qqueue.h>
#include "Route.h"
#include "VariablesClass.h"
#include "GameConnecter.h"
#include "Profile.h"
#include "AttackMethode.h"
class AutoHunting  : public QThread
{
	Q_OBJECT
public:
	AutoHunting(QObject *parent, std::shared_ptr<VariablesClass> var, std::shared_ptr<GameConnecter> gameConnector, Route route, Profile* profile);
	~AutoHunting();
	void run() {
		while (true){
			msleep(SLEEP_TIME);
			if (!var->getSettings().getKeepHuntingAutomaticly() || !var->getSettings().getKeepAnalyzeMiniMap()) {
				msleep(SLEEP_TIME * 30);
				continue;
			}
			if (playerIsFighting())
				continue;
			if (!updatePlayerCurrentPos())
				continue;
			bool isInNextNode = checkIfPlayerIsInDesiredNode();
			if (isInNextNode) {
				bool endOfRoute = lastAchivedPoint + 2 == route.size();
				lastAchivedPoint = endOfRoute ? -1 : lastAchivedPoint + 1;
			}
			if (playerIsMoving())
				continue;
			moveToNextNode();
		}
	}
signals:
	void updateHeadingPointInGUI(QString);
	void updateEnemiesAmountInGUI(int);
private:
	std::shared_ptr<VariablesClass> var;
	std::shared_ptr<GameConnecter> gameConnector;
	Profile* profile;
	Point3D currentPos;
	int lastAchivedPoint = -1;
	Route route;
	const int SLEEP_TIME = 20;
	QQueue<QPoint> lastPositions;
	bool atLastLoopPlayerWasFighting = false;
	QVector<AttackMethode> attackMethodes = {};
	int lastAnalyzeEnemiesNumber = 0;
	int minEnemiesToStop = 2;
	int minEnemiesToContinue = 0;

	qint64 lastTimeMovedToNextNode = now();
	qint64 lastTimePressedAttack = now();
	QMap <QString, qint64> attacksMethodesTimers;
	int minPeriodBetweenAttackingMob = 1700;
	int minPeriodBetweenMovingToNodes = 2000;
	qint64 lastTimeSpecialAttackUsed = now();

	QPoint getDistFromOnePtToAnother(QPoint start, QPoint end) {
		return QPoint(end.x() - start.x(), end.y() - start.y());
	}
	QPoint addTwoPoints(QPoint start, QPoint end) {
		return QPoint(end.x() + start.x(), end.y() + start.y());
	}
	void addNewPosition(QPoint pt) {
		if (lastPositions.size() < 3)
			lastPositions.push_back(pt);
		else {
			lastPositions.push_back(pt);
			lastPositions.pop_front();
		}
	}
	bool playerIsMoving() {
		if (lastPositions.size() < 2)
			return false;

		bool playerIsMoving = false;
		for (int i = 1; i < lastPositions.size(); i++){
			if (lastPositions[i] != lastPositions[0]) {
				playerIsMoving = true;
				break;
			}
		}
		return playerIsMoving;
	}
	bool checkIfPlayerIsInDesiredNode() {
		return route.checkIfPositionIsOnListOnIndex(currentPos, lastAchivedPoint + 1);
	}
	void moveToNextNode() {
		qint64 nowTime = now();
		if (nowTime <= lastTimeMovedToNextNode + minPeriodBetweenMovingToNodes)
			return;
		lastTimeMovedToNextNode = nowTime;
		QPoint nextNodePosOnWholeMap = route.getPoint(lastAchivedPoint + 1).getPosition().getXY();
		QPoint fromPlayerToTargetOnWholeMap = getDistFromOnePtToAnother(currentPos.getXY(), nextNodePosOnWholeMap);
		QPoint miniMapFrameStartOnWholeScreen = var->getMiniMap().getFrameMiniMap().topLeft();
		QPoint playerPosOnWholeScreen = addTwoPoints(miniMapFrameStartOnWholeScreen, QPoint(53,54)); 
		QPoint whereToClick = addTwoPoints(playerPosOnWholeScreen, fromPlayerToTargetOnWholeMap);
		QString msgToDisplayToUser = QString("Zmierzam do : %1").arg(QString::number(lastAchivedPoint + 1));
		QString msgToConsole = QString("%1, x=%2, y=%3 ").arg(msgToDisplayToUser, QString::number(fromPlayerToTargetOnWholeMap.x()), QString::number(fromPlayerToTargetOnWholeMap.y()));
		var->log(msgToConsole, true, true, true);
		emit updateHeadingPointInGUI(msgToDisplayToUser);
		gameConnector->clickLeft(whereToClick);
	}
	bool updatePlayerCurrentPos() {
		var->getPosition().clear();
		int triesLeft = 50;
		Point3D currentPosTmp;
		do {
			msleep(SLEEP_TIME);
			currentPosTmp = var->getPosition().getPlayerPos();
			triesLeft--;
		} while (!currentPosTmp.isValid() && triesLeft > 0);
		if (!currentPosTmp.isValid())
			return false;
		currentPos = currentPosTmp;
		addNewPosition(currentPos.getXY());
		return true;
	}
	void useAttackMethode(const AttackMethode& methode) {
		qint64 nowTime = now();
		attacksMethodesTimers.insert(methode.getName(), nowTime);
		lastTimeSpecialAttackUsed = nowTime;
		//var->log("Used " + methode.getName(), true, true, true);
		msleep(50);
		gameConnector->sendKeyStrokeToProcess(methode.getKey());
	}
	bool canUseMethode(const AttackMethode& methode) {
		qint64 nowTime = now();
		const int ADD_TIME = 333;
		qint64 minGeneralTime = (methode.getCdGroup() * 1000) + lastTimeSpecialAttackUsed + ADD_TIME;
		if (nowTime < minGeneralTime)
			return false;

		qint64 lastTimeThisMethodeUsed = attacksMethodesTimers.value(methode.getName(), 0);
		qint64 minSpecificTime = (methode.getCd() * 1000) + lastTimeThisMethodeUsed + ADD_TIME;
		if (nowTime < minSpecificTime)
			return false;

		return true;
	}
	void useSpecialAttackIfNeeded() {
		for each (auto methode in attackMethodes){
			if (!canUseMethode(methode))
				continue;
			useAttackMethode(methode);
		}
	}
	bool playerIsFighting() {
		int enemiesOnScreen = var->getBattleList().getEnemisAmout();
		emit updateEnemiesAmountInGUI(enemiesOnScreen);
		bool isFighting = enemiesOnScreen >= minEnemiesToStop;
		if (isFighting) {
			if (!atLastLoopPlayerWasFighting) {//stop and fight
				gameConnector->sendKeyStrokeToProcess(VK_ESCAPE,1);
				msleep(20);
				lastPositions.empty();
				atLastLoopPlayerWasFighting = true;
				QString msgToDisplayToUser = "W stanie walki";
				emit updateHeadingPointInGUI(msgToDisplayToUser);
			}
			keepAtackingTargetOnBattleList();
			useSpecialAttackIfNeeded();
		}
		else {//start moving
			if (atLastLoopPlayerWasFighting) {
				atLastLoopPlayerWasFighting = false;
				gameConnector->autoLootAroundPlayer();
			}
		}
		return isFighting;
	}
	qint64 now() {
		return QDateTime::currentMSecsSinceEpoch();
	}
	void keepAtackingTargetOnBattleList() {
		qint64 nowTime = now();
		if (var->getBattleList().firstEnemieIsInRedFrame())
			return;
		if (nowTime < minPeriodBetweenAttackingMob + lastTimePressedAttack)
			return;
		gameConnector->sendKeyStrokeToProcess(VK_ESCAPE);
		msleep(50);
		gameConnector->sendKeyStrokeToProcess(profile->getAutoAttackKey());
		lastTimePressedAttack = nowTime;
	}
};
