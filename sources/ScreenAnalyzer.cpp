#include "ScreenAnalyzer.h"

ScreenAnalyzer::ScreenAnalyzer(QObject *parent, VariablesClass *varClass, Profile* prof)
	: QThread(parent){
	var = varClass;

	listOfPotionNamesToLookFor.clear();
	QStringList nameOfAllMethodesFromProf;
	nameOfAllMethodesFromProf.append(prof->healthRestoreMethodeNames);
	nameOfAllMethodesFromProf.append(prof->manaRestoreMethodeNames);
	for each (QString var in nameOfAllMethodesFromProf){
		bool isPotionName = var.contains(QString(" Potion"));
		if(isPotionName)
			listOfPotionNamesToLookFor.push_back(var);
	}

}

ScreenAnalyzer::~ScreenAnalyzer(){
}

void ScreenAnalyzer::run() {	
	while (true) {
		if (!enableScreenAnalyzer) {
			msleep(6 * timeBetweenNextCheckingsOfScrennShotFolder);
			continue;
		}
		mainLoop();
	}
}

int ScreenAnalyzer::reCalibrate(){
	var->caliState = var->DURING_CALIBRATION;
	notifyOtherProcessOfStateOfAnalyzer(false);
	qDebug() << "Recalibration";
	int res = calibrate();
	return res;
}

int ScreenAnalyzer::loadScreen(QImage& img){
	QString nameOfImgToCapture;
	ERR code = (ERR)getNameOfLastTakenScreenShotForSure(nameOfImgToCapture, 55);
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
	if (litOfFIles.size() == 0) 
		return QString(); //diag err

	QList<long long> listOfOnlyDatesAndTimes;
	//geting QStrin with last digit of year, 2 digits of month, 2 digits of day
	//2 digits of hour, 2 digits of minute, 2 of seconds, 3 of miliseconds
	for each (QString var in litOfFIles)
		listOfOnlyDatesAndTimes << var.remove("-").remove("_").mid(3,14).toLongLong();

	int index = 0;
	long long biggestValue = 0;
	for (int i = 0; i < listOfOnlyDatesAndTimes.size(); i++) {
		if (listOfOnlyDatesAndTimes[i] > biggestValue) {
			biggestValue = listOfOnlyDatesAndTimes[i];
			index = i;
		}
	}
	if (litOfFIles.size() - 1 < index)
		;//diag err

	return litOfFIles[index];
}

int ScreenAnalyzer::findRectanglesWithPotionsPos(QImage& fullscreen){
	QStringList foundPotions;
	QList<QRect> rectsWithPotions;
	Utilities::findPotionsOnBottomBar(listOfPotionNamesToLookFor, foundPotions, rectsWithPotions, fullscreen);

	bool allPotionsFound = true;
	for each (QString potNameThatShouldBeFound in listOfPotionNamesToLookFor){
		bool found = foundPotions.contains(potNameThatShouldBeFound);
		if (!found)
			allPotionsFound = false;
	}
	if (!allPotionsFound)
		return NOT_ALL_POTS_FOUND;

	for (size_t i = 0; i < rectsWithPotions.size(); i++){
		QRect rectToAdd = rectsWithPotions[i];
		QString nameOfPot = foundPotions[i];
		var->potionName_rectPosOnScreen_map[nameOfPot] = rectToAdd;
	}

	return OK;
}

int ScreenAnalyzer::getNameOfLastTakenScreenShotForSure(QString& toRet, int MaxTries) {
	toRet = "";
	for (int tries = 0; tries < MaxTries; tries++) {
		toRet = getNameOfLastTakenScreenShot();
		if (!toRet.isEmpty())
			return ERROR_CODE::OK;
		else 
			msleep(100);
	}
	return CANT_LOAD_SCREEN_FROM_SCREENSHOT_FOLDER;
}

int ScreenAnalyzer::findIndexesOfRectangleThatContainsSlashes(QImage& fullScreen, QList<QRect> importantFrames, QList<int>& indexesOfFramesWithSlashesVert, QList<int>& indexesOfFramesWithSlashesHor, int& indexOfFrameCombined){
	QImage vertSlashes = Utilities::fromCharToImg(QChar(47));
	QImage horSlashes = Utilities::fromCharToImg(QChar(92));
	QList<int> indexesVert, indexesHor;
	indexOfFrameCombined = -1;
	for (size_t i = 0; i < importantFrames.size(); i++) {
		QImage imgTmp = fullScreen.copy(importantFrames[i]);
		Utilities::imgToBlackAndWhiteAllColors(imgTmp, 250);

		QList<QPoint> pointsVert = Utilities::findStartPositionInImg(&vertSlashes, &imgTmp);
		if (pointsVert.size() == 0)
			;
		else if (pointsVert.size() == 1)
			indexesVert.push_back(i);
		else if (pointsVert.size() == 2) {
			indexOfFrameCombined = i;
			indexesVert.push_back(i);
		}

		QList<QPoint> pointsHor = Utilities::findStartPositionInImg(&horSlashes, &imgTmp);
		if (pointsHor.size() == 0)
			;
		else if (pointsHor.size() == 1)
			indexesHor.push_back(i);
		else if (pointsHor.size() == 2) {
			indexOfFrameCombined = i;
			indexesHor.push_back(i);
		}
	}
	if (indexesVert.size() == 0 && indexesHor.size() == 0)
		return NO_SLASHES_FOUND_IN_GAME_SCREEN;
	else {
		indexesOfFramesWithSlashesHor = indexesHor;
		indexesOfFramesWithSlashesVert = indexesVert;
		return OK;
	}
}

void ScreenAnalyzer::TEST_setPositionHealthImhs(QString pathToFolderWithDiffrentPositionsStylesScreen, QString pathToOutPutFolder){
	QImage combined, health, mana, manaShield;
	QDir directory(pathToFolderWithDiffrentPositionsStylesScreen);
	QStringList listOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
	for (int i = 0; i < listOfFIles.size(); i++) {
		QString nameOfFile = listOfFIles[i];
		qDebug() << QString::number(i);
		QString pathToPng = pathToFolderWithDiffrentPositionsStylesScreen + "\\" + nameOfFile;
		QImage img;
		img.load(pathToPng);
		QList<QRect> importantRects;
		findWindowsOnScreen(img, importantRects);
		int indCombined;
		int indHealth;
		int indMana;
		int indManaShield;
		int howTheyShouldBeRotated;
		
		setPositionHealthImgs(img, importantRects, indHealth, indMana, indManaShield, indCombined, howTheyShouldBeRotated);

		health = img.copy(importantRects[indHealth]);
		QString healthStr, manaStr, ManaShieldStr, combinedStr;

		Utilities::rotateImgToRight(&health, howTheyShouldBeRotated);
		Utilities::imgToBlackAndWhiteAllColors(health, 240);
		healthStr = Utilities::imgWithStrToStr(&health);
		qDebug() << "Health: " + healthStr;

		bool thereIsCombinedBox = indCombined != -1;
		bool manaFound = indMana != -1;
		bool manaShieldFound = indManaShield != -1;
		if (thereIsCombinedBox) {
			QRect rect = importantRects.at(indCombined);
			combined = img.copy(rect);

			Utilities::rotateImgToRight(&combined, howTheyShouldBeRotated);
			Utilities::imgToBlackAndWhiteAllColors(combined, 240);
			QString combinedStr = Utilities::imgWithStrToStr(&combined);
			qDebug() << "combinedStr: " + combinedStr;
		}
		else if (!thereIsCombinedBox && manaShieldFound && manaFound) {
			QRect rect = importantRects.at(indMana);
			mana = img.copy(rect);
			rect = importantRects.at(indManaShield);
			manaShield = img.copy(rect);

			Utilities::rotateImgToRight(&mana, howTheyShouldBeRotated);
			Utilities::imgToBlackAndWhiteAllColors(mana, 240);
			QString manaStr = Utilities::imgWithStrToStr(&mana);
			qDebug() << "manaStr: " + manaStr;

			Utilities::rotateImgToRight(&manaShield, howTheyShouldBeRotated);
			Utilities::imgToBlackAndWhiteAllColors(manaShield, 240);
			QString ManaShieldStr = Utilities::imgWithStrToStr(&manaShield);
			qDebug() << "ManaShieldStr: " + ManaShieldStr;
		}
		else if (!thereIsCombinedBox && !manaShieldFound && manaFound) {
			QRect rect = importantRects.at(indMana);
			mana = img.copy(rect);

			Utilities::rotateImgToRight(&mana, howTheyShouldBeRotated);
			Utilities::imgToBlackAndWhiteAllColors(mana, 240);
			QString manaStr = Utilities::imgWithStrToStr(&mana);
			qDebug() << "manaStr: " + manaStr;
		}
		else
			return;

		//QString name = QDateTime::currentDateTime().toString("mmss_mmm_");
		QString name = "";

		QString h =  "Health_";
		QString m = "Mana_";
		QString c = "Combined_";
		QString ms = "ManaShield_";
		QString path = pathToOutPutFolder + "\\_";
		QString finalName_h = path +name + h + nameOfFile;
		QString finalName_m = path + name + m + nameOfFile;
		QString finalName_ms = path + name + ms + nameOfFile;
		QString finalName_c = path + name + c + nameOfFile;
		health.save(finalName_h);
		if (thereIsCombinedBox)
			combined.save(finalName_c);
		else if (!thereIsCombinedBox && manaShieldFound && manaFound) {
			mana.save(finalName_m);
			manaShield.save(finalName_ms);
		}
		else if (!thereIsCombinedBox && !manaShieldFound && manaFound)
			mana.save(finalName_m);
		else
			return;
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
	QDir dir(pathToScreenFolder);
	dir.setNameFilters(QStringList() << "*.*");
	if (dir.entryList().size() == 0)
		return;
	dir.setFilter(QDir::Files);
	foreach(QString dirFile, dir.entryList())
		dir.remove(dirFile);
}

void ScreenAnalyzer::mainLoop(){
	sleep(1);
	calibrate();
	while (enableScreenAnalyzer){
		msleep(timeBetweenNextCheckingsOfScrennShotFolder);
		QImage img;
		ERROR_CODE openCorrectly = (ERROR_CODE)loadScreen(img);
		bool isCalibrated = var->caliState == var->CALIBRATED;
		if (!isCalibrated) {
			reCalibrate();
			continue;
		}
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

int ScreenAnalyzer::calibrate(){
	var->lastTimeCalibrationUsed = Utilities::getCurrentTimeInMiliSeconds();
	qDebug() << "ScreenAnalyzer::calibrate()";
	QImage fullScreen;

	ERROR_CODE res1 = (ERROR_CODE)loadScreen(fullScreen);
	if (res1 != OK) 
		return res1;

	QList<QRect> importantRectangles;
	ERROR_CODE res2 = (ERROR_CODE)findWindowsOnScreen(fullScreen, importantRectangles);
	if (res2 != OK) {
		//qDebug() << "ScreenAnalyzer::findMiniMapWindow(QImage fullScreen) failed";
		return res2;
	}

	ERROR_CODE res3 = (ERROR_CODE)findRectanglesWithPotionsPos(fullScreen);
	//if (res3 != OK)
		//return res2;//TODO here fun shouldn't end, it should just notify that there are no enough pots found

	ERROR_CODE res4 = (ERROR_CODE)categorizeWindows(fullScreen, importantRectangles, frames);
	if (res4 == OK) 
		var->caliState = var->CALIBRATED;
	else {
		var->caliState = var->NOT_CALIBRATED;
		return ERROR_CODE::NO_POSTION_ASSIGNED_TO_SLASHES;
	}
		return OK;
}

int ScreenAnalyzer::cutImportantImgsFromWholeScreenAndSendThemToVarClass(QImage& fullscreen){
	bool healthFrameFound = frames.healthFrame != QRect();
	bool manaFrameFound = frames.manaFrame != QRect();
	bool manaShieldFound = frames.manaShieldFrame != QRect();
	bool combinedBoxFound = frames.combinedFrame != QRect();

	bool notEnoughFramesFound = !((manaFrameFound || combinedBoxFound) && healthFrameFound);
	if(notEnoughFramesFound)
		return NO_ENOUGH_FRAMES_FOUND;

	if (healthFrameFound) {
		QImage healthValueImg = fullscreen.copy(frames.healthFrame);
		if (frames.howTheyShouldBeRotated != 0)
			Utilities::rotateImgToRight(&healthValueImg, frames.howTheyShouldBeRotated);
		var->var_healthPieceImg = healthValueImg;
	}
	if (manaFrameFound) {
		QImage manaValueImg = fullscreen.copy(frames.manaFrame);
		if (frames.howTheyShouldBeRotated != 0)
			Utilities::rotateImgToRight(&manaValueImg, frames.howTheyShouldBeRotated);
		var->var_manaPieceImg = manaValueImg;
	}
	if (manaShieldFound) {
		QImage manaShieldValueImg = fullscreen.copy(frames.manaShieldFrame);
		if (frames.howTheyShouldBeRotated != 0)
			Utilities::rotateImgToRight(&manaShieldValueImg, frames.howTheyShouldBeRotated);
		var->var_manaShieldPieceImg = manaShieldValueImg;
	}
	if (combinedBoxFound) {
		QImage combinedValueImg = fullscreen.copy(frames.combinedFrame);
		if (frames.howTheyShouldBeRotated != 0)
			Utilities::rotateImgToRight(&combinedValueImg, frames.howTheyShouldBeRotated);
		var->var_combinedBoxPieceImg = combinedValueImg;
	}
	var->healthFound = healthFrameFound;
	var->manaFound = manaFrameFound;
	var->manaShieldFound = manaShieldFound;
	var->combinedFound = combinedBoxFound;
	return OK;
}

int ScreenAnalyzer::categorizeWindows(QImage& fullscreen, QList<QRect>& importantRectangles, Frames& frame){
	//5 cause 1-minimap 2-gameScreen 3-text input 4-health 5-mana, those 5 have to be found
	if (importantRectangles.size() < 5)
		return NO_ENOUGH_FRAMES_FOUND;

	QList<int> surphacesOfFrames;
	for each (QRect var in importantRectangles){
		int surf = var.width() * var.height();
		surphacesOfFrames.push_back(surf);
	}
	int indexOfBiggestValue = 0;
	int previousBiggestValue = 0;
	for (size_t i = 0; i < surphacesOfFrames.size(); i++){
		if (surphacesOfFrames[i] > previousBiggestValue) {
			previousBiggestValue = surphacesOfFrames[i];
			indexOfBiggestValue = i;
		}
	}
	frame.miniMapFrame = importantRectangles.at(indexOfBiggestValue);
	importantRectangles.removeAt(indexOfBiggestValue);

	int indexOfHealth, indexOfMana, indexOfManaShield, howTheyShouldBeRotated, indexOfCombinedBox;
	setPositionHealthImgs(fullscreen, importantRectangles, indexOfHealth, indexOfMana, indexOfManaShield, indexOfCombinedBox, howTheyShouldBeRotated);
	frame.howTheyShouldBeRotated = howTheyShouldBeRotated;

	bool healthFound = indexOfHealth != -1 && indexOfHealth >= 0 && indexOfHealth<= importantRectangles.size();
	bool manaFound = indexOfMana != -1 && indexOfMana >= 0 && indexOfMana <= importantRectangles.size();
	bool manaShieldFound = indexOfManaShield != -1 && indexOfManaShield >= 0 && indexOfManaShield <= importantRectangles.size();
	bool combinedBoxFound = indexOfCombinedBox != -1 && indexOfCombinedBox >= 0 && indexOfCombinedBox <= importantRectangles.size();

	bool enoughFramesFound = healthFound && ( manaFound || combinedBoxFound);
	if(!enoughFramesFound)
		return ERROR_CODE::ERROR_IN_SETTING_POSITION_OF_INTERFACE;

	bool deleteHelath = false , deleteMana = false, deleteManaShield = false, deleteCombined = false;
	if (healthFound) {
		frame.healthFrame = importantRectangles[indexOfHealth];
		deleteHelath = true;
	}	

	if (manaFound) {
		frame.manaFrame = importantRectangles[indexOfMana];
		deleteHelath = true;
	}

	if (manaShieldFound) {
		frame.manaShieldFrame = importantRectangles[indexOfManaShield];
		deleteManaShield = true;
	}

	if (combinedBoxFound) {
		frame.combinedFrame = importantRectangles[indexOfCombinedBox];
		deleteCombined = true;
	}
	QList<QRect> rectsToDelete;
	if (deleteHelath) rectsToDelete.push_back(importantRectangles[indexOfHealth]);
	if (deleteMana) rectsToDelete.push_back(importantRectangles[indexOfMana]);
	if (deleteManaShield) rectsToDelete.push_back(importantRectangles[indexOfManaShield]);
	if (deleteCombined) rectsToDelete.push_back(importantRectangles[indexOfCombinedBox]);
	for each (QRect var in rectsToDelete)
		importantRectangles.removeOne(var);
	return OK;
}

void ScreenAnalyzer::sortByXAndYPoints(QList<QPoint>& points, QList<QPoint>& pointsSortedByX, QList<QPoint>& pointsSortedByY) {
	QMap<int, int> sortedByX; //x is key
	QMap<int, int> sortedByY; //y is key
	QList<QPoint> copyOfInput = points;
	for each (QPoint var in copyOfInput) {
		sortedByX.insert(var.x(), var.y());
		sortedByY.insert(var.y(), var.x());//inverted order of XY
	}
	QList<QPoint>sortedByXToRet;
	QList<QPoint>sortedByYToRet;
	for (auto mapEle : sortedByX.keys()){
		int x = mapEle;
		int y = sortedByX.value(mapEle);
		sortedByXToRet.push_back(QPoint(x, y));
	}
	for (auto mapEle : sortedByY.keys()) {
		int y = mapEle;
		int x = sortedByY.value(mapEle);
		sortedByYToRet.push_back(QPoint(x, y));
	}

	//it's possibility that two points in vector have diffrent one of cordinates but
	//second cordinate differ by few pixels, next part of fun is designed to
	//make set for both cordinates the same value
	bool fixPoints = true; //turn on/off that fixing
	if (fixPoints) {
		bool needToReWriteLists = false;
		const int MAX_DIFFRENCE_IN_PIXELS = 4;
		for (int i = 1; i < sortedByXToRet.size(); i++) {
			bool areTheSame = sortedByXToRet[i].x() - sortedByXToRet[i - 1].x() <= MAX_DIFFRENCE_IN_PIXELS;
			if (areTheSame) {
				needToReWriteLists = true;
				int toSet = sortedByXToRet[i - 1].x();
				sortedByXToRet[i].setX(toSet);
				//making the same change in second container
				int yValueOfmodifiedPoint = sortedByXToRet[i].y();
				for (QPoint& point : sortedByYToRet) {
					if (point.y() == yValueOfmodifiedPoint)
						point.setX(toSet);
				}
			}
		}
		for (int i = 1; i < sortedByYToRet.size(); i++) {
			bool areTheSame = sortedByYToRet[i].y() - sortedByYToRet[i - 1].y() <= MAX_DIFFRENCE_IN_PIXELS;
			if (areTheSame) {
				needToReWriteLists = true;
				int toSet = sortedByYToRet[i - 1].y();
				sortedByYToRet[i].setY(toSet);
				int xValueOfmodifiedPoint = sortedByYToRet[i].x();
				for (QPoint& point : sortedByXToRet) {
					if (point.x() == xValueOfmodifiedPoint)
						point.setY(toSet);
				}
			}
		}
		if (needToReWriteLists) {
			//ReWriting begining of fun to eliminate duplicates
			QMap<int, int> sortedByX_TMP; //x is key
			QMap<int, int> sortedByY_TMP; //y is key
			QList<QPoint> copyOfInput = points;

			for each (QPoint var in sortedByXToRet) 
				sortedByX_TMP.insert(var.x(), var.y());

			for each (QPoint var in sortedByYToRet) 
				sortedByY_TMP.insert(var.y(), var.x());//inverted order
			
			QList<QPoint>sortedByXToRet_TMP;
			QList<QPoint>sortedByYToRet_TMP;
			for (auto mapEle : sortedByX_TMP.keys()) {
				int x = mapEle;
				int y = sortedByX.value(mapEle);
				sortedByXToRet_TMP.push_back(QPoint(x, y));
			}
			for (auto mapEle : sortedByY_TMP.keys()) {
				int y = mapEle;
				int x = sortedByY.value(mapEle);
				sortedByYToRet_TMP.push_back(QPoint(x, y));
			}
			pointsSortedByX = sortedByXToRet_TMP;
			pointsSortedByY = sortedByYToRet_TMP;
			return;
		}//end of rewriting
	}//end of slightly correcting

	pointsSortedByX = sortedByXToRet;
	pointsSortedByY = sortedByYToRet;
}

int ScreenAnalyzer::sortByXAndYRects(QList<QRect>& inputRects, QList<QRect>& rectsSortedByPosX, QList<QRect>& rectsSortedByPosY) {
	if (inputRects.size() == 0)
		return UNDEFINED_ERROR;
	
	QList<QRect> sortedByX, sortedByY;
	sortedByX.push_back(inputRects[0]);
	sortedByY.push_back(inputRects[0]);

	for (int i = 1; i < inputRects.size(); i++) {
		QRect rect = inputRects[i];
		int startPosX = rect.x();
		int startPosY = rect.y(); 

		int indexToInsertNewElement_X = -1;
		int indexToInsertNewElement_Y = -1;

		int size = sortedByX.size();
		for (int i = 0; i < size; i++){
			if (startPosX <= sortedByX[i].x()) {
				indexToInsertNewElement_X = i;
				break;
			}
		}
		for (int i = 0; i < size; i++) {
			if (startPosY <= sortedByY[i].y()) {
				indexToInsertNewElement_Y = i;
				break;
			}
		}
		if (indexToInsertNewElement_X == -1)
			indexToInsertNewElement_X = size;
		if (indexToInsertNewElement_Y == -1)
			indexToInsertNewElement_Y = size;
		sortedByX.insert(indexToInsertNewElement_X, rect);
		sortedByY.insert(indexToInsertNewElement_Y, rect);
	}
	rectsSortedByPosX = sortedByX;
	rectsSortedByPosY = sortedByY;
	return OK;
}

int ScreenAnalyzer::findWindowsOnScreen(QImage& fullScreen, QList<QRect>& importantRectangles){
	int width = fullScreen.width();
	int height = fullScreen.height();
	auto black = qRgb(0, 0, 0);
	const uint minAcceptableValue = 17;
	const uint maxAcceptableValue = 29;
	QList<QPoint> startOfFrames;
	bool isPixelOfFrame;
	for (size_t x = 0; x < width-2; x++) {
		for (size_t y = 0; y < height-2; y++) {
			isPixelOfFrame = Utilities::isItPixelFromFrame(fullScreen.pixel(x, y), minAcceptableValue, maxAcceptableValue, true);
			if (!isPixelOfFrame) continue;
			//next four pixel next shouldn't be frame pix
			isPixelOfFrame = Utilities::isItPixelFromFrame(fullScreen.pixel(x+1, y+1), minAcceptableValue, maxAcceptableValue, false);
			if (isPixelOfFrame) continue;

			isPixelOfFrame = Utilities::isItPixelFromFrame(fullScreen.pixel(x+1, y+2), minAcceptableValue, maxAcceptableValue, false);
			if (isPixelOfFrame) continue;

			isPixelOfFrame = Utilities::isItPixelFromFrame(fullScreen.pixel(x+2, y+1), minAcceptableValue, maxAcceptableValue, false);
			if (isPixelOfFrame) continue;

			isPixelOfFrame = Utilities::isItPixelFromFrame(fullScreen.pixel(x+2, y+2), minAcceptableValue, maxAcceptableValue, false);
			if (isPixelOfFrame) continue;

			isPixelOfFrame = Utilities::isItPixelFromFrame(fullScreen.pixel(x+1, y), minAcceptableValue, maxAcceptableValue, true);
			if (!isPixelOfFrame) continue;

			isPixelOfFrame = Utilities::isItPixelFromFrame(fullScreen.pixel(x+2, y), minAcceptableValue, maxAcceptableValue, true);
			if (!isPixelOfFrame) continue;

			isPixelOfFrame = Utilities::isItPixelFromFrame(fullScreen.pixel(x, y+1), minAcceptableValue, maxAcceptableValue, true);
			if (!isPixelOfFrame) continue;

			isPixelOfFrame = Utilities::isItPixelFromFrame(fullScreen.pixel(x, y+2), minAcceptableValue, maxAcceptableValue, true);
			if (!isPixelOfFrame) continue;

			startOfFrames.push_back(QPoint(x, y));
		}
	}
	//looking for rectangles from start points of frames
	QList<QRect> frameToRet;
	for each (QPoint startPoint in startOfFrames){
		int currentWidth = 0;
		int currentHeight = 0;
		for (size_t x = startPoint.x(); x < width; x++){
			uint color = fullScreen.pixel(x, startPoint.y());
			bool isPixOfFrame = Utilities::isItPixelFromFrame(color, minAcceptableValue, maxAcceptableValue, true);
			if (isPixOfFrame)
				currentWidth++;
			else
				break;
		}
		for (size_t y = startPoint.y(); y < height; y++) {
			uint color = fullScreen.pixel(startPoint.x(), y);
			bool isPixOfFrame = Utilities::isItPixelFromFrame(color, minAcceptableValue, maxAcceptableValue, true);
			if (isPixOfFrame)
				currentHeight++;
			else
				break;
		}
		if (currentWidth > 0 && currentHeight > 0){
			int x = startPoint.x();
			int y = startPoint.y();
			int w = currentWidth;
			int h = currentHeight;
			if (w < 5 || h < 5)//removing too small imgs
				continue;
			frameToRet.push_back(QRect(x+1,y+1,w-1,h-1));
		}
	}

	importantRectangles = frameToRet;

	ERR toRet = frameToRet.size() > 0 ? OK : NO_FRAMES_FOUND;
	return toRet;
}

int ScreenAnalyzer::setPositionHealthImgs(QImage& fullscreen, QList<QRect> listOfImportantRectangles, int& indexOfHealth, int& indexOfMana, int& indexOfManaShield, int& indexOfManaAndManaShieldCombined, int& howTheyShouldBeRotated) {
	QList<int> indexesOfRectWithSlashVert;
	QList<int> indexesOfRectWithSlashHor;
	int indexOfFrameWithManaShieldAndMana;
	findIndexesOfRectangleThatContainsSlashes(fullscreen, listOfImportantRectangles, indexesOfRectWithSlashVert, indexesOfRectWithSlashHor, indexOfFrameWithManaShieldAndMana);
	enum positionsOfInterface { TOP, RIGHT, DOWN, LEFT };

	positionsOfInterface position;
	QPoint midOfScreen(fullscreen.width() / 2, fullscreen.height() / 2);

	if (indexesOfRectWithSlashVert.size() != 0) {
		QRect first = listOfImportantRectangles[indexesOfRectWithSlashVert[0]];
		position = first.x() < midOfScreen.x() ? LEFT : RIGHT;
	}
	else if (indexesOfRectWithSlashHor.size() != 0) {
		QRect first = listOfImportantRectangles[indexesOfRectWithSlashHor[0]];
		position = first.y() < midOfScreen.y() ? TOP : DOWN;
	}
	else {
		return NO_SLASHES_FOUND_IN_GAME_SCREEN;
	}

	bool combined = indexOfFrameWithManaShieldAndMana != -1;
	switch (position) {
		case TOP: case DOWN: {
			int size = indexesOfRectWithSlashHor.size();
			howTheyShouldBeRotated = 0;
			QList<QRect> sortedByX, sortedByY, rectangles;
			for each (int var in indexesOfRectWithSlashHor)
				rectangles.push_back(listOfImportantRectangles[var]);
			sortByXAndYRects(rectangles, sortedByX, sortedByY);

			if (combined && size == 2) {
				bool isParallelStyle = sortedByY[0].y() != sortedByY[1].y();
				if (isParallelStyle) {
					indexOfHealth = listOfImportantRectangles.indexOf(sortedByY[0]);
					indexOfMana = -1;
					indexOfManaShield = -1;
					indexOfManaAndManaShieldCombined = listOfImportantRectangles.indexOf(sortedByY[1]);
				}
				else {
					indexOfHealth = listOfImportantRectangles.indexOf(sortedByX[0]);
					indexOfMana = -1;
					indexOfManaShield = -1;
					indexOfManaAndManaShieldCombined = listOfImportantRectangles.indexOf(sortedByX[1]);
				}
			}
			else if (!combined && size == 3) {
				indexOfHealth = listOfImportantRectangles.indexOf(sortedByX[0]);
				rectangles.removeOne(sortedByX[0]);
				sortByXAndYRects(rectangles, sortedByX, sortedByY);
				indexOfMana = listOfImportantRectangles.indexOf(sortedByY[0]);
				indexOfManaShield = listOfImportantRectangles.indexOf(sortedByY[1]);
				indexOfManaAndManaShieldCombined = -1;
			}
			else if (!combined && size == 2) {
				indexOfManaAndManaShieldCombined = -1;
				indexOfManaShield = -1;
				bool isParallelStyle = sortedByY[0].y() == sortedByY[1].y();
				if (isParallelStyle){
					indexOfHealth = listOfImportantRectangles.indexOf(sortedByX[0]);
					indexOfMana = listOfImportantRectangles.indexOf(sortedByX[1]);
				}
				else {
					indexOfHealth = listOfImportantRectangles.indexOf(sortedByY[0]);
					indexOfMana = listOfImportantRectangles.indexOf(sortedByY[1]);
				}
			}
			else {
				return ERROR_IN_SETTING_POSITION_OF_INTERFACE;
			}

			for each (QRect var in sortedByX) 
				listOfImportantRectangles.removeOne(var);

			break;
		};
		case LEFT: {
			int size = indexesOfRectWithSlashVert.size();
			howTheyShouldBeRotated = 1;//  1*90degree to right
			QList<QRect> sortedByX, sortedByY, rectangles;
			for each (int var in indexesOfRectWithSlashVert)
				rectangles.push_back(listOfImportantRectangles[var]);
			sortByXAndYRects(rectangles, sortedByX, sortedByY);

			if (size == 2 && !combined) {
				//no mana shield
				int w = listOfImportantRectangles[indexesOfRectWithSlashVert[0]].width();
				//25 is more less width of big bar, smaller is half of its width
				bool isParallelStyle = sortedByX[0].x() != sortedByX[1].x();
				if (isParallelStyle) {
					indexOfHealth = listOfImportantRectangles.indexOf(sortedByX[0]);
					indexOfMana = listOfImportantRectangles.indexOf(sortedByX[1]);
					indexOfManaShield = -1;
					indexOfManaAndManaShieldCombined = -1;
				}
				else {
					indexOfHealth = listOfImportantRectangles.indexOf(sortedByY[0]);
					indexOfMana = listOfImportantRectangles.indexOf(sortedByY[1]);
					indexOfManaShield = -1;
					indexOfManaAndManaShieldCombined = -1;
				}
			}
			else if (size == 2 && combined) {//mana shield, DEFAULT, PARALLEL AND COMPACT STYLE
				indexOfMana = -1;
				indexOfManaShield = -1;
				indexOfManaAndManaShieldCombined = indexOfFrameWithManaShieldAndMana;
				indexesOfRectWithSlashVert.removeOne(indexOfFrameWithManaShieldAndMana);
				indexOfHealth = indexesOfRectWithSlashVert[0];
			}
			else if (size == 3 && !combined) {//mana shields, LARGE
				indexOfHealth = listOfImportantRectangles.indexOf(sortedByX[1]);
				indexOfMana = listOfImportantRectangles.indexOf(sortedByX[0]);
				indexOfManaShield = listOfImportantRectangles.indexOf(sortedByX[2]);
				indexOfManaAndManaShieldCombined = -1;
			}
			else
				return ERROR_IN_SETTING_POSITION_OF_INTERFACE;

			for each (QRect var in sortedByX)
				listOfImportantRectangles.removeOne(var);

			break;
		};
		case RIGHT: {
			int size = indexesOfRectWithSlashVert.size();
			howTheyShouldBeRotated = -1;//  -1*90degree to right
			QList<QRect> sortedByX, sortedByY, rectangles;
			for each (int var in indexesOfRectWithSlashVert)
				rectangles.push_back(listOfImportantRectangles[var]);

			sortByXAndYRects(rectangles, sortedByX, sortedByY);
			if (size == 2 && !combined) {//no mana shield
				bool isParallelStyle = sortedByY[0].y() == sortedByY[1].y();

				if (isParallelStyle) {
					indexOfHealth = listOfImportantRectangles.indexOf(sortedByX[0]);
					indexOfMana = listOfImportantRectangles.indexOf(sortedByX[1]);
					indexOfManaShield = -1;
					indexOfManaAndManaShieldCombined = -1;
				}
				else {
					indexOfHealth = listOfImportantRectangles.indexOf(sortedByY[0]);
					indexOfMana = listOfImportantRectangles.indexOf(sortedByY[1]);
					indexOfManaShield = -1;
					indexOfManaAndManaShieldCombined = -1;
				}
			}
			else if (size == 2 && combined) {//mana shield, DEFAULT, PARALLEL AND COMPACT STYLE
				indexOfMana = -1;
				indexOfManaShield = -1;
				indexOfManaAndManaShieldCombined = indexOfFrameWithManaShieldAndMana;
				indexesOfRectWithSlashVert.removeOne(indexOfFrameWithManaShieldAndMana);
				indexOfHealth = indexesOfRectWithSlashVert[0];
			}
			else if (size == 3 && !combined) {//mana shields, LARGE
				indexOfHealth = listOfImportantRectangles.indexOf(sortedByX[1]);
				indexOfManaAndManaShieldCombined = -1;
				indexOfMana = listOfImportantRectangles.indexOf(sortedByX[2]);
				indexOfManaShield = listOfImportantRectangles.indexOf(sortedByX[0]);
			}
			else {
				return ERROR_IN_SETTING_POSITION_OF_INTERFACE;
			}

			for each (QRect var in sortedByX)
				listOfImportantRectangles.removeOne(var);

			break;
		};
		default: {
			return ERROR_IN_SETTING_POSITION_OF_INTERFACE;
			break;
		}
	}
	return OK;
}

