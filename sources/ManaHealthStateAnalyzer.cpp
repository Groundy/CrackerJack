#include "ManaHealthStateAnalyzer.h"
#include <QChar>

ManaHealthStateAnalyzer::ManaHealthStateAnalyzer(QObject *parent, VariablesClass* varClass)
	: QThread(parent){
	var = varClass;
}

ManaHealthStateAnalyzer::~ManaHealthStateAnalyzer(){
}

void ManaHealthStateAnalyzer::run(){
	sleep(4);
	if(var->useAdvancedShearch)
		mainLoop();
	else
		SIMPLE_mainloop();
}

void ManaHealthStateAnalyzer::imgsToBlackAndWhite(){
	Utilities::imgToBlackAndWhiteAllColors(&healthImg, 240);
	Utilities::imgToBlackAndWhiteAllColors(&manaImg, 240);
	if (isManaShieldActive)
		Utilities::imgToBlackAndWhiteAllColors(&manaShieldImg,240);
}

void ManaHealthStateAnalyzer::getInfoFromVarClass(){
	bool healthIsNull = (var->var_healthPieceImg == nullptr);
	bool manaIsNull = var->var_manaPieceImg == NULL;
	bool manaShieldIsNull = var->var_manaShieldPieceImg == NULL;
	QImage emptyImg = QImage();
	if (!healthIsNull)
		healthImg = *var->var_healthPieceImg;
	else
		healthImg = emptyImg;

	if (!manaIsNull)
		manaImg = *var->var_manaPieceImg;
	else
		manaImg = emptyImg;

	if (!manaShieldIsNull)
		manaShieldImg = *var->var_manaShieldPieceImg;
	else
		manaShieldImg = emptyImg;
	rotationNeeded = var->rotationNeededForPointsAbove;
	isManaShieldActive = var->isManaShieldActive;

}

void ManaHealthStateAnalyzer::setImagesToAnalyze(){
	Utilities::imgToBlackAndWhiteAllColors(&healthImg, 240);
	Utilities::imgToBlackAndWhiteAllColors(&manaImg, 240);
	if(isManaShieldActive)
		Utilities::imgToBlackAndWhiteAllColors(&manaShieldImg, 240);
}

void ManaHealthStateAnalyzer::rotateImgsInfNeeded(){
	bool skip = !(rotationNeeded == 1 || rotationNeeded == -1);
	if (skip)
		return;
	Utilities::rotateImgToRight(&healthImg, rotationNeeded);
	Utilities::rotateImgToRight(&manaImg, rotationNeeded);
	if (isManaShieldActive)
		Utilities::rotateImgToRight(&manaShieldImg, rotationNeeded);
}

void ManaHealthStateAnalyzer::SIMPLE_mainloop() {
	while (true) {
		if (!shouldThisThreadBeActive) {
			msleep(miliSecBetweenCheckingForNewValuesImg * 5);
			qDebug() << "ManaHealthStateAnalyzer:: simple mainLoop skipped, it isn't enabled";
			continue;
		}
		QImage healthBar = var->healthBarImg;
		QImage manaBar = var->manaBarImg;

		int blueManaPixels = 0;
		int redHealthPixels = 0;
		for (int x = 0; x < healthBar.width(); x++) {
			RGBstruct helthPixel = healthBar.pixel(x, 0);
			RGBstruct manaPixel = manaBar.pixel(x, 0);
			if (helthPixel.r > 180)
				redHealthPixels++;
			if (manaPixel.b > 180)
				blueManaPixels++;
		}
		int percentage_health = (100 * redHealthPixels) / healthBar.width();
		int percentage_mana = (100 * blueManaPixels) / healthBar.width();

		emit sendValueToMainThread(QString::number(percentage_health), QString::number(percentage_mana), "Error");
		msleep(miliSecBetweenCheckingForNewValuesImg );
	}
}

void ManaHealthStateAnalyzer::mainLoop(){
	bool cont;
	while (true) {
		if (!shouldThisThreadBeActive) {
			msleep(miliSecBetweenCheckingForNewValuesImg * 5);
			qDebug() << "ManaHealthStateAnalyzer:: mainLoop skipped, it isn't enabled";
			continue;
		}
		getInfoFromVarClass();
		if (!checkIfImgWereLoadedCorrectly()) {
			msleep(miliSecBetweenCheckingForNewValuesImg);
			//emit demandReCalibration();
			qDebug() << "ManaHealthStateAnalyzer:: mainLoop skipped, error in reading imgs";
			continue;
		}
		if (!var->CALIBRATED) {
			msleep(miliSecBetweenCheckingForNewValuesImg * 5);
			qDebug() << "ManaHealthStateAnalyzer:: mainLoop skipped, error lack of calibration";
			//emit demandReCalibration();
			continue;
		}
		rotateImgsInfNeeded();
		imgsToBlackAndWhite();
		cutBorders();
		bool succes = changeImgsToStrings();
		if (!succes) {
			qDebug() << "ManaHealthStateAnalyzer:: mainLoop skipped, error in changing img to strings";
			//emit demandReCalibration();
			msleep(miliSecBetweenCheckingForNewValuesImg);
			continue;
		}
		setHealthManaValues();
		QString healthStr,manaStr,manaShieldStr;
		makeStringsForSignalToSend(&healthStr, &manaStr, &manaShieldStr);
		emit sendValueToMainThread(healthStr, manaStr, manaShieldStr);
		msleep(miliSecBetweenCheckingForNewValuesImg);
	}
}

bool ManaHealthStateAnalyzer::changeImgsToStrings(){
	QString healthStr = Utilities::imgWithStrToStr(&healthImg);
	QString manaStr = Utilities::imgWithStrToStr(&manaImg);
	QString manaShieldStr;
	if (isManaShieldActive)
		manaShieldStr = Utilities::imgWithStrToStr(&manaShieldImg);
	else
		manaShieldStr = QString();

	bool reasonToEmit = healthStr.isEmpty() || manaStr.isEmpty();
	bool reasonToEmit2 = !(healthStr.contains("\\") && manaStr.contains("\\"));
	if (reasonToEmit || reasonToEmit2){
		emit demandReCalibration();
		return false;//diag err
	}

	healthValueStr = healthStr;
	manaValueStr = manaStr;
	manaShieldValueStr = manaShieldStr;
	return true;
}

void ManaHealthStateAnalyzer::cutBorders(){
	Utilities::cutBlackBordersOfImg(&healthImg);
	Utilities::cutBlackBordersOfImg(&manaImg);
	if (isManaShieldActive)
		Utilities::cutBlackBordersOfImg(&manaShieldImg);
}

bool ManaHealthStateAnalyzer::checkIfImgWereLoadedCorrectly() {
	QImage emptyImg = QImage();
	bool good1 = healthImg != emptyImg;
	bool good2 = manaImg != emptyImg;
	bool good3 = manaShieldImg != emptyImg;
	bool loadedCorrectly;
	if (isManaShieldActive)
		loadedCorrectly = good1 && good2 && good3;
	else
		loadedCorrectly = good1 && good2;

	if (loadedCorrectly)
		return true;
	else 
		return false;
	
}

void ManaHealthStateAnalyzer::setHealthManaValues(){
	int healthValueToRet = -1;
	int manaValueToRet = -1;
	int manaShieldValueToRet = -1;

	deletePartOfStrSideOfBracket_Outside(&healthValueStr);
	deletePartOfStrSideOfBracket_Outside(&manaValueStr);

	changeStrWithValueToValue(healthValueStr, &healthValueToRet);
	changeStrWithValueToValue(manaValueStr, &manaValueToRet);

	if (isManaShieldActive) {
		deletePartOfStrOutSideOfBracket_Inside(&manaShieldValueStr);
		changeStrWithValueToValue(manaShieldValueStr, &manaShieldValueToRet);
	}

	if (healthValueToRet >= 0 && healthValueToRet <= 100)
		health = healthValueToRet;
	else
		health = -1;
	if (manaValueToRet >= 0 && manaValueToRet <= 100)
		mana = manaValueToRet;
	else
		mana = -1;
	if (manaShieldValueToRet >= 0 && manaShieldValueToRet <= 100)
		manaShield = manaShieldValueToRet;
	else
		manaShield = -1;
}

void ManaHealthStateAnalyzer::deletePartOfStrSideOfBracket_Outside(QString* strToEdit){
	QString toDel_TMp = *strToEdit;
	bool haveOpeningBracket = strToEdit->contains('(');
	bool haveClosingBracket = strToEdit->contains(')');
	int indexOfOpeningBracket = haveOpeningBracket ? strToEdit->indexOf('(') : 0;
	int indexOfClosingBracket = haveClosingBracket ? strToEdit->indexOf(')') : 0;

	if (haveOpeningBracket && haveClosingBracket) {
		int startPosToDelete = indexOfOpeningBracket;
		int widthToDelete = indexOfClosingBracket - startPosToDelete;
		QString tmp = strToEdit->remove(startPosToDelete,widthToDelete);
		*strToEdit = tmp;
		return;
	}
	else if (!haveOpeningBracket && haveClosingBracket) {
		int startPosToDelete = 0;
		int widthToDelete = indexOfClosingBracket;
		QString tmp = strToEdit->remove(startPosToDelete, widthToDelete);
		*strToEdit = tmp;
		return;
	}
	else if (haveOpeningBracket && !haveClosingBracket) {
		int startPosToDelete = indexOfOpeningBracket;
		int widthToDelete = strToEdit->size() - startPosToDelete;
		QString tmp = strToEdit->remove(startPosToDelete, widthToDelete);
		*strToEdit = tmp;
		return;
	}
	//required format after curentHealth//maxHealth
}

void ManaHealthStateAnalyzer::deletePartOfStrOutSideOfBracket_Inside(QString* strToEdit){
	bool haveOpeningBracket = strToEdit->contains('(');
	bool haveClosingBracket = strToEdit->contains(')');
	int indexOfOpeningBracket = haveOpeningBracket ? strToEdit->indexOf('(') : 0;
	int indexOfClosingBracket = haveClosingBracket ? strToEdit->indexOf(')') : 0;

	if (haveOpeningBracket && haveClosingBracket) {
		int startPosToDelete = 0;
		int widthToDelete = indexOfOpeningBracket+1;
		QString tmp = strToEdit->remove(startPosToDelete, widthToDelete);
		*strToEdit = tmp;

		indexOfClosingBracket = haveClosingBracket ? strToEdit->indexOf(')') : 0;

		startPosToDelete = indexOfClosingBracket;
		widthToDelete = strToEdit->size() - startPosToDelete;
		tmp = strToEdit->remove(startPosToDelete, widthToDelete);
		*strToEdit = tmp;

		return;
	}
	else{
		//diag err
	}
	//required format after curentHealth//maxHealth
}

void ManaHealthStateAnalyzer::changeStrWithValueToValue(QString in, int* outValue){
	QStringList partsOfString = in.split("\\");
	int PercentageToRet = -1;

	if (partsOfString.size() == 2) {

		//deleting  escape sequence char if needed
		if (partsOfString[1].right(1) == QChar(0)) {
			int size = partsOfString[1].size();
			QString tmp = partsOfString[1].left(size - 1);
			partsOfString[1] = tmp;
		}

		int currentValue = partsOfString[0].toInt();
		int maxValue = partsOfString[1].toInt();
		bool properValues =  currentValue <= maxValue;
		if (properValues && maxValue !=0)
			PercentageToRet = (100 * currentValue) / maxValue;
		else if (properValues && maxValue == 0)//manaShieldCase
			PercentageToRet = 0;
		else {
			PercentageToRet = 0;
			return; //diag err Wrong Parameter after reading values from IMG;
		}
	}
	else {
		*outValue = 0;
		return;//diag err Wrong Parameter, there are more or less than one \\ char in str with values
	}
	*outValue = PercentageToRet;
}

void ManaHealthStateAnalyzer::makeStringsForSignalToSend(QString* healthOut, QString* manaOut, QString* manaShieldOut){
	bool healthCorrect = !(health == -1);
	bool manaCorrect = !(mana == -1);
	bool manaShieldCorrect = !(manaShield == -1);
	QString err = "Error";

	if (healthCorrect)
		*healthOut = QString::number(health);
	else 
		*healthOut = err;

	if (manaCorrect)
		*manaOut = QString::number(mana);
	else
		*manaOut = err;

	if (manaShieldCorrect)
		*manaShieldOut = QString::number(manaShield);
	else
		*manaShieldOut = err;
}

