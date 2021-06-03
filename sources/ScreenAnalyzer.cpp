#include "ScreenAnalyzer.h"

ScreenAnalyzer::ScreenAnalyzer(QObject *parent, VariablesClass *varClass, Profile* profToSet)
	: QThread(parent){
	var = varClass;
	profile = profToSet;
	setUpScreenFolder();
	/*
	[CHANGE]
	QStringList listOfPotionNamesToLookFor;
	QStringList nameOfAllMethodesFromProf;
	nameOfAllMethodesFromProf.append(prof->healthRestoreMethodeNames);
	nameOfAllMethodesFromProf.append(prof->manaRestoreMethodeNames);
	for each (QString var in nameOfAllMethodesFromProf){
		bool isPotionName = var.contains(QString(" Potion"));
		if(isPotionName)
			listOfPotionNamesToLookFor.push_back(var);
	}
	*/
}

ScreenAnalyzer::~ScreenAnalyzer(){
	this->terminate();
}

void ScreenAnalyzer::run() {	
	while (true) {
		Sleep(1000);
		QImage img;
		ERROR_CODE openCorrectly = (ERROR_CODE)loadScreen(img);
		if (openCorrectly != OK)
			continue;
		Calibrator calibrator(img, *var, profile);
		int retCode = calibrator.calibrateManaAndHealthBar();
		if (retCode != OK)
			continue;
		mainLoop();
	}
}

int ScreenAnalyzer::loadScreen(QImage& img){
	QString nameOfImgToCapture;
	ERROR_CODE code = (ERROR_CODE)getNameOfLastTakenScreenShotForSure(nameOfImgToCapture, 55);
	if (code != OK)
		return code;
	QString pathToImg = pathToScreenFolder + QString("\\") + nameOfImgToCapture;
	auto openCorrectly = img.load(pathToImg);
	int toRet = openCorrectly ? OK : CANT_LOAD_SCREEN_FROM_SCREENSHOT_FOLDER;
	return toRet;
}

QString ScreenAnalyzer::getNameOfLastTakenScreenShot(){
	QDir directory(pathToScreenFolder);
	QStringList litOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
	if (litOfFIles.isEmpty())
		return QString();


	QList<long long> listOfOnlyDatesAndTimes;
	//geting QStrin with last digit of year, 2 digits of month, 2 digits of day
	//2 digits of hour, 2 digits of minute, 2 of seconds, 3 of miliseconds
	for each (QString var in litOfFIles)
		listOfOnlyDatesAndTimes << var.remove("-").remove("_").mid(3,14).toLongLong();

	int index = -1;
	long long biggestValue = 0;
	for (int i = 0; i < listOfOnlyDatesAndTimes.size(); i++) {
		if (listOfOnlyDatesAndTimes[i] > biggestValue) {
			biggestValue = listOfOnlyDatesAndTimes[i];
			index = i;
		}
	}

	bool indexIsOutOfRange = litOfFIles.size() - 1 < index || index < 0;
	QString toRet = !indexIsOutOfRange ? litOfFIles[index] : "";
	return toRet;
}

int ScreenAnalyzer::getNameOfLastTakenScreenShotForSure(QString& toRet, int MaxTries) {
	for (int tries = 0; tries < MaxTries; tries++) {
		toRet = getNameOfLastTakenScreenShot();
		if (!toRet.isEmpty())
			return ERROR_CODE::OK;
		else 
			msleep(100);
	}
	Logger::logPotenialBug("No files In screenshot folder", "ScreenAnalyzer", "getNameOfLastTakenScreenShot");
	return CANT_LOAD_SCREEN_FROM_SCREENSHOT_FOLDER;
}

void ScreenAnalyzer::setUpScreenFolder(){
	QDir dir = QDir::tempPath();
	dir.cdUp();
	QString pathToTmpFolder = dir.path();

	QString pathToTibiaDir = pathToTmpFolder + "/Tibia";
	bool TibiaDirExist = QDir(pathToTibiaDir).exists();
	if (!TibiaDirExist) {
		//TODO tu jest fatal error
		Logger::logPotenialBug("Can't find game instalation directory", "ScreenAnalyzer", "setUpScreenFolder");
		return;
	}
	QString wholePathToScreenFolder = pathToTibiaDir + "/packages/Tibia/screenshots";
	bool screenFolderExist = QDir(wholePathToScreenFolder).exists();
	if (!screenFolderExist) {
		//TODO tu jest fatal error
		Logger::logPotenialBug("Screenshot folder inside game directory does not exist", "ScreenAnalyzer", "setUpScreenFolder");
		return;
	}
	pathToScreenFolder = wholePathToScreenFolder;
}

void ScreenAnalyzer::notifyOtherProcessOfStateOfAnalyzer(bool worksGood){
	if (isManaHealthClassEnabledToAnalyzeImgs && worksGood)
		;
	else if (!isManaHealthClassEnabledToAnalyzeImgs && worksGood) {
		isManaHealthClassEnabledToAnalyzeImgs = true;
		emit sendAllowenceToAnalyze(true);
	}
	else if (isManaHealthClassEnabledToAnalyzeImgs && !worksGood) {
		isManaHealthClassEnabledToAnalyzeImgs = false;
		emit sendAllowenceToAnalyze(false);
	}
	else if (!isManaHealthClassEnabledToAnalyzeImgs && !worksGood)
		;
}

void ScreenAnalyzer::deleteScreenShotFolder(){
	QDir dir(pathToScreenFolder);
	dir.setNameFilters(QStringList() << "*.*");
	if (dir.entryList().size() == 0)
		return;
	dir.setFilter(QDir::Files);
	foreach(QString dirFile, dir.entryList())
		dir.remove(dirFile);
}

void ScreenAnalyzer::mainLoop(){
	while (enableScreenAnalyzer){
		msleep(timeBetweenNextCheckingsOfScrennShotFolder);
		QImage img;
		ERROR_CODE openCorrectly = (ERROR_CODE)loadScreen(img);
		if (openCorrectly == OK) {
			deleteScreenShotFolder();
			notifyOtherProcessOfStateOfAnalyzer(true);
			cutImportantImgsFromWholeScreenAndSendThemToVarClass(img);
			var->wholeImg = img;
			var->newData = true;
		}
		else
			notifyOtherProcessOfStateOfAnalyzer(false);
	}
}

int ScreenAnalyzer::cutImportantImgsFromWholeScreenAndSendThemToVarClass(QImage& fullscreen){
	QRect emptyRect;
	bool healthFrameFound = profile->frames.healthFrame != emptyRect;
	bool manaFrameFound = profile->frames.manaFrame != emptyRect;
	bool manaShieldFound = profile->frames.manaShieldFrame != emptyRect;
	bool combinedBoxFound = profile->frames.combinedFrame != emptyRect;

	bool notEnoughFramesFound = !((manaFrameFound || combinedBoxFound) && healthFrameFound);
	if(notEnoughFramesFound)
		return NO_ENOUGH_FRAMES_FOUND;

	if (healthFrameFound) {
		QImage healthValueImg = fullscreen.copy(profile->frames.healthFrame);
		if (profile->frames.howTheyShouldBeRotated != 0)
			Utilities::rotateImgToRight(&healthValueImg, profile->frames.howTheyShouldBeRotated);
		var->var_healthPieceImg = healthValueImg;
	}
	if (manaFrameFound) {
		QImage manaValueImg = fullscreen.copy(profile->frames.manaFrame);
		if (profile->frames.howTheyShouldBeRotated != 0)
			Utilities::rotateImgToRight(&manaValueImg, profile->frames.howTheyShouldBeRotated);
		var->var_manaPieceImg = manaValueImg;
	}
	if (manaShieldFound) {
		QImage manaShieldValueImg = fullscreen.copy(profile->frames.manaShieldFrame);
		if (profile->frames.howTheyShouldBeRotated != 0)
			Utilities::rotateImgToRight(&manaShieldValueImg, profile->frames.howTheyShouldBeRotated);
		var->var_manaShieldPieceImg = manaShieldValueImg;
	}
	if (combinedBoxFound) {
		QImage combinedValueImg = fullscreen.copy(profile->frames.combinedFrame);
		if (profile->frames.howTheyShouldBeRotated != 0)
			Utilities::rotateImgToRight(&combinedValueImg, profile->frames.howTheyShouldBeRotated);
		var->var_combinedBoxPieceImg = combinedValueImg;
	}
	var->healthFound = healthFrameFound;
	var->manaFound = manaFrameFound;
	var->manaShieldFound = manaShieldFound;
	var->combinedFound = combinedBoxFound;
	return OK;
}
