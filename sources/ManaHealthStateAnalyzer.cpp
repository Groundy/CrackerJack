#include "ManaHealthStateAnalyzer.h"
#include <QChar>

ManaHealthStateAnalyzer::ManaHealthStateAnalyzer(QObject *parent, Profile* profile, VariablesClass* varClass)
	: QThread(parent){
	var = varClass;

	lifeThreshholds = profile->healthRestorePercentages;;
	manaThreshholds= profile->ManaRestoreMethodesPercentage;
	healthKeys = profile->healthKeys;
	manaKeys = profile->ManaKeys;
	QStringList restorationMethodeList_Health = profile->healthRestoreMethodeNames;
	QStringList restorationMethodeList_Mana = profile->manaRestoreMethodeNames;
	setupRestorationMethodes(restorationMethodeList_Health, restorationMethodeList_Mana);
	/*
	for each (auto var in restorationMethodeList_Health)
		lastTimeUsedHealthMethode.push_back(0);
	lastTimeUsed_Potion = 0;
	lastTimeUsed_Spell = 0;
	*/
}

ManaHealthStateAnalyzer::~ManaHealthStateAnalyzer(){
	this->terminate();
}

void ManaHealthStateAnalyzer::run(){
	mainLoop();
}

void ManaHealthStateAnalyzer::setThreadEnabilityToRun(bool stateToSet) {
	shouldThisThreadBeActive = stateToSet;
}

bool ManaHealthStateAnalyzer::getInfoFromVarClass() {
	if (var->newData) {
		var->newData = false;
		healthImg = var->var_healthPieceImg;
		manaImg = var->var_manaPieceImg;
		manaShieldImg = var->var_manaShieldPieceImg;
		combinedImg = var->var_combinedBoxPieceImg;
		healthFound = var->healthFound;
		manaFound = var->manaFound;
		manaShieldFound = var->manaShieldFound;
		combinedFound = var->combinedFound;
		return true;
	}
	else
		return false;
}

void ManaHealthStateAnalyzer::mainLoop(){
	while (true) {
		bool everythingIsOk = checkIfEverythingIsCorrectToProcess();
		if (!everythingIsOk)
			continue;
		getValuesFromStringsToGlobablVariables();
		PreapareAndSendInfoToGuiInMainThread();
		writeDataToVariableClass();
		sleepAppropirateTimeToNextAnalyze();
		getAmountsOfPotions();
		Key healthKey, manaKey;
		bool sucess = getKeyThatShouldBeSendToKeySenderClass(healthKey, manaKey) == OK;
		bool thereIsHealthKey = (healthKey.number != -1) && sucess;
		bool thereIsManaKey = (manaKey.number != -1) && sucess;
		if (thereIsHealthKey)
			//Utilities::sendKeyStrokeToProcess(healthKey, var->var_pidOfGame, var->var_winTitleOfGame);
		if (thereIsManaKey) {
			Sleep(extraTimeToWaitBetweenManaPotUse);
			//Utilities::sendKeyStrokeToProcess(manaKey, var->var_pidOfGame, var->var_winTitleOfGame);
		}
	}
}

int ManaHealthStateAnalyzer::changeImgsToStrings(){
	QString healthStr, manaStr, manaShieldStr, combinedStr, tmp;
	if (healthFound) {
		healthStr = Utilities::imgWithStrToStr(&healthImg);
		tmp = healthStr.remove("\0");
		healthStr = tmp;
	}
	if (manaFound) {
		manaStr = Utilities::imgWithStrToStr(&manaImg);
		tmp = manaStr.remove("\0");
		manaStr = tmp;
	}
	if (manaShieldFound) {
		manaShieldStr = Utilities::imgWithStrToStr(&manaShieldImg);
		tmp = manaShieldStr.remove("\0");
		manaShieldStr = tmp;
	}
	if (combinedFound) {
		combinedStr = Utilities::imgWithStrToStr(&combinedImg);
		tmp = combinedStr.remove("\0");
		combinedStr = tmp;
	}
	bool isHealthCorrect = healthStr.size() >= 3;
	bool isManaCorrect = manaStr.size() >= 3 || combinedStr.size() >= 3;
	bool minimumRequiredMade = isHealthCorrect && isManaCorrect;
	if (minimumRequiredMade) {
		healthValueStr = healthStr;
		manaValueStr = manaStr;
		manaShieldValueStr = manaShieldStr;
		combinedValueStr = combinedStr;
		return OK;
	}
	else
		return WRONG_STR_OF_VALUES;

}

void ManaHealthStateAnalyzer::getValuesFromStringsToGlobablVariables(){
	if (healthFound)
		getValuesFromStringRegularCase(healthValueStr, health, maxHealth);
	if (combinedFound)	
		getValuesFromStringOfCombinedBox(combinedValueStr, &mana, &maxMana, &manaShield, &maxManaShield);
	if (manaShieldFound)
		getValuesFromStringRegularCase(manaShieldValueStr, manaShield, maxManaShield);
	if (manaFound)
		getValuesFromStringRegularCase(manaValueStr, mana, maxMana);
}

void ManaHealthStateAnalyzer::PreapareAndSendInfoToGuiInMainThread(){
	QString healthStr,manaStr,manaShieldStr;
	makeStringsForSignalToSend(&healthStr, &manaStr, &manaShieldStr);
	emit sendValueToMainThread(healthStr, manaStr, manaShieldStr);
}

int ManaHealthStateAnalyzer::getValuesFromStringRegularCase(QString in, int& current, int& max){
	//wanted form of input minVal/maxVal
	QStringList partOfStr = in.split("\\");
	if (partOfStr.size() != 2)
		return ERROR_CODES::WRONG_STR_OF_VALUES;
	int minToRet = partOfStr[0].toInt();
	int maxToRet = partOfStr[1].toInt();
	
	bool error1 = maxToRet < 0 || maxToRet > 90000;//mana of 3000 lvl mage, no probability that there would be bigger value needed
	bool error2 = minToRet < 0 || minToRet > maxToRet;
	if( error1 || error2)
		return ERROR_CODES::WRONG_STR_OF_VALUES;

	current = minToRet;
	max = maxToRet;
	return OK;
}

int ManaHealthStateAnalyzer::getValuesFromStringOfCombinedBox(QString in, int* currentMana, int* maxMana, int* currentManaShield, int* maxManaShield){
	//wanted form of input minVal/maxVal(minShieldValue/maxShieldValue)
	in.remove(")");
	QStringList foo = in.split("(");
	if (foo.size() != 2)
		return ERROR_CODES::WRONG_STR_OF_VALUES;
	QString manaStr = foo[0];
	QString shieldStr = foo[1];

	int minManaToRet, maxManaToRet, minShieldToRet, maxShieldToRet;
	int res = getValuesFromStringRegularCase(manaStr, minManaToRet, maxManaToRet);
	int res2 = getValuesFromStringRegularCase(shieldStr, minShieldToRet, maxShieldToRet);

	if (res == OK) {
		*currentMana = minManaToRet;
		*maxMana = maxManaToRet;
	}
	else
		return ERROR_CODES::WRONG_STR_OF_VALUES;

	if (res2 == OK) {
		*currentManaShield = minShieldToRet;
		*maxManaShield = maxShieldToRet;
	}
	else
		return ERROR_CODES::WRONG_STR_OF_VALUES;

	return OK;
}

int ManaHealthStateAnalyzer::makeStringsForSignalToSend(QString* healthOut, QString* manaOut, QString* manaShieldOut){
	if (maxHealth == 0 || maxMana == 0)
		return ZERO_AS_MAX_VALUE;
	float value = (health * 100.0f) / maxHealth;
	healthPercentage = value;
	QString healthPercentageToRet = QString::number(value,'f',1) + "%";
	value = (mana * 100.0f) / maxMana;
	manaPercentage = value;
	QString manaPercentageToRet = QString::number(value, 'f', 1) + "%";
	QString manaShieldPercentageToRet;
	if (maxManaShield <= 0)
		manaShieldPercentageToRet = "not found or zero";
	else {
		if (maxManaShield == 0)
			return ZERO_AS_MAX_VALUE;
		value = (manaShield * 100.0f) / maxManaShield;
		manaShieldPercentage = value;
		manaShieldPercentageToRet = QString::number(value, 'f', 1) + "%";
	}
	*healthOut = healthPercentageToRet;
	*manaOut = manaPercentageToRet;
	*manaShieldOut = manaShieldPercentageToRet;
	return OK;
}

int ManaHealthStateAnalyzer::findNearestThresholdIndex(int currentValue, QList<int> thresholds, int& out_index){
	int listSize = thresholds.size();
	bool wrongInput = currentValue < 0 || currentValue > 100 || listSize > 5 || listSize < 0;
	if (wrongInput) {
		out_index = -1;
		return ERROR_CODES::WRONG_INPUT_PARAMETERS;
	}
	if (listSize == 0) {
		out_index = -1;
		return OK;
	}

	int indexToRet = -1;
	thresholds.push_back(0);
	for (int i = 0; i < thresholds.size(); i++) {
		if (currentValue > thresholds[i]) {
			indexToRet = i - 1;
			break;
		}
	}
	out_index = indexToRet;
	return OK;
}

bool ManaHealthStateAnalyzer::checkIfEverythingIsCorrectToProcess(){
	if (!shouldThisThreadBeActive) {
		msleep(miliSecBetweenCheckingForNewValuesImg * 5);
		return false;
	}

	bool skip = !var->HealthAndManaRestorationShouldBeActive;
	if(skip){
		msleep(miliSecBetweenCheckingForNewValuesImg * 5);
		return false;
	}
	bool dataReady = getInfoFromVarClass();
	if (!dataReady)
		return false;
	int res = changeImgsToStrings();
	if (res != OK) {
		emit demandReCalibration();
		QString wordToSend = StringResource::languageIsPl() ? QString::fromLocal8Bit("Kalibracja") : "Calibrating";
		emit sendValueToMainThread(wordToSend, wordToSend, wordToSend);
		msleep(miliSecBetweenCheckingForNewValuesImg*5);
		return false;
	}
	return true;
}

void ManaHealthStateAnalyzer::writeDataToVariableClass(){
	float healthInfoToSend = (100.0f * health)/maxHealth;
	float manaInfoToSend = (100.0f * mana) / maxMana;
	float manaShieldInfoToSend;
	if (maxManaShield != 0)
		manaShieldInfoToSend = (100.0f * manaShield) / maxManaShield;
	else
	manaShieldInfoToSend = 0;
	var->health = healthInfoToSend;
	var->mana = manaInfoToSend;
	var->manashield = manaShieldInfoToSend;
	var->newData = true;
}

void ManaHealthStateAnalyzer::setupRestorationMethodes(QStringList listOfRestorationMethode_Health, QStringList listOfRestorationMethode_Mana){
	JsonParser parser;
	QList<Utilities::RestoreMethode> spellsAndPotions;
	QList<Utilities::Item> manaPotions;

	parser.getHealthRestoreMethodes(listOfRestorationMethode_Health, spellsAndPotions);
	parser.getManaRestoreMethodes(listOfRestorationMethode_Mana, manaPotions);

	healthMethodes = spellsAndPotions;
	manaMethodes = manaPotions;
}

int ManaHealthStateAnalyzer::getKeyThatShouldBeSendToKeySenderClass(Key& healthKey, Key& manaKey){
	int indexOfHealth;
	auto res1 = findNearestThresholdIndex(healthPercentage, lifeThreshholds, indexOfHealth);
	if (res1!= OK)
		return res1;

	int indexOfMana;
	auto res2 = findNearestThresholdIndex(manaPercentage, manaThreshholds, indexOfMana);
	if (res2 != OK)
		return res2;


	bool potionSlotIsUsed = false;
	bool spellSlotIsUsed = false;
	Key potionKey = -1, spellKey = -1;

	int i = indexOfHealth;
	LONG64 currentTime = Utilities::getCurrentTimeInMiliSeconds();
	while (i >= 0) {
		Utilities::RestoreMethode::TypeOfMethode type = healthMethodes[i].type;

		if (type == Utilities::RestoreMethode::TypeOfMethode::POTION && !potionSlotIsUsed) {
			LONG64 nextTimeThisItemWillBeAvaible = ((LONG64)healthMethodes[i].cd * 1000) + var->lastTimeMethodeUsed_Healing[i];
			LONG64 nextTimeAnyItemWillBeAvaible = var->lastTimeUsed_item + 1000;
			bool useThis = max(nextTimeThisItemWillBeAvaible, nextTimeAnyItemWillBeAvaible) <= currentTime;
			if (useThis){
				potionKey = healthKeys[i];
				potionSlotIsUsed = true;
				var->lastTimeUsed_item = currentTime;
				var->lastTimeMethodeUsed_Healing[i] = currentTime;
			}
		}
		else if (type == Utilities::RestoreMethode::TypeOfMethode::SPELL && !spellSlotIsUsed) {
			LONG64 lastTimeUsed = var->lastTimeMethodeUsed_Healing[i];
			LONG64 minTimeCd = ((LONG64)healthMethodes[i].cd * 1000) + lastTimeUsed;
			LONG64 minTimeCdGroup = ((LONG64)healthMethodes[i].cdGroup * 1000) + lastTimeUsed;
			LONG64 nextTimeAnySpellWillBeAvaible = var->lastTimeUsed_spell_healing + 1000;
			LONG64 nextTimeThisSpellWillBeAvaible = max(max(minTimeCd, minTimeCdGroup), nextTimeAnySpellWillBeAvaible);
			bool canUseNow =  nextTimeThisSpellWillBeAvaible <= currentTime;
			bool haveManaForIt = mana >= healthMethodes[i].mana;
			if (canUseNow && haveManaForIt) {
				spellKey = healthKeys[i];
				spellSlotIsUsed = true;
				var->lastTimeUsed_spell_healing = currentTime;
				var->lastTimeMethodeUsed_Healing[i] = currentTime;
			}
		}
		i--;
	}

	i = potionSlotIsUsed ? -1 : indexOfMana;
	while (i >= 0){
		LONG64 nextTimeThisItemWillBeAvaible = var->lastTimeUsed_item + 1000;
		bool isReady = nextTimeThisItemWillBeAvaible <= currentTime;
		if (isReady) {
			potionKey = manaKeys[i];
			potionSlotIsUsed = true;
			var->lastTimeUsed_item = currentTime;
		}
		i--;
	}

	manaKey = potionKey;
	healthKey = spellKey;
	return OK;
}

void ManaHealthStateAnalyzer::sleepAppropirateTimeToNextAnalyze(){
	LONG64 currentTime = Utilities::getCurrentTimeInMiliSeconds();
	LONG64 time = 1015;
	LONG64 timeToSleep = time - (currentTime - lastTimeAnalyzed);
	if (timeToSleep > time)
		timeToSleep = time;
	if (timeToSleep < 0)
		timeToSleep = 0;
	lastTimeAnalyzed = currentTime + timeToSleep;
	Sleep(timeToSleep);
}

void ManaHealthStateAnalyzer::getAmountsOfPotions() {
	QMap<QString,QRect> map_copy = var->potionName_rectPosOnScreen_map;
	QList<int> amountOfPots;
	QStringList namesOfPots;
	QImage wholeImg = var->wholeImg;
	for each (QString nameOfPot in map_copy.keys()) {
		QRect rect = map_copy[nameOfPot];
		if (rect.isEmpty())
			continue;
		QImage img = wholeImg.copy(rect);
		int amount = Utilities::getNumberFromBottomBar(img);
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
