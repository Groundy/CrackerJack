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
			if (!var->getSettingKeepHuntingAutomaticly() || !var->getSettingKeepAnalyzeMiniMap()) {
				msleep(SLEEP_TIME * 30);
				continue;
			}

			var->getPlayerPos(true);//clearPos
			int triesLeft = 50;
			Point3D currentPosTmp;
			do{
				msleep(SLEEP_TIME);
				currentPosTmp = var->getPlayerPos(false);
				triesLeft--;
			} while (!currentPosTmp.isValid() && triesLeft > 0);
			if(!currentPosTmp.isValid())
				continue;

			currentPos = currentPosTmp;
			addNewPosition(currentPos.getXY());

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

private:
	std::shared_ptr<VariablesClass> var;
	std::shared_ptr<GameConnecter> gameConnector;
	Point3D currentPos;
	int lastPosition = -1;
	Route route;
	const int SLEEP_TIME = 20;
	QQueue<QPoint> lastPositions;

	QPoint getDistFromOnePtToAnother(QPoint start, QPoint end) {
		return QPoint(end.x() - start.x(), end.y() - start.y());
	}
	QPoint addTwoPoints(QPoint start, QPoint end) {
		return QPoint(end.x() + start.x(), end.y() + start.y());
	}
	void addNewPosition(QPoint pt) {
		if (lastPositions.size() < 10)
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
		QPoint miniMapFrameStartOnWholeScreen = var->getFrameMiniMap().topLeft();
		QPoint playerPosOnWholeScreen = addTwoPoints(miniMapFrameStartOnWholeScreen, QPoint(53,54)); 
		QPoint whereToClick = addTwoPoints(playerPosOnWholeScreen, fromPlayerToTargetOnWholeMap);
		gameConnector->clickLeft(whereToClick);
	}
};
