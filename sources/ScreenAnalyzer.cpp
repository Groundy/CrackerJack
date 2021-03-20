#include "ScreenAnalyzer.h"


ScreenAnalyzer::ScreenAnalyzer(QObject *parent, VariablesClass *varClass)
	: QThread(parent){
	var = varClass;
}

ScreenAnalyzer::~ScreenAnalyzer(){
}

void ScreenAnalyzer::run() {	
	while (true) {
		if (!enableScreenAnalyzer) {
			msleep(6 * timeBetweenNextCheckingsOfScrennShotFolder);
			continue;
		}

		if (var->useAdvancedShearch)
			mainLoop();
		else
			SIMPLE_mainLoop();
	}
}

void ScreenAnalyzer::reCalibrate(){
	if (var->useAdvancedShearch) {
		if (var->caliState != var->DURING_CALIBRATION) {
			var->caliState = var->DURING_CALIBRATION;
			qDebug() << "Recalibration";
			calibrate();
		}
	}
	else {
		qDebug() << "Recalibration simple";
		SIMPLE_calibration();
	}
}

void ScreenAnalyzer::SIMPLE_mainLoop(){
	SIMPLE_calibration();
	msleep(this->timeBetweenNextCheckingsOfScrennShotFolder);
	bool endLoop = false;
	while (!endLoop){
		QImage scr;
		loadScreen(&scr);
		QImage healthBarToSend = scr.copy(healthBar);
		QImage manabarToSend = scr.copy(manaBar);
		var->healthBarImg = healthBarToSend;
		var->manaBarImg = manabarToSend;
		msleep(timeBetweenNextCheckingsOfScrennShotFolder);
	}
}

int ScreenAnalyzer::loadScreen(QImage* img){
	QString nameOfImgToCapture;
	int code = getNameOfLastTakenScreenShotForSure(nameOfImgToCapture, 15);
	if (code != OK)
		return code;
	QString pathToImg = pathToScreenFolder + QString("\\") + nameOfImgToCapture;
	bool openCorrectly = img->load(pathToImg);
	if (openCorrectly)
		return OK;
	else
		return CANT_LOAD_SCREEN_FROM_SCREENSHOT_FOLDER;
}

void ScreenAnalyzer::SIMPLE_calibration(){
	QImage fullScreen;
	loadScreen(&fullScreen);
	if (fullScreen == QImage()); //diag err
	const int distanceFromStartBarsToEndOfScreen = 170;
	int startPosX = fullScreen.width() - distanceFromStartBarsToEndOfScreen;
	int startPosY = 0;
	int width = distanceFromStartBarsToEndOfScreen;
	int height = fullScreen.height();
	if (startPosX < 0); //diag err img is smaller than 155 pix, but it should be
	QRect oneFourthOfScreen(startPosX, startPosY, width, height);
	QImage partOfFullScreen = fullScreen.copy(oneFourthOfScreen);

	QImage fullScreenBlue(partOfFullScreen);
	QImage fullScreenRed(partOfFullScreen);

	auto minRed = qRgb(140, 0, 0);
	auto maxRed = qRgb(255, 150, 150);

	auto minBlue = qRgb(45, 45, 140);
	auto maxBlue = qRgb(170, 170, 255);

	auto red = qRgb(255, 0, 0);
	auto blue = qRgb(0, 0, 255);

	Utilities::imgToOneColor(&fullScreenRed, minRed, maxRed, red, true);
	Utilities::imgToOneColor(&fullScreenBlue, minBlue, maxBlue, blue, true);

	Utilities::imgToBlackAndWhiteOneColor(&fullScreenRed,254);///
	Utilities::imgToBlackAndWhiteOneColor(&fullScreenBlue, 255);///


	//200 201 are chars defined in Utilites::getQmapWithCodes();
	QImage healthIconImg = Utilities::fromCharToImg(200);
	QImage manaIconImg = Utilities::fromCharToImg(201);

	QList<QPoint> healthIconList = Utilities::findStartPositionInImg(&healthIconImg, &fullScreenRed);
	QList<QPoint> manaIconList = Utilities::findStartPositionInImg(&manaIconImg, &fullScreenBlue);
	//can't find mana and health icon to analize
	if (healthIconList.size() != 1 || manaIconList.size() != 1);//diag err

	QPoint healthIconCorner = healthIconList[0];
	QPoint manaIconCorner = manaIconList[0];

	int healthIconCenterY = healthIconCorner.y() + healthIconImg.height() / 2;
	int manaIconCenterY = manaIconCorner.y() + manaIconImg.height() / 2;

	QPoint vectorFromCornerOfHealthIconToStartHealthBar(13,4);
	QPoint vectorFromCornerOfManaIconToStartManaBar(15,4);

	QPoint startOfHealthBar = vectorFromCornerOfHealthIconToStartHealthBar + healthIconCorner;
	QPoint startOfManaBar = vectorFromCornerOfManaIconToStartManaBar + manaIconCorner;

	int x_pos_start_health = fullScreen.width() - distanceFromStartBarsToEndOfScreen + startOfHealthBar.x();
	int x_pos_start_mana = fullScreen.width() - distanceFromStartBarsToEndOfScreen + startOfManaBar.x();
	int y_pos_start_health = startOfHealthBar.y();
	int y_pos_start_mana = startOfManaBar.y();
	int heightToCut = 1;
	int lenghtFromStartToEndOfBar = 92;
	
	QRect partOfBarThanShouldBeReturned_Healing(x_pos_start_health, y_pos_start_health, lenghtFromStartToEndOfBar, heightToCut);
	QRect partOfBarThanShouldBeReturned_Mana(x_pos_start_mana, y_pos_start_mana, lenghtFromStartToEndOfBar, heightToCut);

	healthBar = partOfBarThanShouldBeReturned_Healing;
	manaBar = partOfBarThanShouldBeReturned_Mana;

	//QImage helathBar = fullScreen.copy(partOfBarThanShouldBeReturned_Healing);
	//QImage manaBar = fullScreen.copy(partOfBarThanShouldBeReturned_Mana);

	//Utilities::saveImgToOutPutFolder(&helathBar, "he");///
	//Utilities::saveImgToOutPutFolder(&manaBar, "ma");///
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

int ScreenAnalyzer::getNameOfLastTakenScreenShotForSure(QString& toRet, int MaxTries) {
	toRet = "";
	for (int tries = 0; tries < MaxTries; tries++) {
		toRet = getNameOfLastTakenScreenShot();
		if (!toRet.isEmpty())
			return ERROR_CODE::OK;
		else 
			msleep(123);
	}
	return CANT_LOAD_SCREEN_FROM_SCREENSHOT_FOLDER;
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
	sleep(2);
	calibrate();
	while (enableScreenAnalyzer){
		QImage img;
		int openCorrectly = loadScreen(&img);
		if (openCorrectly == OK) {
			deleteScreenShotFolder();
			splitToPieces(&img);
		}
		msleep(timeBetweenNextCheckingsOfScrennShotFolder);
	}
}

int ScreenAnalyzer::calibrate(){
	//this fun looking for certain pixel sequences and save their locations for
	//further img splitting
	qDebug() << "ScreenAnalyzer::calibrate()";
	QImage img;
	int openCorrectly = loadScreen(&img);
	if (openCorrectly == OK) {
		Utilities::imgToBlackAndWhiteAllColors(&img, 240);
		int interfaceIsVert = determineManaHealthManaShieldPosVer(&img);
		int interfaceIsHor = determineManaHealthManaShieldPosHor(&img);
		if (interfaceIsVert != OK && interfaceIsHor != OK) {
			var->caliState = var->NOT_CALIBRATED;
			qDebug() << "Calibration failed";//diag err
			return NO_SLASHES_FOUND_IN_GAME_SCREEN;
		}
		int res = setImgRectsForHealthAnalyzerClass(&img);
		if(res == OK){
			var->caliState = var->CALIBRATED;
			qDebug() << "Calibration completed";
			return OK;
		}
		else {
			return res;
		}
	}
	else {
		var->caliState = var->NOT_CALIBRATED;
		qDebug() << "Calibration failed";
		return CANT_LOAD_SCREEN_FROM_SCREENSHOT_FOLDER;
	}
}

void ScreenAnalyzer::sortByXAndY(QList<QPoint>* points, QList<QPoint>* pointsSortedByX, QList<QPoint>* pointsSortedByY) {
	QMap<int, int> sortedByX; //x is key
	QMap<int, int> sortedByY; //y is key
	QList<QPoint> copyOfInput = *points;
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
	*pointsSortedByX = sortedByXToRet;
	*pointsSortedByY = sortedByYToRet;
}

int ScreenAnalyzer::determineManaHealthManaShieldPosVer(QImage* fullImg) {
	QImage slash = Utilities::fromCharToImg(47);
	QList<QPoint> slashesPos = Utilities::findStartPositionInImg(&slash, fullImg);
	if (slashesPos.size() == 0) 
		return NO_SLASHES_FOUND_IN_GAME_SCREEN;
	

	QSet<int> uncialXPosVer;
	QSet<int> uncialYPosVer;
	for each (QPoint var in slashesPos) {
		uncialXPosVer.insert(var.x());
		uncialYPosVer.insert(var.y());
	}
	int numberOfXPosVer = uncialXPosVer.size();
	int numberOfPosVer = uncialYPosVer.size();

	QList<QPoint>* sortedByX = new QList<QPoint>;
	QList<QPoint>* sortedByY = new QList<QPoint>;
	sortByXAndY(&slashesPos, sortedByX, sortedByY);

	const int halfWidthOfScreen = fullImg->width() / 2;
	const bool isManaShield = slashesPos.size() == 3 ? true : false;
	const int sizeX = sortedByX->size();
	const int sizeY = sortedByY->size();
	bool positionLeft = slashesPos.first().x() < halfWidthOfScreen;
	bool noPosAssigned = false;
	if (positionLeft){//Position LEFT
		howTheyShouldBeRotatedInRight = 1;
		if (isManaShield) {
			if (sizeX == 1 && sizeY == 3) {//compact or default style
				healthSlash = sortedByY->at(0);
				manaSlash = sortedByY->at(2);
				manaShieldSlash = sortedByY->at(1);
			}
			else if (sizeX == 3 && sizeY == 2) {//large style
				healthSlash = sortedByY->at(0);
				manaSlash = sortedByX->at(0);
				manaShieldSlash = sortedByX->at(2);
			}
			else if (sizeX == 2 && sizeY == 3) {//parallel style
				healthSlash = sortedByX->at(0);
				manaSlash = sortedByY->at(2);
				manaShieldSlash = sortedByY->at(0);
			}
			else {
				healthSlash = QPoint(-1, -1);
				manaSlash = QPoint(-1, -1);
				manaShieldSlash = QPoint(-1, -1);
				noPosAssigned = true;
			}
		}//isManaShield If
		else {
			manaShieldSlash = QPoint(-1, -1);
			if (sizeX == 1 && sizeY == 2) {//compact,default or style
				healthSlash = sortedByY->at(0);
				manaSlash = sortedByY->at(1);
			}
			else if (sizeX == 2 && sizeY == 1) {//parallel style
				healthSlash = sortedByX->at(0);
				manaSlash = sortedByX->at(1);
			}
			else {
				healthSlash = QPoint(-1, -1);
				manaSlash = QPoint(-1, -1);
				noPosAssigned = true;
			}
		}
	}//Position Lef If 
	else {//Position RIGHT
		howTheyShouldBeRotatedInRight = -1;
		if (isManaShield) {
			if (sizeX == 1 && sizeY == 3) {//compact or default style
				healthSlash = sortedByY->at(0);
				manaSlash = sortedByY->at(1);
				manaShieldSlash = sortedByY->at(2);
			}
			else if (sizeX == 3 && sizeY == 2) {//large style
				healthSlash = sortedByY->at(0);
				manaSlash = sortedByX->at(0);
				manaShieldSlash = sortedByX->at(2);
			}
			else if (sizeX == 2 && sizeY == 3) {//parallel style
				healthSlash = sortedByX->at(0);
				manaSlash = sortedByY->at(0);
				manaShieldSlash = sortedByY->at(2);
			}
			else{//else
				healthSlash = QPoint(-1, -1);
				manaSlash = QPoint(-1, -1);
				manaShieldSlash = QPoint(-1, -1);
				noPosAssigned = true;
			}
		}
		else {
			manaShieldSlash = QPoint(-1,-1);
			if (sizeX == 2 && sizeY == 1) {//compact,default or large style
				healthSlash = sortedByY->at(0);
				manaSlash = sortedByY->at(1);
			}
			else if (sizeX == 2 && sizeY == 3) {//parallel style
				healthSlash = sortedByX->at(0);
				manaSlash = sortedByX->at(1);
			}
			else{//else
				healthSlash = QPoint(-1, -1);
				manaSlash = QPoint(-1, -1);
				noPosAssigned = true;
			}
		}
	}

	delete sortedByX;
	delete sortedByY;
	if (noPosAssigned)
		return NO_POSTION_ASSIGNED_TO_SLASHES;
	else
		return OK;
}

int ScreenAnalyzer::splitToPieces(QImage* fullImg){
	//qDebug() << "ScreenAnalyzer::splitToPieces(QImage* fullImg)";
	bool manaShieldFound = !(manaShieldRectImg.x() < 0 || manaShieldRectImg.y() < 0);
	int width = fullImg->width();
	int height = fullImg->height();

	setImgRectsForHealthAnalyzerClass(fullImg);

	//preparing data
	QImage healthPieceImgToSave = fullImg->copy(healthRectImg);
	QImage manaPieceImgToSave = fullImg->copy(manaRectImg);
	QImage manaShieldPieceImgToSave;
	if (manaShieldFound) 
		manaShieldPieceImgToSave = fullImg->copy(manaShieldRectImg);
	//tmp
	Utilities::saveImgToOutPutFolder(&healthPieceImgToSave, "H");
	Utilities::saveImgToOutPutFolder(&manaPieceImgToSave, "M");
	Utilities::saveImgToOutPutFolder(&manaShieldPieceImgToSave, "MS");

	bool toBreak1 = healthPieceImgToSave.width() == width;
	bool toBreak2 = healthPieceImgToSave.height() == height;
	bool toBreak3 = manaPieceImgToSave.width() == width;
	bool toBreak4 = manaPieceImgToSave.height() == height;
	bool toBreak5 = manaShieldPieceImgToSave.width() == width;
	bool toBreak6 = manaShieldPieceImgToSave.height() == height;

	bool toBreakFinal;
	if (manaShieldFound)
		toBreakFinal = toBreak1 || toBreak2 || toBreak3 || toBreak4 || toBreak5 || toBreak6;
	else
		toBreakFinal = toBreak1 || toBreak2 || toBreak3 || toBreak4;

	if (!toBreakFinal) {
		*var->var_healthPieceImg = healthPieceImgToSave;
		*var->var_manaPieceImg = manaPieceImgToSave;
		if (manaShieldFound)
			*var->var_manaShieldPieceImg = manaShieldPieceImgToSave;
		var->rotationNeededForPointsAbove = howTheyShouldBeRotatedInRight;
		return true;
	}
	else {
		*var->var_healthPieceImg = QImage();
		*var->var_manaPieceImg = QImage();
		if (manaShieldFound)
			*var->var_manaShieldPieceImg = QImage();
		var->rotationNeededForPointsAbove = howTheyShouldBeRotatedInRight;
		return false;
	}

}

int ScreenAnalyzer::setImgRectsForHealthAnalyzerClass(QImage* full){
	bool foundHealthSlash = healthSlash.x() > 0 && healthSlash.y() > 0 ? true : false;
	bool foundManaSlash = manaSlash.x() > 0 && manaSlash.y() > 0 ? true : false;
	bool foundManaShieldSlash = manaShieldSlash.x() > 0 && manaShieldSlash.y() > 0 ? true : false;
	if (!(foundHealthSlash && foundManaSlash))
		return NO_SLASHES_FOUND_IN_GAME_SCREEN; //diag err

	QList<QPoint> centresOfImgs;
	centresOfImgs.push_back(healthSlash);
	centresOfImgs.push_back(manaSlash);
	if (foundManaShieldSlash)
		centresOfImgs.push_back(manaShieldSlash);

	QList<QRect*> imgsWithValues;
	imgsWithValues.push_back(&healthRectImg);
	imgsWithValues.push_back(&manaRectImg);
	if (foundManaShieldSlash)
		imgsWithValues.push_back(&manaShieldRectImg);

	const int maxIndex_X = full->width();
	const int maxIndex_Y = full->height();
	
	for (int i = 0; i < centresOfImgs.size(); i++) {
		int startPosition_X = 0;
		int startPosition_Y = 0;
		int width = 0;
		int height = 0;
		if (howTheyShouldBeRotatedInRight == 0){//hortizontal
			startPosition_X = centresOfImgs[i].x() - widthFromSlashChar;
			startPosition_Y = centresOfImgs[i].y() - heightOfHealthImgStr;
			height = heightOfHealthImgStr;
			width = 2 * widthFromSlashChar;
			if (startPosition_X < 0)
				startPosition_X = 0;
			if (startPosition_Y < 0)
				startPosition_Y = 0;
			if (startPosition_X + width >= maxIndex_X)
				width = maxIndex_X - startPosition_X;
			if (startPosition_Y + height >= maxIndex_Y)
				height = maxIndex_Y - startPosition_Y;
		}
		else{//vertical
			startPosition_X = centresOfImgs[i].x();
			startPosition_Y = centresOfImgs[i].y() - widthFromSlashChar;
			height = 2 * widthFromSlashChar;
			width = heightOfHealthImgStr;
			if (startPosition_X < 0)
				startPosition_X = 0;
			if (startPosition_Y < 0)
				startPosition_Y = 0;
			if (startPosition_X + width >= maxIndex_X)
				width = maxIndex_X - startPosition_X;
			if (startPosition_Y + height >= maxIndex_Y)
				height = maxIndex_Y - startPosition_Y;
		}
		QRect toAdd(startPosition_X, startPosition_Y, width, height);
		*imgsWithValues[i] = toAdd;
	}
	return OK;
}

int ScreenAnalyzer::determineManaHealthManaShieldPosHor(QImage *imgFull) {
	QImage slashes = Utilities::fromCharToImg(92);
	QList<QPoint> slashesPos = Utilities::findStartPositionInImg(&slashes, imgFull);
	if (slashesPos.size() == 0)
		return NO_SLASHES_FOUND_IN_GAME_SCREEN;

	QSet<int> uncialXPosHor;
	QSet<int> uncialYPosHor;
	for each (QPoint var in slashesPos) {
		uncialXPosHor.insert(var.x());
		uncialYPosHor.insert(var.y());
	}
	int numberOfXPosHor = uncialXPosHor.size();
	int numberOfYPosHor = uncialYPosHor.size();

	const int halfHeighOfScreen = imgFull->height() / 2;
	QList<QPoint>* sortedByX = new QList<QPoint>;
	QList<QPoint>* sortedByY = new QList<QPoint>;
	sortByXAndY(&slashesPos, sortedByX, sortedByY);

	bool magicShieldFound = (slashesPos.size() == 3);
	bool topPosition = (slashesPos.first().y() < halfHeighOfScreen);
	QPoint errPosition = QPoint(-1, -1);
	howTheyShouldBeRotatedInRight = 0;
	int sizeX = sortedByX->size();
	int sizeY = sortedByY->size();
	bool retNoPosition = false;
	if (topPosition) {
		if (magicShieldFound) {
			if (sizeX == 3 && sizeY == 2) {//parallel style
				manaSlash = sortedByX->at(0);
				healthSlash = sortedByX->at(1);
				manaShieldSlash = sortedByX->at(2);
			}
			else if (sizeX == 3 && sizeY == 1) {//default and compact style
				healthSlash = sortedByX->at(0);
				manaSlash = sortedByX->at(1);
				manaShieldSlash = sortedByX->at(2);
			}
			else if (sizeX == 2 && sizeY == 3) {//large style
				healthSlash = sortedByX->at(0);
				manaSlash = sortedByY->at(0);
				manaShieldSlash = sortedByY->at(2);
			}
			else
				retNoPosition = true;
		}
		else if (!magicShieldFound) {
			manaShieldSlash = errPosition;
			if (sizeX == 2) {//default, compact, large style
				healthSlash = sortedByX->at(0);
				manaSlash = sortedByX->at(1);
			}
			else if (sizeX == 1 && sizeY == 2) {//parallel style
				healthSlash = sortedByY->at(0);
				manaSlash = sortedByY->at(1);
			}
			else{
				healthSlash = errPosition;
				manaSlash = errPosition;
				retNoPosition = true;
			}

		}
	}
	else if(!topPosition){
		if (magicShieldFound) {
			if (sizeX == 3 && sizeY == 2) {//parallel style
				manaSlash = sortedByX->at(0);
				healthSlash = sortedByX->at(1);
				manaShieldSlash = sortedByX->at(2);
			}
			else if (sizeX == 3 && sizeY == 1) {//default and compact style
				healthSlash = sortedByX->at(0);
				manaSlash = sortedByX->at(1);
				manaShieldSlash = sortedByX->at(2);
			}
			else if (sizeX == 2 && sizeY == 3) {//large style
				healthSlash = sortedByX->at(0);
				manaSlash = sortedByY->at(0);
				manaShieldSlash = sortedByY->at(2);
			}
			else
				retNoPosition = true;
		}
		else if (!magicShieldFound) {
			manaShieldSlash = errPosition;
			if (sizeX == 2) {//default, compact, large style
				healthSlash = sortedByX->at(0);
				manaSlash = sortedByX->at(1);
			}
			else if (sizeX == 1 && sizeY == 2) {//parallel style
				healthSlash = sortedByY->at(0);
				manaSlash = sortedByY->at(1);
			}
			else {
				healthSlash = errPosition;
				manaSlash = errPosition;
				retNoPosition = true;
			}
		}
	}


	delete sortedByX;
	delete sortedByY;
	if (retNoPosition)
		return NO_POSTION_ASSIGNED_TO_SLASHES;
	else
		return OK;
}

