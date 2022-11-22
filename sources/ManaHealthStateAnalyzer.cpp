#include "ManaHealthStateAnalyzer.h"
#include <QChar>
#include <ImgEditor.h>

ManaHealthStateAnalyzer::ManaHealthStateAnalyzer(QObject *parent, Profile* profile, std::shared_ptr<VariablesClass> var)
	: QThread(parent), var(var){
	populateHealthManaMaps(profile);

	//setupRestorationMethodes(restorationMethodeList_Health, restorationMethodeList_Mana);
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

void ManaHealthStateAnalyzer::getInfoFromVarClass() {
	healthImg = var->var_healthPieceImg;
	manaImg = var->var_manaPieceImg;
	manaShieldImg = var->var_manaShieldPieceImg;
	combinedImg = var->var_combinedBoxPieceImg;
	healthFound = var->healthFound;
	manaFound = var->manaFound;
	manaShieldFound = var->manaShieldFound;
	combinedFound = var->combinedFound;
}

void ManaHealthStateAnalyzer::mainLoop(){
	/*
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
		bool thereIsHealthKey = (healthKey.getKeyVal() != -1) && sucess;
		bool thereIsManaKey = (manaKey.getKeyVal() != -1) && sucess;
		if (thereIsHealthKey)
			//Utilities::sendKeyStrokeToProcess(healthKey, var->var_pidOfGame, var->var_winTitleOfGame);
		if (thereIsManaKey) {
			Sleep(extraTimeToWaitBetweenManaPotUse);
			//Utilities::sendKeyStrokeToProcess(manaKey, var->var_pidOfGame, var->var_winTitleOfGame);
		}
	}
	*/
}

bool ManaHealthStateAnalyzer::changeImgsToStrings(){
	try{
		QString healthStr = QString();
		if (healthFound)
			healthStr = ImgEditor::imgWithStrToStr(healthImg).remove("\0");

		QString manaStr = QString();
		if (manaFound) 
			manaStr = ImgEditor::imgWithStrToStr(manaImg).remove("\0");

		QString manaShieldStr = QString();
		if (manaShieldFound) 
			manaShieldStr = ImgEditor::imgWithStrToStr(manaShieldImg).remove("\0");

		QString combinedStr = QString();
		if (combinedFound)
			combinedStr = ImgEditor::imgWithStrToStr(combinedImg).remove("\0");

		const int MIN_LENGTH_FOR_CORR_STR = 3;
		const bool healthCorrect = healthStr.size() >= MIN_LENGTH_FOR_CORR_STR;
		const bool manaCorrect = manaStr.size() >= MIN_LENGTH_FOR_CORR_STR || combinedStr.size() >= MIN_LENGTH_FOR_CORR_STR;
		if (!healthCorrect || !manaCorrect)
			throw std::exception("Error in converting ims with health/mana bars to str form");
	

		healthValueStr = healthStr;
		manaValueStr = manaStr;
		manaShieldValueStr = manaShieldStr;
		combinedValueStr = combinedStr;
		return true;
	}
	catch (const std::exception& e){
		qDebug() << e.what();
		return false;
	}
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
	bool healthHasProperValues = health <= maxHealth && health >= 0 && maxHealth > 0;
	double healthPerToSend = healthHasProperValues ? ((100.0 * health) / maxHealth) : NULL;

	bool manaHasProperValues = mana <= maxMana && mana >= 0 && maxMana > 0;
	double manaPerToSend = manaHasProperValues ? ((100.0 * mana) / maxMana) : NULL;

	bool manaShieldHasProperValue = manaShield <= maxManaShield && manaShield >= 0 && maxManaShield > 0;
	double manaShieldPerToSend = manaShieldHasProperValue ? ((100.0 * manaShield) / maxManaShield) : NULL;

	emit sendValueToMainThread(healthPerToSend, manaPerToSend, manaShieldPerToSend);
}

int ManaHealthStateAnalyzer::getValuesFromStringRegularCase(QString in, int& current, int& max){
	try{
		const QStringList partOfStr = in.split("\\");//wanted form of input currentVal/maxVal
		if (partOfStr.size() != 2)
			throw std::exception(QString("ManaHealthAnalyzer recived wrong input, input = " + in).toStdString().c_str());
		const int currentVal = partOfStr[0].toInt();
		const int maxVal = partOfStr[1].toInt();
		const int MAX_POSSIBLE_VALUE = 100000;
		const bool wrongValues = currentVal > MAX_POSSIBLE_VALUE || currentVal > maxVal || currentVal < 0 || maxVal < 0;
		if (wrongValues)
			throw std::exception("Wrong int values in splittling str currentVal/maxVal");
		current = currentVal;
		max = maxVal;
		return true;
	}
	catch (const std::exception& e){
		qDebug() << e.what();
		return false;
	}
}

int ManaHealthStateAnalyzer::getValuesFromStringOfCombinedBox(QString in, int* currentMana, int* maxMana, int* currentManaShield, int* maxManaShield){
	//wanted form of input manaMinVal/maxVal(minShieldValue/maxShieldValue)
	try{
		bool inputOk = in.count("/") == 3 && in.count("(") == 1 && in.count(")") == 1;
		if (!inputOk) {
			QString msg = "Wrong input in splitting str with values of mana combined with magic shield, input = " + in;
			throw std::exception(msg.toStdString().c_str());
		}

		in.remove(")");
		QStringList parts = in.split("(");
		if (parts.size() != 2)
			throw std::exception("error in splitting  str with values of mana combined with magic shield");

		QString manaStr = parts[0];
		QString shieldStr = parts[1];
		int currentManaTMP, maxManaTMP, currentManaShieldTMP, maxManaShieldTMP;
		bool splittingOk1 = getValuesFromStringRegularCase(manaStr, currentManaTMP, maxManaTMP);
		bool splittingOk2 = getValuesFromStringRegularCase(shieldStr, currentManaShieldTMP, maxManaShieldTMP);
		bool okResult = splittingOk1 && splittingOk2 && maxManaTMP > 0;
		if (!okResult)
			throw std::exception("error in splitting  str with values of mana combined with magic shield");

		*currentMana = currentManaTMP;
		*maxMana = maxManaTMP;
		*currentManaShield = currentManaShieldTMP;
		*maxManaShield = maxManaShieldTMP;
		return true;
	}
	catch (const std::exception& e){
		qDebug() << e.what();
		return false;
	}
}

int ManaHealthStateAnalyzer::findNearestThresholdIndex(int currentValue, const QVector<int>& thresholds){
	try{
		bool wrongInput = currentValue < 0 || currentValue > 100 || thresholds.size() > 5 || thresholds.size() < 0;
		if (wrongInput)
			throw std::exception("Wrong input passed to fun looking for neareast threshold");

		if (thresholds.size() == 0)
			return -1;

		for (int i = 0; i < thresholds.size(); i++) {
			if (currentValue > thresholds[i])
				return (i == 0) ? 0 : i - 1;
		}
		return -1;//not sure
	}
	catch (const std::exception& e){
		qDebug() << e.what();
		return -1;
	}
}

bool ManaHealthStateAnalyzer::checkIfEverythingIsCorrectToProcess(){
	/*
	if (!shouldThisThreadBeActive) {
		msleep(miliSecBetweenCheckingForNewValuesImg * 5);
		return false;
	}

	bool skip = !var->HealthAndManaRestorationShouldBeActive;
	if(skip){
		msleep(miliSecBetweenCheckingForNewValuesImg * 5);
		return false;
	}
	getInfoFromVarClass();
	int res = changeImgsToStrings();
	if (res != OK) {
		emit demandReCalibration();
		emit sendValueToMainThread(NULL, NULL, NULL);
		msleep(miliSecBetweenCheckingForNewValuesImg*5);
		return false;
	}	*/
	return true;

}

void ManaHealthStateAnalyzer::writeDataToVariableClass(){
	double currentHealthPercentage = (100.0f * health)/maxHealth;
	double currentManaPercentage = (100.0f * mana) / maxMana;
	double currentMSPercentage = (maxManaShield != 0) ? (100.0f * manaShield) / maxManaShield : 0;
	var->setCurrentHealthPercentage(currentHealthPercentage);
	var->setCurrentManaPercentage(currentManaPercentage);
	var->setCurrentMSPercentage(currentMSPercentage);
}

void ManaHealthStateAnalyzer::setupRestorationMethodes(QStringList listOfRestorationMethode_Health, QStringList listOfRestorationMethode_Mana){
	/*
	JsonParser parser;
	QList<Utilities::RestoreMethode> spellsAndPotions;
	QList<Item> manaPotions;

	parser.getHealthRestoreMethodes(listOfRestorationMethode_Health, spellsAndPotions);
	parser.getManaRestoreMethodes(listOfRestorationMethode_Mana, manaPotions);

	healthMethodes = spellsAndPotions;
	manaMethodes = manaPotions;
	*/
}

bool ManaHealthStateAnalyzer::getKeysForConnectorClass(Key& healthKey, Key& manaKey) {
	/*
		int indexOfHealthThreshold = findNearestThresholdIndex(healthPercentage, healthThresholds);
	int indexOfManaThreshold = findNearestThresholdIndex(manaPercentage, manaThresholds);

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
				//potionKey = healthKeys[i];//todo
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
				//spellKey = healthKeys[i];//todo
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
			//potionKey = manaKeys[i];//todo
			potionSlotIsUsed = true;
			var->lastTimeUsed_item = currentTime;
		}
		i--;
	}

	manaKey = potionKey;
	healthKey = spellKey;
	return OK;
	*/
	return true;
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
	QImage wholeImg;
	var->getCopyOfCurrentFullImg(wholeImg);
	for each (QString nameOfPot in map_copy.keys()) {
		QRect rect = map_copy[nameOfPot];
		if (rect.isEmpty())
			continue;
		QImage img = wholeImg.copy(rect);
		int amount = ImgEditor::getNumberFromBottomBar(img);
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