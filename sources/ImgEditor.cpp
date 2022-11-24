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
		QList<QPoint> listOfStartingPoints = ImgEditor::findStartPositionInImg_mulitpeImgs(listWithLightAndDarkLetterImg, imgToShearchWithin);// (listWithLightAndDarkLetterImg, imgToShearchWithin);
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
		//Logger::logPotenialBug("Wrong structure of char code for char: " + QString(CharToImg), "Utilities", "fromCharToImg");
		return QImage();
	}
}
void ImgEditor::rotateImgToRight(QImage& imgToRotate, int timesToRotateRight){
	QTransform rotating;
	qreal degreeToRotateToRight = (qreal)timesToRotateRight * 90;
	rotating.rotate(degreeToRotateToRight,Qt::Axis::ZAxis);
	imgToRotate = imgToRotate.transformed(rotating);
}
bool ImgEditor::isItPixelFromFrame(const uint& color, const int& minValueAcceptable, const int& maxValueAcceptable, bool requireSameValuesOfRGB){
	RGBstruct rgb(color);

	int maxValueFound = rgb.getMaxColVal();
	if (maxValueFound > maxValueAcceptable)
		return false;

	int minValueFound = rgb.getMinColVal();
	if (minValueFound <  minValueAcceptable)
		return false;


	if (requireSameValuesOfRGB) {
		if (maxValueFound - minValueFound > 1)
			return false;
	}

	return true;
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
QList<QPoint> ImgEditor::findStartPositionInImg_mulitpeImgs(QList<QImage*> imgsToFind, QImage& imgToShareWithin) {
	// this fun return starting points from imgToSharePoints than consist pixels from one of imgsToFind

	QImage::Format formatBig = imgToShareWithin.format();
	for each (QImage * var in imgsToFind) {
		if (formatBig != var->format()) {
			////Logger::logPotenialBug("Not all imgs have the same format!", "Calibrator", "findStartPositionInImg_mulitpeImgs");
			return QList<QPoint>();//[TO DO zamienic ta funkcje zeby zwracala error code]
		}
	}

	const int WIDTH_OF_FIRST_IMG = imgsToFind[0]->width();
	const int HEIGHT_OF_FIRST_IMG = imgsToFind[0]->height();
	for each (QImage * var in imgsToFind) {
		bool widthIsGood = var->width() == WIDTH_OF_FIRST_IMG;
		bool heightIsGood = var->height() == HEIGHT_OF_FIRST_IMG;
		bool isAllRight = widthIsGood && heightIsGood;
		if (!isAllRight) {
			//Logger::logPotenialBug("Not all imgs have the same size!", "Calibrator", "findStartPositionInImg_mulitpeImgs");
			return QList<QPoint>();//[TO DO zamienic ta funkcje zeby zwracala error code]
		}
	}

	const int MAX_WIDTH_VAL_TO_SHEARCH = imgToShareWithin.width() - WIDTH_OF_FIRST_IMG;
	const int MAX_HEIGHT_VAL_TO_SHEARCH = imgToShareWithin.height() - HEIGHT_OF_FIRST_IMG;

	QList<QPoint> startPointsListToRet;
	for (int x = 0; x <= MAX_WIDTH_VAL_TO_SHEARCH; x++) {
		for (int y = 0; y <= MAX_HEIGHT_VAL_TO_SHEARCH; y++) {
			bool atLeastOnePixIsMatched = false;
			uint pixFromImg_big = imgToShareWithin.pixel(x, y);

			for each (QImage * var in imgsToFind) {
				uint pixFromImg_small = var->pixel(0, 0);
				if (pixFromImg_big == pixFromImg_small) {
					atLeastOnePixIsMatched = true;
					break;
				}
			}

			if (atLeastOnePixIsMatched) {
				//first pix matched, looking for more
				int wrongPixels = WIDTH_OF_FIRST_IMG * HEIGHT_OF_FIRST_IMG;
				for (int x_TMP = 0; x_TMP < WIDTH_OF_FIRST_IMG; x_TMP++) {
					for (int y_TMP = 0; y_TMP < HEIGHT_OF_FIRST_IMG; y_TMP++) {
						uint pixFromImg_big2 = imgToShareWithin.pixel(x + x_TMP, y + y_TMP);
						bool metReq = false;
						for each (QImage * var in imgsToFind) {
							uint pixFromImg_small2 = var->pixel(x_TMP, y_TMP);
							bool pixMatched = pixFromImg_big2 == pixFromImg_small2;
							if (pixMatched) {
								metReq = true;
								break;
							}
						}
						if (metReq)
							wrongPixels--;
					}
				}
				if (wrongPixels == 0)
					startPointsListToRet.push_back(QPoint(x, y));
			}
		}
	}
	return startPointsListToRet;
}
QList<QPoint> ImgEditor::findStartPositionInImg(const QImage& imgToFind, const QImage& imgToSearchWithin) {
	//fun return list of start positions of imgToFind, position is lef down corner
	const int WIDTH_SMALL_PIC = imgToFind.width();
	const int HEIGHT_SMALL_PIC = imgToFind.height();
	const int WIDTH_BIG_PIC = imgToSearchWithin.width();
	const int HEIGHT_BIG_PIC = imgToSearchWithin.height();

	bool errEmpty = WIDTH_SMALL_PIC <= 0 || HEIGHT_SMALL_PIC <= 0 || WIDTH_BIG_PIC <= 0 || HEIGHT_BIG_PIC <= 0;
	bool errWidth = WIDTH_SMALL_PIC >= WIDTH_BIG_PIC;
	bool errHeight = HEIGHT_SMALL_PIC >= HEIGHT_BIG_PIC;
	bool errFormat = imgToFind.format() != imgToSearchWithin.format();
	bool anyErr = errWidth || errHeight || errFormat || errEmpty;
	if (anyErr) {
		//Logger::logPotenialBug("Wrong input of two img", "Calibrator", "findStartPositionInImg");
		return QList<QPoint>();
	}

	const int MAX_X_INDEX_TO_CHECK = WIDTH_BIG_PIC - WIDTH_SMALL_PIC;
	const int MAX_Y_INDEX_TO_CHECK = HEIGHT_BIG_PIC - HEIGHT_SMALL_PIC;

	QList<QPoint> startPointsListToRet;
	for (int x = 0; x <= MAX_X_INDEX_TO_CHECK; x++) {
		for (int y = 0; y <= MAX_Y_INDEX_TO_CHECK; y++) {
			uint pixSmallImg = imgToFind.pixel(0, 0);
			uint pixBigImg = imgToSearchWithin.pixel(x, y);
			bool firstPixelMatched = pixSmallImg == pixBigImg;
			if (firstPixelMatched) {
				bool foundPosition = true;
				for (int x_TMP = 1; x_TMP < WIDTH_SMALL_PIC; x_TMP++) {
					for (int y_TMP = 1; y_TMP < HEIGHT_SMALL_PIC; y_TMP++) {
						pixSmallImg = imgToFind.pixel(x_TMP, y_TMP);
						pixBigImg = imgToSearchWithin.pixel(x + x_TMP, y + y_TMP);
						bool pixNotMatched = pixSmallImg != pixBigImg;
						if (pixNotMatched) {
							x_TMP = WIDTH_SMALL_PIC;
							y_TMP = HEIGHT_SMALL_PIC;
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



