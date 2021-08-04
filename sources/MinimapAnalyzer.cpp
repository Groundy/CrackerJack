#include "MinimapAnalyzer.h"
typedef QList<int> Histogram;

MinimapAnalyzer::MinimapAnalyzer(){
}
MinimapAnalyzer::MinimapAnalyzer(VariablesClass* var) {
	test_varClass = var;
	test();
}

MinimapAnalyzer::~MinimapAnalyzer(){
}

QList<Position3D> MinimapAnalyzer::findPlayerPositionOnMap(QImage& miniMapSnippet, ShearchArea shearchArea)
{
	return QList<Position3D>();
}

QList<Position3D> MinimapAnalyzer::findPlayerPositionOnMap(QImage& miniMapSnippet, int floor)
{
	return QList<Position3D>();
}

QList<Position3D> MinimapAnalyzer::findPlayerPositionOnMap(QImage& miniMapSnippet)
{
	return QList<Position3D>();
}

QString MinimapAnalyzer::getNameOfMapFileToLoad(int floor, bool regularMapType){
	QString type = regularMapType ? "map" : "path";
	QString number = QString::number(floor);
	if (number.size() == 1)
		number.push_front(QString::number(0));

	QString toRet = "floor-" + number + "-" + type + ".png";
	return toRet;
}

QString MinimapAnalyzer::setPathToFolderMap(){
	//todo przerobic w final version
	QDir dir = QDir::current();
	dir.cd("maps");
	return dir.absolutePath();
}

QImage MinimapAnalyzer::setSliderImg(){
	const QString SLIDER_IMG_CODE = "4_5_#155#155#155_#101#101#101_#101#101#101_#101#101#101_#37#38#38_#101#101#101_#101#101#101_#101#101#101_#101#101#101_#101#101#101_#101#101#101_#101#101#101_#101#101#101_#101#101#101_#101#101#101_#120#120#120_#120#120#120_#120#120#120_#120#120#120_#120#120#120_";
	return Utilities::getImageFromAdvancedCode(SLIDER_IMG_CODE);
}

uint MinimapAnalyzer::getFloorNumber(){
	uint toRet = 0;

	QRect rectWithSliderOnly = QRect(109, 0, minimapWithSlider.width() - 109, minimapWithSlider.height());
	QImage imgWithSliderOnly = minimapWithSlider.copy(rectWithSliderOnly);
	QList<QPoint> sliderPosition = Calibrator::findStartPositionInImg(SLIDER_IMG, imgWithSliderOnly);
	if (sliderPosition.size() != 1) {
		Logger::logPotenialBug("Slider of floor not found or found multiple times", "getFloorNumber", "MinimapAnalyzer");
		return 0;
	}

	int yPosOfSlider = sliderPosition.first().y();
	const int LOWEST_POSITION = 44;
	const int ONE_FLOOR_DIFFRENCE = 4;
	toRet = (yPosOfSlider - LOWEST_POSITION) / 4;


	bool resultIsNotPossible = toRet < 0 || toRet > 15;
	if (resultIsNotPossible) {
		Logger::logPotenialBug("Result is not possible", "getFloorNumber", "MinimapAnalyzer");
		return 0;
	}
	return toRet;
}

QList<uint> MinimapAnalyzer::getAllPossibleMapColors(){
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
	QImage map, snipet;
	map.load("C:\\Users\\ADMIN\\Desktop\\map2.png");
	while (true) {
		if (test_varClass->wholeImg.width() > 0) {
			snipet = test_varClass->wholeImg.copy(1753, 5, 106, 109);
			test_varClass->wholeImg = QImage();
		}
		else {
			Sleep(100);
			continue;
		}
		qint64 start = QDateTime::currentMSecsSinceEpoch();
		auto res = test_findPlayerOnMap(snipet, map);
		qint64 diff = QDateTime::currentMSecsSinceEpoch() - start;
		qDebug() << res << " diff:" << QString::number(diff);

	}
}

QList<QRect> MinimapAnalyzer::fillListWithRectsPosOfMiniMapParts(){
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
		//todo log
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
		return QPoint();		//todo logg
	
	QList<QImage> imgs = test_splitMiniMapScreenToListWithoutCross(snipet);
	for (size_t i = 0; i < 8; i++){
		QRect rect = test_cutImgToShearchFromDarkAndWater(imgs[i]);
		bool isEmpty = imgs[i].width() == 0 || imgs[i].height() == 0;
		if (isEmpty) {
			//() << "Part " + QString::number(i) + " is empty.";
			continue;
		}

		QList<QPoint> startPoints = test_findPlayerOnMap(imgs[i], map);
		//qDebug() << "Part " + QString::number(i) + " found: " + QString::number(startPoints.size()) + " similarities";
		if (startPoints.size() != 1) {
			continue;
		}

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












/*
void MinimapAnalyzer::testHistogramFunc(){
	QImage map, snipet;
	QString pathToMap = "C:\\Users\\ADMIN\\Desktop\\map.png";
	QString pathToSnipet = "C:\\Users\\ADMIN\\Desktop\\snipet.png";
	bool ok1 = map.load(pathToMap);
	bool ok2 = snipet.load(pathToSnipet);

	auto t = splitScreenToFitImg(map, snipet);
	Histogram modelHist = getRGBHistorgram(snipet);
	double biggestSimilarity = 0.0;
	QPoint point;
	for each (auto splited in t) {
		QImage testImg = splited.second;
		Histogram testHist = getRGBHistorgram(testImg);
		double similartity = getSimiliarityOfTwoHistograms(modelHist, testHist);
		if (similartity > biggestSimilarity) {
			biggestSimilarity = similartity;
			point = splited.first;
		}
	}
	
	int x = (point.x() - 1) * snipet.width();
	int y = (point.y() - 1) * snipet.height();
	int w = snipet.width() * 3;
	int h = snipet.height() * 3;
	QImage img = map.copy(x,y,w,h);
	Utilities::TOOL_saveImgToOutPutFolder(&img, NULL);
	
	int stop = 3;
}


Histogram MinimapAnalyzer::getRGBHistorgram(QImage& img) {
	QVector<int> histogram(15);
	histogram.fill(0);
	for (int x = 0; x < img.width(); x++){
		for (int y = 0; y < img.height(); y++) {
			uint col = RGBstruct(img.pixel(x, y)).toUint();//horrible solution
			int indexToIncrement = allPosibleColorsOnTheMap.indexOf(col);
			histogram[indexToIncrement]++;
		}
	}
	return histogram.toList();
}

QList<QPair<QPoint, QImage>> MinimapAnalyzer::splitScreenToFitImg(const QImage& imgToSplit, const QImage& imgToGetSizeFrom){
	const int WIDTH = imgToGetSizeFrom.width();
	const int HEIGHT = imgToGetSizeFrom.height();
	// todo dodac obsluge bledow
	const int X_AXIS_SIZE = imgToSplit.width() / WIDTH;
	const int Y_AXIS_SIZE = imgToSplit.height() / HEIGHT;

	QList<QPair<QPoint, QImage>> toRet;
	for (int x = 0; x < X_AXIS_SIZE; x++){
		for (int y = 0; y < Y_AXIS_SIZE; y++) {
			QRect partOfFrameToCopy(x * WIDTH, y * HEIGHT, WIDTH, HEIGHT);
			QImage imgToAdd = imgToSplit.copy(partOfFrameToCopy);

			//QString name = QString::number(x) + "_" + QString::number(y);
			//Utilities::TOOL_saveImgToOutPutFolder(&imgToAdd, &name);

			//todo niepotrzebne kopiowania, poprawic 
			QPair<QPoint, QImage> toAdd(QPoint(x, y), imgToAdd);
			toRet.push_back(toAdd);
		}
	}
	return toRet;
}

double MinimapAnalyzer::getSimiliarityOfTwoHistograms(const Histogram modelHistogram, const Histogram testedHistogram){
	bool diffrentSizesOfHistograms = modelHistogram.size() != testedHistogram.size();
	bool wrongSizeOfHistograms = modelHistogram.size() != allPosibleColorsOnTheMap.size();
	//todo dodac do loga sytuacje bledu

	int difference = 0;
	int modelHistogramSum = 0;
	for (int i = 0; i < modelHistogram.size(); i++) {
		//if (i == 3) continue;//water case
		int valueModel = modelHistogram[i];
		int valueTest = testedHistogram[i];
		int differenceOnOneColor = abs(valueModel - valueTest);
		difference += differenceOnOneColor;
		modelHistogramSum += 2*modelHistogram[i];
	}

	double toRet = 100.0 - (difference * 100.0 / modelHistogramSum);
	return toRet;
}
*/

