#pragma once
#include <QThread>
#include <qrect.h>
#include <qstringlist.h>
#include <qqueue.h>
#include "Route.h"
#include "VariablesClass.hpp"
#include "GameConnecter.h"
#include "Profile.h"
#include "AttackMethode.hpp"
#include "MinimapAnalyzer.h"
#include <qsound.h>
class AutoHunting  : public QThread
{
	Q_OBJECT
public:
	AutoHunting(QObject *parent, QSharedPointer<VariablesClass> var, QSharedPointer<GameConnecter> gameConnector, Route route, Profile* profile);
	~AutoHunting();
	void run();
	QObject* getMiniMapAnalyzer();
signals:
	void updateHeadingPointInGUI(QString);
	void updateEnemiesAmountInGUI(int);
private:
	qint64 lastTimeMovedToNextNode = now();
	qint64 lastTimePressedAttack = now();
	qint64 lastTimeEqChanged = now();
	qint64 lastTimeSpecialAttackUsed = now();
	qint64 lastTimeAlarmRang = now();
	const int minPeriodBetweenAttackingMob = 1700;
	const int minPeriodBetweenMovingToNodes = 2000;
	const int breakBetweenAlarms = 3000;
	QMap <QString, qint64> attacksMethodesTimers;

	QSharedPointer<VariablesClass> var;
	QSharedPointer<GameConnecter> gameConnector;
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
	MinimapAnalyzer* miniMapAnalyzer;
	QStringList alloweNamesOnBattleList = QStringList() << "Falcon Paladin" << "PreceptorLazare" << "Monk" << "SwampTroll" ;

	QPoint getDistFromOnePtToAnother(QPoint start, QPoint end);
	QPoint addTwoPoints(QPoint start, QPoint end);
	void addNewPosition(QPoint pt);
	bool playerIsMoving();
	bool checkIfPlayerIsInDesiredNode();
	void moveToNextNode();
	bool updatePlayerCurrentPos();
	void useAttackMethode(const AttackMethode& methode);
	bool canUseMethode(const AttackMethode& methode);
	void useSpecialAttackIfNeeded();
	bool playerIsFighting();
	qint64 now();
	void keepAtackingTargetOnBattleList();
	bool playerFoundOnBattleList() {
		QStringList enemiesNamesOnBattleList = var->getBattleList().getUniqueMonstersNames();
		//bool precoreFound = enemiesNamesOnBattleList.contains("PreceptorLazare");
		bool precoreFound = enemiesNamesOnBattleList.contains("SwampTroll");
		handleEquipment(precoreFound);
		bool playerFound = false;
		for each (QString name in enemiesNamesOnBattleList) {
			bool nameIsAllowed = alloweNamesOnBattleList.contains(name);
			if (nameIsAllowed)
				continue;
			playerFound = true;
			break;
		}
		if (!playerFound)
			return playerFound;
		if (var->getSettings().playSoundWhenIntruderIsOnScreen)
			playSound();
		return playerFound;
	}
	inline void playSound() {
		qint64 currentTime = now();
		bool canAlreadyRangAlaram = currentTime >= lastTimeAlarmRang + breakBetweenAlarms;
		if (!canAlreadyRangAlaram) return;
		lastTimeAlarmRang = currentTime;

		qDebug() << var->sound->fileName();
		qDebug() << var->sound->loopsRemaining();
		QSound::play(var->sound->fileName());
		var->sound->setLoops(1);
		var->sound->play();
		msleep(1000);
	}
	void clickOnMiddleOfCompass() {
		//doing it prevents losing focus after click on minimap!
		QPoint pt = var->getMiniMap().getCompasMiddlePoint();
		if (pt.isNull())
			return;
		gameConnector->clickLeft(pt);
	}
	void handleEquipment(bool targetIsOnScreen) {
		const int sleepTime = 700;
		qint64 currentTime = now();
		if (lastTimeEqChanged + 1500 > currentTime)
			return;
		lastTimeEqChanged = currentTime;
		Equipment& eq = var->getEquipment();
		bool okHelmet = true, okWeapon = true, okArmor = true;
		bool wearingHelmet = eq.wearsEquipment(Equipment::EqRect::Helmet, okHelmet);
		bool wearingWeapon = eq.wearsEquipment(Equipment::EqRect::Weapon, okWeapon);
		bool wearingArmor = eq.wearsEquipment(Equipment::EqRect::Armor, okArmor);

		bool changeHelmet = okHelmet && ((wearingHelmet && !targetIsOnScreen) || (!wearingHelmet && targetIsOnScreen));
		if (changeHelmet) {
			gameConnector->sendKeyStrokeToProcessWithShift(Key("8"));
			msleep(sleepTime);
		}

		bool changeWeapon = okWeapon && ((wearingWeapon && !targetIsOnScreen) || (!wearingWeapon && targetIsOnScreen));
		if (wearingWeapon) {
			gameConnector->sendKeyStrokeToProcessWithShift(Key("9"));
			msleep(sleepTime);
		}

		bool changeArmor = okArmor && ((wearingArmor && !targetIsOnScreen) || (!wearingArmor && targetIsOnScreen));
		if (changeArmor) {
			gameConnector->sendKeyStrokeToProcessWithShift(Key("7"));
			msleep(sleepTime);
		}

	}
};

