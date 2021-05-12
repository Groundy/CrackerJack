#include "Utilities.h"
#include <QRgb>

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
	 if (res == QMessageBox::StandardButton::Yes)
		 return true;
	 else
		 return false;
 }

 void Utilities::sendKeyStrokeToProcess(Key key, unsigned int PID, QString nameOfWindow) {
    LPCWSTR nameOfWindowLPCWSTR = convert_StrToLPCWSTR(nameOfWindow);
    HWND handler = FindWindow(NULL, nameOfWindowLPCWSTR);
    if (handler == NULL)
        ;//diag err
    DWORD tmp = PID;
    DWORD hThread = GetWindowThreadProcessId(handler, &tmp);
    if (hThread != NULL) {
        int g =0x1470001;
        PostMessage(handler, WM_KEYDOWN, key.number, g);
        PostMessage(handler, WM_KEYUP, key.number, g);
    }
    else
        ;//errToDo
 }
 
 void Utilities::imgToBlackAndWhiteOneColor(QImage* img, int threshold) {
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
			 if (redIsEnough || greenIsEnough || blueIsEnough)
				 img->setPixel(x, y, qRgb(255, 255, 255));
			 else
				 img->setPixel(x, y, qRgb(0, 0, 0));
		 }
	 }
 }

 void Utilities::imgToBlackAndWhiteAllColors(QImage* img, int threshold) {
	 int width = img->width();
	 int height = img->height();
	 uint pixelColor;
	 bool redIsEnough, greenIsEnough, blueIsEnough;
	 for (size_t x = 0; x < width; x++) {
		 for (size_t y = 0; y < height; y++) {
			 pixelColor = img->pixel(x, y);
			 RGBstruct rgb(pixelColor);
			 redIsEnough = rgb.r >= threshold ? true : false;
			 greenIsEnough = rgb.g >= threshold ? true : false;
			 blueIsEnough = rgb.b >= threshold ? true : false;
			 if (redIsEnough && greenIsEnough && blueIsEnough)
				 img->setPixel(x, y, qRgb(255, 255, 255));
			 else
				 img->setPixel(x, y, qRgb(0, 0, 0));
		 }
	 }
 }

 void Utilities::imgToOneColor(QImage* img, QRgb minimalColorValues, QRgb maxColorValues, QRgb colorToSet, bool allOfThem){
	 int width = img->width();
	 int height = img->height();
	 auto black = qRgb(0, 0, 0);
	 uint minValue = (uint)minimalColorValues;
	 uint maxValue = (uint)maxColorValues;
	 RGBstruct minV(minValue);
	 RGBstruct maxV(maxValue);
	 bool redIsEnough, greenIsEnough, blueIsEnough;
	 bool setGiveColor;

	 for (size_t x = 0; x < width; x++) {
		 for (size_t y = 0; y < height; y++) {
			 QRgb colorOfPixel = img->pixel(x, y);
			 RGBstruct current(colorOfPixel);

			 redIsEnough = (current.r >= minV.r) && (current.r <= maxV.r) ? true : false;
			 greenIsEnough = (current.g >= minV.g) && (current.g <= maxV.g) ? true : false;
			 blueIsEnough = (current.b >= minV.b) && (current.b <= maxV.b) ? true : false;
			 
			 if (allOfThem)
				 setGiveColor = (redIsEnough && greenIsEnough && blueIsEnough);
			 else
				 setGiveColor = (redIsEnough || greenIsEnough || blueIsEnough);
		 
			 if (setGiveColor)
				 img->setPixel(x, y, colorToSet);
			 else
				 img->setPixel(x, y, black);
		 }
	 }
 }

 void Utilities::imgAvoideOneColor(QImage* img, QRgb minimalColorValues, QRgb maxColorValues, bool allOfThem){
	 int width = img->width();
	 int height = img->height();
	 auto black = qRgb(0, 0, 0);
	 uint minValue = (uint)minimalColorValues;
	 uint maxValue = (uint)maxColorValues;
	 RGBstruct minV(minValue);
	 RGBstruct maxV(maxValue);
	 bool redIsEnough, greenIsEnough, blueIsEnough;
	 bool setGiveColor;

	 for (size_t x = 0; x < width; x++) {
		 for (size_t y = 0; y < height; y++) {
			 QRgb colorOfPixel = img->pixel(x, y);
			 RGBstruct current(colorOfPixel);

			 redIsEnough = (current.r >= minV.r) && (current.r <= maxV.r) ? true : false;
			 greenIsEnough = (current.g >= minV.g) && (current.g <= maxV.g) ? true : false;
			 blueIsEnough = (current.b >= minV.b) && (current.b <= maxV.b) ? true : false;

			 if (allOfThem)
				 setGiveColor = (redIsEnough && greenIsEnough && blueIsEnough);
			 else
				 setGiveColor = (redIsEnough || greenIsEnough || blueIsEnough);

			 if (setGiveColor)
				 img->setPixel(x, y, black);
		 }
	 }
 }

 void Utilities::changeGreyPixelsToBlack(QImage* img, int minVal, int maxVal) {
	 int width = img->size().width();
	 int height = img->size().height();
	 for (size_t x = 0; x < width; x++) {
		 for (size_t y = 0; y < height; y++) {
			 uint pixVal = img->pixel(x,y);
			 bool shouldBeBlacked = RGBstruct::isPixelInRangeOfGrey(pixVal, minVal, maxVal);
			 if (shouldBeBlacked)
				 img->setPixel(x, y, 0);
		 }
	 }
 }

 QStringList Utilities::TOOL_getCodesOfAllInFolder_regular(QString pathToInputFolder, QString pathToOutputFolder) {
	 QDir directory(pathToInputFolder);
	 QStringList litOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
	 QList<QString> list;
	 for (size_t i = 0; i < litOfFIles.size(); i++) {
		 QString pathToFile = pathToInputFolder + "\\" + litOfFIles[i];
		 QImage* img = new QImage(pathToFile);
		 Utilities::imgToBlackAndWhiteOneColor(img, 200);
		 QString name = pathToOutputFolder + "\\" + litOfFIles[i];
		 img->save(name);
		 QString code = letterImgToLetterCodeStr(img);

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

 void Utilities::cutBlackBordersOfImg(QImage* img) {
	 int linesOfBlackrows_TOP = 0, linesOfBlackrows_DOWN = 0, linesOfBlackrows_RIGHT = 0, linesOfBlackrows_LEFT = 0;
	 int w = img->width();
	 int h = img->height();
	 auto black = qRgb(0, 0, 0);
	 QPoint pt;
	 bool isBlack;
	 for (size_t x = 0; x < w; x++) {
		 for (size_t y = 0; y < h; y++) {
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

	 for (int x = w - 1; x >= 0; x--) {
		 for (int y = 0; y < h; y++) {
			 pt.setX(x);
			 pt.setY(y);
			 isBlack = img->pixel(pt) == black ? true : false;
			 if (!isBlack) {
				 linesOfBlackrows_RIGHT = w - x - 1;
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

	 for (int y = h - 1; y >= 0; y--) {
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

 void Utilities::cutImgWithLettersToSingleLettersImgList(QImage* img, QList<QImage>* letterImages) {
	 QList<int> colThatAreNotBlack;
	 int w = img->width();
	 int h = img->height();
	 auto black = qRgb(0, 0, 0);
	 for (int x = 0; x < w; x++) {
		 for (int y = 0; y < h; y++) {
			 if (img->pixel(x, y) != black) {
				 y = h; //ending inner loop
				 colThatAreNotBlack.push_back(x);
			 }
		 }
	 }
	 if (colThatAreNotBlack.size() == 0) {
		 QList<QImage> toRet;
		 toRet.push_back(*img);
		 *letterImages = toRet;
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
		 QImage letter = img->copy(indexesOfStartOfLetter[i], 0, widths[i], img->height());
		 letterImages->push_back(letter);
	 }
 }

 QString Utilities::imgWithStrToStr(QImage* img) {
	 imgToBlackAndWhiteAllColors(img, 240);
	 cutBlackBordersOfImg(img);
	 QList<QImage>* imgs = new QList<QImage>;
	 cutImgWithLettersToSingleLettersImgList(img, imgs);
	 QString toRet;
	 for (size_t i = 0; i < imgs->size(); i++) {
		 QImage tmp = imgs->at(i);
		 QImage* tmpPointer = &tmp;
		 cutBlackBordersOfImg(tmpPointer);
		 QString letterCode = letterImgToLetterCodeStr(tmpPointer);
		 QChar letter = StrCodeToQChar(letterCode);
		 toRet.append(letter);
	 }
	 delete imgs;
	 return toRet;
 }

 QString Utilities::letterImgToLetterCodeStr(QImage* SingleLetterImg) {
	 int width = SingleLetterImg->width();
	 int height = SingleLetterImg->height();
	 uint pixelColor;
	 QString toRet = QString::number(width) + QString("_") + QString::number(height) + QString("_");
	 int sum;
	 for (size_t x = 0; x < width; x++) {
		 for (size_t y = 0; y < height; y++) {
			 pixelColor = SingleLetterImg->pixel(x, y);
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

 QList<QPoint> Utilities::findStartPositionInImg(QImage* imgToFind, QImage* imgToShareWithin){
	 //it can be opitimalized, now we compare the same pixels few times
	 //fun return list of start positions of imgToFind, position is lef down corner
	 QImage::Format format1 = imgToFind->format();
	 QImage::Format format2 = imgToShareWithin->format();

	 const int widthSmall = imgToFind->width();
	 const int heightSmall = imgToFind->height();
	 const int widthBig = imgToShareWithin->width();
	 const int heightBig = imgToShareWithin->height();


	 bool errWidth = widthSmall >= widthBig;
	 bool errHeight = heightSmall >= heightBig;
	 bool errFormat = format1 != format2;
	 bool anyErr = errWidth || errHeight || errFormat;
	 if (anyErr);//diag /err

	 const int maxPixIndThatShoudldBeTested_X = widthBig - widthSmall;
	 const int maxPixIndThatShoudldBeTested_Y = heightBig - heightSmall;

	 QList<QPoint> startPointsListToRet;
	 QRgb pixFirst, pixSecond;
	 bool foundPosition;

	 for (int x = 0; x <= maxPixIndThatShoudldBeTested_X; x++){
		 for (int y = 0; y <= maxPixIndThatShoudldBeTested_Y; y++){
			 pixFirst = imgToFind->pixel(0, 0);
			 pixSecond = imgToShareWithin->pixel(x, y);
			 if (pixFirst == pixSecond){
				 //first pix matched, looking for more
				 foundPosition = true;
				 for (int x_TMP = 1; x_TMP < widthSmall; x_TMP++){
					 for (int y_TMP = 1; y_TMP < heightSmall; y_TMP++){
						 pixFirst = imgToFind->pixel(x_TMP,y_TMP);
						 pixSecond = imgToShareWithin->pixel(x + x_TMP, y + y_TMP);
						 if (pixFirst != pixSecond) {
							 x_TMP = widthSmall;
							 y_TMP = heightSmall;
							 foundPosition = false;
						 }
					 }
				 }
				 if (foundPosition)
					 startPointsListToRet.push_back(QPoint(x, y));
			 }
		 }
	 }
	 return startPointsListToRet;
 }

QImage Utilities::fromCharToImg(QChar CharToImg){
	auto mapWithCodes = getQmapWithCodes();
	QString keyCode = mapWithCodes.key(CharToImg);
	QStringList parts;
	parts = keyCode.split("_");
	int widthOfLetter = parts[0].toInt();
	int heightOfLetter = parts[1].toInt();
	QString code = parts[2];
	QImage imgToRet(widthOfLetter, heightOfLetter, QImage::Format::Format_ARGB32);
	int i = 0;
	int sizeOfStr = code.size();
	for (size_t x = 0; x < widthOfLetter; x++) {
		for (size_t y = 0; y < heightOfLetter; y++) {
			if (code[i] == '1')
				imgToRet.setPixel(x, y, qRgba(255,255,255,255));
			else if (code[i] == '0')
				imgToRet.setPixel(x, y, qRgba(0, 0, 0, 255));
			else
				;//diag err
			i++;
		}
	}
	return imgToRet;
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
	if (minValueFound >= minValueAcceptable && maxValueFound <= maxValueAcceptable)
		return true;
	else
		return false;
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
			auto pixToSet = qRgb(r, g, b);
			imgToCreate.setPixel(x, y, pixToSet);
		}
	}
	return imgToCreate;
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
	}

	for each (QString var in list) 
		qDebug() << var;
	
	return list;
}

QMap<QString, int> Utilities::getMapWithNumbersFromBottomBar(){
	QMap<QString, int> toRet;
	toRet.insert("4_6_#125#125#125_#223#223#223_#223#223#223_#223#223#223_#223#223#223_#180#180#180_#208#208#208_#83#83#83_#0#0#0_#0#0#0_#29#29#29_#223#223#223_#194#194#194_#125#125#125_#56#56#56_#56#56#56_#111#111#111_#223#223#223_#83#83#83_#223#223#223_#223#223#223_#223#223#223_#223#223#223_#111#111#111_", 0);
	toRet.insert("2_6_#125#125#125_#223#223#223_#167#167#167_#167#167#167_#167#167#167_#223#223#223_#125#125#125_#167#167#167_#167#167#167_#167#167#167_#167#167#167_#223#223#223_", 1);
	toRet.insert("4_6_#167#167#167_#111#111#111_#39#39#39_#4#4#4_#125#125#125_#223#223#223_#223#223#223_#56#56#56_#0#0#0_#83#83#83_#208#208#208_#223#223#223_#194#194#194_#153#153#153_#139#139#139_#223#223#223_#56#56#56_#223#223#223_#83#83#83_#223#223#223_#194#194#194_#69#69#69_#0#0#0_#223#223#223_", 2);
	toRet.insert("2_4_#153#153#153_#194#194#194_#180#180#180_#139#139#139_#223#223#223_#111#111#111_#180#180#180_#223#223#223_", 3);
	toRet.insert("3_4_#194#194#194_#83#83#83_#111#111#111_#167#167#167_#194#194#194_#167#167#167_#194#194#194_#208#208#208_#111#111#111_#111#111#111_#167#167#167_#194#194#194_", 4);
	toRet.insert("4_6_#167#167#167_#223#223#223_#223#223#223_#83#83#83_#41#41#41_#223#223#223_#167#167#167_#97#97#97_#180#180#180_#56#56#56_#0#0#0_#223#223#223_#167#167#167_#56#56#56_#167#167#167_#167#167#167_#125#125#125_#223#223#223_#167#167#167_#56#56#56_#69#69#69_#223#223#223_#223#223#223_#111#111#111_", 5);
	toRet.insert("4_5_#208#208#208_#223#223#223_#223#223#223_#223#223#223_#167#167#167_#139#139#139_#167#167#167_#56#56#56_#29#29#29_#223#223#223_#56#56#56_#167#167#167_#111#111#111_#69#69#69_#223#223#223_#41#41#41_#125#125#125_#223#223#223_#223#223#223_#139#139#139_", 6);
	toRet.insert("2_4_#167#167#167_#56#56#56_#29#29#29_#153#153#153_#167#167#167_#153#153#153_#223#223#223_#208#208#208_", 7);
	toRet.insert("4_6_#139#139#139_#223#223#223_#208#208#208_#194#194#194_#194#194#194_#194#194#194_#223#223#223_#29#29#29_#194#194#194_#111#111#111_#0#0#0_#223#223#223_#208#208#208_#97#97#97_#167#167#167_#194#194#194_#56#56#56_#223#223#223_#125#125#125_#223#223#223_#139#139#139_#208#208#208_#223#223#223_#167#167#167_", 8);
	toRet.insert("2_6_#223#223#223_#29#29#29_#0#0#0_#223#223#223_#0#0#0_#223#223#223_#194#194#194_#125#125#125_#56#56#56_#208#208#208_#153#153#153_#194#194#194_", 9);
	return toRet;
}

int Utilities::getNumberFromBottomBar(QImage* bottomBar){
	auto map = Utilities::getMapWithNumbersFromBottomBar();
	QStringList listOfCodes = map.keys();
	QMap<int, int> anotherMap; // <positionX, value>
	for (size_t i = 0; i < listOfCodes.size(); i++){
		QImage numerImg = Utilities::getImageFromAdvancedCode(listOfCodes[i]);
		int digit = map[listOfCodes[i]];
		auto listOfStartingPoints = findStartPositionInImg(&numerImg, bottomBar);
		for each (QPoint var in listOfStartingPoints)
			anotherMap.insert(var.x(), digit);
	}
	QString strToRe;
	for each (auto key in anotherMap.keys())
		strToRe.push_back(QString::number(anotherMap[key]));
	 
	return strToRe.toInt();
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
	Utilities::imgToOneColor(&screeOfFrames, minBlack, maxBlack, white, true);

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
	Utilities::saveImgToOutPutFolder(&screeOfFrames, "");

	QImage startOfFrame = QImage(3, 3, QImage::Format::Format_RGB32);
}
*/

/*
 unsigned int Utilities::getPIDofProcess(QString nameOfProcess){
     QList<QString> names;
     QList<unsigned int> ids;
     unsigned int toRet;
     getListOfProcess(names, ids);
     for (int i = 0; i < names.size(); i++) {
         if (names[i].contains(nameOfProcess)) {
             if (ids.size() >= i) 
                 return ids[i];
             else
                 return 0;
         }
     }
     return 0;
 }
 */

void Utilities::saveImgToOutPutFolder(QImage* img, QString *extraName){
	QString prefixOfName;
	if (*extraName == NULL)
		prefixOfName = QDateTime::currentDateTime().toString("mm_ss_zzz");
	else
		prefixOfName = *extraName;
	QString fullname = VariablesClass::outPutFolderPath() + "\\_" + prefixOfName + "_" + ".png";
	img->save(fullname);
}

LPCWSTR Utilities::convert_StrToLPCWSTR(QString str){
     return (const wchar_t*)str.utf16();
 }

 