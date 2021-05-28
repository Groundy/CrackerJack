#include "Utilities.h"
#include <QRgb>
#include <QtCore/qsettings.h>
#include <Calibrator.h>

int Utilities::showMessageBox(QString title = "CrackerJack", QString text = "", QFlags<QMessageBox::StandardButton> buttons = QMessageBox::Ok) {
	QMessageBox box;
	box.setText(text);
	box.setWindowTitle(title);
	box.setStandardButtons(buttons);
	//box.setDefaultButton(buttons);
	int ret = box.exec();
	return ret;
}

bool Utilities::showMessageBox_NO_YES(QString title, QString text)
 {
	 QFlags<QMessageBox::StandardButton> flags = { QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No };
	 int res = Utilities::showMessageBox(title, text, flags);
	 bool toRet = res == QMessageBox::StandardButton::Yes;
	 return toRet;
 }

bool Utilities::sendKeyStrokeToProcess(Key key, unsigned int PID, QString nameOfWindow) {
	LPCWSTR nameOfWindowLPCWSTR = convert_StrToLPCWSTR(nameOfWindow);
    HWND handler = FindWindow(NULL, nameOfWindowLPCWSTR);
	if (handler == NULL) {
		Logger::logPotenialBug("Can't get handler to window: " + nameOfWindow, "Utilities", "sendKeyStrokeToProcess");
        return false;
	}
    DWORD tmp = PID;
    DWORD hThread = GetWindowThreadProcessId(handler, &tmp);
    if (hThread != NULL) {
        int g =0x1470001;
        PostMessage(handler, WM_KEYDOWN, key.number, g);
        PostMessage(handler, WM_KEYUP, key.number, g);
		return true;
    }
	else{
		Logger::logPotenialBug("Can't get thread PID for used handler", "Utilities", "sendKeyStrokeToProcess");
		return false;
	}
 }

void Utilities::imgToBlackAndWhiteOneColor(QImage& img, int threshold) {
	const int WIDTH = img.width();
	const int HEIGHT = img.height();
	const uint BLACK = qRgb(0, 0, 0);
	const uint WHITE = qRgb(255, 255, 255);
	for (size_t x = 0; x < WIDTH; x++) {
		for (size_t y = 0; y < HEIGHT; y++) {
			uint pixelColor = img.pixel(x, y);
			RGBstruct rgb(pixelColor);
			bool redIsEnough = rgb.r >= threshold;
			bool greenIsEnough = rgb.g >= threshold;
			bool blueIsEnough = rgb.b >= threshold;
			bool setWhite = redIsEnough || greenIsEnough || blueIsEnough;
			uint toSet = setWhite ? WHITE : BLACK;
			img.setPixel(x, y, toSet);
		}
	}
}

void Utilities::imgToBlackAndWhiteAllColors(QImage& img, int threshold) {
	const int WIDTH = img.width();
	const int HEIGHT = img.height();
	const uint BLACK = qRgb(0, 0, 0);
	const uint WHITE = qRgb(255, 255, 255);
	for (size_t x = 0; x < WIDTH; x++) {
		for (size_t y = 0; y < HEIGHT; y++) {
			uint pixelColor = img.pixel(x, y);
			RGBstruct rgb(pixelColor);
			bool redIsEnough = rgb.r >= threshold;
			bool greenIsEnough = rgb.g >= threshold;
			bool blueIsEnough = rgb.b >= threshold;
			bool setWhite = redIsEnough && greenIsEnough && blueIsEnough;
			uint pixToSet = setWhite ? WHITE : BLACK;
			img.setPixel(x, y, pixToSet);
		}
	}
 }

void Utilities::cutBlackBordersOfImg(QImage& img) {
	 int linesOfBlackrows_TOP = 0, linesOfBlackrows_DOWN = 0, linesOfBlackrows_RIGHT = 0, linesOfBlackrows_LEFT = 0;
	 const int WIDTH = img.width();
	 const int HEIGHT = img.height();
	 const uint BLACK = qRgb(0, 0, 0);
	 for (int x = 0; x < WIDTH; x++) {
		 for (int y = 0; y < HEIGHT; y++) {
			 bool isBlack = img.pixel(x, y) == BLACK;
			 if (!isBlack) {
				 linesOfBlackrows_LEFT = x;
				 x = WIDTH; //endOfLoop
				 y = WIDTH; //endOfLoop
			 }
		 }
	 }

	 for (int x = WIDTH - 1; x >= 0; x--) {
		 for (int y = 0; y < HEIGHT; y++) {
			 bool isBlack = img.pixel(x, y) == BLACK;
			 if (!isBlack) {
				 linesOfBlackrows_RIGHT = WIDTH - x - 1;
				 x = -1;//endOfLoop
				 y = HEIGHT;//endOfLoop
			 }
		 }
	 }

	 for (int y = 0; y < HEIGHT; y++) {
		 for (int x = 0; x < WIDTH; x++) {
			 bool isBlack = img.pixel(x, y) == BLACK;
			 if (!isBlack) {
				 linesOfBlackrows_TOP = y;
				 x = WIDTH;//endOfLoop
				 y = HEIGHT;//endOfLoop
			 }
		 }
	 }

	 for (int y = HEIGHT - 1; y >= 0; y--) {
		 for (int x = 0; x < WIDTH; x++) {
			 bool isBlack = img.pixel(x, y) == BLACK;
			 if (!isBlack) {
				 linesOfBlackrows_DOWN = HEIGHT - y - 1;
				 x = WIDTH;//endOfLoop
				 y = -1;//endOfLoop
			 }
		 }
	 }

	 int anotherParametr_x = WIDTH - linesOfBlackrows_RIGHT - linesOfBlackrows_LEFT;
	 int anotherParametr_y = HEIGHT - linesOfBlackrows_TOP - linesOfBlackrows_DOWN;

	 int widthToCut = anotherParametr_x >= 0 ? anotherParametr_x : 0;
	 int heightToCut = anotherParametr_y >= 0 ? anotherParametr_y : 0;
	 img = img.copy(linesOfBlackrows_LEFT, linesOfBlackrows_TOP, widthToCut, heightToCut);
 }

void Utilities::cutImgWithLettersToSingleLettersImgList(QImage& img, QList<QImage>& letterImages) {
	 QList<int> colThatAreNotBlack;
	 const int WIDTH = img.width();
	 const int HEIGHT = img.height();
	 const int BLACK = qRgb(0, 0, 0);
	 for (int x = 0; x < HEIGHT; x++) {
		 for (int y = 0; y < HEIGHT; y++) {
			 if (img.pixel(x, y) != BLACK) {
				 y = HEIGHT; //ending inner loop
				 colThatAreNotBlack.push_back(x);
			 }
		 }
	 }
	 if (colThatAreNotBlack.isEmpty()) {
		 letterImages.push_back(img);
		 return;
	 }

	 QList<int> indexesOfStartOfLetter, indexesOfEndsOfLetters;
	 indexesOfStartOfLetter.push_back(0);
	 for (int i = 0; i < colThatAreNotBlack.size() - 1; i++) {
		 if (colThatAreNotBlack[i + 1] - colThatAreNotBlack[i] > 1) {
			 indexesOfEndsOfLetters.push_back(colThatAreNotBlack[i]);
		 }
	 }
	 for (int i = 1; i < colThatAreNotBlack.size(); i++) {
		 if (colThatAreNotBlack[i] - colThatAreNotBlack[i - 1] > 1) {
			 indexesOfStartOfLetter.push_back(colThatAreNotBlack[i]);
		 }
	 }
	 indexesOfEndsOfLetters.push_back(colThatAreNotBlack.last());
	 QList<int> widths;

	 for (size_t i = 0; i < indexesOfStartOfLetter.size(); i++)
		 widths.push_back(indexesOfEndsOfLetters[i] - indexesOfStartOfLetter[i] + 1);

	 for (size_t i = 0; i < indexesOfStartOfLetter.size(); i++) {
		 QImage letter = img.copy(indexesOfStartOfLetter[i], 0, widths[i], img.height());
		 letterImages.push_back(letter);
	 }
 }

QString Utilities::imgWithStrToStr(QImage* img) {
	 imgToBlackAndWhiteAllColors(*img, 240);
	 cutBlackBordersOfImg(*img);
	 QList<QImage>* imgs = new QList<QImage>;
	 cutImgWithLettersToSingleLettersImgList(*img, *imgs);
	 QString toRet;
	 for (size_t i = 0; i < imgs->size(); i++) {
		 QImage tmp = imgs->at(i);
		 QImage* tmpPointer = &tmp;
		 cutBlackBordersOfImg(*tmpPointer);
		 QString letterCode = letterImgToLetterCodeStr(tmpPointer);
		 QChar letter = StrCodeToQChar(letterCode);
		 toRet.append(letter);
	 }
	 delete imgs;
	 return toRet;
 }

QString Utilities::letterImgToLetterCodeStr(QImage* SingleLetterImg) {
	 const int WIDTH = SingleLetterImg->width();
	 const int HEIGHT = SingleLetterImg->height();
	 const QString FLOOR = QString("_");
	 const QString ZERO = QString("0"), ONE = QString("1");
	 QString toRet = QString::number(WIDTH) + FLOOR + QString::number(HEIGHT) + FLOOR;
	 for (size_t x = 0; x < WIDTH; x++) {
		 for (size_t y = 0; y < HEIGHT; y++) {
			 uint pixelColor = SingleLetterImg->pixel(x, y);
			 RGBstruct rgb(pixelColor);
			 int sum = rgb.b + rgb.r + rgb.g;
			 QString toAppend = sum == 0 ? ZERO : ONE;
			 toRet.append(toAppend);
		 }
	 }
	 return toRet;
 }

QChar Utilities::StrCodeToQChar(QString code){
	 //todo, it's terrible solution, it has to be changed
	QMap<QString, QChar> letters = getQmapWithCodes();
	return letters[code];
 }

QMap<QString, QChar>  Utilities::getQmapWithCodes() {
	 QMap<QString, QChar> letters;
	 //char 200 it's some weird char but here it's symbol of healthIcon
	 letters.insert("9_9_101110000011111000111111100111111110011111111111111110111111100111111000011110000",QChar(200));
	 //char 201 it's some weird char but here it's symbol of manaIcon
	 letters.insert("10_9_000100001001100111001111111111111110111111100111111100111110000111110000100100000100000000", QChar(201));
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
	 //slash, in program it's slash between current health and max health
	 letters.insert("11_6_110000110000011000011000001100001100001100000110000110000011000011", QChar(47));
	 //backslash char but in program it's slash above but rotated
	 letters.insert("6_11_000000000110000000111100001111100001111100001111000000011000000000", QChar(92));
	 //opening bracket
	 letters.insert("4_11_00011111000011111111101110000011110000000001", QChar(40));
	 //closing bracket
	 letters.insert("4_11_10000000001111000001110111111111000011111000", QChar(41));
	 
	 return letters;
 }

QImage Utilities::fromCharToImg(QChar CharToImg){
	auto mapWithCodes = getQmapWithCodes();
	QString keyCode = mapWithCodes.key(CharToImg);
	QStringList parts;
	parts = keyCode.split("_");
	if (parts.size() == 3) {
		const int WIDTH = parts[0].toInt();
		const int HEIGHT = parts[1].toInt();
		QString code = parts[2];

		const uint BLACK = qRgb(0, 0, 0);
		const uint WHITE = qRgb(255, 255, 255);
		QImage imgToRet(WIDTH, HEIGHT, QImage::Format::Format_ARGB32);
		int i = 0;
		for (size_t x = 0; x < WIDTH; x++) {
			for (size_t y = 0; y < HEIGHT; y++) {
				bool setWhite = code[i] == '1';
				uint toSet = setWhite ? WHITE : BLACK;
				imgToRet.setPixel(x, y, toSet);
				i++;
			}
		}
		return imgToRet;
	}
	else {
		Logger::logPotenialBug("Wrong structure of char code for char: " + QString(CharToImg), "Utilities", "fromCharToImg");
		return QImage();
	}
}

void Utilities::rotateImgToRight(QImage* imgToRotate, int timesToRotateRight){
	QTransform rotating;
	qreal degreeToRotateToRight = timesToRotateRight * 90;
	rotating.rotate(degreeToRotateToRight,Qt::Axis::ZAxis);
	QImage tmp = imgToRotate->transformed(rotating);
	*imgToRotate = tmp;
}

bool Utilities::isItPixelFromFrame(uint color, int minValueAcceptable, int maxValueAcceptable, bool requireSameValuesOfRGB){
	RGBstruct rgb(color);
	int minValueFound = min(min(rgb.r, rgb.g), rgb.b);
	int maxValueFound = max(max(rgb.r, rgb.g), rgb.b);
	if (requireSameValuesOfRGB) {
		if (maxValueFound - minValueFound > 1)
			return false;
	}
	bool isPixOfFrame = minValueFound >= minValueAcceptable && maxValueFound <= maxValueAcceptable;
	return isPixOfFrame;
}

long long Utilities::getCurrentTimeInMiliSeconds() {
	QDateTime date = QDateTime::currentDateTime();
	qint64 mseconds = date.currentMSecsSinceEpoch();
	return mseconds;
}

QImage Utilities::getImageFromAdvancedCode(QString codeOfImg){
	QStringList partsOfCode = codeOfImg.split(QString("_"),Qt::SkipEmptyParts);
	int width = partsOfCode[0].toInt();
	int height = partsOfCode[1].toInt();
	partsOfCode.removeFirst();
	partsOfCode.removeFirst();
	QImage imgToCreate(width, height, QImage::Format::Format_ARGB32);
	int i = 0;
	for (size_t x = 0; x < width; x++){
		for (size_t y = 0; y < height; y++){
			QStringList rgb = partsOfCode[i].split(QString("#"), Qt::SkipEmptyParts);
			i++;
			int r = rgb[0].toInt();
			int g = rgb[1].toInt();
			int b = rgb[2].toInt();
			uint pixToSet = qRgb(r, g, b);
			imgToCreate.setPixel(x, y, pixToSet);
		}
	}
	return imgToCreate;
}

void Utilities::getMapWithNumbersFromBottomBar(QMap<QString, int>& lightToRet, QMap<QString, int>& darkToRet){
	QMap<QString, int> lightVersion;
	lightVersion.insert("4_6_#125#125#125_#223#223#223_#223#223#223_#223#223#223_#223#223#223_#180#180#180_#208#208#208_#83#83#83_#0#0#0_#0#0#0_#29#29#29_#223#223#223_#194#194#194_#125#125#125_#56#56#56_#56#56#56_#111#111#111_#223#223#223_#83#83#83_#223#223#223_#223#223#223_#223#223#223_#223#223#223_#111#111#111_", 0);
	lightVersion.insert("2_6_#125#125#125_#223#223#223_#167#167#167_#167#167#167_#167#167#167_#223#223#223_#125#125#125_#167#167#167_#167#167#167_#167#167#167_#167#167#167_#223#223#223_", 1);
	lightVersion.insert("2_4_#56#56#56_#0#0#0_#83#83#83_#208#208#208_#153#153#153_#139#139#139_#223#223#223_#56#56#56_", 2);
	lightVersion.insert("2_4_#153#153#153_#194#194#194_#180#180#180_#139#139#139_#223#223#223_#111#111#111_#180#180#180_#223#223#223_", 3);
	lightVersion.insert("3_4_#194#194#194_#83#83#83_#111#111#111_#167#167#167_#194#194#194_#167#167#167_#194#194#194_#208#208#208_#111#111#111_#111#111#111_#167#167#167_#194#194#194_", 4);
	lightVersion.insert("4_6_#167#167#167_#223#223#223_#223#223#223_#83#83#83_#41#41#41_#223#223#223_#167#167#167_#97#97#97_#180#180#180_#56#56#56_#0#0#0_#223#223#223_#167#167#167_#56#56#56_#167#167#167_#167#167#167_#125#125#125_#223#223#223_#167#167#167_#56#56#56_#69#69#69_#223#223#223_#223#223#223_#111#111#111_", 5);
	lightVersion.insert("4_5_#208#208#208_#223#223#223_#223#223#223_#223#223#223_#167#167#167_#139#139#139_#167#167#167_#56#56#56_#29#29#29_#223#223#223_#56#56#56_#167#167#167_#111#111#111_#69#69#69_#223#223#223_#41#41#41_#125#125#125_#223#223#223_#223#223#223_#139#139#139_", 6);
	lightVersion.insert("2_4_#167#167#167_#56#56#56_#29#29#29_#153#153#153_#167#167#167_#153#153#153_#223#223#223_#208#208#208_", 7);
	lightVersion.insert("4_6_#139#139#139_#223#223#223_#208#208#208_#194#194#194_#194#194#194_#194#194#194_#223#223#223_#29#29#29_#194#194#194_#111#111#111_#0#0#0_#223#223#223_#208#208#208_#97#97#97_#167#167#167_#194#194#194_#56#56#56_#223#223#223_#125#125#125_#223#223#223_#139#139#139_#208#208#208_#223#223#223_#167#167#167_", 8);
	lightVersion.insert("2_6_#223#223#223_#29#29#29_#0#0#0_#223#223#223_#0#0#0_#223#223#223_#194#194#194_#125#125#125_#56#56#56_#208#208#208_#153#153#153_#194#194#194_", 9);
	lightToRet = lightVersion;

	QMap<QString, int> darkVersion;
	darkVersion.insert("4_6_#37#37#37_#66#66#66_#66#66#66_#66#66#66_#66#66#66_#54#54#54_#62#62#62_#25#25#25_#0#0#0_#0#0#0_#9#9#9_#66#66#66_#58#58#58_#37#37#37_#17#17#17_#17#17#17_#33#33#33_#66#66#66_#25#25#25_#66#66#66_#66#66#66_#66#66#66_#66#66#66_#33#33#33_",0);
	darkVersion.insert("2_6_#37#37#37_#66#66#66_#50#50#50_#50#50#50_#50#50#50_#66#66#66_#37#37#37_#50#50#50_#50#50#50_#50#50#50_#50#50#50_#66#66#66_", 1);
	darkVersion.insert("2_4_#17#17#17_#0#0#0_#25#25#25_#62#62#62_#46#46#46_#41#41#41_#66#66#66_#17#17#17_", 2);
	darkVersion.insert("2_4_#46#46#46_#58#58#58_#54#54#54_#41#41#41_#66#66#66_#33#33#33_#54#54#54_#66#66#66_", 3);
	darkVersion.insert("3_4_#58#58#58_#25#25#25_#33#33#33_#50#50#50_#58#58#58_#50#50#50_#58#58#58_#62#62#62_#33#33#33_#33#33#33_#50#50#50_#58#58#58_", 4);
	darkVersion.insert("4_6_#50#50#50_#66#66#66_#66#66#66_#25#25#25_#12#12#12_#66#66#66_#50#50#50_#29#29#29_#54#54#54_#17#17#17_#0#0#0_#66#66#66_#50#50#50_#17#17#17_#50#50#50_#50#50#50_#37#37#37_#66#66#66_#50#50#50_#17#17#17_#21#21#21_#66#66#66_#66#66#66_#33#33#33_", 5);
	darkVersion.insert("4_5_#62#62#62_#66#66#66_#66#66#66_#66#66#66_#50#50#50_#41#41#41_#50#50#50_#17#17#17_#9#9#9_#66#66#66_#17#17#17_#50#50#50_#33#33#33_#21#21#21_#66#66#66_#12#12#12_#37#37#37_#66#66#66_#66#66#66_#41#41#41_", 6);
	darkVersion.insert("2_4_#50#50#50_#17#17#17_#9#9#9_#46#46#46_#50#50#50_#46#46#46_#66#66#66_#62#62#62_", 7);
	darkVersion.insert("4_6_#41#41#41_#66#66#66_#62#62#62_#58#58#58_#58#58#58_#58#58#58_#66#66#66_#9#9#9_#58#58#58_#33#33#33_#0#0#0_#66#66#66_#62#62#62_#29#29#29_#50#50#50_#58#58#58_#17#17#17_#66#66#66_#37#37#37_#66#66#66_#41#41#41_#62#62#62_#66#66#66_#50#50#50_", 8);
	darkVersion.insert("2_6_#66#66#66_#9#9#9_#0#0#0_#66#66#66_#0#0#0_#66#66#66_#58#58#58_#37#37#37_#17#17#17_#62#62#62_#46#46#46_#58#58#58_", 9);
	darkToRet = darkVersion;
}

int Utilities::getNumberFromBottomBar(QImage& imgToShearchWithin){
	QMap<QString, int> lightMap, darkMap;
	Utilities::getMapWithNumbersFromBottomBar(lightMap, darkMap);
	QStringList lightCodes = lightMap.keys();
	QStringList darkCodes = darkMap.keys();

	QMap<int, int> anotherMap; // <positionX, value>

	for (size_t i = 0; i < lightCodes.size(); i++){
		QList<QImage*> listWithLightAndDarkLetterImg;
		QImage darkLetter = Utilities::getImageFromAdvancedCode(darkCodes[i]);
		QImage lightLetter = Utilities::getImageFromAdvancedCode(lightCodes[i]);
		listWithLightAndDarkLetterImg.push_back(&darkLetter);
		listWithLightAndDarkLetterImg.push_back(&lightLetter);

		int digit = lightMap[lightCodes[i]];
		QList<QPoint> listOfStartingPoints = Calibrator::findStartPositionInImg_mulitpeImgs(listWithLightAndDarkLetterImg, imgToShearchWithin);// (listWithLightAndDarkLetterImg, imgToShearchWithin);
		for each (QPoint var in listOfStartingPoints)
			anotherMap.insert(var.x(), digit);
	}

	QString strToRe;
	for each (auto key in anotherMap.keys())
		strToRe.push_back(QString::number(anotherMap[key]));
	 
	return strToRe.toInt();
}

QMap<Utilities::FieldsOfIniFile, QString> Utilities::get_Field_NamesFromIni_map(){
	typedef Utilities::FieldsOfIniFile Field;
	QMap<Field, QString> toRet;

	toRet.insert(Field::LANGUAGE, "language");
	toRet.insert(Field::LAST_USED_LOGIN, "lastLogin");
	toRet.insert(Field::LAST_USED_PROFILE_NAME, "lastProfile");
	toRet.insert(Field::PATH_TO_TIBIA_FOLDER, "pathToGameFolder");

	return toRet;
}

QString Utilities::readFromIniFile(FieldsOfIniFile nameOfField){
	QSettings setttings("settings.ini", QSettings::IniFormat);
	QMap<Utilities::FieldsOfIniFile, QString> map = get_Field_NamesFromIni_map();
	QString nameOfFieldAsStr = map[nameOfField];
	QString readVal = setttings.value(nameOfFieldAsStr).toString();
	return readVal;
}

void Utilities::writeIniFile(FieldsOfIniFile nameOfField, QString value){
	QSettings setttings("settings.ini", QSettings::IniFormat);
	QMap<Utilities::FieldsOfIniFile, QString> map = get_Field_NamesFromIni_map();
	QString nameOfFieldAsStr = map[nameOfField];
	setttings.setValue(nameOfFieldAsStr, value);
}

void Utilities::saveImgToOutPutFolder(QImage* img, QString *extraName){
	QString prefixOfName;
	if (extraName == NULL)
		prefixOfName = QDateTime::currentDateTime().toString("mm_ss_zzz");
	else
		prefixOfName = *extraName;
	QString fullname = "C:\\Users\\ADMIN\\Desktop\\output\\_" + prefixOfName + ".png";
	img->save(fullname);
}

LPCWSTR Utilities::convert_StrToLPCWSTR(QString str){
     return (const wchar_t*)str.utf16();
 }




void Utilities::TOOL_convertMapsFromOrgNameToSqrName(QString inputFolder) {
	for (size_t i = 0; i < 16; i++) {
		qDebug() << QString::number(i);
		QString path = inputFolder + "\\" + QString::number(i);
		QDir directory(path);
		QStringList litOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
		for each (QString var in litOfFIles) {
			QStringList partOfNames = var.split("_");
			partOfNames[2] = QString::number(partOfNames[2].toInt() / 256);
			partOfNames[3] = QString::number(partOfNames[3].toInt() / 256);
			QString newName = partOfNames.join("_");
			QString pathToFileOld = path + "\\" + var;
			QString pathToFileNew = path + "\\" + newName;
			QFile::rename(pathToFileOld, pathToFileNew);
		}
	}
}

void Utilities::TOOL_generateMapAsText(QString inputFolder){
	//it takes so fucking long approximately 210s
	const long start_xPos = 124, start_yPos = 120;
	const long end_xPos_included = 133, end_yPos_included = 128;
	const long width = 256 * (end_xPos_included - start_xPos + 1);
	const long height = 256 * (end_yPos_included - start_yPos + 1);
	const QString floor("_");
	const QString minimapStr = "Minimap";//_Color_126_124_2.png"
	const QString color = "Color";
	const QString end = ".png";

	for (long level = 0; level < 16; level++) {
		QString name = QString::number(level) + ".txt";
		QFile fOut(name);
		fOut.open(QFile::WriteOnly | QFile::Text);
		QTextStream s(&fOut);
		for (long fileNamePosX = start_xPos; fileNamePosX <= end_xPos_included; fileNamePosX++) {
			for (long fileNamePosY = start_yPos; fileNamePosY <= end_yPos_included; fileNamePosY++) {
				qDebug() << "[" + QString::number(fileNamePosX - start_xPos) + "," + QString::number(fileNamePosY - start_yPos) + "]_"+ QString::number(level);

				QString fileXPos = QString::number(fileNamePosX);
				QString fileYPos = QString::number(fileNamePosY);
				QString fileName = minimapStr + floor + color + floor + fileXPos + floor + fileYPos + floor + QString::number(level) + end;
				QString nameOfFileToReadFrom = inputFolder + "\\" + QString::number(level) + "\\" + fileName;
				QImage img;
				bool sucess = img.load(nameOfFileToReadFrom);
				if (sucess) {
					for (long x = 0; x < 256; x++){
						for (long y = 0; y < 256; y++){
							RGBstruct rgb(img.pixel(x, y));
							bool isBlack = rgb.r == 0 && rgb.g == 0 && rgb.b == 0;
							if (isBlack) continue;
							bool isGrass = rgb.r == 0 && rgb.g == 204 && rgb.b == 0;
							if (isGrass) continue;
							bool isWater = rgb.r == 51 && rgb.g == 102 && rgb.b == 153;
							if (isWater) continue;

							QString xPosStr = QString::number(256 * (fileNamePosX - start_xPos) + x);
							QString yPosStr = QString::number(256 * (fileNamePosY - start_yPos) + y);
							QString str = QString::number(rgb.r) + "_" + QString::number(rgb.g) + "_" + QString::number(rgb.b);
							QString toAppend = "[" + xPosStr + "," + yPosStr +"] "+ str;
							s << toAppend << '\n';
						}
					}
				}
			}
		}
		fOut.close();
	}
	
	int endLoop = 4;
}

QStringList Utilities::TOOL_getCodesOfAllInFolder_bottom(QString pathToInputFolder) {
	QDir directory(pathToInputFolder);
	QStringList litOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
	QList<QString> list;
	for (size_t i = 0; i < litOfFIles.size(); i++) {
		QString pathToFile = pathToInputFolder + "\\" + litOfFIles[i];
		QImage* img = new QImage(pathToFile);
		int width = img->width();
		int height = img->height();
		// patern width_height_digits
		// digits = #r#g#b_#r#g#b_#r#g#b_#r#g#b_#r#g#b_#r#g#b 
		QString hash = QString("#");
		QString floor = QString("_");
		QString strWithCode = QString::number(width) + floor + QString::number(height) + floor;
		
		for (size_t x = 0; x < width; x++){
			for (size_t y = 0; y < height; y++){
				uint pixVal = (uint)img->pixel(x, y);
				RGBstruct rgb(pixVal);
				QString r = QString::number(rgb.r);
				QString g = QString::number(rgb.g);
				QString b = QString::number(rgb.b);
				QString strToAppend = hash + r + hash + g + hash + b + floor;
				strWithCode.append(strToAppend);
			}
		}
		list.push_back(strWithCode);
		QString toDisplay = litOfFIles[i] + "______" + strWithCode;
		qDebug() << toDisplay;
	}

	
	return list;
}

QStringList Utilities::TOOL_getCodesOfAllInFolder_regular(QString pathToInputFolder, QString pathToOutputFolder) {
	 QDir directory(pathToInputFolder);
	 QStringList litOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
	 QList<QString> list;
	 for (size_t i = 0; i < litOfFIles.size(); i++) {
		 QString pathToFile = pathToInputFolder + "\\" + litOfFIles[i];
		 QImage img = QImage(pathToFile);
		 Utilities::imgToBlackAndWhiteOneColor(img, 200);
		 QString name = pathToOutputFolder + "\\" + litOfFIles[i];
		 img.save(name);
		 QString code = letterImgToLetterCodeStr(&img);

		 //changing "slash.png" to "slash"
		 QString tmp = litOfFIles[i].left(litOfFIles[i].size() - 4);
		 litOfFIles[i] = tmp;

		 list.push_back(litOfFIles[i] + QString("___") + code);
	 }

	 for each (QString var in list) {
		 qDebug() << var;
	 }
	 return list;
 }

/*
void Utilities::UNSUED_findBoredersOfFrames(QImage fullScreen){
	QImage screeOfFrames(fullScreen);
	auto  minBlack = qRgb(19, 19, 19);
	auto  maxBlack = qRgb(29, 29, 29);
	auto minBlack2 = qRgb(108, 108, 108);
	auto maxBlack2 = qRgb(125, 125, 125);
	auto black = qRgb(0, 0, 0);
	auto white = qRgb(255, 255, 255);
	Utilities::UNUSED_imgToOneColor(screeOfFrames, minBlack, maxBlack, white, true);

	int width = screeOfFrames.width();
	int height = screeOfFrames.height();
	uint pix;
	bool blueIsEnough, redIsEnough, greenIsEnough;
	bool blueIsEnough1, redIsEnough1, greenIsEnough1;
	bool finalBool;
	RGBstruct minV(minBlack);
	RGBstruct maxV(maxBlack);
	RGBstruct minV2(minBlack2);
	RGBstruct maxV2(maxBlack2);
	uint toSet;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			pix = fullScreen.pixel(x, y);
			RGBstruct rgb(pix);
			blueIsEnough = (rgb.b >= minV.b) && (rgb.b <= maxV.b);
			blueIsEnough1 = (rgb.b >= minV2.b) && (rgb.b <= maxV2.b);
			redIsEnough = (rgb.r >= minV.r) && (rgb.r <= maxV.r);
			redIsEnough1 = (rgb.r >= minV2.r) && (rgb.r <= maxV2.r);
			greenIsEnough = (rgb.g >= minV.g) && (rgb.g <= maxV.g);
			greenIsEnough1 = (rgb.g >= minV2.g) && (rgb.g <= maxV2.g);
			finalBool = (blueIsEnough || blueIsEnough1) && (redIsEnough || redIsEnough1) && (greenIsEnough || greenIsEnough1);
			toSet = finalBool ? white : black;
			screeOfFrames.setPixel(x, y, toSet);
		}
	}
	Utilities::saveImgToOutPutFolder(&screeOfFrames, NULL);

	QImage startOfFrame = QImage(3, 3, QImage::Format::Format_RGB32);
}

void Utilities::UNUSED_imgToOneColor(QImage& img, QRgb minimalColorValues, QRgb maxColorValues, QRgb colorToSet, bool allOfThem){
	 int width = img.width();
	 int height = img.height();
	 auto black = qRgb(0, 0, 0);
	 uint minValue = (uint)minimalColorValues;
	 uint maxValue = (uint)maxColorValues;
	 RGBstruct minV(minValue);
	 RGBstruct maxV(maxValue);
	 bool setGiveColor;

	 for (size_t x = 0; x < width; x++) {
		 for (size_t y = 0; y < height; y++) {
			 QRgb colorOfPixel = img.pixel(x, y);
			 RGBstruct current(colorOfPixel);

			 bool redIsEnough = (current.r >= minV.r) && (current.r <= maxV.r);
			 bool greenIsEnough = (current.g >= minV.g) && (current.g <= maxV.g);
			 bool blueIsEnough = (current.b >= minV.b) && (current.b <= maxV.b);
			 
			 if (allOfThem)
				 setGiveColor = (redIsEnough && greenIsEnough && blueIsEnough);
			 else
				 setGiveColor = (redIsEnough || greenIsEnough || blueIsEnough);
		 
			 if (setGiveColor)
				 img.setPixel(x, y, colorToSet);
			 else
				 img.setPixel(x, y, black);
		 }
	 }
 }
 
void Utilities::UNUSED_imgAvoideOneColor(QImage& img, QRgb minimalColorValues, QRgb maxColorValues, bool allOfThem){
	 int width = img.width();
	 int height = img.height();
	 RGBstruct minV((uint)minimalColorValues);
	 RGBstruct maxV((uint)maxColorValues);

	 for (size_t x = 0; x < width; x++) {
		 for (size_t y = 0; y < height; y++) {
			 QRgb colorOfPixel = img.pixel(x, y);
			 RGBstruct current(colorOfPixel);

			 bool redIsEnough = (current.r >= minV.r) && (current.r <= maxV.r);
			 bool greenIsEnough = (current.g >= minV.g) && (current.g <= maxV.g);
			 bool blueIsEnough = (current.b >= minV.b) && (current.b <= maxV.b);

			 bool setGiveColor;
			 allOfThem = allOfThem ? (redIsEnough && greenIsEnough && blueIsEnough) : (redIsEnough || greenIsEnough || blueIsEnough);

			 if (setGiveColor)
				 img.setPixel(x, y, qRgb(0, 0, 0));
		 }
	 }
 }

void Utilities::UNUSED_changeGreyPixelsToBlack(QImage& img, int minGreyVal, int maxGreyVal) {
	 int width = img.width();
	 int height = img.height();
	 for (size_t x = 0; x < width; x++) {
		 for (size_t y = 0; y < height; y++) {
			 uint pixVal = img.pixel(x,y);
			 bool shouldBeBlacked = RGBstruct::isPixelInRangeOfGrey(pixVal, minGreyVal, maxGreyVal);
			 if (shouldBeBlacked)
				 img.setPixel(x, y, qRgb(0,0,0));
		 }
	 }
 }

 int Utilities::UNUSED_modifyBit(int numberToEdit, int postition, int zeroOrOne){
	int mask = 1 << postition;
	return (numberToEdit & ~mask) | ((zeroOrOne << postition) & mask);
}
*/ 