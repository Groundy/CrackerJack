#include "MinimapAnalyzer.h"

MinimapAnalyzer::MinimapAnalyzer(QObject* parent, std::shared_ptr<VariablesClass> var)
	: QThread(parent), var(var) {
}
MinimapAnalyzer::~MinimapAnalyzer() {};

void MinimapAnalyzer::run() {
	QMap<int, QImage*> floorsMaps;
	while (true) {
		msleep(SLEEP_TIME_BETWEEN_LOOPS);
		if (!var->getSettings().getKeepAnalyzeMiniMap()) {
			msleep(SLEEP_TIME_BETWEEN_LOOPS);
			continue;
		}
		QImage miniMap, miniMapLayer;
		var->getMiniMap().getImgMiniMap(miniMap);
		var->getMiniMap().getImgMiniMapLayer(miniMapLayer);
		if (miniMap.isNull() || miniMapLayer.isNull())
			continue;

		int currentLayer = getCurrentLayer(miniMapLayer);
		if (!floorsMaps.contains(currentLayer)) {
			QString path = PathResource::getPathToMergedColorMap(currentLayer);
			floorsMaps.insert(currentLayer, new QImage(path));
		}
		auto currentPosition = findPlayerPosition(miniMap, floorsMaps[currentLayer]);
		previousPosition = currentPosition;

		if (!currentPosition.isNull())
			sendPostitionsToGUI(QString::number(currentPosition.x()), QString::number(currentPosition.y()), QString::number(currentLayer));
		else
			sendPostitionsToGUI("?", "?", QString::number(currentLayer));

		Point3D toSet(currentPosition.x(), currentPosition.y(), currentLayer);
		if (toSet.isValid())
			var->getPosition().setPlayerPos(toSet);
		else
			var->getPosition().clear();
	}
}
QImage MinimapAnalyzer::setSliderImg() {
	QString path = PathResource::getPathToMiniMapSliderImg();
	QImage img;
	img.load(path);
	return img;
}
int MinimapAnalyzer::getCurrentLayer(const QImage& layerImg) {
	QList<QPoint> startPostions = ImgEditor::findStartPositionInImg(LAYER_SLIDER_IMG, layerImg);
	if (startPostions.size() != 1)
		return -100;

	const int HIGHEST_LAYER_Y_POS = 2;
	const int HEIGHT_DIFF_FOR_ONE_LAYER = 4;
	int yPosition = startPostions[0].y() - HIGHEST_LAYER_Y_POS;
	int layerDiffToHighestLayer = yPosition / HEIGHT_DIFF_FOR_ONE_LAYER;
	int layer = MAX_LAYER - layerDiffToHighestLayer;

	if (layer >= MIN_LAYER && layer <= MAX_LAYER)
		return layer;
	else
		return -100;
}
QVector<QRect> MinimapAnalyzer::getMiniMapPartsPostions() {
	//5 is player mark
	QVector<QRect> imgParts;
	imgParts.push_back(QRect(0, 0, 53, 54));//1
	imgParts.push_back(QRect(53, 0, 2, 52));//2
	imgParts.push_back(QRect(55, 0, 51, 54));//3
	imgParts.push_back(QRect(0, 54, 51, 2));//4
	imgParts.push_back(QRect(57, 54, 49, 2));//6
	imgParts.push_back(QRect(0, 56, 53, 53));//7
	imgParts.push_back(QRect(53, 58, 2, 51));//8
	imgParts.push_back(QRect(55, 56, 51, 53));//9

	return imgParts;
}
QList<QImage> MinimapAnalyzer::splitMiniMap(const QImage& wholeMiniMap) {
	if (wholeMiniMap.size() != PROPER_MINIMAP_IMG_SIZE)
		return QList<QImage>();

	QList<QImage> toRet = {};
	for each (QRect rect in MINI_MAP_PARTS_POSITIONS) {
		toRet.push_back(wholeMiniMap.copy(rect));
	}
	return toRet;
}
QPoint MinimapAnalyzer::findPlayerPosition(const QImage& miniMap, const QImage* wholeMap) {
	try{
		const QList<QImage> miniMapParts = splitMiniMap(miniMap);
		QList<QList<QPoint>> pointsFromThreads;
		for (size_t i = 0; i < miniMapParts.size(); i++)
			pointsFromThreads.push_back(QList<QPoint>());

		QList<ImgShearcher*> threads;
		QRect frame = getFrameToLookByPreviousPos();
		for (size_t i = 0; i < miniMapParts.size(); i++) {
			auto thread = new ImgShearcher(this, miniMapParts[i], *wholeMap, pointsFromThreads[i], frame);
			threads.push_back(thread);
			thread->start();
		}

		bool keepWaiting = false;
		do {
			keepWaiting = false;
			msleep(50);
			for (size_t i = 0; i < threads.size(); i++) {
				if (threads[i]->isRunning())
					keepWaiting = true;
				else {
					bool foundPoint = pointsFromThreads[i].size() == 1;
					if (foundPoint) {
						keepWaiting = false;
						break;
					}
				}
			}
		} while (keepWaiting);

		for (size_t i = 0; i < threads.size(); i++)
			threads[i]->terminate();
		msleep(5);
		for (size_t i = 0; i < threads.size(); i++)
			delete threads[i];

		for (size_t i = 0; i < pointsFromThreads.size(); i++) {
			auto currentList = &pointsFromThreads[i];
			if (currentList->size() != 1)
				continue;

			QSize vectorToPlayerPos = getVectorToPlayerFromImgPieceIndex(i);
			QPoint playerPos(vectorToPlayerPos.width() + currentList[0].first().x(), vectorToPlayerPos.height() + currentList[0].first().y());
			return playerPos;
		}
		return QPoint();
	}
	catch (const std::exception& e){
		Utilities::ring();
		var->logger.log("!!!!!!!!ERROR IN THREADS!!!!!!", true, true, true);
		return QPoint();
	}

}
QList<QPoint> MinimapAnalyzer::findStartPosOfImgMap(const QImage& imgToFind, const QImage& imgToSearchWithin, QRect frameInBigWindow) {
	try {
		const int WIDTH_SMALL_PIC = imgToFind.width();
		const int HEIGHT_SMALL_PIC = imgToFind.height();
		const int WIDTH_BIG_PIC = imgToSearchWithin.width();
		const int HEIGHT_BIG_PIC = imgToSearchWithin.height();

		if (imgToFind.isNull() || imgToSearchWithin.isNull())
			throw std::exception("Cant find postion, one of imgs is null");
		if (WIDTH_SMALL_PIC > WIDTH_BIG_PIC)
			throw std::exception("Cant find postion, Wrong imgs size");
		if (HEIGHT_SMALL_PIC > HEIGHT_BIG_PIC)
			throw std::exception("Cant find postion, Wrong imgs size");
		if (imgToFind.format() != imgToSearchWithin.format())
			throw std::exception("Cant find postion, wrong formats");
		if (frameInBigWindow.right() > WIDTH_BIG_PIC)
			throw std::exception("Cant find postion, Wrong frame size");
		if (frameInBigWindow.bottom() > HEIGHT_BIG_PIC)
			throw std::exception("Cant find postion, Wrong frame size");


		const int maxIndexToCheckX = frameInBigWindow.isEmpty() ? WIDTH_BIG_PIC - WIDTH_SMALL_PIC : frameInBigWindow.right();
		const int maxIndexToCheckY = frameInBigWindow.isEmpty() ? HEIGHT_BIG_PIC - HEIGHT_SMALL_PIC : frameInBigWindow.bottom();
		const int minIndexToCheckX = frameInBigWindow.isEmpty() ? 0 : frameInBigWindow.left();
		const int minIndexToCheckY = frameInBigWindow.isEmpty() ? 0 : frameInBigWindow.top();

		QList<QPoint> startPointsListToRet;
		for (int x = minIndexToCheckX; x <= maxIndexToCheckX; x++) {
			for (int y = minIndexToCheckY; y <= maxIndexToCheckY; y++) {
				uint pixSmallImg = imgToFind.pixel(0, 0);
				uint pixBigImg = imgToSearchWithin.pixel(x, y);
				if (pixSmallImg != pixBigImg)
					continue;

				bool foundPosition = true;
				for (int offsetX = 1; offsetX < WIDTH_SMALL_PIC; offsetX++) {
					for (int offsetY = 1; offsetY < HEIGHT_SMALL_PIC; offsetY++) {
						pixSmallImg = imgToFind.pixel(offsetX, offsetY);
						pixBigImg = imgToSearchWithin.pixel(x + offsetX, y + offsetY);
						if (pixBigImg == pixSmallImg)
							continue;
						//break both loops
						offsetX = WIDTH_SMALL_PIC;
						offsetY = HEIGHT_SMALL_PIC;
						foundPosition = false;
					}
				}
				if (foundPosition)
					startPointsListToRet.push_back(QPoint(x, y));

			}
		}
		return startPointsListToRet;
	}
	catch (const std::exception& e) {
		Logger::staticLog(e.what());
		return QList<QPoint>();
	}
}
QRect MinimapAnalyzer::getFrameToLookByPreviousPos() {
	if (previousPosition.isNull())
		return QRect();

	const int SIZE = 220 / 2;
	QRect frameToLookWithin = QRect(previousPosition.x() - SIZE, previousPosition.y() - SIZE, 2 * SIZE, 2 * SIZE);
	return frameToLookWithin;
}
QSize MinimapAnalyzer::getVectorToPlayerFromImgPieceIndex(int index) {
	QSize vectorToPlayerPos;
	switch (index)
	{
	case 0: vectorToPlayerPos = QSize(53, 54); break;//topLeft
	case 1: vectorToPlayerPos = QSize(0, 54); break;//top
	case 2: vectorToPlayerPos = QSize(-55, 54); break;//topRight
	case 3: vectorToPlayerPos = QSize(+53, 0); break;//left
	case 4: vectorToPlayerPos = QSize(-4, 0); break;//right
	case 5: vectorToPlayerPos = QSize(+53, -2); break;//downLeft
	case 6: vectorToPlayerPos = QSize(0, -4); break;//down
	case 7: vectorToPlayerPos = QSize(-2, -2); break;//downRight
	}
	return vectorToPlayerPos;
}




/*
QList<uint> MinimapAnalyzer::test_getAllPossibleMapColors(){
	QList<uint> toRet;
	toRet.push_back(RGBstruct(0, 0, 0).toUint());
	toRet.push_back(RGBstruct(0, 102, 0).toUint());
	toRet.push_back(RGBstruct(0, 204, 0).toUint());
	toRet.push_back(RGBstruct(51, 102, 153).toUint());
	toRet.push_back(RGBstruct(102, 102, 102).toUint());
	toRet.push_back(RGBstruct(153, 51, 0).toUint());
	toRet.push_back(RGBstruct(153, 102, 51).toUint());
	toRet.push_back(RGBstruct(153, 153, 153).toUint());
	toRet.push_back(RGBstruct(153, 255, 102).toUint());
	toRet.push_back(RGBstruct(204, 255, 255).toUint());
	toRet.push_back(RGBstruct(255, 51, 0).toUint());
	toRet.push_back(RGBstruct(255, 102, 0).toUint());
	toRet.push_back(RGBstruct(255, 204, 153).toUint());
	toRet.push_back(RGBstruct(255, 255, 0).toUint());
	toRet.push_back(RGBstruct(255, 255,255).toUint());
	return toRet;
}
void MinimapAnalyzer::test(){
}
QList<QRect> MinimapAnalyzer::test_fillListWithRectsPosOfMiniMapParts(){
	QList<QRect> toCut;
	toCut.push_back(QRect(0, 0, 53, 54));
	toCut.push_back(QRect(53, 0, 2, 52));
	toCut.push_back(QRect(55, 0, 51, 54));

	toCut.push_back(QRect(0, 54, 51, 2));
	toCut.push_back(QRect(57, 54, 49, 2));

	toCut.push_back(QRect(0, 56, 53, 53));
	toCut.push_back(QRect(53, 58, 2, 51));
	toCut.push_back(QRect(55, 56, 51, 53));
	return toCut;
}
QRect MinimapAnalyzer::test_cutImgToShearchFromDarkAndWater(QImage& img){
	//return Qrect that was cut;
	int cutLines_TOP = 0, cutLines_LEFT = 0, cutLines_DOWN = 0, cutLines_RIGHT = 0;
	const int WIDTH = img.width();
	const int HEIGHT = img.height();
	constexpr uint DARK = qRgb(0, 0, 0);
	constexpr uint WATER = qRgb(51,102,153);

	int x = 0;
	int y = 0;
	for (; x < WIDTH; x++) {
		for (; y < HEIGHT; y++) {
			uint pixCol = img.pixel(x, y);
			bool shouldBeChecked = pixCol != WATER && pixCol != DARK;
			if (shouldBeChecked) {
				cutLines_LEFT = x;
				x = WIDTH; //endOfLoop
				y = HEIGHT; //endOfLoop
			}
		}
	}
	bool AllPixShouldBeChecked = x == WIDTH && y == HEIGHT;
	if (AllPixShouldBeChecked) {
		QRect toRet{ WIDTH, HEIGHT, 0, 0 };
		img = QImage();
		return toRet;
	}

	for (int x = WIDTH - 1; x >= cutLines_LEFT; x--) {
		for (int y = 0; y < HEIGHT; y++) {
			uint pixCol = img.pixel(x, y);
			bool shouldBeChecked = pixCol != WATER && pixCol != DARK;
			if (shouldBeChecked) {
				cutLines_RIGHT = WIDTH - x - 1;
				x = -1;//endOfLoop
				y = HEIGHT;//endOfLoop
			}
		}
	}

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = cutLines_LEFT; x < WIDTH - cutLines_RIGHT -1; x++) {
			uint pixCol = img.pixel(x, y);
			bool shouldBeChecked = pixCol != WATER && pixCol != DARK;
			if (shouldBeChecked) {
				cutLines_TOP = y;
				x = WIDTH;//endOfLoop
				y = HEIGHT;//endOfLoop
			}
		}
	}

	for (int y = HEIGHT - 1; y >= cutLines_TOP; y--) {
		for (int x = cutLines_LEFT; x < WIDTH - cutLines_RIGHT -1; x++) {
			uint pixCol = img.pixel(x, y);
			bool shouldBeChecked = pixCol != WATER && pixCol != DARK;
			if (shouldBeChecked) {
				cutLines_DOWN = HEIGHT - y - 1;
				x = WIDTH;//endOfLoop
				y = -1;//endOfLoop
			}
		}
	}
	int anotherParametr_x = WIDTH - cutLines_RIGHT - cutLines_LEFT;
	int anotherParametr_y = HEIGHT - cutLines_TOP - cutLines_DOWN;

	int widthToCut = anotherParametr_x >= 0 ? anotherParametr_x : 0;
	int heightToCut = anotherParametr_y >= 0 ? anotherParametr_y : 0;
	QRect rectThatShouldBeCut{cutLines_LEFT, cutLines_TOP, widthToCut, heightToCut};
	img = img.copy(rectThatShouldBeCut);
	return rectThatShouldBeCut;
}
void MinimapAnalyzer::test_TEST_PERFORMANCE(){
	QImage map;
	QString pathToMap = "C:\\Users\\ADMIN\\Desktop\\" + QString("map2") + ".png";
	map.load(pathToMap);
	//auto o = test_cutImgToShearchFromDarkAndWater(map);
	int failed = 0;
	for (size_t i = 1; i < 1000; i++){
		QPair<QImage, QPoint> pair = test_cutRandomAreaFromMap(map);
		{
			QImage test = pair.first;
			test_cutImgToShearchFromDarkAndWater(test);
			if (test.width() == 0) {
				i--;
				continue;
			}
		}
		QImage imgCopy = pair.first;

		qint64 start = QDateTime::currentMSecsSinceEpoch();
		QList<QPoint> res2 = test_findPlayerOnMap(imgCopy, map);
		qint64 diff = QDateTime::currentMSecsSinceEpoch() - start;

		//QPoint res2 = test_findPlayerPositionByParts(imgCopy, map);
		//qDebug() << QString::number(i) + "---random point: " << pair.second << " found: " << res2;
		if (res2.isEmpty()) {
			failed++;
			//test_showImg(pair.first);
			qDebug() << QString::number(failed);
		}
		else {
			bool right = (res2.first() == pair.second);
			qDebug() << QString::number(i) << right << " diff:" + QString::number(diff);
			if (!right)
				failed++;//test_showImg(pair.first);
		}

	}
	int b = 5;
}
QPair<QImage, QPoint> MinimapAnalyzer::test_cutRandomAreaFromMap(QImage& map){
	QDateTime dateTimeObj;
	QRandomGenerator gen(dateTimeObj.currentMSecsSinceEpoch());
	int startPosX = gen.generate64() % (map.width() - WIDTH_OF_MAP_ONLY_AREA);
	int startPosY = gen.generate64() % (map.height() - HEIGHT_OF_WHOLE_PASSED_IMG);
	QRect randomRectFromMap(startPosX, startPosY, WIDTH_OF_MAP_ONLY_AREA, HEIGHT_OF_WHOLE_PASSED_IMG);
	QImage snipet = map.copy(randomRectFromMap);
	QPoint startPosition(startPosX,startPosY);
	QPair<QImage, QPoint> toRet{snipet, startPosition};
	return toRet;
}
QList<QImage> MinimapAnalyzer::test_splitMiniMapScreenToListWithoutCross(QImage& miniMapScreen){
	QList<QImage> toRet;
	for each (QRect rect in test_miniMapParts) {
		QImage imgToCut = miniMapScreen.copy(rect);
		toRet.push_back(imgToCut);
	}
	return toRet;
}
QPoint MinimapAnalyzer::test_findPlayerPositionByParts(QImage& snipet, QImage& map){
	bool errWrongSize = snipet.width() != WIDTH_OF_MAP_ONLY_AREA || snipet.height() != HEIGHT_OF_WHOLE_PASSED_IMG;
	if (errWrongSize) 
		return QPoint();
	
	QList<QImage> imgs = test_splitMiniMapScreenToListWithoutCross(snipet);
	for (size_t i = 0; i < 8; i++){
		QRect rect = test_cutImgToShearchFromDarkAndWater(imgs[i]);
		bool isEmpty = imgs[i].width() == 0 || imgs[i].height() == 0;
		if (isEmpty)
			continue;

		QList<QPoint> startPoints = test_findPlayerOnMap(imgs[i], map);
		if (startPoints.size() != 1)
			continue;

		QPoint playerPosOnWorldMap = startPoints.first() - rect.topLeft() - test_miniMapParts[i].topLeft();// +POSITION_OF_PLAYER_ON_MINIMAP;
		return playerPosOnWorldMap;
	}
	return QPoint();
}
QList<QPoint> MinimapAnalyzer::test_findPlayerOnMap(QImage& snipet, QImage& map){
	QList<QPoint> startPointsListToRet;
	QImage snipetCopy = snipet;
	QRect rect = test_cutImgToShearchFromDarkAndWater(snipetCopy);
	if (snipetCopy.width() == 0 || snipetCopy.height() == 0)
		return startPointsListToRet;

	const int MAX_PIX_IND_Y = snipetCopy.height() - 1;
	const int MAX_PIX_IND_X = snipetCopy.width() - 1;

	const int MAX_Y_INDEX_TO_CHECK = map.height() - snipet.height() - 1;
	const int MAX_X_INDEX_TO_CHECK = map.width() - snipet.width() - 1;

	for (int y = 0; y <= MAX_Y_INDEX_TO_CHECK; y++) {
		for (int x = 0; x <= MAX_X_INDEX_TO_CHECK; x++) {

			uint smallPixCol = snipetCopy.pixel(0, 0);
			uint bigPixCol = map.pixel(x, y);
			bool oneOfColoursIsForbidden = smallPixCol == RED_COLOR_ON_MAP || bigPixCol == RED_COLOR_ON_MAP;
			if (oneOfColoursIsForbidden) {
				bool pixMatch = smallPixCol == bigPixCol;
				if (!pixMatch)
					continue;
			}

			smallPixCol = snipetCopy.pixel(MAX_PIX_IND_X, 0);
			bigPixCol = map.pixel(x + MAX_PIX_IND_X, y);
			oneOfColoursIsForbidden = smallPixCol == RED_COLOR_ON_MAP || bigPixCol == RED_COLOR_ON_MAP;
			if (oneOfColoursIsForbidden) {
				bool pixMatch = smallPixCol == bigPixCol;
				if (!pixMatch)
					continue;
			}
			smallPixCol = snipetCopy.pixel(0, MAX_PIX_IND_Y);
			bigPixCol = map.pixel(x, y + MAX_PIX_IND_Y);
			oneOfColoursIsForbidden = smallPixCol == RED_COLOR_ON_MAP || bigPixCol == RED_COLOR_ON_MAP;
			if (oneOfColoursIsForbidden) {
				bool pixMatch = smallPixCol == bigPixCol;
				if (!pixMatch)
					continue;
			}
			smallPixCol = snipetCopy.pixel(MAX_PIX_IND_X, MAX_PIX_IND_Y);
			bigPixCol = map.pixel(x + MAX_PIX_IND_X, y + MAX_PIX_IND_Y);
			oneOfColoursIsForbidden = smallPixCol == RED_COLOR_ON_MAP || bigPixCol == RED_COLOR_ON_MAP;
			if (oneOfColoursIsForbidden) {
				bool pixMatch = smallPixCol == bigPixCol;
				if (!pixMatch)
					continue;
			}

			int x_TMP = rect.x(), y_TMP = rect.y();
			const int MAX_X = x_TMP + rect.width();
			const int MAX_Y = y_TMP + rect.height();
			bool imgMatch = false;
			for (; x_TMP < MAX_X; x_TMP++) {
				for (; y_TMP < MAX_Y; y_TMP++) {
					if (forbiddenPixPositions.contains(QPoint(x_TMP,y_TMP)))
						continue;
					smallPixCol = snipet.pixel(x_TMP, y_TMP);
					if (smallPixCol == RED_COLOR_ON_MAP)
						continue;
					bigPixCol = map.pixel(x + x_TMP, y + y_TMP);
					if (bigPixCol == RED_COLOR_ON_MAP)
						continue;
					bool pixAreDiffrent = smallPixCol != bigPixCol;
					if (pixAreDiffrent) {
						imgMatch = false;
						x_TMP = MAX_X + 10; //loopBreak
						y_TMP = MAX_Y + 10; //loopBreak
						continue;
					}
					imgMatch = true;
				}
			}
			if (imgMatch && x_TMP == MAX_X && y_TMP == MAX_Y)
				//startPointsListToRet.push_back(QPoint(x, y));
				return QList<QPoint> {QPoint(x,y)};
			
		}
	}

	return startPointsListToRet;
}
void MinimapAnalyzer::test_showImg(QImage img){
	QMessageBox msgBox;
	msgBox.setIconPixmap(QPixmap::fromImage(img));
	msgBox.exec();
}
QList<QPoint> MinimapAnalyzer::test_fillForbiddenPixPositions(){
	QList<QPoint> toRet;
	toRet.push_back(QPoint(51, 54));
	toRet.push_back(QPoint(51, 55));
	toRet.push_back(QPoint(52, 54));
	toRet.push_back(QPoint(52, 55));
	toRet.push_back(QPoint(53, 52));
	toRet.push_back(QPoint(53, 53));
	toRet.push_back(QPoint(53, 54));
	toRet.push_back(QPoint(53, 55));
	toRet.push_back(QPoint(53, 56));
	toRet.push_back(QPoint(53, 57));
	toRet.push_back(QPoint(54, 52));
	toRet.push_back(QPoint(54, 53));
	toRet.push_back(QPoint(54, 54));
	toRet.push_back(QPoint(54, 55));
	toRet.push_back(QPoint(54, 56));
	toRet.push_back(QPoint(54, 57));
	toRet.push_back(QPoint(55, 54));
	toRet.push_back(QPoint(55, 55));
	toRet.push_back(QPoint(56, 54));
	toRet.push_back(QPoint(56, 55));
	return toRet;
}
*/