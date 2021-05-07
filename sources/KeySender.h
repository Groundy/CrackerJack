#pragma once
#include <qthread.h>
#include "Profile.h"
#include "Key.h"
#include "qdatetime.h"
#include "qkeysequence.h"
#include "Utilities.h"
#include "VariablesClass.h"
class KeySender : public QThread
{
	Q_OBJECT

public:
	enum ERROR_CODE {
		OK = 0,
		UNDEFINED_ERROR = 2,
		NOT_THRESHOLDS_ON_THE_LIST = 4,
		WRONG_PARAMETERS = 8,
	};
	KeySender(QObject *parent, Profile* profile, VariablesClass* variableClass);
	~KeySender();
	void run();
private:
	VariablesClass* var;
	long long lastTime_HealingSpell = 0;
	long long lastTime_Item = 0;

	void mainLoop();
	/*
	void getInfoFromProfile(Profile* profile);
	bool getInfoFromVarClass(float* health, float* mana, float* manashield);
	int getMiliSecondsNeededToWaitToUseHealingSpell();
	void sendHealingKeyStrokeToGame(float currentHealthPercentage);
	*/
};
