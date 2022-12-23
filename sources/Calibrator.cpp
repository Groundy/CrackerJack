#include "Calibrator.h"
Calibrator::Calibrator(std::shared_ptr<VariablesClass> var)
	: var(var){
}
Calibrator::~Calibrator()
{
}

//public
bool Calibrator::calibrateBasicAreas(const QImage& fullscreen){
	QList<QRect> importantRectangles;
	bool windowsFound = findWindowsOnScreen(fullscreen, importantRectangles);
	if (!windowsFound)
		return false;

	bool windowsCategorized = categorizeWindows(fullscreen, importantRectangles);
	if(!windowsCategorized)
		return false;
	
	return true;
}
bool Calibrator::calibrateBattleArea(const QImage& fullscreen) {
	auto rectangles = getOutsideFramesOfOpenEntitiesOnSideBars(fullscreen);
	if (rectangles.size() == 0)
		return false;

	QImage battleListMark(PathResource::getPathToBattleList());
	if (battleListMark.isNull())
		return false;

	for each (QRect rect in rectangles){
		auto battleMarksPos = ImgEditor::findStartPositionInImg(battleListMark, fullscreen, rect);
		if (battleMarksPos.size() != 1)
			continue;

		var->getBattleList().setFrame(rect);
		return true;
	}

	var->getBattleList().setFrame(QRect());
	return false;
}
bool Calibrator::calibrateStoreButton(const QImage& fullImage) {
	QImage img(PathResource::getPathToStoreButton());
	QRect rect = fullImage.rect();
	rect.setLeft(rect.width() * 0.75);//shearch only witing last 25% of fulll screen, on right side
	QPoint storeButtonPosition = ImgEditor::findExactStartPositionInImg(img, fullImage, rect);
	if (storeButtonPosition.isNull())
		return false;
	QRect storeButtonRect(storeButtonPosition, img.size());
	var->getEquipment().setStoreRect(storeButtonRect);
	return true;
}
void Calibrator::test(QString pathToFilesWithScreens) {
	auto var = std::shared_ptr<VariablesClass>();
	int totalTime = 0;
	try {
		QStringList namesOfScreenShots;
		QDir screenShotFolder(pathToFilesWithScreens);
		QStringList listOfFIlesNames = screenShotFolder.entryList(QDir::Files);

		for each (QString fileName in listOfFIlesNames) {
			QString pathToFile = screenShotFolder.absoluteFilePath(fileName);
			QImage fullScreen;
			bool loaded = fullScreen.load(pathToFile);
			if (!loaded)
				throw std::exception("cant load img!");

			auto calibrator = Calibrator(var);
			auto start = QDateTime::currentMSecsSinceEpoch();
			bool categroized = calibrator.calibrateBasicAreas(fullScreen);
			auto time = QDateTime::currentMSecsSinceEpoch() - start;
			if (categroized)
				qDebug() << "analyzed in : " + QString::number(time);
			else {
				QString msg = QString("cant sort windows in %1").arg(fileName);
				throw std::exception(msg.toStdString().c_str());
			}
			totalTime += time;
		}
		qDebug() << "avg time : " + QString::number(totalTime / listOfFIlesNames.size());
	}
	catch (const std::exception& e) {
		qDebug() << e.what();
	}
}

//private
Calibrator::SlashesIndexes Calibrator::getIndexesOfImgsWithSlashes(const QImage& fullScreen, const QList<QRect>& importantFrames) {
	const QImage slashYImg = ImgEditor::fromCharToImg(QChar(47));
	const QImage slashXImg = ImgEditor::fromCharToImg(QChar(92));
	SlashesIndexes indexes;
	for (size_t i = 0; i < importantFrames.size(); i++) {
		QImage imgTmp = fullScreen.copy(importantFrames[i]);
		ImgEditor::imgToBlackAndWhiteAllColors(imgTmp, 250);

		QList<QPoint> pointsX = ImgEditor::findStartPositionInImg(slashXImg, imgTmp);
		if (pointsX.size() == 1)
			indexes.slashesX.push_back(i);
		else if (pointsX.size() == 2) {
			indexes.combinedIndex = i;
			indexes.slashesX.push_back(i);
		}
		if (indexes.isValid())
			break;
		

		QList<QPoint> pointsY = ImgEditor::findStartPositionInImg(slashYImg, imgTmp);
		if (pointsY.size() == 1)
			indexes.slashesY.push_back(i);
		else if (pointsY.size() == 2) {
			indexes.combinedIndex = i;
			indexes.slashesY.push_back(i);
		}
		if (indexes.isValid())
			break;

	}
	return indexes;
}
Calibrator::Indexes Calibrator::getIndexesOfHealthManaBars(const QImage& fullscreen, const QList<QRect>& listOfImportantRectangles) {
	try {
		Indexes indexes;
		SlashesIndexes slashesIndexes = getIndexesOfImgsWithSlashes(fullscreen, listOfImportantRectangles);
		if (!slashesIndexes.isValid())
			throw std::exception("No enough frames with slashes on full screen");
		indexes.combined = slashesIndexes.combinedIndex;

		int SlashesXSize = slashesIndexes.sizeX();
		int SlashesYSize = slashesIndexes.sizeY();
		bool foundSlsahesX = SlashesXSize >= 2 && SlashesYSize == 0;
		bool foundSlsahesY = SlashesYSize >= 2 && SlashesXSize == 0;
		if (!foundSlsahesY && !foundSlsahesX)
			throw std::exception("Wrong slsashes frame in configuration!");

		BarsPostion position;
		QPoint midOfScreen(fullscreen.width() / 2, fullscreen.height() / 2);
		if (foundSlsahesY) {
			QRect firstRectangle = listOfImportantRectangles[slashesIndexes.slashesY[0]];
			position = firstRectangle.x() < midOfScreen.x() ? LEFT : RIGHT;
		}
		else {
			QRect firstRectangle = listOfImportantRectangles[slashesIndexes.slashesX[0]];
			position = firstRectangle.y() < midOfScreen.y() ? TOP : DOWN;
		}

		ManaShieldType shieldType;
		if (indexes.combined != -1)
			shieldType = COMBINED;
		else {
			if (SlashesXSize == 3 || SlashesYSize == 3)
				shieldType = SEPARATE;
			else if (SlashesXSize == 2 || SlashesYSize == 2)
				shieldType = NO_SHIELD;
		}

		switch (position) {
		case LEFT: {
			indexes.rotation = 1;
			QList<QRect> sortedByX, sortedByY, rectangles;
			for each (int var in slashesIndexes.slashesY)
				rectangles.push_back(listOfImportantRectangles[var]);
			sortByXY(rectangles, sortedByX, sortedByY);
			switch (shieldType) {
			case NO_SHIELD: {
				//25 is more less width of big bar, smaller is half of its width
				int w = listOfImportantRectangles[slashesIndexes.slashesY[0]].width();
				indexes.shield = -1;
				indexes.combined = -1;
				bool isParallelStyle = sortedByX[0].x() != sortedByX[1].x();
				if (isParallelStyle) {
					indexes.health = listOfImportantRectangles.indexOf(sortedByX[0]);
					indexes.mana = listOfImportantRectangles.indexOf(sortedByX[1]);
				}
				else {
					indexes.health = listOfImportantRectangles.indexOf(sortedByY[0]);
					indexes.mana = listOfImportantRectangles.indexOf(sortedByY[1]);
				}
				break;
			}
			case COMBINED: {
				indexes.mana = -1;
				indexes.shield = -1;
				indexes.health = slashesIndexes.slashesY[1];
				break;
			}
			case SEPARATE: {
				indexes.health = listOfImportantRectangles.indexOf(sortedByX[1]);
				indexes.mana = listOfImportantRectangles.indexOf(sortedByX[0]);
				indexes.shield = listOfImportantRectangles.indexOf(sortedByX[2]);
				indexes.combined = -1;
				break;
			}
			}
			break;
		}
		case RIGHT: {
			indexes.rotation = -1;
			QList<QRect> sortedByX, sortedByY, rectangles;
			for each (int var in slashesIndexes.slashesY)
				rectangles.push_back(listOfImportantRectangles[var]);
			sortByXY(rectangles, sortedByX, sortedByY);
			switch (shieldType) {
			case NO_SHIELD: {
				bool isParallelStyle = sortedByY[0].y() == sortedByY[1].y();
				indexes.shield = -1;
				indexes.combined = -1;
				if (isParallelStyle) {
					indexes.health = listOfImportantRectangles.indexOf(sortedByX[0]);
					indexes.mana = listOfImportantRectangles.indexOf(sortedByX[1]);
				}
				else {
					indexes.health = listOfImportantRectangles.indexOf(sortedByY[0]);
					indexes.mana = listOfImportantRectangles.indexOf(sortedByY[1]);
				}
				break;
			}
			case COMBINED: {
				indexes.mana = -1;
				indexes.shield = -1;
				indexes.health = slashesIndexes.slashesY[1];
				break;
			}
			case SEPARATE: {
				indexes.health = listOfImportantRectangles.indexOf(sortedByX[1]);
				indexes.combined = -1;
				indexes.mana = listOfImportantRectangles.indexOf(sortedByX[2]);
				indexes.shield = listOfImportantRectangles.indexOf(sortedByX[0]);
				break;
			}
			}
			break;
		}
		case DOWN: case TOP: {
			indexes.rotation = 0;
			QList<QRect> sortedByX, sortedByY, rectangles;
			for each (int var in slashesIndexes.slashesX)
				rectangles.push_back(listOfImportantRectangles[var]);
			sortByXY(rectangles, sortedByX, sortedByY);
			switch (shieldType) {
			case NO_SHIELD: {
				indexes.combined = -1;
				indexes.shield = -1;
				bool isParallelStyle = sortedByY[0].y() == sortedByY[1].y();
				if (isParallelStyle) {
					indexes.health = listOfImportantRectangles.indexOf(sortedByX[0]);
					indexes.mana = listOfImportantRectangles.indexOf(sortedByX[1]);
				}
				else {
					indexes.health = listOfImportantRectangles.indexOf(sortedByY[0]);
					indexes.mana = listOfImportantRectangles.indexOf(sortedByY[1]);
				}
				break;
			}
			case COMBINED: {
				bool isParallelStyle = sortedByY[0].y() != sortedByY[1].y();
				indexes.mana = -1;
				indexes.shield = -1;
				if (isParallelStyle) {
					indexes.health = listOfImportantRectangles.indexOf(sortedByY[0]);
					indexes.combined = listOfImportantRectangles.indexOf(sortedByY[1]);
				}
				else {
					indexes.health = listOfImportantRectangles.indexOf(sortedByX[0]);
					indexes.combined = listOfImportantRectangles.indexOf(sortedByX[1]);
				}
				break;
			}
			case SEPARATE: {
				indexes.health = listOfImportantRectangles.indexOf(sortedByX[0]);
				rectangles.removeOne(sortedByX[0]);
				sortByXY(rectangles, sortedByX, sortedByY);
				indexes.mana = listOfImportantRectangles.indexOf(sortedByY[0]);
				indexes.shield = listOfImportantRectangles.indexOf(sortedByY[1]);
				indexes.combined = -1;
				break;
			}
			}
			break;
		}
		}
		return indexes;
	}
	catch (const std::exception& e) {
		qDebug() << e.what();
		return Indexes();
	}
}
bool Calibrator::categorizeWindows(const QImage& fullscreen, QList<QRect>& importantRectangles) {
	//4 cause 1-minimap 2-gameScreen 3-health 4-mana, those 4 have to be found
	if (importantRectangles.size() < 4)
		return false;

	//game Frame
	{
		QRect biggestRect;
		int biggestSurface = 0;
		for (size_t i = 0; i < importantRectangles.size(); i++){
			int surf = importantRectangles[i].width() * importantRectangles[i].height();
			if (surf > biggestSurface) {
				biggestSurface = surf;
				biggestRect = importantRectangles[i];
			}
		}
		var->getGameWindow().setFrameMainGameWindow(biggestRect);
		importantRectangles.removeOne(biggestRect);
	}

	//indexOfHealth, indexOfMana, indexOfManaShield, howTheyShouldBeRotated, indexOfCombinedBox;
	{
		Indexes indexes = getIndexesOfHealthManaBars(fullscreen, importantRectangles);
		int size = importantRectangles.size();
		if (!indexes.isValid(size))
			return false;

		var->getVitalitty().setRotation(indexes.rotation);
		QVector<QRect> rectsToDelete;
		if (indexes.healthFound(size)) {
			var->getVitalitty().setHealthArea(importantRectangles[indexes.health]);
			rectsToDelete.push_back(importantRectangles[indexes.health]);
		}

		if (indexes.manaFound(size)) {
			var->getVitalitty().setManaArea(importantRectangles[indexes.mana]);
			rectsToDelete.push_back(importantRectangles[indexes.mana]);
		}

		if (indexes.shieldFound(size)) {
			var->getVitalitty().setMSArea(importantRectangles[indexes.shield]);
			rectsToDelete.push_back(importantRectangles[indexes.shield]);
		}

		if (indexes.combinedFound(size)) {
			var->getVitalitty().setCombinedArea(importantRectangles[indexes.combined]);
			rectsToDelete.push_back(importantRectangles[indexes.combined]);
		}

		for each (QRect var in rectsToDelete)
			importantRectangles.removeOne(var);
	}

	//miniMap Frame
	{
		QList<QRect> sortedX, sortedY;
		sortByXY(importantRectangles, sortedX, sortedY);
		var->getMiniMap().setFrameMiniMap(sortedX.last());
		importantRectangles.removeOne(sortedX.last());
	}

	return true;
}
bool Calibrator::findWindowsOnScreen(const QImage& fullScreen, QList<QRect>& importantRectangles) {
	const int WIDTH = fullScreen.width();
	const int HEIGHT = fullScreen.height();
	const uint MIN_ACCEPTABLE_VAL = 17;
	const uint MAX_ACCEPTABLE_VAL = 29;

	QList<QPoint> startOfFrames = getStartOfPossibleFrames(fullScreen, MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL);
	QList<QRect> areasWithoutFrames = getAreasInsideFrames(fullScreen, startOfFrames, 10);
	QList<QRect> areasWithinFullFrames = filterAreasCoveredByFrameFromBottomRight(fullScreen, areasWithoutFrames);
	importantRectangles = areasWithinFullFrames;
	return areasWithoutFrames.size() >= 4;
}
void Calibrator::sortByXY(QList<QPoint>& points, QList<QPoint>& sortedByX, QList<QPoint>& sortedByY) {
	QMultiMap<int, QPoint> mapX;
	QMultiMap<int, QPoint> mapY;
	for each (QPoint point in points) {
		mapX.insert(point.x(), point);
		mapY.insert(point.y(), point);
	}
	sortedByX = mapX.values();
	sortedByY = mapY.values();
}
void Calibrator::sortByXY(QList<QRect>& inputRects, QList<QRect>& sortedByX, QList<QRect>& sortedByY) {
	QMultiMap<int, QRect> mapX;
	QMultiMap<int, QRect> mapY;
	for each (QRect rect in inputRects) {
		mapX.insert(rect.x(), rect);
		mapY.insert(rect.y(), rect);
	}
	sortedByX = mapX.values();
	sortedByY = mapY.values();
}
QList<QPoint> Calibrator::getStartOfPossibleFrames(const QImage& fullScreen, int minVal, int maxVal){
	const int WIDTH = fullScreen.width();
	const int HEIGHT = fullScreen.height();
	const uint MIN_ACCEPTABLE_VAL = 17;
	const uint MAX_ACCEPTABLE_VAL = 29;
	QList<QPoint> topLeftCorners;
	for (size_t x = 1; x < WIDTH - 2; x++) {
		for (size_t y = 1; y < HEIGHT - 2; y++) {
			bool isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x, y), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
			if (!isPixelOfFrame) continue;

			isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x - 1, y - 1), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, false);
			if (isPixelOfFrame) continue;

			isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 1, y + 1), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, false);
			if (isPixelOfFrame) continue;

			isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 1, y + 2), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, false);
			if (isPixelOfFrame) continue;

			isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 2, y + 1), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, false);
			if (isPixelOfFrame) continue;

			isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 2, y + 2), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, false);
			if (isPixelOfFrame) continue;

			isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 1, y), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
			if (!isPixelOfFrame) continue;

			isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 2, y), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
			if (!isPixelOfFrame) continue;

			isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x, y + 1), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
			if (!isPixelOfFrame) continue;

			isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x, y + 2), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
			if (!isPixelOfFrame) continue;

			topLeftCorners.push_back(QPoint(x, y));
		}
	}
	return topLeftCorners;
}
QList<QRect> Calibrator::getAreasInsideFrames(const QImage& fullScreen, const QList<QPoint>& startOfFrames, const int MIN_DIM) {
	const int WIDTH = fullScreen.width();
	const int HEIGHT = fullScreen.height();
	const uint MIN_ACCEPTABLE_VAL = 17;
	const uint MAX_ACCEPTABLE_VAL = 29;
	QList<QRect> frameToRet;
	for each (QPoint startPoint in startOfFrames) {
		int currentWidth = 0;
		for (size_t x = startPoint.x(); x < WIDTH; x++) {
			uint color = fullScreen.pixel(x, startPoint.y());
			bool isPixOfFrame = ImgEditor::isItPixelFromFrame(color, MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
			if (isPixOfFrame)
				currentWidth++;
			else
				break;
		}
		if (currentWidth < MIN_DIM)
			continue;

		int currentHeight = 0;
		for (size_t y = startPoint.y(); y < HEIGHT; y++) {
			uint color = fullScreen.pixel(startPoint.x(), y);
			bool isPixOfFrame = ImgEditor::isItPixelFromFrame(color, MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
			if (isPixOfFrame)
				currentHeight++;
			else
				break;
		}
		if (currentHeight < MIN_DIM)
			continue;

		int x = startPoint.x() + 1;
		int y = startPoint.y() + 1;
		int w = currentWidth - 2;
		int h = currentHeight - 2;
		frameToRet.push_back(QRect(x, y, w, h));
	}
	return frameToRet;
}
QList<QRect> Calibrator::filterAreasCoveredByFrameFromBottomRight(const QImage& fullScreen, const QList<QRect>& areas) {
	const int WIDTH = fullScreen.width();
	const int HEIGHT = fullScreen.height();
	const uint MIN_ACCEPTABLE_VAL = 100;
	const uint MAX_ACCEPTABLE_VAL = 130;
	QList<QRect> toRet;
	for each (QRect area in areas) {
		if (area.bottom() + 1 >= fullScreen.height())
			throw std::exception("a");
		if (area.right() + 1 >= fullScreen.width())
			throw std::exception("b");

		//botom of area
		bool skip = false;
		for (size_t x = 0; x < area.width(); x++) {
			uint color = fullScreen.pixel(area.left() + x, area.bottom() + 1);
			bool framPix = ImgEditor::isItPixelFromFrame(color, MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
			if (!framPix) {
				skip = true;
				break;
			}
		}
		if (skip)
			continue;

		//right 
		for (size_t y = 0; y < area.height(); y++) {
			uint color = fullScreen.pixel(area.right() + 1, area.top() + y);
			bool framPix = ImgEditor::isItPixelFromFrame(color, MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
			if (!framPix) {
				skip = true;
				break;
			}
		}
		if (skip)
			continue;

		toRet.push_back(area);
	}
	return toRet;
}
QList<QRect> Calibrator::getOutsideFramesOfOpenEntitiesOnSideBars(const QImage& wholeScreen) {
	const QImage startOfSideBarEntity(PathResource::getPathToSideBarEntityStart());
	if (startOfSideBarEntity.isNull())
		return {};

	QList<QRect> outerFramesOfSideBarsEntity = {};
	QList<QPoint> startPoints = ImgEditor::findStartPositionInImg(startOfSideBarEntity, wholeScreen);
	const QImage endOfSideBarEntity(PathResource::getPathToSideBarEntityEnd());
	for each (QPoint currPt in startPoints){
		QRect rectToSheachWithIn(currPt, QSize(wholeScreen.width() - currPt.x(), wholeScreen.height() - currPt.y()));
		auto endsOfFrame = ImgEditor::findStartPositionInImg(endOfSideBarEntity, wholeScreen, rectToSheachWithIn);
		if (endsOfFrame.size() == 0)
			continue;
		QList<QPoint> sortedByX, sortedByY;
		sortByXY(endsOfFrame, sortedByX, sortedByY);
		QPoint pt = sortedByY[0];
		QSize size(pt.x() + endOfSideBarEntity.width() - currPt.x(), pt.y() + endOfSideBarEntity.height() - currPt.y());
		QRect toAdd(currPt, size);
		//wholeScreen.copy(toAdd).save(QString("C:\\Users\\szczy\\Desktop\\inne\\%1.png").arg(QString::number(QDateTime::currentMSecsSinceEpoch())));
		outerFramesOfSideBarsEntity.push_back(toAdd);
	}
	return outerFramesOfSideBarsEntity;
}

/*
bool Calibrator::getRectsFromProfile(QList<QRect>& importRectsFromProf) {
	importRectsFromProf.clear();
	//they have to be;
	bool isEmpty_gameFram = var->frames.gameFrame.isEmpty();
	bool isEmpty_healthFrame = var->frames.healthFrame.isEmpty();
	bool isEmpty_minimapFrame = var->frames.miniMapFrame.isEmpty();

	if (isEmpty_gameFram || isEmpty_healthFrame || isEmpty_minimapFrame)
		return false;

	//one of them have to be;
	bool isEmpty_manaFrame = var->frames.manaFrame.isEmpty();
	bool isEmpty_combinedFrame = var->frames.combinedFrame.isEmpty();

	if (isEmpty_manaFrame && isEmpty_combinedFrame)
		return false;

	//one that can be or not
	bool isEmpty_manaShieldFrame = var->frames.manaShieldFrame.isEmpty();

	importRectsFromProf.push_back(var->frames.gameFrame);
	importRectsFromProf.push_back(var->frames.healthFrame);
	importRectsFromProf.push_back(var->frames.miniMapFrame);

	if (!isEmpty_manaFrame)
		importRectsFromProf.push_back(var->frames.manaFrame);
	if (!isEmpty_combinedFrame)
		importRectsFromProf.push_back(var->frames.combinedFrame);
	if (!isEmpty_manaShieldFrame)
		importRectsFromProf.push_back(var->frames.manaShieldFrame);

	return true;
}
*/
/*
void Calibrator::TEST_setPositionHealthImhs(QString pathToFolderWithDiffrentPositionsStylesScreen, QString pathToOutPutFolder) {
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

		ImgEditor::rotateImgToRight(health, howTheyShouldBeRotated);
		ImgEditor::imgToBlackAndWhiteAllColors(health, 240);
		healthStr = ImgEditor::imgWithStrToStr(health);
		qDebug() << "Health: " + healthStr;

		bool thereIsCombinedBox = indCombined != -1;
		bool manaFound = indMana != -1;
		bool manaShieldFound = indManaShield != -1;
		if (thereIsCombinedBox) {
			QRect rect = importantRects.at(indCombined);
			combined = img.copy(rect);

			ImgEditor::rotateImgToRight(combined, howTheyShouldBeRotated);
			ImgEditor::imgToBlackAndWhiteAllColors(combined, 240);
			QString combinedStr = ImgEditor::imgWithStrToStr(combined);
			qDebug() << "combinedStr: " + combinedStr;
		}
		else if (!thereIsCombinedBox && manaShieldFound && manaFound) {
			QRect rect = importantRects.at(indMana);
			mana = img.copy(rect);
			rect = importantRects.at(indManaShield);
			manaShield = img.copy(rect);

			ImgEditor::rotateImgToRight(mana, howTheyShouldBeRotated);
			ImgEditor::imgToBlackAndWhiteAllColors(mana, 240);
			QString manaStr = ImgEditor::imgWithStrToStr(mana);
			qDebug() << "manaStr: " + manaStr;

			ImgEditor::rotateImgToRight(manaShield, howTheyShouldBeRotated);
			ImgEditor::imgToBlackAndWhiteAllColors(manaShield, 240);
			QString ManaShieldStr = ImgEditor::imgWithStrToStr(manaShield);
			qDebug() << "ManaShieldStr: " + ManaShieldStr;
		}
		else if (!thereIsCombinedBox && !manaShieldFound && manaFound) {
			QRect rect = importantRects.at(indMana);
			mana = img.copy(rect);

			ImgEditor::rotateImgToRight(mana, howTheyShouldBeRotated);
			ImgEditor::imgToBlackAndWhiteAllColors(mana, 240);
			QString manaStr = ImgEditor::imgWithStrToStr(mana);
			qDebug() << "manaStr: " + manaStr;
		}
		else
			return;

		//QString name = QDateTime::currentDateTime().toString("mmss_mmm_");
		QString name = "";

		QString h = "Health_";
		QString m = "Mana_";
		QString c = "Combined_";
		QString ms = "ManaShield_";
		QString path = pathToOutPutFolder + "\\_";
		QString finalName_h = path + name + h + nameOfFile;
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
*/
/*void Calibrator::getMapWithPotionsImgCodes(QMap<QString, QString>& name_code_light, QMap<QString, QString>& name_code_dark) {
	name_code_light.clear();
	name_code_light.insert("Great Health Potion", "17_2_#139#7#0_#50#3#0_#170#6#0_#109#6#0_#199#2#0_#205#4#0_#228#2#0_#222#2#0_#247#33#39_#242#2#0_#243#105#129_#244#33#39_#236#86#105_#219#58#66_#157#8#0_#163#38#39_#183#5#0_#160#5#0_#171#5#0_#199#6#0_#144#7#0_#160#6#0_#146#7#0_#124#9#0_#147#8#0_#137#7#0_#150#3#0_#142#7#0_#146#4#0_#147#4#0_#117#3#0_#106#4#0_#117#4#0_#101#4#0_");
	name_code_light.insert("Great Mana Potion", "18_2_#136#46#100_#46#12#35_#158#53#118_#110#37#82_#175#69#125_#180#67#130_#184#68#134_#168#61#123_#227#123#174_#184#67#132_#213#113#165_#199#93#151_#148#44#112_#163#53#122_#136#40#103_#131#40#99_#123#27#100_#115#30#91_#131#40#99_#147#40#114_#137#44#103_#135#40#103_#143#48#106_#138#46#102_#140#44#105_#131#41#98_#131#48#96_#121#32#95_#90#18#72_#128#44#95_#59#5#51_#58#5#49_#63#5#54_#55#5#47_#18#2#15_#13#1#11_");
	name_code_light.insert("Great Spirit Potion", "4_2_#168#115#59_#191#146#89_#151#92#34_#188#143#89_#128#68#0_#119#63#0_#121#58#0_#112#59#0_");
	name_code_light.insert("Health Potion", "9_2_#201#27#21_#177#35#0_#248#68#117_#194#4#13_#252#75#114_#220#17#74_#208#10#38_#195#7#55_#151#0#1_#158#0#10_#126#16#0_#130#9#0_#131#19#0_#134#16#0_#93#13#0_#92#17#0_#83#14#0_#98#16#0_");
	name_code_light.insert("Mana Potion", "9_2_#201#21#194_#177#0#142_#199#68#248_#184#4#194_#213#75#252_#162#17#220_#179#10#208_#147#7#195_#149#0#151_#148#0#158_#126#0#110_#130#0#120_#131#0#112_#134#0#118_#93#0#80_#92#0#75_#83#0#69_#98#0#82_");
	name_code_light.insert("Strong Health Potion", "13_2_#197#0#10_#177#38#0_#238#0#66_#194#0#7_#254#0#28_#229#0#46_#231#0#16_#234#0#17_#206#2#1_#187#0#2_#228#1#3_#170#8#1_#215#4#2_#136#11#0_#148#10#0_#165#0#21_#143#10#0_#143#10#0_#143#10#0_#136#11#0_#136#11#0_#112#15#0_#92#18#0_#92#18#0_#92#18#0_#92#18#0_");
	name_code_light.insert("Strong Mana Potion", "13_2_#183#0#126_#177#0#77_#213#0#246_#194#0#133_#221#0#250_#218#0#209_#206#0#167_#193#0#176_#153#0#96_#158#0#106_#143#0#82_#139#0#79_#145#0#80_#136#0#78_#136#0#78_#165#0#128_#143#0#83_#143#0#83_#143#0#83_#136#0#78_#136#0#78_#112#0#58_#92#0#42_#92#0#42_#92#0#42_#92#0#42_");
	name_code_light.insert("Supreme Health Potion", "3_3_#212#2#0_#221#33#39_#53#43#40_#211#2#0_#174#6#0_#157#8#0_#48#44#44_#48#44#44_#48#44#44_");
	name_code_light.insert("Ultimate Health Potion", "3_3_#221#33#39_#236#86#105_#219#58#66_#174#6#0_#157#8#0_#163#38#39_#48#44#44_#48#44#44_#160#5#0_");
	name_code_light.insert("Ultimate Mana Potion", "4_2_#213#113#165_#199#93#151_#148#44#112_#163#53#122_#136#40#103_#131#40#99_#48#44#44_#115#30#91_");
	name_code_light.insert("Ultimate Spirit Potion", "4_2_#168#115#59_#191#146#89_#151#92#34_#188#143#89_#128#68#0_#119#63#0_#48#44#44_#112#59#0_");

	name_code_dark.clear();
	name_code_dark.insert("Great Mana Potion", "18_2_#41#14#30_#14#4#10_#47#16#35_#33#11#24_#52#21#37_#54#20#39_#55#20#40_#50#18#37_#68#37#52_#55#20#39_#63#34#49_#59#28#45_#44#13#33_#49#16#36_#41#12#31_#39#12#30_#37#8#30_#34#9#27_#39#12#30_#44#12#34_#41#13#31_#40#12#31_#43#14#32_#41#14#30_#42#13#31_#39#12#29_#39#14#29_#36#10#28_#27#5#21_#38#13#28_#18#1#15_#17#1#15_#19#1#16_#16#1#14_#5#1#4_#4#0#3_");
	name_code_dark.insert("Great Spirit Potion", "4_2_#50#34#18_#57#44#27_#45#27#10_#56#43#27_#38#20#0_#35#19#0_#36#17#0_#33#18#0_");
	name_code_dark.insert("Health Potion", "9_2_#60#8#6_#53#10#0_#74#20#35_#58#1#4_#75#22#34_#66#5#22_#62#3#11_#58#2#16_#45#0#0_#47#0#3_#38#5#0_#39#3#0_#39#6#0_#40#5#0_#28#4#0_#27#5#0_#25#4#0_#29#5#0_");
	name_code_dark.insert("Mana Potion", "9_2_#60#6#58_#53#0#42_#59#20#74_#55#1#58_#63#22#75_#48#5#66_#53#3#62_#44#2#58_#44#0#45_#44#0#47_#38#0#33_#39#0#36_#39#0#33_#40#0#35_#28#0#24_#27#0#22_#25#0#21_#29#0#24_");
	name_code_dark.insert("Strong Health Potion", "13_2_#59#0#3_#53#11#0_#71#0#20_#58#0#2_#76#0#8_#68#0#14_#69#0#5_#70#0#5_#61#1#0_#56#0#1_#68#0#1_#51#2#0_#64#1#1_#41#3#0_#44#3#0_#49#0#6_#43#3#0_#43#3#0_#43#3#0_#41#3#0_#41#3#0_#33#4#0_#27#5#0_#27#5#0_#27#5#0_#27#5#0_");
	name_code_dark.insert("Strong Mana Potion", "13_2_#55#0#38_#53#0#23_#63#0#73_#58#0#40_#66#0#75_#65#0#62_#61#0#50_#58#0#52_#46#0#29_#47#0#32_#43#0#24_#41#0#24_#43#0#24_#41#0#23_#41#0#23_#49#0#38_#43#0#25_#43#0#25_#43#0#25_#41#0#23_#41#0#23_#33#0#17_#27#0#13_#27#0#13_#27#0#13_#27#0#13_");
	name_code_dark.insert("Supreme Health Potion", "3_3_#63#1#0_#66#10#12_#16#13#12_#63#1#0_#52#2#0_#47#2#0_#14#13#13_#14#13#13_#14#13#13_");
	name_code_dark.insert("Ultimate Health Potion", "3_3_#66#10#12_#70#26#31_#65#17#20_#52#2#0_#47#2#0_#49#11#12_#14#13#13_#14#13#13_#48#1#0_");
	name_code_dark.insert("Ultimate Mana Potion", "4_2_#63#34#49_#59#28#45_#44#13#33_#49#16#36_#41#12#31_#39#12#30_#14#13#13_#34#9#27_");
	name_code_dark.insert("Ultimate Spirit Potion", "4_2_#50#34#18_#57#44#27_#45#27#10_#56#43#27_#38#20#0_#35#19#0_#14#13#13_#33#18#0_");
	name_code_dark.insert("Great Health Potion", "17_2_#41#2#0_#15#1#0_#51#2#0_#32#2#0_#59#1#0_#61#1#0_#68#1#0_#66#1#0_#74#10#12_#72#1#0_#72#31#38_#73#10#12_#70#26#31_#65#17#20_#47#2#0_#49#11#12_#55#1#0_#48#1#0_#51#1#0_#59#2#0_#43#2#0_#48#2#0_#44#2#0_#37#3#0_#44#2#0_#41#2#0_#45#1#0_#42#2#0_#44#1#0_#44#1#0_#35#1#0_#32#1#0_#35#1#0_#30#1#0_");
}*/
/*bool Calibrator::fillRectWithPotsInVarClass(QImage& fullscreen, QStringList nameOfPotionsToLookFor) {
	QStringList foundPotions;
	QList<QRect> rectsWithPotions;
	findPotionsOnBottomBar(nameOfPotionsToLookFor, foundPotions, rectsWithPotions, fullscreen);

	bool allPotionsFound = true;
	for each (QString potNameThatShouldBeFound in nameOfPotionsToLookFor) {
		bool found = foundPotions.contains(potNameThatShouldBeFound);
		if (!found)
			allPotionsFound = false;
	}
	if (!allPotionsFound)
		return false;

	for (size_t i = 0; i < rectsWithPotions.size(); i++) {
		QRect rectToAdd = rectsWithPotions[i];
		QString nameOfPot = foundPotions[i];
		var->potionName_rectPosOnScreen_map[nameOfPot] = rectToAdd;//[CHANGE]
	}

	return true;
}*/
/*bool Calibrator::findPotionsOnBottomBar(QStringList namesOfPotionsToFind, QStringList& namesOfPotionosFound, QList<QRect>& rectsWithFoundPots, QImage& bottomBarImg) {
	bool wrongInput = namesOfPotionsToFind.size() == 0 || bottomBarImg.width() <= 0 || bottomBarImg.height() <= 0;
	if (wrongInput) {
		//Logger::logPotenialBug("Wrong(empty) input", "Calibrator", "findPotionsOnBottomBar");
		return false;
	}

	QMap<QString, QString> map_light, map_dark;
	getMapWithPotionsImgCodes(map_light, map_dark);

	QStringList allPotionsNames = map_light.keys();
	for each (QString var in allPotionsNames) {
		bool shouldBeRemoved = !namesOfPotionsToFind.contains(var);
		if (shouldBeRemoved) {
			map_light.remove(var);
			map_dark.remove(var);
		}
	}

	QList<QImage> potionsImg_light, potionsImg_dark;
	for each (QString potName in map_light.keys()) {
		QString code_light = map_light[potName];
		QString code_dark = map_dark[potName];
		QImage imgToAdd;
		imgToAdd = ImgEditor::getImageFromAdvancedCode(code_light);
		potionsImg_light.push_back(imgToAdd);
		imgToAdd = ImgEditor::getImageFromAdvancedCode(code_dark);
		potionsImg_dark.push_back(imgToAdd);
	}

	QStringList namesOfFoundPotions;
	QList<QPoint> potionsOfStartingPotions;
	for (size_t i = 0; i < map_dark.size(); i++) {
		QList<QImage*> potionToLookFor;
		potionToLookFor.push_back(&potionsImg_light[i]);
		potionToLookFor.push_back(&potionsImg_dark[i]);
		QList<QPoint> pointsOfStart = ImgEditor::findStartPositionInImg_mulitpeImgs(potionToLookFor, bottomBarImg);
		if (pointsOfStart.size() == 1) {
			potionsOfStartingPotions.push_back(pointsOfStart.first());
			namesOfFoundPotions.push_back(map_dark.keys()[i]);
		}
		//else if(pointsOfStart.size() > 1)
			//Logger::logPotenialBug("More than one occurance of Potion in Image", "Calibrator", "findPotionsOnBottomBar");
	}

	const int WIDTH_HEIGHT_PIC = 32;
	const RGBstruct LEFT_BOTTOM_COLOUR(66, 66, 66);
	const RGBstruct PIX_ABOVE_COLOUR(112, 113, 113);
	const int WIDTH = bottomBarImg.width();
	const int HEIGHT = bottomBarImg.height();
	QList<QRect> rectToRet;
	for (size_t i = 0; i < potionsOfStartingPotions.size(); i++) {
		QPoint& someWhereInPic = potionsOfStartingPotions[i];
		int curr_x = someWhereInPic.x();
		int curr_y = someWhereInPic.y();

		int min_x = max(curr_x - WIDTH_HEIGHT_PIC, 0);
		int min_y = curr_y;
		int max_x = min(curr_x + WIDTH_HEIGHT_PIC, WIDTH);
		int max_y = min(curr_y + WIDTH_HEIGHT_PIC, HEIGHT);

		for (size_t x = min_x; x < max_x; x++) {
			for (size_t y = min_y; y < max_y; y++) {
				RGBstruct rgb(bottomBarImg.pixel(x, y));
				RGBstruct pixAboveRgb(bottomBarImg.pixel(x, y - 1));
				bool goodPix1 = rgb == LEFT_BOTTOM_COLOUR;
				bool goodPix2 = pixAboveRgb == PIX_ABOVE_COLOUR;
				if (goodPix1 && goodPix2) {
					someWhereInPic.setX(x);
					someWhereInPic.setY(y);
					x = max_x;//break
					y = max_y;//break
				}
			}
		}

		int width = WIDTH_HEIGHT_PIC;
		int height = WIDTH_HEIGHT_PIC / 3;
		int start_x = someWhereInPic.x() + 1;
		int start_y = someWhereInPic.y() - WIDTH_HEIGHT_PIC + 2 * height + 1;

		bool err = start_x < 0 || (start_x + width) > WIDTH || start_y < 0 || (start_y + height) > HEIGHT;
		//if(err)
			//Logger::logPotenialBug("Out of range cordinates of rect to cut, only part of it will be cut", "Calibrator", "findPotionsOnBottomBar");

		QRect toAdd(start_x, start_y, width, height);
		rectToRet.push_back(toAdd);
	}

	namesOfPotionosFound = namesOfFoundPotions;
	rectsWithFoundPots = rectToRet;

	return true;
}*/