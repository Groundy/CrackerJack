#include "ImgEditor.h"

ImgEditor::ImgEditor() {}
ImgEditor::~ImgEditor() {}

QImage ImgEditor::fromCharToImg(QChar CharToImg){
	QString keyCode = normalLettersMap.key(CharToImg);
	QStringList parts;
	parts = keyCode.split("_");
	if (parts.size() == 3) {
		const int WIDTH = parts[0].toInt();
		const int HEIGHT = parts[1].toInt();
		QString code = parts[2];
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
    int anotherParametr_x = WIDTH - linesOfBlackRows_RIGHT - linesOfBlackRows_LEFT;
    int anotherParametr_y = HEIGHT - linesOfBlackRows_TOP - linesOfBlackRows_DOWN;

    int widthToCut = anotherParametr_x >= 0 ? anotherParametr_x : 0;
    int heightToCut = anotherParametr_y >= 0 ? anotherParametr_y : 0;
    img = img.copy(linesOfBlackRows_LEFT, linesOfBlackRows_TOP, widthToCut, heightToCut);
 }
QList<QImage> ImgEditor::cutImgWithLettersToSingleLettersImgList(QImage& img) {
	QList<QImage> letterImages;
	QList<int> colThatAreNotBlack;
	const int WIDTH = img.width();
    const int HEIGHT = img.height();
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
       return letterImages;
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
	return letterImages;
 }
QString ImgEditor::imgWithStrToStr(QImage& img, int threshold) {
	imgToBlackAndWhiteAllColors(img, threshold);
	cutBlackBordersOfImg(img);
    QList<QImage> lettersImgs = cutImgWithLettersToSingleLettersImgList(img);
    QString toRet;
	for each (QImage img in lettersImgs){
	    cutBlackBordersOfImg(img);
	    QString letterCode = binaryLetterImgToCode(img);
	    QString letter = normalLettersMap.value(letterCode);
		if (letter == '\0')
			continue;
		toRet.append(letter);
	}
    return toRet;
 }
QString ImgEditor::binaryLetterImgToCode(QImage& singleLetterImg) {
    const int WIDTH = singleLetterImg.width();
    const int HEIGHT = singleLetterImg.height();
    const QString ZERO = QString("0"), ONE = QString("1");
	QString toRet = QString("%1_%2_").arg(QString::number(WIDTH), QString::number(HEIGHT));
    for (size_t x = 0; x < WIDTH; x++) {
	    for (size_t y = 0; y < HEIGHT; y++) {
		    bool isBlack = singleLetterImg.pixel(x, y) == BLACK;
		    toRet.append(isBlack ? ZERO : ONE);
	    }
    }
    return toRet;
 }
QImage ImgEditor::getColorImageFromCode(const QString& codeOfImg){
	try{
		QStringList partsOfCode = codeOfImg.split(QString("#"), Qt::SkipEmptyParts);
		if(partsOfCode.size() != 3)
			throw std::exception(" Wrong format of color img code!");

		const int WIDTH = partsOfCode[0].toInt();
		const int HEIGHT = partsOfCode[1].toInt();

		if (WIDTH <= 0 || HEIGHT <= 0)
			throw std::exception(" Wrong format of color img code!, sizesz should be positive!");


		const int HEX_LENGTH = 8;
		if (partsOfCode[2].size() % HEX_LENGTH != 0)
			throw std::exception(" Wrong format of color img code!, pixValues are not n*8 length!");

		QStringList hexValuesOfPixels = {};
		for (size_t i = 0; i <= partsOfCode[2].size(); i += HEX_LENGTH)
			hexValuesOfPixels.push_back(partsOfCode[3].mid(i, HEX_LENGTH));


		QImage imgToCreate(WIDTH, HEIGHT, QImage::Format::Format_ARGB32);
		int i = 0;
		for (size_t x = 0; x < WIDTH; x++){
			for (size_t y = 0; y < HEIGHT; y++){
				uint pixVal = hexValuesOfPixels[i].toUInt();
				imgToCreate.setPixel(x, y, pixVal);
				i++;
			}
		}
		return imgToCreate;
	}
	catch (const std::exception& e){
		Logger::staticLog(e.what());
		return QImage();
	}
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
void ImgEditor::saveMultipleImgs(const QImage& fullImg, QList<QRect> frames, QString pathToFolder, QString fileNameWithoutExtension) {
	for (int i = 0; i < frames.size(); i++) {
		QImage img = fullImg.copy(frames[i]);
		QString fullPath = QString("%1\\%2_%3.png").arg(pathToFolder, QString::number(i), fileNameWithoutExtension);
		img.save(fullPath);
	}
}
void ImgEditor::saveMultipleImgs(const QList<QImage>& imgs, QString pathToFolder, QString fileNameWithoutExtension) {
	for (int i = 0; i < imgs.size(); i++) {
		QString fullPath = QString("%1\\%2_%3.png").arg(pathToFolder, QString::number(i), fileNameWithoutExtension);
		imgs[i].save(fullPath);
	}
}
QList<QPoint> ImgEditor::findStartPositionInImg(const QImage& imgToFind, const QImage& imgToSearchWithin, QRect frameInBigWindow) {
	try{
		const int WIDTH_SMALL_PIC = imgToFind.width();
		const int HEIGHT_SMALL_PIC = imgToFind.height();
		const int WIDTH_BIG_PIC = imgToSearchWithin.width();
		const int HEIGHT_BIG_PIC = imgToSearchWithin.height();

		if (imgToFind.isNull() || imgToSearchWithin.isNull())
			throw std::exception("Cant find postion, one of imgs is null");
		if(WIDTH_SMALL_PIC > WIDTH_BIG_PIC)
			throw std::exception("Cant find postion, Wrong imgs size");
		if (HEIGHT_SMALL_PIC > HEIGHT_BIG_PIC)
			throw std::exception("Cant find postion, Wrong imgs size");
		if(imgToFind.format() != imgToSearchWithin.format())
			throw std::exception("Cant find postion, wrong formats");
		if(frameInBigWindow.right() > WIDTH_BIG_PIC)
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
	catch (const std::exception& e){
		Logger::staticLog(e.what());
		return QList<QPoint>();
	}
}
QPoint ImgEditor::findExactStartPositionInImg(const QImage& imgToFind, const QImage& imgToSearchWithin, QRect frameInBigWindow) {
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

		QList<QPoint> startPoints;
		for (int x = minIndexToCheckX; x <= maxIndexToCheckX; x++) {
			for (int y = minIndexToCheckY; y <= maxIndexToCheckY; y++) {
				if (startPoints.size() > 1)
					throw std::exception("more than one point found in findExactStartPoint fun!");

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
					startPoints.push_back(QPoint(x, y));
			}
		}
		if (startPoints.size() != 1)
			throw std::exception("not exactly one position found in findExactStartPoint fun!");
		return startPoints[0];
	}
	catch (const std::exception& e) {
		Logger::staticLog(e.what());
		return QPoint();
	}
}