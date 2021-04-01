#include "ScreenAnalyzer.h"

ScreenAnalyzer::ScreenAnalyzer(QObject *parent, VariablesClass *varClass)
	: QThread(parent){
	var = varClass;
}

ScreenAnalyzer::~ScreenAnalyzer(){
}

void ScreenAnalyzer::run() {	
	QString in = "C:\\Users\\ADMIN\\Desktop\\screenshots";
	QString out = in + "2";
	TEST_setPositionHealthImhs(in, out);
	int breakpointInt;
	while (true) {
		if (!enableScreenAnalyzer) {
			msleep(6 * timeBetweenNextCheckingsOfScrennShotFolder);
			continue;
		}
		mainLoop();
	}
}

void ScreenAnalyzer::reCalibrate(){
	if (var->caliState != var->DURING_CALIBRATION) {
		var->caliState = var->DURING_CALIBRATION;
		qDebug() << "Recalibration";
		calibrate();
	}
}

int ScreenAnalyzer::loadScreen(QImage* img){
	QString nameOfImgToCapture;
	auto code = (ERROR_CODE)getNameOfLastTakenScreenShotForSure(nameOfImgToCapture, 55);
	if (code != OK)
		return code;
	QString pathToImg = pathToScreenFolder + QString("\\") + nameOfImgToCapture;
	auto openCorrectly = img->load(pathToImg);
	if (openCorrectly)
		return OK;
	else
		return CANT_LOAD_SCREEN_FROM_SCREENSHOT_FOLDER;
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
			msleep(100);
	}
	return CANT_LOAD_SCREEN_FROM_SCREENSHOT_FOLDER;
}

void ScreenAnalyzer::TEST_setPositionHealthImhs(QString pathToFolderWithDiffrentPositionsStylesScreen, QString pathToOutPutFolder){
	QImage combined, health, mana, manaShield;
	QDir directory(pathToFolderWithDiffrentPositionsStylesScreen);
	QStringList listOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
	// tmp i=0
	for (int i = 10; i < listOfFIles.size(); i++) {
		QString nameOfFile = listOfFIles[i];
		qDebug() << QString::number(i);
		QString pathToPng = pathToFolderWithDiffrentPositionsStylesScreen + "\\" + nameOfFile;
		QImage img;
		img.load(pathToPng);
		QList<QRect> importantRects;
		findWindowsOnScreen(img, &importantRects);
		bool areBarsCombined;
		int indHealth;
		int indMana;
		int indManaShield;
		int howTheyShouldBeRotated;
		setPositionHealthImgs(img, importantRects, &areBarsCombined, &indHealth, &indMana, &indManaShield, &howTheyShouldBeRotated);

		health = img.copy(importantRects[indHealth]);
		QString healthStr, manaStr, ManaShieldStr, combinedStr;

		Utilities::rotateImgToRight(&health, howTheyShouldBeRotated);
		Utilities::imgToBlackAndWhiteAllColors(&health, 240);
		healthStr = Utilities::imgWithStrToStr(&health);
		qDebug() << "Health: " + healthStr;

		if (indManaShield != -1){
			if (areBarsCombined) {
				QRect rect = importantRects.at(indManaShield);
				combined = img.copy(rect);

				Utilities::rotateImgToRight(&combined, howTheyShouldBeRotated);
				Utilities::imgToBlackAndWhiteAllColors(&combined, 240);
				QString combinedStr = Utilities::imgWithStrToStr(&combined);
				qDebug() << "combinedStr: " + combinedStr;
			}
			else{
				QRect rect = importantRects.at(indMana);
				mana = img.copy(rect);
				rect = importantRects.at(indManaShield);
				manaShield = img.copy(rect);

				Utilities::rotateImgToRight(&mana, howTheyShouldBeRotated);
				Utilities::imgToBlackAndWhiteAllColors(&mana, 240);
				QString manaStr = Utilities::imgWithStrToStr(&mana);
				qDebug() << "manaStr: " + manaStr;

				Utilities::rotateImgToRight(&manaShield, howTheyShouldBeRotated);
				Utilities::imgToBlackAndWhiteAllColors(&manaShield, 240);
				QString ManaShieldStr = Utilities::imgWithStrToStr(&manaShield);
				qDebug() << "ManaShieldStr: " + ManaShieldStr;
			}

		}
		QString name = QDateTime::currentDateTime().toString("mmss_mmm_");

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
		if(areBarsCombined)
			combined.save(finalName_c);
		else{
			mana.save(finalName_m);
			manaShield.save(finalName_ms);
		} 
	}
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
	sleep(4);
	calibrate();
	while (enableScreenAnalyzer){
		QImage img;
		auto openCorrectly = (ERROR_CODE)loadScreen(&img);
		if (var->caliState != var->CALIBRATED) 
			calibrate();
		if (openCorrectly == OK) {
			deleteScreenShotFolder();
			//auto res = (ERROR_CODE)splitToPieces(&img);
			auto res = OK;//tmp;
			if (res == OK && stateOfAnalyzer == false) {
				emit sendAllowenceToAnalyze(true);
				stateOfAnalyzer = true;
			}
			else if (res != OK && stateOfAnalyzer == true) {
				calibrate();
			 	emit sendAllowenceToAnalyze(false);
				stateOfAnalyzer = false;
			}
			else if (res != OK && stateOfAnalyzer == false) 
				calibrate();
			else
				;
		}
		msleep(timeBetweenNextCheckingsOfScrennShotFolder);
	}
}

int ScreenAnalyzer::calibrate(){
	qDebug() << "ScreenAnalyzer::calibrate()";
	QImage fullScreen;
	ERROR_CODE res1 = (ERROR_CODE)loadScreen(&fullScreen);

	if (res1 != OK) {
		qDebug() << "ScreenAnalyzer::calibrate() failed";
		return res1;
	}
	QList<QRect> importantRectangles;
	ERROR_CODE res2 = (ERROR_CODE)findWindowsOnScreen(fullScreen, &importantRectangles);
	if (res2 != OK) {
		qDebug() << "ScreenAnalyzer::findMiniMapWindow(QImage fullScreen) failed";
		return res2;
	}
	Frames frames;
	ERROR_CODE res3 = (ERROR_CODE)categorizeWindows(fullScreen, importantRectangles, &frames);

	return OK;
}

int ScreenAnalyzer::categorizeWindows(QImage fullscreen, QList<QRect> importantRectangles, Frames* frame){
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
	frame->miniMapFrame = importantRectangles.at(indexOfBiggestValue);
	importantRectangles.removeAt(indexOfBiggestValue);

	bool* isManaAndManaShieldTogether = false;
	int *indexOfHealth, *indexOfMana, *indexOfManaShield, *howTheyShouldBeRotated;
	setPositionHealthImgs(fullscreen, importantRectangles, isManaAndManaShieldTogether, indexOfHealth, indexOfMana, indexOfManaShield, howTheyShouldBeRotated);
}

void ScreenAnalyzer::sortByXAndYPoints(QList<QPoint>* points, QList<QPoint>* pointsSortedByX, QList<QPoint>* pointsSortedByY) {
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
			QList<QPoint> copyOfInput = *points;

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
			*pointsSortedByX = sortedByXToRet_TMP;
			*pointsSortedByY = sortedByYToRet_TMP;
			return;
		}//end of rewriting
	}//end of slightly correcting

	*pointsSortedByX = sortedByXToRet;
	*pointsSortedByY = sortedByYToRet;
}

void ScreenAnalyzer::sortByXAndYRects(QList<QRect> inputRects, QList<int>* indexesOfRectsSortedByPosX, QList<int>* indexesOfRectsSortedByPosY) {
	QList<QRect> sortedByX, sortedByY;
	for (int i = 0; i < inputRects.size(); i++) {
		QRect rect = inputRects[i];
		int startPosX = rect.x();
		int startPosY = rect.y();

		if (i == 0) {
			sortedByX.push_back(rect);
			sortedByY.push_back(rect);
			continue;
		}

		int size = sortedByX.size();
		for (int i = 0; i < size; i++){
			if (startPosX > sortedByX[i].x()) {
			
			}
		}
		size = sortedByY.size();
		for (int i = 0; i < size; i++) {

		}

	}
		int tmp = 4;
}

int ScreenAnalyzer::findWindowsOnScreen(QImage fullScreen, QList<QRect>* importantRectangles){
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

	*importantRectangles = frameToRet;
	if (frameToRet.size() > 0)
		return OK;
	else
		return NO_FRAMES_FOUND;
}

int ScreenAnalyzer::setPositionHealthImgs(QImage fullscreen, QList<QRect> listOfImportantRectangles, bool* manaAndManashieldAreToghere, int* indexOfHealth, int* indexOfMana, int* indexOfManaShield, int* howTheyShouldBeRotated) {
	QList<int> indexesOfRectWithSlashVert;
	QList<int> indexesOfRectWithSlashHor;
	QImage vertSlashes = Utilities::fromCharToImg(QChar(47));
	QImage horSlashes = Utilities::fromCharToImg(QChar(92));
	int indexOfFrameWithManaShieldAndMana = -1;
	for (size_t i = 0; i < listOfImportantRectangles.size(); i++) {
		QImage imgTmp = fullscreen.copy(listOfImportantRectangles[i]);
		Utilities::imgToBlackAndWhiteAllColors(&imgTmp, 250);

		QList<QPoint> pointsVert = Utilities::findStartPositionInImg(&vertSlashes, &imgTmp);
		if (pointsVert.size() == 0)
			;
		else if (pointsVert.size() == 1)
			indexesOfRectWithSlashVert.push_back(i);
		else if (pointsVert.size() == 2) {
			indexOfFrameWithManaShieldAndMana = i;
			indexesOfRectWithSlashVert.push_back(i);
		}

		QList<QPoint> pointsHor = Utilities::findStartPositionInImg(&horSlashes, &imgTmp);
		if (pointsHor.size() == 0)
			;
		else if (pointsHor.size() == 1)
			indexesOfRectWithSlashHor.push_back(i);
		else if (pointsHor.size() == 2) {
			indexOfFrameWithManaShieldAndMana = i;
			indexesOfRectWithSlashHor.push_back(i);
		}
	}
	if (indexesOfRectWithSlashVert.size() == 0 && indexesOfRectWithSlashHor.size() == 0)
		return NO_SLASHES_FOUND_IN_GAME_SCREEN;

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
	else
		return NO_SLASHES_FOUND_IN_GAME_SCREEN;

	bool combined = indexOfFrameWithManaShieldAndMana != -1;
	*manaAndManashieldAreToghere = combined;
	switch (position) {
	case TOP: case DOWN: {
		int size = indexesOfRectWithSlashHor.size();
		*howTheyShouldBeRotated = 0;
		if (combined && size == 2) {
			*manaAndManashieldAreToghere = true;
			*indexOfMana = indexOfFrameWithManaShieldAndMana;
			*indexOfManaShield = indexOfFrameWithManaShieldAndMana;
			indexesOfRectWithSlashHor.removeOne(indexOfFrameWithManaShieldAndMana);
			*indexOfHealth = indexesOfRectWithSlashHor[0];
		}
		else if (!combined && size == 3) {
			//QMap<surface, indexInList>, Qmap to sort them from smallest to biggest surface
			QMap<int, int> surfaces;
			for each (int index in indexesOfRectWithSlashHor) {
				QRect currentRect = listOfImportantRectangles[index];
				int surface = currentRect.width() * currentRect.height();
				surfaces.insert(surface, index);
			}
			int indexOfLargestRect = surfaces.last();
			indexesOfRectWithSlashHor.removeOne(indexOfLargestRect);
			//QMap<positionSortedByY, indexInList>, Qmap to sort them from smallest to biggest Y position
			QMap<int, int> sortedByY;
			for each (int index in indexesOfRectWithSlashHor) {
				int yPosition = listOfImportantRectangles[index].y();
				sortedByY.insert(yPosition, index);
			}
			*indexOfHealth = indexOfLargestRect;
			*indexOfMana = sortedByY.first();
			*indexOfManaShield = sortedByY.last();
		}
		else if (!combined && size == 2) {
			QRect firstRect = listOfImportantRectangles[indexesOfRectWithSlashHor[0]];
			QRect secondRect = listOfImportantRectangles[indexesOfRectWithSlashHor[1]];
			bool areRectInCorrectOrder = firstRect.x() < secondRect.x() ? true : false;
			if (areRectInCorrectOrder) {
				*indexOfHealth = indexesOfRectWithSlashHor[0];
				*indexOfMana = indexesOfRectWithSlashHor[1];
			}
			else{
				*indexOfHealth = indexesOfRectWithSlashHor[1];
				*indexOfMana = indexesOfRectWithSlashHor[0];
			}
			*indexOfManaShield = -1;
		}
		else
			return ERROR_IN_SETTING_POSITION_OF_INTERFACE;


		break;
	};
	case LEFT: {
		int size = indexesOfRectWithSlashVert.size();
		//  1*90degree to right
		*howTheyShouldBeRotated = 1;
		if (size == 2 && !combined) {
			//no mana shield
			int w = listOfImportantRectangles[indexesOfRectWithSlashVert[0]].width();
			//25 is more less width of big bar, smaller is half of its width
			bool isParallelStyle = w < 20 ? true : false;
			if (isParallelStyle) {
				//QMap<XPosition, indexOnList
				QMap<int, int> sortedByX;
				for each (int index in indexesOfRectWithSlashVert) {
					int startPos = listOfImportantRectangles[index].x();
					sortedByX.insert(startPos, index);
				}
				*indexOfHealth = sortedByX[0];
				*indexOfMana = sortedByX[1];
				*indexOfManaShield = -1;
					
			}
			else {
				//QMap<YPosition, indexOnList
				QMap<int, int> sortedByY;
				for each (int index in indexesOfRectWithSlashVert) {
					int startPos = listOfImportantRectangles[index].y();
					sortedByY.insert(startPos, index);
				}
				*indexOfHealth = sortedByY[0];
				*indexOfMana = sortedByY[1];
				*indexOfManaShield = -1;

			}
		}
		else if (size == 2 && combined) {
			//mana shield, DEFAULT, PARALLEL AND COMPACT STYLE
			*indexOfMana = indexOfFrameWithManaShieldAndMana;
			*indexOfManaShield = indexOfFrameWithManaShieldAndMana;
			indexesOfRectWithSlashVert.removeOne(indexOfFrameWithManaShieldAndMana);
			*indexOfHealth = indexesOfRectWithSlashVert[0];
		}
		else if (size == 3 && !combined) {
			//mana shields, LARGE

			//Qmap<startPosX, indexinList>
			QMap<int, int> startPosSortedByX;
			for each (int index in indexesOfRectWithSlashVert){
				int startPosX = listOfImportantRectangles[index].x();
				startPosSortedByX.insert(startPosX, index);
			}
			*indexOfHealth = startPosSortedByX[1];
			*indexOfMana = startPosSortedByX[0];
			*indexOfManaShield = startPosSortedByX[2];

		}
		else
			return ERROR_IN_SETTING_POSITION_OF_INTERFACE;
		break;
	};
	case RIGHT: {
		int size = indexesOfRectWithSlashVert.size();
		//  -1*90degree to right
		*howTheyShouldBeRotated = -1;
		if (size == 2 && !combined) {
			//no mana shield

			int w = listOfImportantRectangles[indexesOfRectWithSlashVert[0]].width();
			//25 is more less width of big bar, smaller is half of its width
			bool isParallelStyle = w < 20 ? true : false;
			if (isParallelStyle) {
				//QMap<XPosition, indexOnList
				QMap<int, int> sortedByX;
				for each (int index in indexesOfRectWithSlashVert) {
					int startPos = listOfImportantRectangles[index].x();
					sortedByX.insert(startPos, index);
				}
				*indexOfHealth = sortedByX.first();
				*indexOfMana = sortedByX.last();
				*indexOfManaShield = -1;

			}
			else {
				//QMap<YPosition, indexOnList
				QMap<int, int> sortedByY;
				for each (int index in indexesOfRectWithSlashVert) {
					int startPos = listOfImportantRectangles[index].y();
					sortedByY.insert(startPos, index);
				}
				*indexOfHealth = sortedByY.first();
				*indexOfMana = sortedByY.last();
				*indexOfManaShield = -1;

			}
		}
		else if (size == 2 && combined) {
			//mana shield, DEFAULT, PARALLEL AND COMPACT STYLE
			*indexOfMana = indexOfFrameWithManaShieldAndMana;
			*indexOfManaShield = indexOfFrameWithManaShieldAndMana;
			indexesOfRectWithSlashVert.removeOne(indexOfFrameWithManaShieldAndMana);
			*indexOfHealth = indexesOfRectWithSlashVert[0];
		}
		else if (size == 3 && !combined) {
			//mana shields, LARGE
			QList<int> sortedByX, sortedByY;
			QList<QRect> rectangles;

			for each (int var in indexesOfRectWithSlashVert)
				rectangles.push_back(listOfImportantRectangles[var]);
			
			sortByXAndYRects(listOfImportantRectangles, &sortedByX, &sortedByY);
			/*
			QMap<int, int> startPosSortedByX;
			for each (int index in indexesOfRectWithSlashVert) {
				int startPosX = listOfImportantRectangles[index].x();
				//prevent from adding item with same key value to qmap
				if (startPosSortedByX.contains(startPosX)) 
					startPosX++;
				startPosSortedByX.insert(startPosX, index);
			}
			const QList<int> keysOfKeyMap = startPosSortedByX.uniqueKeys();
			*indexOfHealth = startPosSortedByX.value(keysOfKeyMap[1]);
			*indexOfMana = startPosSortedByX.value(keysOfKeyMap[2]);
			*indexOfManaShield = startPosSortedByX.value(keysOfKeyMap[0]);
			*/
		}
		else
			return ERROR_IN_SETTING_POSITION_OF_INTERFACE;
		break;
	};
	default: {
		return ERROR_IN_SETTING_POSITION_OF_INTERFACE;
		break;
	}
	}
	return OK;
}

/*int ScreenAnalyzer::determineManaHealthManaShieldPosVer(QImage* fullImg) {
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
}*/

/*int ScreenAnalyzer::splitToPieces(QImage* fullImg){
	//qDebug() << "ScreenAnalyzer::splitToPieces(QImage* fullImg)";
	bool manaShieldFound = manaShieldRectImg.x() > 0 && manaShieldRectImg.y() > 0;
	int width = fullImg->width();
	int height = fullImg->height();

	setImgRectsForHealthAnalyzerClass(fullImg);

	//preparing data
	QImage healthPieceImgToSave = fullImg->copy(healthRectImg);
	QImage manaPieceImgToSave = fullImg->copy(manaRectImg);
	QImage manaShieldPieceImgToSave;
	if (manaShieldFound) 
		manaShieldPieceImgToSave = fullImg->copy(manaShieldRectImg);

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
		var->var_healthPieceImg = healthPieceImgToSave;
		var->var_manaPieceImg = manaPieceImgToSave;
		if (manaShieldFound)
			var->var_manaShieldPieceImg = manaShieldPieceImgToSave;
		else
			var->var_manaShieldPieceImg = QImage();//??is is good?
		var->rotationNeededForPointsAbove = howTheyShouldBeRotatedInRight;
		var->isManaShieldActive = manaShieldFound;
		return OK;
	}
	else {
		var->var_healthPieceImg = QImage();
		var->var_manaPieceImg = QImage();
		if (manaShieldFound)
			var->var_manaShieldPieceImg = QImage();
		var->rotationNeededForPointsAbove = howTheyShouldBeRotatedInRight;
		return NO_RECTANGLE_COPYING_WHOLE_SCREEN_TO_VAR;
	}
}*/

/*int ScreenAnalyzer::setImgRectsForHealthAnalyzerClass(QImage* full){
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
			startPosition_X = centresOfImgs[i].x() - WIDTH_FROM_SLASH;
			startPosition_Y = centresOfImgs[i].y();
 			height = HEIGHT_OF_HEALING_SLASH;
			width = 2 * WIDTH_FROM_SLASH;
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
			startPosition_Y = centresOfImgs[i].y() - WIDTH_FROM_SLASH;
			height = 2 * WIDTH_FROM_SLASH;
			width = HEIGHT_OF_HEALING_SLASH;
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
}*/

/*
int ScreenAnalyzer::calibrate_ManaHealthManaShield(QImage* fullScreenBlackAndWhite){
	//this fun looking for certain pixel sequences and save their locations for
	//further img splitting
	auto interfaceIsVert = (ERROR_CODE)determineManaHealthManaShieldPosVer(fullScreenBlackAndWhite);
	auto interfaceIsHor = (ERROR_CODE)determineManaHealthManaShieldPosHor(fullScreenBlackAndWhite);
	if (interfaceIsVert != OK && interfaceIsHor != OK) {
		var->caliState = var->NOT_CALIBRATED;
		qDebug() << "Calibration Health/Mana/ManaShield completed";
		return NO_SLASHES_FOUND_IN_GAME_SCREEN;
	}
	auto res = (ERROR_CODE)setImgRectsForHealthAnalyzerClass(fullScreenBlackAndWhite);
	if(res == OK){
		var->caliState = var->CALIBRATED;
		qDebug() << "Calibration Health/Mana/ManaShield completed";
		return OK;
	}
	else {
		qDebug() << "Calibration Health/Mana/ManaShield completed";
		return res;
	}
}
*/


