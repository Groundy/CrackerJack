#include "ManaHealthStateAnalyzer.h"
#include <QChar>

ManaHealthStateAnalyzer::ManaHealthStateAnalyzer(QObject *parent, Profile* profile, VariablesClass* varClass)
	: QThread(parent){
	var = varClass;

	lifeThreshholds = profile->healthRestorePercentages;;
	manaThreshholds= profile->ManaRestoreMethodesPercentage;
	healthKeys = profile->healthKeys;
	manaKeys = profile->ManaKeys;
	namesOfHealthRestoreMethodes = profile->healthRestoreMethodeNames;
	namesOfManaRestoreMethodes = profile->manaRestoreMethodeNames;
}

ManaHealthStateAnalyzer::~ManaHealthStateAnalyzer(){
}

void ManaHealthStateAnalyzer::run(){
	mainLoop();
}

void ManaHealthStateAnalyzer::setThreadEnabilityToRun(bool stateToSet) {
	qDebug() << "ManaHealthStateAnalyzer::setThreadEnabilityToRun " << stateToSet;
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
		msleep(miliSecBetweenCheckingForNewValuesImg);
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
		getValuesFromStringRegularCase(healthValueStr, &health, &maxHealth);
	if (combinedFound)	
		getValuesFromStringOfCombinedBox(combinedValueStr, &mana, &maxMana, &manaShield, &maxManaShield);
	if (manaShieldFound)
		getValuesFromStringRegularCase(manaShieldValueStr, &manaShield, &maxManaShield);
	if (manaFound)
		getValuesFromStringRegularCase(manaValueStr, &mana, &maxMana);
}

void ManaHealthStateAnalyzer::PreapareAndSendInfoToGuiInMainThread(){
	QString healthStr,manaStr,manaShieldStr;
	makeStringsForSignalToSend(&healthStr, &manaStr, &manaShieldStr);
	emit sendValueToMainThread(healthStr, manaStr, manaShieldStr);
}

int ManaHealthStateAnalyzer::getValuesFromStringRegularCase(QString in, int* current, int* max){
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

	*current = minToRet;
	*max = maxToRet;
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
	int res = getValuesFromStringRegularCase(manaStr, &minManaToRet, &maxManaToRet);
	int res2 = getValuesFromStringRegularCase(shieldStr, &minShieldToRet, &maxShieldToRet);

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
	QString healthPercentageToRet = QString::number(value,'f',1) + "%";
	value = (mana * 100.0f) / maxMana;
	QString manaPercentageToRet = QString::number(value, 'f', 1) + "%";
	QString manaShieldPercentageToRet;
	if (maxManaShield <= 0)
		manaShieldPercentageToRet = "not found or zero";
	else {
		if (maxManaShield == 0)
			return ZERO_AS_MAX_VALUE;
		value = (manaShield * 100.0f) / maxManaShield;
		manaShieldPercentageToRet = QString::number(value, 'f', 1) + "%";
	}
	*healthOut = healthPercentageToRet;
	*manaOut = manaPercentageToRet;
	*manaShieldOut = manaShieldPercentageToRet;
	return OK;
}

int ManaHealthStateAnalyzer::findNearestThresholdIndex(int currentValue, QList<int> thresholds, int* out_index){
	if (currentValue < 0 || currentValue >100 || thresholds.size() > 5) {
		*out_index = -1;
		return ERROR_CODES::WRONG_INPUT_PARAMETERS;
	}

	int vectSize = thresholds.size();
	int indexToRet = -1;
	if (vectSize > 0) {
		for (int i = vectSize - 1; i >= 0; i--) {
			int currentThreshold = thresholds[i];
			if (currentValue < currentThreshold) {
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

bool ManaHealthStateAnalyzer::checkIfEverythingIsCorrectToProcess(){
	if (!shouldThisThreadBeActive) {
		msleep(miliSecBetweenCheckingForNewValuesImg * 5);
		//qDebug() << "ManaHealthStateAnalyzer:: mainLoop skipped, it isn't enabled";
		return false;
	}
	if (var->caliState != var-> CALIBRATED) {
		//qDebug() << "ManaHealthStateAnalyzer:: mainLoop skipped, error lack of calibration";
		emit sendValueToMainThread("Calibrating", "Calibrating", "Calibrating");
		emit demandReCalibration();
		msleep(miliSecBetweenCheckingForNewValuesImg * 5);
		return false;
	}
	bool dataReady = getInfoFromVarClass();
	if (!dataReady)
		return false;
	int res = changeImgsToStrings();
	if (res != OK) {
		//qDebug() << "ManaHealthStateAnalyzer:: mainLoop skipped, error in changing img to strings";
		emit demandReCalibration();
		emit sendValueToMainThread("Calibrating", "Calibrating", "Calibrating");
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

void ManaHealthStateAnalyzer::fillListsWithMethodesOfRestoring(QList<QObject>* listOfHealthRestoration, QList<Utilities::Item>* listOfManaRestore, QList<QString> namesOfHealthMethodes, QList<QString> namesOfManaMethodes){

}

void ManaHealthStateAnalyzer::findKeysThatShouldBePassedToKeySender(){

}

