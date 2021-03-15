#include "ScreenAnalyzer.h"


ScreenAnalyzer::ScreenAnalyzer(QObject *parent, VariablesClass *varClass)
	: QThread(parent){
	var = varClass;
	setPriority(QThread::Priority::HighestPriority);
	initQmap();
}

ScreenAnalyzer::~ScreenAnalyzer()
{
}

void ScreenAnalyzer::run() {
	TMP2();
	//mainLoop();
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

void ScreenAnalyzer::TMP_save(QImage* img){
	QString toSave = "C:\\Users\\ADMIN\\Desktop\\tested\\";
	QString timeString = QDateTime::currentDateTime().toString("hh-mm-ss-mmm");
	toSave.append(timeString);
	toSave.append("kurwa.png");
	bool kurwa = img->save(toSave);

}

void ScreenAnalyzer::TMP(){
	const QString path = "C:\\Users\\ADMIN\\Desktop\\LETTERS";
	QDir directory(path);
	QStringList litOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
	const QString path2 = "C:\\Users\\ADMIN\\Desktop\\blackLetters";
	QList<QString> list;
	for (size_t i = 0; i < litOfFIles.size(); i++){
		QString pathToFile = path + "\\" + litOfFIles[i];
		QImage* img = new QImage(pathToFile);
		imgToBlackAndWhiteOneColor(img, 200);
		QString name = path2 + "\\" + litOfFIles[i];
		img->save(name);
		QString code = TMP_letterImgToString(img);
		list.push_back(litOfFIles[i].left(1) + QString("___") + code);
	}
	for each (QString var in list){
		qDebug() << var;
	}
}

void ScreenAnalyzer::TMP2(){
	QString path = "C:\\Users\\ADMIN\\Desktop\\ttt.png";
	QString filename = "C:\\Users\\ADMIN\\Desktop\\chujPrzed.png";
	QString filename3 = "C:\\Users\\ADMIN\\Desktop\\chujPo.png";
	QImage* img = new QImage(path);
	QImage* tt = new QImage(img->copy(386, 11, 78, 14));
	for (int i = 0; i < 512; i++) {
		QChar ccc(i);
		qDebug() << ccc;
	}
	imgWithStrToStr(tt);
	int g = 4;
}

void ScreenAnalyzer::changeGreyPixelsToBlack(QImage* img, int minVal, int maxVal){
	int width = img->size().width();
	int height = img->size().height();
	QPoint point;
	for (size_t x = 0; x < width; x++)	{
		for (size_t y = 0; y < height; y++)		{
			point.setX(x);
			point.setY(y);
			uint pixVal = img->pixel(point);
			bool shouldBeBlacked = RGBstruct::isPixelInRangeOfGrey(pixVal, minVal, maxVal);
			if(shouldBeBlacked)
				img->setPixel(point, 0);
		}
	}
}

void ScreenAnalyzer::deleteScreenShotFolder(){
	QDir dir(pathToScreenFolder);
	dir.setNameFilters(QStringList() << "*.*");
	dir.setFilter(QDir::Files);
	foreach(QString dirFile, dir.entryList())
		dir.remove(dirFile);
}

void ScreenAnalyzer::imgToBlackAndWhiteOneColor(QImage* img, int threshold){
	int width = img->width();
	int height = img->height();
	uint pixelColor;
	bool redIsEnough, greenIsEnough, blueIsEnough;
	for (size_t x = 0; x < width; x++){
		for (size_t y = 0; y < height; y++){
			pixelColor = img->pixel(QPoint(x, y));
			RGBstruct rgb(pixelColor);
			redIsEnough = rgb.r >= threshold ? true : false;
			greenIsEnough = rgb.g >= threshold ? true : false;
			blueIsEnough = rgb.b >= threshold ? true : false;
			if (redIsEnough || greenIsEnough || blueIsEnough)
				img->setPixel(x, y, 0xffffff);
			else
				img->setPixel(x, y, 0);
		}
	}
}

void ScreenAnalyzer::imgToBlackAndWhiteAllColors(QImage* img, int threshold) {
	int width = img->width();
	int height = img->height();
	uint pixelColor;
	bool redIsEnough, greenIsEnough, blueIsEnough;
	for (size_t x = 0; x < width; x++) {
		for (size_t y = 0; y < height; y++) {
			pixelColor = img->pixel(QPoint(x, y));
			RGBstruct rgb(pixelColor);
			redIsEnough = rgb.r >= threshold ? true : false;
			greenIsEnough = rgb.g >= threshold ? true : false;
			blueIsEnough = rgb.b >= threshold ? true : false;
			if (redIsEnough && greenIsEnough && blueIsEnough)
				img->setPixel(x, y, qRgb(255,255,255));
			else
				img->setPixel(x, y, qRgb(0, 0, 0));
		}
	}
}

QString ScreenAnalyzer::imgWithStrToStr(QImage* img){
	imgToBlackAndWhiteAllColors(img, 240);
	cutBlackBordersOfImg(img);
	QList<QImage>* imgs = new QList<QImage>;
	cutImgWithLettersToSingleLettersImgList(img, imgs);
	QString toRet;
	for (size_t i = 0; i < imgs->size(); i++) {
		QImage tmp = imgs->at(i);
		QImage* tmpPointer = &tmp;
		cutBlackBordersOfImg(tmpPointer);
		QString letterCode = TMP_letterImgToString(tmpPointer);
		QChar letter = letters[letterCode];
		toRet.append(letter);		
	}
	qDebug() << toRet;
	return toRet;
}

QString ScreenAnalyzer::TMP_letterImgToString(QImage* img){
	int width = img->width();
	int height = img->height();
	uint pixelColor;
	QString toRet = QString::number(width) + QString("_") + QString::number(height) + QString("_");
	int sum;
	for (size_t x = 0; x < width; x++) {
		for (size_t y = 0; y < height; y++) {
			pixelColor = img->pixel(QPoint(x, y));
			RGBstruct rgb(pixelColor);
			sum = rgb.b + rgb.r + rgb.g;
			if (sum == 0)
				toRet.append(QString("0"));
			else
				toRet.append(QString("1"));
		}
	}
	return toRet;
}

void ScreenAnalyzer::initQmap(){
	letters.insert("6_8_011111101111111110000001100000011111111101111110", '0');
	letters.insert("4_8_01000001111111111111111100000001", '1');
	letters.insert("6_8_010000111100011110001101100110011111000101100001", '2');
	letters.insert("6_8_010000101100001110010001100100011111111101101110", '3');
	letters.insert("6_8_000011000001010000100100011111111111111100000100", '4');
	letters.insert("6_8_000000101111001111110001100100011001111110001110", '5');
	letters.insert("6_8_001111100111111111010001100100011001111100001110", '6');
	letters.insert("6_8_100000001000001110001111101111001111000011000000", '7');
	letters.insert("6_8_011011101111111110010001100100011111111101101110", '8');
	letters.insert("6_8_011100001111100110001001100010111111111001111100", '9');
	letters.insert("6_6_000110101111101001101001111111011111", 'a');
	letters.insert("6_9_111111111111111111000100001000100001000111111000011110", 'b');
	letters.insert("5_6_011110111111100001100001100001", 'c');
	letters.insert("6_9_000011110000111111000100001000100001111111111111111111", 'd');
	letters.insert("6_6_011110111111101001101001111001011010", 'e');
	letters.insert("5_9_000100000011111111111111111100100000100100000", 'f');
	letters.insert("6_8_011110001111110110000101100001011111111111111110", 'g');
	letters.insert("6_9_111111111111111111000100000000100000000111111000011111", 'h');
	letters.insert("2_8_1011111110111111", 'i');
	letters.insert("4_10_0000000001001000000110111111111011111110", 'j');
	letters.insert("6_9_111111111111111111000001100000011110000110011000100001", 'k');
	letters.insert("2_9_111111111111111111", 'l');
	letters.insert("10_6_111111111111100000100000111111011111100000100000111111011111", 'm');
	letters.insert("6_6_111111111111100000100000111111011111", 'n');
	letters.insert("6_6_011110111111100001100001111111011110", 'o');
	letters.insert("6_8_111111111111111110000100100001001111110001111000", 'p');
	letters.insert("5_6_111111111111010000110000110000", 'r');
	letters.insert("5_6_011001111101101101101111100110", 's');
	letters.insert("5_8_0010000011111110111111110010000100100001", 't');
	letters.insert("6_6_111110111111000001000001111111111111", 'u');
	letters.insert("6_6_111000111110000111000111111110111000", 'v');
	letters.insert("8_6_111100111111000011111100111100000011111111111100", 'w');
	letters.insert("6_6_110011111111001100001100111111110011", 'x');
	letters.insert("6_8_110000001111001100111111001111001111000011000000", 'y');
	letters.insert("5_6_100011100111101101111001110001", 'z');
	letters.insert("7_8_00000111001111111111110011000100111111000011111100000111", 'A');
	letters.insert("6_8_111111111111111110010001100100011111111101101110", 'B');
	letters.insert("6_8_011111101111111110000001100000011000000101000010", 'C');
	letters.insert("7_8_11111111111111111000000110000001110000110111111000111100", 'D');
	letters.insert("6_8_111111111111111110010001100100011001000110010001", 'E');
	letters.insert("6_8_111111111111111110010000100100001001000010010000", 'F');
	letters.insert("7_8_01111110111111111000000110000001100010011000111101001111", 'G');
	letters.insert("7_8_11111111111111110001000000010000000100001111111111111111", 'H');
	letters.insert("4_8_10000001111111111111111110000001", 'I');
	letters.insert("5_8_0000000110000001100000011111111111111110", 'J');
	letters.insert("6_8_111111111111111100111100011001101100001110000001", 'K');
	letters.insert("6_8_111111111111111100000001000000010000000100000001", 'L');
	letters.insert("8_8_1111111111100000011100000011100000010000001000000111111111111111", 'M');
	letters.insert("7_8_11111111011000000011000000011000000011000000011011111111", 'N');
	letters.insert("7_8_01111110111111111000000110000001100000011111111101111110", 'O');
	letters.insert("6_8_111111111111111110001000100010001111100001110000", 'P');
	letters.insert("7_8_11111111111111111000100010001100111111100111001100000001", 'R');
	letters.insert("6_8_011100101111100110011001100110011001111101001110", 'S');
	letters.insert("8_8_1000000010000000100000001111111111111111100000001000000010000000", 'T');
	letters.insert("7_8_11111110111111110000000100000001000000011111111111111110", 'U');
	letters.insert("6_8_111000001111110000011111000111111111110011100000", 'V');
	letters.insert("0_8_11100000111111000001111100001111111110001111100000001111000111111111110011100000", 'W');
	letters.insert("6_8_110000111110011100111100001111001110011111000011", 'X');
	letters.insert("6_8_110000001111000000111111001111111111000011000000", 'Y');
	letters.insert("6_8_100001111000111110011101101110011111000111100001", 'Z');
	letters.insert("110000110000011000011000001100001100001100000110000110000011000011", QChar(47));
	letters.insert("000000000110000000111100001111100001111100001111000000011000000000", QChar(47));
}

void ScreenAnalyzer::cutBlackBordersOfImg(QImage* img){
	int linesOfBlackrows_TOP = 0, linesOfBlackrows_DOWN = 0, linesOfBlackrows_RIGHT = 0, linesOfBlackrows_LEFT = 0;
	int w = img->width();
	int h = img->height();
	auto black = qRgb(0, 0, 0);
	QPoint pt;
	bool isBlack;
	for (size_t x = 0; x < w; x++)	{
		for (size_t y = 0; y < h; y++){
			pt.setX(x);
			pt.setY(y);
			isBlack = img->pixel(pt) == black ? true : false;
			if (!isBlack) {
				linesOfBlackrows_LEFT = x;
				//endOfLoop
				x = w;
				y = h;
			}
		}
	}

	for (int x = w-1; x >= 0; x--) {
		for (int y = 0; y < h; y++) {
			pt.setX(x);
			pt.setY(y);
			isBlack = img->pixel(pt) == black ? true : false;
			if (!isBlack) {
				linesOfBlackrows_RIGHT = w-x-1;
				//endOfLoop
				x = -1;
				y = h;
			}
		}
	}

	for (size_t y = 0; y < h; y++) {
		for (size_t x = 0; x < w; x++) {
			pt.setX(x);
			pt.setY(y);
			isBlack = img->pixel(pt) == black ? true : false;
			if (!isBlack) {
				linesOfBlackrows_TOP = y;
				//endOfLoop
				x = w;
				y = h;
			}
		}
	}

	for (int y = h-1; y >= 0; y--) {
		for (int x = 0; x < w; x++) {
			pt.setX(x);
			pt.setY(y);
			isBlack = img->pixel(pt) == black ? true : false;
			if (!isBlack) {
				linesOfBlackrows_DOWN = h - y - 1;
				//endOfLoop
				x = w;
				y = -1;
			}
		}
	}

	int anotherParametr_x = w - linesOfBlackrows_RIGHT - linesOfBlackrows_LEFT;
	int anotherParametr_y = h - linesOfBlackrows_TOP - linesOfBlackrows_DOWN;

	int widthToCut = anotherParametr_x >= 0 ? anotherParametr_x : 0;
	int heightToCut = anotherParametr_y >= 0 ? anotherParametr_y : 0;
	QImage imgToRet = img->copy(linesOfBlackrows_LEFT, linesOfBlackrows_TOP, widthToCut, heightToCut);
	*img = imgToRet;
}

void ScreenAnalyzer::cutImgWithLettersToSingleLettersImgList(QImage* img, QList<QImage>* letterImages){
	QList<int> colThatAreNotBlack;
	int w = img->width();
	int h = img->height();
	auto black = qRgb(0, 0, 0);
	for (int x = 0; x < w; x++){
		for (int y = 0; y < h; y++){
			if (img->pixel(x, y) != black) {
				y = h; //ending inner loop
				colThatAreNotBlack.push_back(x);
			}
		}
	}
	QList<int> indexesOfStartOfLetter, indexesOfEndsOfLetters;
	indexesOfStartOfLetter.push_back(0);
	for (int i = 0; i < colThatAreNotBlack.size() - 1; i++){
		if (colThatAreNotBlack[i + 1] - colThatAreNotBlack[i] > 1) {
			indexesOfEndsOfLetters.push_back(colThatAreNotBlack[i]);
		}
	}
	for (int i = 1; i < colThatAreNotBlack.size(); i++) {
		if (colThatAreNotBlack[i] - colThatAreNotBlack[i-1] > 1) {
			indexesOfStartOfLetter.push_back(colThatAreNotBlack[i]);
		}
	}
	indexesOfEndsOfLetters.push_back(colThatAreNotBlack.last());
	QList<int> widths;

	for (size_t i = 0; i < indexesOfStartOfLetter.size(); i++)
		widths.push_back(indexesOfEndsOfLetters[i] - indexesOfStartOfLetter[i]+1);
	
	for (size_t i = 0; i < indexesOfStartOfLetter.size(); i++){
		QImage letter = img->copy(indexesOfStartOfLetter[i], 0, widths[i], img->height());
		letterImages->push_back(letter);
	}
}

void ScreenAnalyzer::mainLoop(){
	while (enableScreenAnalyzer){
		QString nameOfImgToCapture = getNameOfLastTakenScreenShot();
		QString pathToImg = pathToScreenFolder + QString("\\") + nameOfImgToCapture;
		QImage* img = new QImage;
		bool openCorrectly = img->load(pathToImg);
		if (openCorrectly) {
			var->var_img = img;
			//deleteScreenShotFolder();
		}
		else{
			delete img;
			continue;
		}

		changeGreyPixelsToBlack(var->var_img, 40, 120);

		TMP_save(var->var_img);
		delete img;
	}
}
