#pragma once
#include <QThread>
#include <qrect.h>

#include <qqueue.h>
#include "Route.h"
#include "VariablesClass.h"
#include "GameConnecter.h"
#include "AttackMethode.h"
class AutoHunting  : public QThread
{
	Q_OBJECT
public:
	AutoHunting(QObject *parent, std::shared_ptr<VariablesClass> var, std::shared_ptr<GameConnecter> gameConnector, Route route, QVector<AttackMethode> attackMethodes);
	~AutoHunting();
	void run() {
		while (true){
			msleep(SLEEP_TIME);
			if (!var->getSettings().getKeepHuntingAutomaticly() || !var->getSettings().getKeepAnalyzeMiniMap()) {
				msleep(SLEEP_TIME * 30);
				continue;
			}

			if (!updatePlayerCurrentPos())
				continue;

			if (playerIsFighting())
				continue;

			int currentIndex = getIndexOfCurrentPlayerNode();
			bool playerInOneOfNodes = currentIndex != - 1;
			bool playerIsOnSameNodeAgain = currentIndex == lastPosition && playerInOneOfNodes;

			if (playerInOneOfNodes && !playerIsOnSameNodeAgain) {
				lastPosition = currentIndex;
				QString msg = QString("Osiagnieto punkt trasy nr %1").arg(QString::number(currentIndex));
				var->log(msg, false, true, false);
				moveToNextNode();
			}
			else if(!playerIsMoving())
				moveToNextNode();
		}
	}
signals:
	void updateHeadingPointInGUI(QString);
	void updateEnemiesAmountInGUI(int);
private:
	std::shared_ptr<VariablesClass> var;
	std::shared_ptr<GameConnecter> gameConnector;
	Point3D currentPos;
	int lastPosition = -1;
	qint64 lastTimePressedAttack = now();
	Route route;
	const int SLEEP_TIME = 20;
	const Key autoAttackKey = Key("F2");
	QQueue<QPoint> lastPositions;
	bool atLastLoopPlayerWasFighting = false;
	int minPeriodBetweenAttackingMob = 1600;
	QVector<AttackMethode> attackMethodes = {};
	QMap <QString, qint64> attacksMethodesTimers;
	qint64 lastTimeSpecialAttackUsed = now();
	int lastAnalyzeEnemiesNumber = 0;
	const int MIN_ENEMIES_TO_STOP = 2;

	QPoint getDistFromOnePtToAnother(QPoint start, QPoint end) {
		return QPoint(end.x() - start.x(), end.y() - start.y());
	}
	QPoint addTwoPoints(QPoint start, QPoint end) {
		return QPoint(end.x() + start.x(), end.y() + start.y());
	}
	void addNewPosition(QPoint pt) {
		if (lastPositions.size() < 4)
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
	int getIndexOfCurrentPlayerNode() {
		return route.getIndexOfPoint(currentPos);
	}
	void moveToNextNode() {
		QPoint nextNodePosOnWholeMap = route.getPoint(lastPosition + 1).getPosition().getXY();
		QPoint fromPlayerToTargetOnWholeMap = getDistFromOnePtToAnother(currentPos.getXY(), nextNodePosOnWholeMap);
		QPoint miniMapFrameStartOnWholeScreen = var->getMiniMap().getFrameMiniMap().topLeft();
		QPoint playerPosOnWholeScreen = addTwoPoints(miniMapFrameStartOnWholeScreen, QPoint(53,54)); 
		QPoint whereToClick = addTwoPoints(playerPosOnWholeScreen, fromPlayerToTargetOnWholeMap);
		QString msgToDisplayToUser = QString("Zmierzam do : %1").arg(QString::number(lastPosition + 1));
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
		gameConnector->sendKeyStrokeToProcess(methode.getKey());
	}
	bool canUseMethode(const AttackMethode& methode) {
		qint64 nowTime = now();
		qint64 minGeneralTime = (methode.getCdGroup() * 1000) + lastTimeSpecialAttackUsed;
		if (nowTime < minGeneralTime)
			return false;

		qint64 lastTimeThisMethodeUsed = attacksMethodesTimers.value(methode.getName(), 0);
		qint64 minSpecificTime = (methode.getCd() * 1000) + lastTimeThisMethodeUsed;
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
		bool isFighting = enemiesOnScreen >= MIN_ENEMIES_TO_STOP;
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
		gameConnector->sendKeyStrokeToProcess(autoAttackKey);
		lastTimePressedAttack = nowTime;
	}
};
