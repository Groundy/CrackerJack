#include "ScreenAnalyzer.h"

ScreenAnalyzer::ScreenAnalyzer(QObject *parent, std::shared_ptr<VariablesClass> var, Profile* profToSet)
	: QThread(parent), var(var){
	profile = profToSet;
	screenShotFolder = setUpScreenFolder();
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
	ERROR_CODE code = (ERROR_CODE)getNameOfLastTakenScreenshotWithTries(nameOfImgToCapture, 55);
	if (code != OK)
		return code;
	QString pathToImg = screenShotFolder.absoluteFilePath(nameOfImgToCapture);
	bool openCorrectly = img.load(pathToImg);
	int toRet = openCorrectly ? OK : CANT_LOAD_SCREEN_FROM_SCREENSHOT_FOLDER;
	return toRet;
}


QString ScreenAnalyzer::getNameOfLastTakenScreenShot(){
	QStringList filtr = QStringList() << "*_*_*_Hotkey.png";
	screenShotFolder.refresh();
	QStringList litOfFIles = screenShotFolder.entryList(filtr, QDir::Files);
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

int ScreenAnalyzer::getNameOfLastTakenScreenshotWithTries(QString& toRet, const int MAX_TRIES) {
	for (int tries = 0; tries < MAX_TRIES; tries++) {
		toRet = getNameOfLastTakenScreenShot();
		if (!toRet.isEmpty())
			return ERROR_CODE::OK;
		else 
			msleep(100);
	}
	Logger::logPotenialBug("No files In screenshot folder", "ScreenAnalyzer", "getNameOfLastTakenScreenShot");
	return CANT_LOAD_SCREEN_FROM_SCREENSHOT_FOLDER;
}

QDir ScreenAnalyzer::setUpScreenFolder(){
	QDir dir = QDir::tempPath();
	bool ok1 = dir.cdUp();
	bool ok2 = dir.cd("Tibia");
	bool ok3 = dir.cd("packages");
	bool ok4 = dir.cd("Tibia");
	bool ok5 = dir.cd("screenshots");
	bool ok = ok1 && ok2 && ok3 && ok4 && ok5;
	if (ok)
		return dir;
	else {
		QString textToUser = tr("Could not find game screenshot folder.");
		QString logText = "Could not find game screenshot folder.";
		Logger::logPotenialBug(logText, "ScreenAnalyzer", "setUpScreenFolder");
		Utilities::showMessageBox_INFO(textToUser);
		return QDir();
	}
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
	QStringList filterPatter("*_*_*_Hotkey.png");//yyyy-MM-dd_hhmmsszzzz_CharNick_Methode.png
	screenShotFolder.setNameFilters(filterPatter);
	screenShotFolder.setFilter(QDir::Files);

	QStringList namesOfFoundFiles = screenShotFolder.entryList();
	foreach(QString dirFile, namesOfFoundFiles)
		screenShotFolder.remove(dirFile);
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
			ImgEditor::rotateImgToRight(healthValueImg, profile->frames.howTheyShouldBeRotated);
		var->var_healthPieceImg = healthValueImg;
	}
	if (manaFrameFound) {
		QImage manaValueImg = fullscreen.copy(profile->frames.manaFrame);
		if (profile->frames.howTheyShouldBeRotated != 0)
			ImgEditor::rotateImgToRight(manaValueImg, profile->frames.howTheyShouldBeRotated);
		var->var_manaPieceImg = manaValueImg;
	}
	if (manaShieldFound) {
		QImage manaShieldValueImg = fullscreen.copy(profile->frames.manaShieldFrame);
		if (profile->frames.howTheyShouldBeRotated != 0)
			ImgEditor::rotateImgToRight(manaShieldValueImg, profile->frames.howTheyShouldBeRotated);
		var->var_manaShieldPieceImg = manaShieldValueImg;
	}
	if (combinedBoxFound) {
		QImage combinedValueImg = fullscreen.copy(profile->frames.combinedFrame);
		if (profile->frames.howTheyShouldBeRotated != 0)
			ImgEditor::rotateImgToRight(combinedValueImg, profile->frames.howTheyShouldBeRotated);
		var->var_combinedBoxPieceImg = combinedValueImg;
	}
	var->healthFound = healthFrameFound;
	var->manaFound = manaFrameFound;
	var->manaShieldFound = manaShieldFound;
	var->combinedFound = combinedBoxFound;
	return OK;
}
