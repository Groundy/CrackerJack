#include "MinimapAnalyzer.h"
typedef QList<int> Histogram;

MinimapAnalyzer::MinimapAnalyzer(){
	test();
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

QList<QPoint> MinimapAnalyzer::test_findPlayerPosition(QImage& snipet, QImage& map, int maxPixThatCanBeDiffrent){
	const int WIDTH_SMALL_PIC = snipet.width();
	const int HEIGHT_SMALL_PIC = snipet.height();
	const int WIDTH_BIG_PIC = map.width();
	const int HEIGHT_BIG_PIC = map.height();

	bool errWidth = WIDTH_SMALL_PIC >= WIDTH_BIG_PIC;
	bool errHeight = HEIGHT_SMALL_PIC >= HEIGHT_BIG_PIC;
	bool errFormat = map.format() != snipet.format();
	bool anyErr = errWidth || errHeight || errFormat;
	if (anyErr) {
		Logger::logPotenialBug("Wrong input of two img", "MinimapAnalyzer", "findPlayerPosition");
		//return QList<QPoint>();
	}

	const int MAX_X_INDEX_TO_CHECK = WIDTH_BIG_PIC - WIDTH_SMALL_PIC;
	const int MAX_Y_INDEX_TO_CHECK = HEIGHT_BIG_PIC - HEIGHT_SMALL_PIC;
	
	QList<QPoint> startPointsListToRet;

	for (int x = 0; x <= MAX_X_INDEX_TO_CHECK; x++) {
		//qDebug() << x * 100.0 / (MAX_X_INDEX_TO_CHECK);
		for (int y = 0; y <= MAX_Y_INDEX_TO_CHECK; y++) {
			uint pixSmallImg = snipet.pixel(0, 0);
			uint pixBigImg = map.pixel(x, y);
			bool firstPixelMatched = pixSmallImg == pixBigImg;	
			if (firstPixelMatched) {
				int pixThatNotMatch = 0;
				bool skipThisImg = false;

				for (int x_TMP = WIDTH_SMALL_PIC -1 ; x_TMP >= 0; x_TMP--) {
					for (int y_TMP = HEIGHT_SMALL_PIC -1 ; y_TMP >= 0; y_TMP--) {
						pixSmallImg = snipet.pixel(x_TMP, y_TMP);
						pixBigImg = map.pixel(x + x_TMP, y + y_TMP);
						bool pixAreDiffrent = pixSmallImg != pixBigImg;
						if (pixAreDiffrent) {
							pixThatNotMatch++;
							if (pixThatNotMatch > maxPixThatCanBeDiffrent) {
								x_TMP = -1; //loopBreak
								y_TMP = -1; //loopBreak
								skipThisImg = true;
							}
						}
					}
				}
				if (!skipThisImg)
					startPointsListToRet.push_back(QPoint(x, y));
			}
		}
	}
	
	return startPointsListToRet;
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
	test_TEST_PERFORMANCE();
	/*
	QImage map;
	bool ok1 = map.load("C:\\Users\\ADMIN\\Desktop\\map.png");

	qint64 previous = 0;
	while (true) {
		Sleep(1);
		if (test_varClass->wholeImg.width() == 0)
			continue;
		QImage snipet = test_varClass->wholeImg.copy(1753,5, 106, 109);
		//Utilities::TOOL_saveImgToOutPutFolder(&var->wholeImg, NULL);
		QPoint res = test_findPlayerPositionByParts(snipet,map);
		qDebug() << res;
		if (res.x() != 0) {
			test_varClass->wholeImg = QImage();
			qint64 curr = QDateTime::currentMSecsSinceEpoch();
			qDebug() << QString::number(curr - previous);
			previous = curr;
		}
	}
	*/
}

QList<QRect> MinimapAnalyzer::fillListWithRectsPosOfMiniMapParts(){
	QList<QRect> toCut;
	toCut.push_back(QRect(0, 0, 53, 54));
	toCut.push_back(QRect(53, 0, 2, 52));
	toCut.push_back(QRect(55, 0, 51, 54));

	toCut.push_back(QRect(0, 54, 49, 2));
	toCut.push_back(QRect(57, 54, 49, 2));

	toCut.push_back(QRect(0, 56, 53, 53));
	toCut.push_back(QRect(53, 58, 2, 51));
	toCut.push_back(QRect(55, 56, 51, 53));
	return toCut;
}

QPoint MinimapAnalyzer::test_cutImgToShearFromDarkAndWater(QImage& img){
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
		img = QImage();
		return QPoint();
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
	img = img.copy(cutLines_LEFT, cutLines_TOP, widthToCut, heightToCut);
	return QPoint{cutLines_LEFT, cutLines_TOP};
}

void MinimapAnalyzer::test_TEST_PERFORMANCE(){
	QImage map;
	QString pathToMap = "C:\\Users\\ADMIN\\Desktop\\map.png";
	map.load(pathToMap);
	QDateTime dateTimeObj;
	LONG64 miliSec_cutting_algo = 0;
	int analyzedImgs = 0;
	int properlyAnalyzedImgs = 0;
	for (size_t i = 0; i < 200; i++){
		qDebug() << QString::number(i);
		QPair<QImage, QPoint> pair = test_cutRandomAreaFromMap(map);
		quint64 t1 = dateTimeObj.currentMSecsSinceEpoch();
		QPoint res2 = test_findPlayerPositionByParts(pair.first, map);
		quint64 t2 = dateTimeObj.currentMSecsSinceEpoch();
		//Utilities::TOOL_saveImgToOutPutFolder(&pair.first, &QString::number(i));
		bool isEmpty = res2.rx() == 0;
		if (!isEmpty) {
			int diff = t2 - t1;
			qDebug() << "result: " << res2;
			qDebug() << "diff: " + QString::number(diff);
			miliSec_cutting_algo += diff;
	
			analyzedImgs++;
			if (pair.second == res2)
				properlyAnalyzedImgs++;
			qDebug() << "mean: " <<QString::number(miliSec_cutting_algo * 1.0 / properlyAnalyzedImgs);
		}
	}
	int b = 5;
}

QPair<QImage, QPoint> MinimapAnalyzer::test_cutRandomAreaFromMap(QImage& map){
	QDateTime dateTimeObj;
	QRandomGenerator gen(dateTimeObj.currentMSecsSinceEpoch());
	int startPosX = gen.generate64() % (MAP_SIZE.width() - WIDTH_OF_MAP_ONLY_AREA);
	int startPosY = gen.generate64() % (MAP_SIZE.height() - HEIGHT_OF_WHOLE_PASSED_IMG);
	QRect randomRectFromMap(startPosX, startPosY, WIDTH_OF_MAP_ONLY_AREA, HEIGHT_OF_WHOLE_PASSED_IMG);
	QImage snipet = map.copy(randomRectFromMap);
	return QPair<QImage, QPoint>{snipet,QPoint(startPosX,startPosY)};
}

QList<QImage> MinimapAnalyzer::test_splitMiniMapScreenToListWithoutCross(QImage& miniMapScreen){
	QList<QImage> toRet;
	for each (QRect rect in miniMapParts) {
		QImage imgToCut = miniMapScreen.copy(rect);
		toRet.push_back(imgToCut);
	}
	return toRet;
}

QPoint MinimapAnalyzer::test_findPlayerPositionByParts(QImage& snipet, QImage& map){
	qint64 start = QDateTime::currentMSecsSinceEpoch();
	bool errWrongSize = snipet.width() != WIDTH_OF_MAP_ONLY_AREA || snipet.height() != HEIGHT_OF_WHOLE_PASSED_IMG;
	if (errWrongSize) {
		//todo logg
		return QPoint();
	}

	QList<QImage> imgs = test_splitMiniMapScreenToListWithoutCross(snipet);
	QList<int> orderOfImgsToCheck{ 1,3,4,6,0,2,5,7 };
	uint BLUE = qRgb(51, 102, 153);
	uint RED = qRgb(255,51,0);
	QList<uint> colsToIgnore{ BLUE, RED};
	for each (int i in orderOfImgsToCheck){
		QImage img = imgs[i];

		QPoint additionalCordinats = test_cutImgToShearFromDarkAndWater(img);
		//qDebug() << img.size();
		bool isEmpty = img.width() == 0 || img.height() == 0;
		if (isEmpty)
			continue;
		//Utilities::TOOL_saveImgToOutPutFolder(&img, &QString::number(i));

		QList<QPoint> startPoints = test_findPlayerOnMap_IgnoreColours(snipet, map, colsToIgnore);
		//qDebug() << "size:" + QString::number(startPoints.size());
		if (startPoints.size() != 1)
			continue;

		int startPosX = startPoints[0].x() - additionalCordinats.x() + miniMapParts[i].x() + POSITION_OF_PLAYER_ON_MINIMAP.x();
		int startPosY = startPoints[0].y() - additionalCordinats.y() + miniMapParts[i].y() + POSITION_OF_PLAYER_ON_MINIMAP.y();
		qint64 end = QDateTime::currentMSecsSinceEpoch();
		//qDebug() << QString::number(end - start);
		return QPoint(startPosX, startPosY);
	}
	return QPoint();
}

QList<QPoint> MinimapAnalyzer::test_findPlayerOnMap_IgnoreColours(QImage& snipet, QImage& map, QList<uint> colorsToIgnore){
	const int WIDTH_SMALL_PIC = snipet.width();
	const int HEIGHT_SMALL_PIC = snipet.height();
	const int WIDTH_BIG_PIC = map.width();
	const int HEIGHT_BIG_PIC = map.height();

	bool errWidth = WIDTH_SMALL_PIC >= WIDTH_BIG_PIC;
	bool errHeight = HEIGHT_SMALL_PIC >= HEIGHT_BIG_PIC;
	bool errFormat = map.format() != snipet.format();
	bool anyErr = errWidth || errHeight || errFormat;
	if (anyErr) {
		Logger::logPotenialBug("Wrong input of two img", "MinimapAnalyzer", "findPlayerPosition");
		//return QList<QPoint>();
	}

	const int MAX_X_INDEX_TO_CHECK = WIDTH_BIG_PIC - WIDTH_SMALL_PIC;
	const int MAX_Y_INDEX_TO_CHECK = HEIGHT_BIG_PIC - HEIGHT_SMALL_PIC;

	QList<QPoint> startPointsListToRet;

	for (int x = 0; x <= MAX_X_INDEX_TO_CHECK; x++) {
		for (int y = 0; y <= MAX_Y_INDEX_TO_CHECK; y++) {
			bool pixMatch_Top_Left = snipet.pixel(0, 0) == map.pixel(x, y);
			if (!pixMatch_Top_Left)
				continue;
			const int MAX_PIX_IND_Y = snipet.height() - 1;
			const int MAX_PIX_IND_X = snipet.width() - 1;
			bool pixMatch_Top_Right = snipet.pixel(MAX_PIX_IND_X, 0) == map.pixel(x + MAX_PIX_IND_X, y);
			if (!pixMatch_Top_Right)
				continue;
			bool pixMatch_Down_Left = snipet.pixel(0, MAX_PIX_IND_Y) == map.pixel(x , y + MAX_PIX_IND_Y);
			if (!pixMatch_Down_Left)
				continue;
			bool pixMatch_Down_Right = snipet.pixel(MAX_PIX_IND_X, MAX_PIX_IND_Y) == map.pixel(x + MAX_PIX_IND_X, MAX_PIX_IND_Y);
			if (!pixMatch_Down_Right)
				continue;



			bool thisImgIsOk = true;
			for (int x_TMP = 0; x_TMP < WIDTH_SMALL_PIC; x_TMP++) {
				for (int y_TMP = 0; y_TMP < HEIGHT_SMALL_PIC; y_TMP++) {
					uint pixSmallImg = snipet.pixel(x_TMP, y_TMP);
					if (colorsToIgnore.contains(pixSmallImg))
						continue;
					uint pixBigImg = map.pixel(x + x_TMP, y + y_TMP);
					if (colorsToIgnore.contains(pixBigImg))
						continue;
					bool pixAreDiffrent = pixSmallImg != pixBigImg;
					if (pixAreDiffrent) {
						x_TMP = WIDTH_SMALL_PIC; //loopBreak
						y_TMP = HEIGHT_SMALL_PIC; //loopBreak
						thisImgIsOk = false;
					}
				}
			}
			if (thisImgIsOk)
				startPointsListToRet.push_back(QPoint(x, y));
			
		}
	}

	return startPointsListToRet;
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

