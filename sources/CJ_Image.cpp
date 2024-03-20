#include "CJ_Image.h"

void CJ_Image::toBlackAndWhite(const int threshold) {
  const int WIDTH  = this->width();
  const int HEIGHT = this->height();
  for (size_t x = 0; x < WIDTH; x++) {
    for (size_t y = 0; y < HEIGHT; y++) {
      const uint pixelColor = this->pixel(x, y);
      const bool setWhite   = RGBstruct(pixelColor).allColsEqualOrAboveThreshold(threshold);
      const uint pixToSet   = setWhite ? WHITE : BLACK;
      this->setPixel(x, y, pixToSet);
    }
  }
}

void CJ_Image::toBlackAndWhiteOneColor(const int threshold) {
  const int WIDTH  = this->width();
  const int HEIGHT = this->height();
  for (size_t x = 0; x < WIDTH; x++) {
    for (size_t y = 0; y < HEIGHT; y++) {
      const uint pixelColor = this->pixel(x, y);
      const bool setWhite   = RGBstruct(pixelColor).oneColEqualOrAboveThreshold(threshold);
      const uint toSet      = setWhite ? WHITE : BLACK;
      this->setPixel(x, y, toSet);
    }
  }
}

void CJ_Image::toBlackAndWhiteExactColor(const QRgb COLOR) {
  for (size_t x = 0; x < this->height(); x++) {
    for (size_t y = 0; y < this->width(); y++) {
      bool setWhite = this->pixel(x, y) == COLOR;
      this->setPixel(x, y, setWhite ? WHITE : BLACK);
    }
  }
}

void CJ_Image::toBlackAndWhiteAllColors(const int threshold) {
  const int WIDTH  = this->width();
  const int HEIGHT = this->height();
  for (size_t x = 0; x < WIDTH; x++) {
    for (size_t y = 0; y < HEIGHT; y++) {
      uint      pixelColor = this->pixel(x, y);
      RGBstruct rgb(pixelColor);
      bool      setWhite = rgb.allColsEqualOrAboveThreshold(threshold);
      uint      pixToSet = setWhite ? WHITE : BLACK;
      this->setPixel(x, y, pixToSet);
    }
  }
}

void CJ_Image::cutBlackBorders() {
  int       linesOfBlackRows_TOP = 0, linesOfBlackRows_DOWN = 0, linesOfBlackRows_RIGHT = 0, linesOfBlackRows_LEFT = 0;
  const int WIDTH   = this->width();
  const int HEIGHT  = this->height();
  bool      process = true;
  for (size_t x = 0; x < WIDTH && process; x++) {
    for (size_t y = 0; y < HEIGHT && process; y++) {
      if (!(this->pixel(x, y) == BLACK)) {
        linesOfBlackRows_LEFT = x;
        process               = true;
      }
    }
  }

  process = true;
  for (size_t x = WIDTH - 1; x >= linesOfBlackRows_LEFT && process; x--) {
    for (size_t y = 0; y < HEIGHT && process; y++) {
      if (!(this->pixel(x, y) == BLACK)) {
        linesOfBlackRows_RIGHT = WIDTH - x - 1;
        process                = false;
      }
    }
  }

  process = true;
  for (size_t y = 0; y < HEIGHT && process; y++) {
    for (size_t x = linesOfBlackRows_LEFT; x < WIDTH - linesOfBlackRows_RIGHT && process; x++) {
      if (!(this->pixel(x, y) == BLACK)) {
        linesOfBlackRows_TOP = y;
        process              = false;
      }
    }
  }

  process = true;
  for (size_t y = HEIGHT - 1; y >= linesOfBlackRows_TOP && process; y--) {
    for (size_t x = linesOfBlackRows_LEFT; x < WIDTH - linesOfBlackRows_RIGHT && process; x++) {
      if (!(this->pixel(x, y) == BLACK)) {
        linesOfBlackRows_DOWN = HEIGHT - y - 1;
        process               = false;
      }
    }
  }
  const uint anotherParametr_x = WIDTH - linesOfBlackRows_RIGHT - linesOfBlackRows_LEFT;
  const uint anotherParametr_y = HEIGHT - linesOfBlackRows_TOP - linesOfBlackRows_DOWN;

  const uint widthToCut  = std::clamp(anotherParametr_x, static_cast<uint>(0), anotherParametr_x);
  const uint heightToCut = std::clamp(anotherParametr_y, static_cast<uint>(0), anotherParametr_y);

  *this = this->copy(linesOfBlackRows_LEFT, linesOfBlackRows_TOP, widthToCut, heightToCut);
}

QList<CJ_Image> CJ_Image::toImageListWithSingleLetters() const {
  QList<CJ_Image> letterImages;
  QList<uint>     colThatAreNotBlack;
  const int       WIDTH  = this->width();
  const int       HEIGHT = this->height();
  for (size_t x = 0; x < WIDTH; x++) {
    for (size_t y = 0; y < HEIGHT; y++) {
      bool isNotEmptyLine = this->pixel(x, y) != BLACK;
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
  for (size_t i = 0; i < colThatAreNotBlack.size() - 1; i++) {
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

  for (size_t i = 0; i < indexesOfStartOfLetter.size(); i++) {
    widths.push_back(indexesOfEndsOfLetters[i] - indexesOfStartOfLetter[i] + 1);
  }

  for (size_t i = 0; i < indexesOfStartOfLetter.size(); i++) {
    QRect  rectToCut(indexesOfStartOfLetter[i], 0, widths[i], this->height());
    QImage letter = this->copy(rectToCut);
    letterImages.push_back(letter);
  }
  return letterImages;
}

QString CJ_Image::binaryLetterImgToCode() const {
  const int     WIDTH  = this->width();
  const int     HEIGHT = this->height();
  const QString ZERO = QString("0"), ONE = QString("1");
  QString       toRet = QString("%1_%2_").arg(QString::number(WIDTH), QString::number(HEIGHT));
  for (size_t x = 0; x < WIDTH; x++) {
    for (size_t y = 0; y < HEIGHT; y++) {
      bool isBlack = this->pixel(x, y) == BLACK;
      toRet.append(isBlack ? ZERO : ONE);
    }
  }
  return toRet;
}

QString CJ_Image::toString(int threshold) {
  this->toBlackAndWhite(threshold);
  this->cutBlackBorders();
  QList<CJ_Image> lettersImgs = this->toImageListWithSingleLetters();
  QString         toRet;
  foreach (CJ_Image img, lettersImgs) {
    img.cutBlackBorders();
    QString letterCode = img.binaryLetterImgToCode();
    QString letter     = standardLetters.value(letterCode);
    if (letter == '\0') {
      continue;
    }
    toRet.append(letter);
  }
  return toRet;
}

QVector<QPoint> CJ_Image::findStartPositionInImg(const QImage& imgToFind, QRect frameInBigWindow) const {
  const int WIDTH_SMALL_PIC  = imgToFind.width();
  const int HEIGHT_SMALL_PIC = imgToFind.height();
  const int WIDTH_BIG_PIC    = this->width();
  const int HEIGHT_BIG_PIC   = this->height();

  if (WIDTH_SMALL_PIC > WIDTH_BIG_PIC) {
    qWarning() << "Cant find postion, Wrong imgs size";
    return QVector<QPoint>();
  }
  if (HEIGHT_SMALL_PIC > HEIGHT_BIG_PIC) {
    qWarning() << "Cant find postion, Wrong imgs size";
    return QVector<QPoint>();
  }
  if (imgToFind.format() != this->format()) {
    qWarning() << "Cant find postion, wrong formats";
    return QVector<QPoint>();
  }
  if (frameInBigWindow.right() > WIDTH_BIG_PIC) {
    qWarning() << "Cant find postion, Wrong frame size";
    return QVector<QPoint>();
  }
  if (frameInBigWindow.bottom() > HEIGHT_BIG_PIC) {
    qWarning() << "Cant find postion, Wrong frame size";
    return QVector<QPoint>();
  }

  const int maxIndexToCheckX = frameInBigWindow.isEmpty() ? WIDTH_BIG_PIC - WIDTH_SMALL_PIC : frameInBigWindow.right();
  const int maxIndexToCheckY = frameInBigWindow.isEmpty() ? HEIGHT_BIG_PIC - HEIGHT_SMALL_PIC : frameInBigWindow.bottom();
  const int minIndexToCheckX = frameInBigWindow.isEmpty() ? 0 : frameInBigWindow.left();
  const int minIndexToCheckY = frameInBigWindow.isEmpty() ? 0 : frameInBigWindow.top();

  QVector<QPoint> startPointsListToRet;
  for (size_t x = minIndexToCheckX; x <= maxIndexToCheckX; x++) {
    for (size_t y = minIndexToCheckY; y <= maxIndexToCheckY; y++) {
      uint pixSmallImg = imgToFind.pixel(0, 0);
      uint pixBigImg   = this->pixel(x, y);
      if (pixSmallImg != pixBigImg) {
        continue;
      }

      bool foundPosition = true;
      for (int offsetX = 1; offsetX < WIDTH_SMALL_PIC; offsetX++) {
        for (int offsetY = 1; offsetY < HEIGHT_SMALL_PIC; offsetY++) {
          pixSmallImg = imgToFind.pixel(offsetX, offsetY);
          pixBigImg   = this->pixel(x + offsetX, y + offsetY);
          if (pixBigImg == pixSmallImg) {
            continue;
          }
          //break both loops
          offsetX       = WIDTH_SMALL_PIC;
          offsetY       = HEIGHT_SMALL_PIC;
          foundPosition = false;
        }
      }
      if (foundPosition) startPointsListToRet.push_back(QPoint(x, y));
    }
  }
  return startPointsListToRet;
}

QPoint CJ_Image::findExactStartPositionInImg(const QImage& imgToFind, QRect frameInBigWindow) const {
  const int WIDTH_SMALL_PIC  = imgToFind.width();
  const int HEIGHT_SMALL_PIC = imgToFind.height();
  const int WIDTH_BIG_PIC    = this->width();
  const int HEIGHT_BIG_PIC   = this->height();

  if (WIDTH_SMALL_PIC > WIDTH_BIG_PIC) {
    qWarning() << "Cant find postion, Wrong imgs size";
    return QPoint();
  }
  if (HEIGHT_SMALL_PIC > HEIGHT_BIG_PIC) {
    qWarning() << "Cant find postion, Wrong imgs size";
    return QPoint();
  }
  if (imgToFind.format() != this->format()) {
    qWarning() << "Cant find postion, wrong formats";
  }
  if (frameInBigWindow.right() > WIDTH_BIG_PIC) {
    qWarning() << "Cant find postion, Wrong frame size";
    return QPoint();
  }
  if (frameInBigWindow.bottom() > HEIGHT_BIG_PIC) {
    qWarning() << "Cant find postion, Wrong frame size";
    return QPoint();
  }

  const int maxIndexToCheckX = frameInBigWindow.isEmpty() ? WIDTH_BIG_PIC - WIDTH_SMALL_PIC : frameInBigWindow.right();
  const int maxIndexToCheckY = frameInBigWindow.isEmpty() ? HEIGHT_BIG_PIC - HEIGHT_SMALL_PIC : frameInBigWindow.bottom();
  const int minIndexToCheckX = frameInBigWindow.isEmpty() ? 0 : frameInBigWindow.left();
  const int minIndexToCheckY = frameInBigWindow.isEmpty() ? 0 : frameInBigWindow.top();

  QVector<QPoint> startPoints;
  for (size_t x = minIndexToCheckX; x <= maxIndexToCheckX; x++) {
    for (size_t y = minIndexToCheckY; y <= maxIndexToCheckY; y++) {
      if (startPoints.size() > 1) {
        qDebug() << "more than one point found in findExactStartPoint fun!";
        return QPoint();
      }

      uint pixSmallImg = imgToFind.pixel(0, 0);
      uint pixBigImg   = this->pixel(x, y);
      if (pixSmallImg != pixBigImg) {
        continue;
      }

      bool foundPosition = true;
      bool process       = true;
      for (int offsetX = 1; offsetX < WIDTH_SMALL_PIC && process; offsetX++) {
        for (int offsetY = 1; offsetY < HEIGHT_SMALL_PIC && process; offsetY++) {
          pixSmallImg = imgToFind.pixel(offsetX, offsetY);
          pixBigImg   = this->pixel(x + offsetX, y + offsetY);
          if (pixBigImg == pixSmallImg) {
            continue;
          }
          process       = false;
          foundPosition = false;
        }
      }
      if (foundPosition) {
        startPoints.push_back(QPoint(x, y));
      }
    }
  }
  if (startPoints.size() != 1) {
    qWarning() << "not exactly one position found in findExactStartPoint fun!";
    return QPoint();
  }
  return startPoints[0];
}

void CJ_Image::rotateImgToRight(const int timesToRotateRight) {
  QTransform rotating;
  qreal      degreeToRotateToRight = (qreal)timesToRotateRight * 90;
  rotating.rotate(degreeToRotateToRight, Qt::Axis::ZAxis);
  *this = this->transformed(rotating);
}

QVector<QPoint> CJ_Image::findStartPositionInImg_mulitpeImgs(const QList<QImage>& imgsToFind) const {
  // this fun return starting points from imgToSharePoints than consist pixels from one of imgsToFind

  QImage::Format formatBig = this->format();
  foreach (const QImage& var, imgsToFind) {
    if (formatBig != var.format()) {
      ////Logger::logPotenialBug("Not all imgs have the same format!", "Calibrator", "findStartPositionInImg_mulitpeImgs");
      return QVector<QPoint>();  //[TO DO zamienic ta funkcje zeby zwracala error code]
    }
  }

  const int WIDTH_OF_FIRST_IMG  = imgsToFind.first().width();
  const int HEIGHT_OF_FIRST_IMG = imgsToFind.first().height();
  foreach (const QImage& var, imgsToFind) {
    bool widthIsGood  = var.width() == WIDTH_OF_FIRST_IMG;
    bool heightIsGood = var.height() == HEIGHT_OF_FIRST_IMG;
    bool isAllRight   = widthIsGood && heightIsGood;
    if (!isAllRight) {
      //Logger::logPotenialBug("Not all imgs have the same size!", "Calibrator", "findStartPositionInImg_mulitpeImgs");
      return QVector<QPoint>();  //[TO DO zamienic ta funkcje zeby zwracala error code]
    }
  }

  const int MAX_WIDTH_VAL_TO_SHEARCH  = this->width() - WIDTH_OF_FIRST_IMG;
  const int MAX_HEIGHT_VAL_TO_SHEARCH = this->height() - HEIGHT_OF_FIRST_IMG;

  QVector<QPoint> startPointsListToRet;
  for (int x = 0; x <= MAX_WIDTH_VAL_TO_SHEARCH; x++) {
    for (int y = 0; y <= MAX_HEIGHT_VAL_TO_SHEARCH; y++) {
      bool atLeastOnePixIsMatched = false;
      uint pixFromImg_big         = this->pixel(x, y);

      foreach (const QImage& var, imgsToFind) {
        uint pixFromImg_small = var.pixel(0, 0);
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
            uint pixFromImg_big2 = this->pixel(x + x_TMP, y + y_TMP);
            bool metReq          = false;
            foreach (const QImage& var, imgsToFind) {
              uint pixFromImg_small2 = var.pixel(x_TMP, y_TMP);
              bool pixMatched        = pixFromImg_big2 == pixFromImg_small2;
              if (pixMatched) {
                metReq = true;
                break;
              }
            }
            if (metReq) {
              wrongPixels--;
            }
          }
        }
        if (wrongPixels == 0) {
          startPointsListToRet.push_back(QPoint(x, y));
        }
      }
    }
  }
  return startPointsListToRet;
}

void CJ_Image::saveWithCurrentTimeStamp() const {
  const QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch());
  const QString path      = PathResource::getPathToTmpSavingImgsDir();
  const QString abs_path  = QString("%1%2.png").arg(path, timestamp);
  this->save(abs_path);
}

void CJ_Image::saveFrames(const QImage& fullImg, const QVector<QRect>& frames, const QString& fileNameWithoutExtension) const {
  const QString tmp_path = PathResource::getPathToTmpSavingImgsDir();
  for (size_t i = 0; i < frames.size(); i++) {
    QImage  img      = fullImg.copy(frames[i]);
    QString fullPath = QString("%1%2_%3.png").arg(tmp_path, QString::number(i), fileNameWithoutExtension);
    img.save(fullPath);
  }
}

void CJ_Image::saveListOfImgs(const QList<QImage>& imgs, const QString& fileNameWithoutExtension) {
  const QString tmp_path = PathResource::getPathToTmpSavingImgsDir();
  for (size_t i = 0; i < imgs.size(); i++) {
    QString fullPath = QString("%1%2_%3.png").arg(tmp_path, QString::number(i), fileNameWithoutExtension);
    imgs[i].save(fullPath);
  }
}

QImage CJ_Image::getColorImageFromCode(const QString& codeOfImg) {
  QStringList partsOfCode = codeOfImg.split(QString("#"), Qt::SkipEmptyParts);
  if (partsOfCode.size() != 3) {
    qWarning() << "Wrong format of color img code!";
    return QImage();
  }

  const uint    WIDTH      = partsOfCode[0].toUInt();
  const uint    HEIGHT     = partsOfCode[1].toUInt();
  constexpr int HEX_LENGTH = 8;

  if (partsOfCode[2].size() % HEX_LENGTH != 0) {
    qWarning() << "Wrong format of color img code !, pixValues are not n * 8 length !";
    return QImage();
  }

  QStringList hexValuesOfPixels = {};
  for (size_t i = 0; i <= partsOfCode[2].size(); i += HEX_LENGTH) {
    hexValuesOfPixels.push_back(partsOfCode[3].mid(i, HEX_LENGTH));
  }

  QImage imgToCreate(WIDTH, HEIGHT, QImage::Format::Format_ARGB32);
  int    i = 0;
  for (size_t x = 0; x < WIDTH; x++) {
    for (size_t y = 0; y < HEIGHT; y++) {
      uint pixVal = hexValuesOfPixels[i].toUInt();
      imgToCreate.setPixel(x, y, pixVal);
      i++;
    }
  }
  return imgToCreate;
}

CJ_Image CJ_Image::fromCharToImg(QChar CharToImg) {
  QString     keyCode = standardLetters.key(CharToImg);
  QStringList parts;
  parts = keyCode.split("_");
  if (parts.size() != 3) {
    //Logger::logPotenialBug("Wrong structure of char code for char: " + QString(CharToImg), "Utilities", "fromCharToImg");
    return CJ_Image();
  }

  const int     WIDTH  = parts[0].toInt();
  const int     HEIGHT = parts[1].toInt();
  const QString code   = parts[2];
  QImage        imgToRet(WIDTH, HEIGHT, QImage::Format::Format_ARGB32);

  int i = 0;
  for (size_t x = 0; x < WIDTH; x++) {
    for (size_t y = 0; y < HEIGHT; y++) {
      const bool setWhite = code[i] == '1';
      const uint toSet    = setWhite ? WHITE : BLACK;
      imgToRet.setPixel(x, y, toSet);
      i++;
    }
  }
  return imgToRet;
}

bool CJ_Image::isItPixelFromFrame(const uint& color, const int& minValueAcceptable, const int& maxValueAcceptable,
                                  bool requireSameValuesOfRGB) {
  RGBstruct rgb(color);

  int maxValueFound = rgb.getMaxColVal();
  if (rgb.getMaxColVal() > maxValueAcceptable) {
    return false;
  }

  int minValueFound = rgb.getMinColVal();
  if (minValueFound < minValueAcceptable) {
    return false;
  }

  if (requireSameValuesOfRGB) {
    if (maxValueFound - minValueFound > 1) {
      return false;
    }
  }

  return true;
}
