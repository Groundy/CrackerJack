#pragma once
#include <QThread>
#include <qrect.h>

#include <qqueue.h>
#include "Route.h"
#include "VariablesClass.h"
#include "GameConnecter.h"
class AutoHunting  : public QThread
{
	Q_OBJECT
public:
	AutoHunting(QObject *parent, std::shared_ptr<VariablesClass> var, std::shared_ptr<GameConnecter> gameConnector, Route route);
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
			int enemiesOnScreen = var->getBattleList().getEnemisAmout();
			emit updateEnemiesAmountInGUI(enemiesOnScreen);
			bool isFighting = enemiesOnScreen > 0;
			if (isFighting) {
				if (!atLastLoopPlayerWasFighting) {
					gameConnector->sendKeyStrokeToProcess(VK_ESCAPE);
					lastPositions.empty();
					atLastLoopPlayerWasFighting = true;
					QString msgToDisplayToUser = "W stanie walki";
					emit updateHeadingPointInGUI(msgToDisplayToUser);
				}
				if (now() >= minPeriodBetweenAttackingMob + lastTimePressedAttack) {
					if (!var->getBattleList().firstEnemieIsInRedFrame()) {
						gameConnector->sendKeyStrokeToProcess(attackKey);
						lastTimePressedAttack = now();
					}
				}
				continue;
			}
			else {
				if (atLastLoopPlayerWasFighting) {
					atLastLoopPlayerWasFighting = false;
					gameConnector->autoLootAroundPlayer();
				}
			}

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
	const Key attackKey = Key("F2");
	QQueue<QPoint> lastPositions;
	bool atLastLoopPlayerWasFighting = false;
	int minPeriodBetweenAttackingMob = 1500;

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
	qint64 now() {
		return QDateTime::currentMSecsSinceEpoch();
	}
};
