#include "MinimapAnalyzer.h"


MinimapAnalyzer::MinimapAnalyzer(){
	QImage map, area;
	QString pathToMap = "C:\\Users\\ADMIN\\Desktop\\map1.png";
	QString pathToArea = "C:\\Users\\ADMIN\\Desktop\\7.png";
	map.load(pathToMap);
	area.load(pathToArea);
	//Utilities::cutBlackBordersOfImg(area);
	auto ret = findPlayerPosition(map, area, 0);
	int stop = 3;
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

QList<QPoint> MinimapAnalyzer::findPlayerPosition(QImage& miniMapSnippet, QImage& ImgToShareWithIn, int maxPixThatCanBeDiffrent){

	const int WIDTH_SMALL_PIC = miniMapSnippet.width();
	const int HEIGHT_SMALL_PIC = miniMapSnippet.height();
	const int WIDTH_BIG_PIC = ImgToShareWithIn.width();
	const int HEIGHT_BIG_PIC = ImgToShareWithIn.height();

	bool errWrongSize = WIDTH_SMALL_PIC != WIDTH_OF_MAP_ONLY_AREA || HEIGHT_SMALL_PIC != HEIGHT_OF_WHOLE_PASSED_IMG;
	bool errWidth = WIDTH_SMALL_PIC >= WIDTH_BIG_PIC;
	bool errHeight = HEIGHT_SMALL_PIC >= HEIGHT_BIG_PIC;
	bool errFormat = miniMapSnippet.format() != ImgToShareWithIn.format();
	bool anyErr = errWidth || errHeight || errFormat || errWrongSize;
	if (anyErr) {
		Logger::logPotenialBug("Wrong input of two img", "MinimapAnalyzer", "findPlayerPosition");
		return QList<QPoint>();
	}

	const int MAX_X_INDEX_TO_CHECK = WIDTH_BIG_PIC - WIDTH_SMALL_PIC;
	const int MAX_Y_INDEX_TO_CHECK = HEIGHT_BIG_PIC - HEIGHT_SMALL_PIC;

	QList<QPoint> startPointsListToRet;
	for (int x = 0; x <= MAX_X_INDEX_TO_CHECK; x++) {
		qDebug() << x * 100 / (MAX_X_INDEX_TO_CHECK);
		for (int y = 0; y <= MAX_Y_INDEX_TO_CHECK; y++) {
			if(forbiddenPixels.contains(QPoint(x,y)))
				continue;
			uint pixSmallImg = miniMapSnippet.pixel(0, 0);
			uint pixBigImg = ImgToShareWithIn.pixel(x, y);
			bool firstPixelMatched = pixSmallImg == pixBigImg;
			if (firstPixelMatched) {
				int x_TMP = 1, y_TMP = 1, pixThatCanBeDiffrent = maxPixThatCanBeDiffrent;
				for (; x_TMP < WIDTH_SMALL_PIC; x_TMP++) {
					for (; y_TMP < HEIGHT_SMALL_PIC; y_TMP++) {
						if (forbiddenPixels.contains(QPoint(x + x_TMP, y + y_TMP)))
							continue;
						pixSmallImg = miniMapSnippet.pixel(x_TMP, y_TMP);
						pixBigImg = ImgToShareWithIn.pixel(x + x_TMP, y + y_TMP);
						bool pixAreDiffrent = pixSmallImg != pixBigImg;
						if (pixAreDiffrent)
							pixThatCanBeDiffrent--;
						if (pixThatCanBeDiffrent < 0) {
							x_TMP = WIDTH_SMALL_PIC + 10; //loopBreak
							y_TMP = HEIGHT_SMALL_PIC + 10; //loopBreak
						}
					}
				}
				bool imgFound = x_TMP == WIDTH_SMALL_PIC && y_TMP == HEIGHT_SMALL_PIC;
				if (imgFound)
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

QList<QPoint> MinimapAnalyzer::fillFobiddenPixList(){
	QList<QPoint> toRet;
	toRet.append(QPoint(51, 54));
	toRet.append(QPoint(51, 55));
	toRet.append(QPoint(52, 54));
	toRet.append(QPoint(52, 55));
	toRet.append(QPoint(53, 52));
	toRet.append(QPoint(53, 53));
	toRet.append(QPoint(53, 54));
	toRet.append(QPoint(53, 55));
	toRet.append(QPoint(53, 56));
	toRet.append(QPoint(53, 57));
	toRet.append(QPoint(54, 52));
	toRet.append(QPoint(54, 53));
	toRet.append(QPoint(54, 54));
	toRet.append(QPoint(54, 55));
	toRet.append(QPoint(54, 56));
	toRet.append(QPoint(54, 57));
	toRet.append(QPoint(55, 54));
	toRet.append(QPoint(55, 55));
	toRet.append(QPoint(56, 54));
	toRet.append(QPoint(56, 55));

	return toRet;
}
