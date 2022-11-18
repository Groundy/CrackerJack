#include "ImgEditor.h"
#include <Calibrator.h>

ImgEditor::ImgEditor(QObject *parent)
	: QObject(parent)
{}

ImgEditor::~ImgEditor()
{}

int ImgEditor::getNumberFromBottomBar(QImage& imgToShearchWithin){
	QMap<QString, int> lightMap, darkMap;
	Utilities::getMapWithNumbersFromBottomBar(lightMap, darkMap);
	QStringList lightCodes = lightMap.keys();
	QStringList darkCodes = darkMap.keys();

	QMap<int, int> anotherMap; // <positionX, value>

	for (size_t i = 0; i < lightCodes.size(); i++){
		QList<QImage*> listWithLightAndDarkLetterImg;
		QImage darkLetter = ImgEditor::getImageFromAdvancedCode(darkCodes[i]);
		QImage lightLetter = ImgEditor::getImageFromAdvancedCode(lightCodes[i]);
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
QImage ImgEditor::fromCharToImg(QChar CharToImg){
	auto mapWithCodes = Utilities::getQmapWithCodes();
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
void ImgEditor::rotateImgToRight(QImage& imgToRotate, int timesToRotateRight){
	QTransform rotating;
	qreal degreeToRotateToRight = (qreal)timesToRotateRight * 90;
	rotating.rotate(degreeToRotateToRight,Qt::Axis::ZAxis);
	imgToRotate = imgToRotate.transformed(rotating);
}
bool ImgEditor::isItPixelFromFrame(uint color, int minValueAcceptable, int maxValueAcceptable, bool requireSameValuesOfRGB){
	RGBstruct rgb(color);
	int minValueFound = rgb.getMinColVal();
	int maxValueFound = rgb.getMaxColVal();
	if (requireSameValuesOfRGB) {
		if (maxValueFound - minValueFound > 1)
			return false;
	}
	bool isPixOfFrame = minValueFound >= minValueAcceptable && maxValueFound <= maxValueAcceptable;
	return isPixOfFrame;
}
void ImgEditor::imgToBlackAndWhiteOneColor(QImage& img, int threshold) {
	const int WIDTH = img.width();
	const int HEIGHT = img.height();
	const uint BLACK = qRgb(0, 0, 0);
	const uint WHITE = qRgb(255, 255, 255);
	for (size_t x = 0; x < WIDTH; x++) {
		for (size_t y = 0; y < HEIGHT; y++) {
			uint pixelColor = img.pixel(x, y);
			bool setWhite = RGBstruct(pixelColor).oneColEqualOrAboveThreshold(threshold);
			uint toSet = setWhite ? WHITE : BLACK;
			img.setPixel(x, y, toSet);
		}
	}
}
void ImgEditor::imgToBlackAndWhiteAllColors(QImage& img, int threshold) {
	const int WIDTH = img.width();
	const int HEIGHT = img.height();
	const uint BLACK = qRgb(0, 0, 0);
	const uint WHITE = qRgb(255, 255, 255);
	for (size_t x = 0; x < WIDTH; x++) {
		for (size_t y = 0; y < HEIGHT; y++) {
			uint pixelColor = img.pixel(x, y);
			RGBstruct rgb(pixelColor);
			bool setWhite = rgb.allColsEqualOrAboveThreshold(threshold);
			uint pixToSet = setWhite ? WHITE : BLACK;
			img.setPixel(x, y, pixToSet);
		}
	}
 }
void ImgEditor::cutBlackBordersOfImg(QImage& img) {
    int linesOfBlackRows_TOP = 0, linesOfBlackRows_DOWN = 0, linesOfBlackRows_RIGHT = 0, linesOfBlackRows_LEFT = 0;
    const int WIDTH = img.width();
    const int HEIGHT = img.height();
    const uint BLACK = qRgb(0, 0, 0);
    for (int x = 0; x < WIDTH; x++) {
	    for (int y = 0; y < HEIGHT; y++) {
		    bool isBlack = img.pixel(x, y) == BLACK;
		    if (!isBlack) {
			    linesOfBlackRows_LEFT = x;
			    x = WIDTH; //endOfLoop
			    y = HEIGHT; //endOfLoop
		    }
	    }
    }

    for (int x = WIDTH - 1; x >= linesOfBlackRows_LEFT; x--) {
	    for (int y = 0; y < HEIGHT; y++) {
		    bool isBlack = img.pixel(x, y) == BLACK;
		    if (!isBlack) {
			    linesOfBlackRows_RIGHT = WIDTH - x - 1;
			    x = -1;//endOfLoop
			    y = HEIGHT;//endOfLoop
		    }
	    }
    }

    for (int y = 0; y < HEIGHT; y++) {
	    for (int x = linesOfBlackRows_LEFT; x < WIDTH - linesOfBlackRows_RIGHT; x++) {
		    bool isBlack = img.pixel(x, y) == BLACK;
		    if (!isBlack) {
			    linesOfBlackRows_TOP = y;
			    x = WIDTH;//endOfLoop
			    y = HEIGHT;//endOfLoop
		    }
	    }
    }

    for (int y = HEIGHT - 1; y >= linesOfBlackRows_TOP; y--) {
	    for (int x = linesOfBlackRows_LEFT; x < WIDTH - linesOfBlackRows_RIGHT; x++) {
		    bool isBlack = img.pixel(x, y) == BLACK;
		    if (!isBlack) {
			    linesOfBlackRows_DOWN = HEIGHT - y - 1;
			    x = WIDTH;//endOfLoop
			    y = -1;//endOfLoop
		    }
	    }
    }
    //tmp to return if modifed algorithm was working wrong
    /*
    *     for (int x = 0; x < WIDTH; x++) {
	    for (int y = 0; y < HEIGHT; y++) {
		    bool isBlack = img.pixel(x, y) == BLACK;
		    if (!isBlack) {
			    linesOfBlackRows_LEFT = x;
			    x = WIDTH; //endOfLoop
			    y = WIDTH; //endOfLoop
		    }
	    }
    }

    for (int x = WIDTH - 1; x >= 0; x--) {
	    for (int y = 0; y < HEIGHT; y++) {
		    bool isBlack = img.pixel(x, y) == BLACK;
		    if (!isBlack) {
			    linesOfBlackRows_RIGHT = WIDTH - x - 1;
			    x = -1;//endOfLoop
			    y = HEIGHT;//endOfLoop
		    }
	    }
    }

    for (int y = 0; y < HEIGHT; y++) {
	    for (int x = 0; x < WIDTH; x++) {
		    bool isBlack = img.pixel(x, y) == BLACK;
		    if (!isBlack) {
			    linesOfBlackRows_TOP = y;
			    x = WIDTH;//endOfLoop
			    y = HEIGHT;//endOfLoop
		    }
	    }
    }

    for (int y = HEIGHT - 1; y >= 0; y--) {
	    for (int x = 0; x < WIDTH; x++) {
		    bool isBlack = img.pixel(x, y) == BLACK;
		    if (!isBlack) {
			    linesOfBlackRows_DOWN = HEIGHT - y - 1;
			    x = WIDTH;//endOfLoop
			    y = -1;//endOfLoop
		    }
	    }
    }
    */

    int anotherParametr_x = WIDTH - linesOfBlackRows_RIGHT - linesOfBlackRows_LEFT;
    int anotherParametr_y = HEIGHT - linesOfBlackRows_TOP - linesOfBlackRows_DOWN;

    int widthToCut = anotherParametr_x >= 0 ? anotherParametr_x : 0;
    int heightToCut = anotherParametr_y >= 0 ? anotherParametr_y : 0;
    img = img.copy(linesOfBlackRows_LEFT, linesOfBlackRows_TOP, widthToCut, heightToCut);
 }
void ImgEditor::cutImgWithLettersToSingleLettersImgList(QImage& img, QList<QImage>& letterImages) {
	QList<int> colThatAreNotBlack;
	const int WIDTH = img.width();
    const int HEIGHT = img.height();
    const uint BLACK = qRgb(0, 0, 0);
    for (int x = 0; x < WIDTH; x++) {
       for (int y = 0; y < HEIGHT; y++) {
	       bool isNotEmptyLine = img.pixel(x, y) != BLACK;
	       if (isNotEmptyLine) {
		       colThatAreNotBlack.push_back(x);
			   y = HEIGHT;
	       }
       }
    }
    if (colThatAreNotBlack.isEmpty()) {
       //letterImages.push_back(img);
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
		QRect rectToCut(indexesOfStartOfLetter[i], 0, widths[i], img.height());
		QImage letter = img.copy(rectToCut);
		letterImages.push_back(letter);
    }
 }
QString ImgEditor::imgWithStrToStr(QImage& img) {
	imgToBlackAndWhiteAllColors(img, 240);
	cutBlackBordersOfImg(img);
    QList<QImage> imgs;
	cutImgWithLettersToSingleLettersImgList(img, imgs);
    QString toRet;
    for (size_t i = 0; i < imgs.size(); i++) {
		QImage tmp = imgs.at(i);
	    cutBlackBordersOfImg(tmp);
	    QString letterCode = letterImgToLetterCodeStr(&tmp);
	    QString letter = Utilities::StrCodeToLetter(letterCode);
		if(letter != '\0')
			toRet.append(letter);
    }
    return toRet;
 }
QString ImgEditor::letterImgToLetterCodeStr(QImage* SingleLetterImg) {
    const int WIDTH = SingleLetterImg->width();
    const int HEIGHT = SingleLetterImg->height();
    const QString FLOOR = QString("_");
    const QString ZERO = QString("0"), ONE = QString("1");
    QString toRet = QString::number(WIDTH) + FLOOR + QString::number(HEIGHT) + FLOOR;
    for (size_t x = 0; x < WIDTH; x++) {
	    for (size_t y = 0; y < HEIGHT; y++) {
		    uint pixelColor = SingleLetterImg->pixel(x, y);
			bool isBlack = RGBstruct(pixelColor).isBlack();
		    QString toAppend = isBlack ? ZERO : ONE;
		    toRet.append(toAppend);
	    }
    }
    return toRet;
 }
QImage ImgEditor::getImageFromAdvancedCode(QString codeOfImg){
	QStringList partsOfCode = codeOfImg.split(QString("_"), Qt::SkipEmptyParts);
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



