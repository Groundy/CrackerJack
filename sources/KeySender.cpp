#include "KeySender.h"

KeySender::KeySender(QObject *parent, Profile* profile, VariablesClass* variableClass)
	: QThread(parent){
	getInfoFromProfile(profile);
	var = variableClass;
}

KeySender::~KeySender(){
}

void KeySender::run(){
	mainLoop();
}

void KeySender::mainLoop(){
	while(true) {
		Sleep(50);
		float health, mana, manashield;
		bool newData = getInfoFromVarClass(&health, &mana, &manashield);
		if (!newData)
			continue;

		sendHealingKeyStrokeToGame(health);

		
		int indexOfManaThreshold;
		findNearestThresholdIndex(mana, manaThreshHolds, &indexOfManaThreshold);
	}
}

unsigned long long KeySender::getCurrentTimeInMiliSeconds(){
	QDateTime date = QDateTime::currentDateTime();
	qint64 mseconds = date.currentMSecsSinceEpoch();
	return mseconds;
}

int KeySender::findNearestThresholdIndex(int value, QList<int> thresholds, int* out_index){
	if (value < 0 || value >100 || thresholds.size() > 5) {
		*out_index = -1;
		return WRONG_PARAMETERS;
	}

	int vectSize = thresholds.size();
	int indexToRet = -1;
	if (vectSize > 0) {
		for (int i = vectSize - 1; i >= 0; i--) {
			int currentThreshold = thresholds[i];
			if (value < currentThreshold){
				indexToRet = i;
				break;
			}
		}
	}
	else 
		return NOT_THRESHOLDS_ON_THE_LIST;
	*out_index = indexToRet;
	return OK;
}

bool KeySender::getInfoFromVarClass(float* health, float* mana, float* manashield) {
	bool newDataAvaible = var->newData;
	if (newDataAvaible) {
		*health = var->health;
		*mana = var->mana;
		*manashield = var->manashield;
		var->newData = false;
	}
	return newDataAvaible;
}

void KeySender::getInfoFromProfile(Profile* profile){
	lifeThreshHolds = profile->healthRestorePercentages;
	manaThreshHolds = profile->ManaRestoreMethodesPercentage;
	healthKeys = profile->healthKeys;
	manaKeys = profile->ManaKeys;
	healthItemTypes = profile->healthItems;
	manaItemTypes = profile->manaItems;
}

int KeySender::getMiliSecondsNeededToWaitToUseHealingSpell(){
	const int COOLDOWN_TIME_IN_MILI_SEC = 1000;
	int timeBetweenSpells = getCurrentTimeInMiliSeconds() - lastTime_HealingSpell;
	int valueToReturn;
	if (timeBetweenSpells > 1100)
		valueToReturn = 0;
	else
		valueToReturn = COOLDOWN_TIME_IN_MILI_SEC - timeBetweenSpells;
	if (valueToReturn < 0 || valueToReturn > 5000)
		return 2000; //diag err
	else
		return valueToReturn;
}

void KeySender::sendHealingKeyStrokeToGame(float currentHealthPercentage){
	int indexOfHealthThreshold;
	findNearestThresholdIndex(currentHealthPercentage, lifeThreshHolds, &indexOfHealthThreshold);
	if (indexOfHealthThreshold != -1) {
		Key key = healthKeys[indexOfHealthThreshold];
		Profile::KEY_ITEM typeOfAction = healthItemTypes[indexOfHealthThreshold];
		int timeToWait;
		if (typeOfAction == Profile::KEY_ITEM::SPELL) 
			timeToWait = getMiliSecondsNeededToWaitToUseHealingSpell();
		Sleep(timeToWait);
		unsigned int pidOfGame = var->var_pidOfGame;
		QString titleOfGameWindow = var->var_winTitleOfGame;
		Utilities::sendKeyStrokeToProcess(key, pidOfGame, titleOfGameWindow);
		lastTime_HealingSpell = getCurrentTimeInMiliSeconds();
	}
}


