#pragma once
#include <qimage.h>

#include "PathResource.h"
#include "RGBstruct.h"
class CJ_Image : public QImage {
 public:
  CJ_Image(const QImage& img) : QImage(img){};
  CJ_Image() = default;

  void toBlackAndWhite(const int threshold) {
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

  void toBlackAndWhiteOneColor(const int threshold) {
    const int WIDTH  = this->width();
    const int HEIGHT = this->height();
    for (size_t x = 0; x < WIDTH; x++) {
      for (size_t y = 0; y < HEIGHT; y++) {
        uint pixelColor = this->pixel(x, y);
        bool setWhite   = RGBstruct(pixelColor).oneColEqualOrAboveThreshold(threshold);
        uint toSet      = setWhite ? WHITE : BLACK;
        this->setPixel(x, y, toSet);
      }
    }
  }

  void toBlackAndWhiteExactColor(const QRgb COLOR) {
    for (int x = 0; x < this->height(); x++) {
      for (int y = 0; y < this->width(); y++) {
        bool setWhite = this->pixel(x, y) == COLOR;
        this->setPixel(x, y, setWhite ? WHITE : BLACK);
      }
    }
  }

  void toBlackAndWhiteAllColors(const int threshold) {
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

  void cutBlackBorders() {
    int       linesOfBlackRows_TOP = 0, linesOfBlackRows_DOWN = 0, linesOfBlackRows_RIGHT = 0, linesOfBlackRows_LEFT = 0;
    const int WIDTH  = this->width();
    const int HEIGHT = this->height();
    for (int x = 0; x < WIDTH; x++) {
      for (int y = 0; y < HEIGHT; y++) {
        bool isBlack = this->pixel(x, y) == BLACK;
        if (!isBlack) {
          linesOfBlackRows_LEFT = x;
          x                     = WIDTH;   //endOfLoop
          y                     = HEIGHT;  //endOfLoop
        }
      }
    }

    for (int x = WIDTH - 1; x >= linesOfBlackRows_LEFT; x--) {
      for (int y = 0; y < HEIGHT; y++) {
        bool isBlack = this->pixel(x, y) == BLACK;
        if (!isBlack) {
          linesOfBlackRows_RIGHT = WIDTH - x - 1;
          x                      = -1;      //endOfLoop
          y                      = HEIGHT;  //endOfLoop
        }
      }
    }

    for (int y = 0; y < HEIGHT; y++) {
      for (int x = linesOfBlackRows_LEFT; x < WIDTH - linesOfBlackRows_RIGHT; x++) {
        bool isBlack = this->pixel(x, y) == BLACK;
        if (!isBlack) {
          linesOfBlackRows_TOP = y;
          x                    = WIDTH;   //endOfLoop
          y                    = HEIGHT;  //endOfLoop
        }
      }
    }

    for (int y = HEIGHT - 1; y >= linesOfBlackRows_TOP; y--) {
      for (int x = linesOfBlackRows_LEFT; x < WIDTH - linesOfBlackRows_RIGHT; x++) {
        bool isBlack = this->pixel(x, y) == BLACK;
        if (!isBlack) {
          linesOfBlackRows_DOWN = HEIGHT - y - 1;
          x                     = WIDTH;  //endOfLoop
          y                     = -1;     //endOfLoop
        }
      }
    }
    int anotherParametr_x = WIDTH - linesOfBlackRows_RIGHT - linesOfBlackRows_LEFT;
    int anotherParametr_y = HEIGHT - linesOfBlackRows_TOP - linesOfBlackRows_DOWN;

    int widthToCut  = anotherParametr_x >= 0 ? anotherParametr_x : 0;
    int heightToCut = anotherParametr_y >= 0 ? anotherParametr_y : 0;

    *this = this->copy(linesOfBlackRows_LEFT, linesOfBlackRows_TOP, widthToCut, heightToCut);
  }

  QList<CJ_Image> toImageListWithSingleLetters() {
    QList<CJ_Image> letterImages;
    QList<int>      colThatAreNotBlack;
    const int       WIDTH  = this->width();
    const int       HEIGHT = this->height();
    for (int x = 0; x < WIDTH; x++) {
      for (int y = 0; y < HEIGHT; y++) {
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

  QString binaryLetterImgToCode() {
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

  QString toString(int threshold = 240) {
    this->toBlackAndWhite(threshold);
    this->cutBlackBorders();
    QList<CJ_Image> lettersImgs = this->toImageListWithSingleLetters();
    QString         toRet;
    auto            normalLettersMap = getCodesMap();
    foreach (CJ_Image img, lettersImgs) {
      img.cutBlackBorders();
      QString letterCode = img.binaryLetterImgToCode();
      QString letter     = normalLettersMap.value(letterCode);
      if (letter == '\0') {
        continue;
      }
      toRet.append(letter);
    }
    return toRet;
  }

  static QImage getColorImageFromCode(const QString& codeOfImg) {
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

  static CJ_Image fromCharToImg(QChar CharToImg) {
    QString     keyCode = getCodesMap().key(CharToImg);
    QStringList parts;
    parts = keyCode.split("_");
    if (parts.size() == 3) {
      const int WIDTH  = parts[0].toInt();
      const int HEIGHT = parts[1].toInt();
      QString   code   = parts[2];
      QImage    imgToRet(WIDTH, HEIGHT, QImage::Format::Format_ARGB32);
      int       i = 0;
      for (size_t x = 0; x < WIDTH; x++) {
        for (size_t y = 0; y < HEIGHT; y++) {
          bool setWhite = code[i] == '1';
          uint toSet    = setWhite ? WHITE : BLACK;
          imgToRet.setPixel(x, y, toSet);
          i++;
        }
      }
      return imgToRet;
    } else {
      //Logger::logPotenialBug("Wrong structure of char code for char: " + QString(CharToImg), "Utilities", "fromCharToImg");
      return CJ_Image();
    }
  }

  QVector<QPoint> findStartPositionInImg(const QImage& imgToFind, QRect frameInBigWindow = QRect()) const {
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
    for (int x = minIndexToCheckX; x <= maxIndexToCheckX; x++) {
      for (int y = minIndexToCheckY; y <= maxIndexToCheckY; y++) {
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

  QPoint findExactStartPositionInImg(const QImage& imgToFind, QRect frameInBigWindow = QRect()) const {
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
    for (int x = minIndexToCheckX; x <= maxIndexToCheckX; x++) {
      for (int y = minIndexToCheckY; y <= maxIndexToCheckY; y++) {
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
        if (foundPosition) startPoints.push_back(QPoint(x, y));
      }
    }
    if (startPoints.size() != 1) {
      qWarning() << "not exactly one position found in findExactStartPoint fun!";
      return QPoint();
    }
    return startPoints[0];
  }

  void rotateImgToRight(const int timesToRotateRight) {
    QTransform rotating;
    qreal      degreeToRotateToRight = (qreal)timesToRotateRight * 90;
    rotating.rotate(degreeToRotateToRight, Qt::Axis::ZAxis);
    *this = this->transformed(rotating);
  }

  static bool isItPixelFromFrame(const uint& color, const int& minValueAcceptable, const int& maxValueAcceptable,
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

  QVector<QPoint> findStartPositionInImg_mulitpeImgs(const QList<QImage>& imgsToFind) const {
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

  void saveWithCurrentTimeStamp() {
    const QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch());
    const QString path      = PathResource::getPathToTmpSavingImgsDir();
    const QString abs_path  = QString("%1%2.png").arg(path, timestamp);
    this->save(abs_path);
  }

  static void saveListOfImgs(const QList<QImage>& imgs, const QString& fileNameWithoutExtension) {
    const QString tmp_path = PathResource::getPathToTmpSavingImgsDir();
    for (int i = 0; i < imgs.size(); i++) {
      QString fullPath = QString("%1%2_%3.png").arg(tmp_path, QString::number(i), fileNameWithoutExtension);
      imgs[i].save(fullPath);
    }
  }

  void saveFrames(const QImage& fullImg, const QVector<QRect>& frames, const QString& fileNameWithoutExtension) {
    const QString tmp_path = PathResource::getPathToTmpSavingImgsDir();
    for (int i = 0; i < frames.size(); i++) {
      QImage  img      = fullImg.copy(frames[i]);
      QString fullPath = QString("%1%2_%3.png").arg(tmp_path, QString::number(i), fileNameWithoutExtension);
      img.save(fullPath);
    }
  }

 private:
  static const uint BLACK = qRgb(0, 0, 0);
  static const uint WHITE = qRgb(255, 255, 255);
  //QMap<QString, QString>        normalLettersMap = getCodesMap();
  static QMap<QString, QString> getCodesMap() {
    QMap<QString, QString> toRet = {};
    toRet.insert("6_8_011111101111111110000001100000011111111101111110", "0");
    toRet.insert("4_8_01000001111111111111111100000001", "1");
    toRet.insert("6_8_010000111100011110001101100110011111000101100001", "2");
    toRet.insert("6_8_010000101100001110010001100100011111111101101110", "3");
    toRet.insert("6_8_000011000001010000100100011111111111111100000100", "4");
    toRet.insert("6_8_000000101111001111110001100100011001111110001110", "5");
    toRet.insert("6_8_001111100111111111010001100100011001111100001110", "6");
    toRet.insert("6_8_100000001000001110001111101111001111000011000000", "7");
    toRet.insert("6_8_011011101111111110010001100100011111111101101110", "8");
    toRet.insert("6_8_011100001111100110001001100010111111111001111100", "9");
    toRet.insert("6_6_000110101111101001101001111111011111", "a");
    toRet.insert("6_9_111111111111111111000100001000100001000111111000011110", "b");
    toRet.insert("5_6_011110111111100001100001100001", "c");
    toRet.insert("6_9_000011110000111111000100001000100001111111111111111111", "d");
    toRet.insert("6_6_011110111111101001101001111001011010", "e");
    toRet.insert("5_9_000100000011111111111111111100100000100100000", "f");
    toRet.insert("6_8_011110001111110110000101100001011111111111111110", "g");
    toRet.insert("6_9_111111111111111111000100000000100000000111111000011111", "h");
    toRet.insert("2_8_1011111110111111", "i");
    toRet.insert("4_10_0000000001001000000110111111111011111110", "j");
    toRet.insert("6_9_111111111111111111000001100000011110000110011000100001", "k");
    toRet.insert("2_9_111111111111111111", "l");
    toRet.insert("10_6_111111111111100000100000111111011111100000100000111111011111", "m");
    toRet.insert("6_6_111111111111100000100000111111011111", "n");
    toRet.insert("6_6_011110111111100001100001111111011110", "o");
    toRet.insert("6_8_111111111111111110000100100001001111110001111000", "p");
    toRet.insert("5_6_111111111111010000110000110000", "r");
    toRet.insert("5_6_011001111101101101101111100110", "s");
    toRet.insert("5_8_0010000011111110111111110010000100100001", "t");
    toRet.insert("6_6_111110111111000001000001111111111111", "u");
    toRet.insert("6_6_111000111110000111000111111110111000", "v");
    toRet.insert("8_6_111100111111000011111100111100000011111111111100", "w");
    toRet.insert("6_6_110011111111001100001100111111110011", "x");
    toRet.insert("6_8_110000001111001100111111001111001111000011000000", "y");
    toRet.insert("5_6_100011100111101101111001110001", "z");
    toRet.insert("7_8_00000111001111111111110011000100111111000011111100000111", "A");
    toRet.insert("6_8_111111111111111110010001100100011111111101101110", "B");
    toRet.insert("6_8_011111101111111110000001100000011000000101000010", "C");
    toRet.insert("7_8_11111111111111111000000110000001110000110111111000111100", "D");
    toRet.insert("6_8_111111111111111110010001100100011001000110010001", "E");
    toRet.insert("6_8_111111111111111110010000100100001001000010010000", "F");
    toRet.insert("7_8_01111110111111111000000110000001100010011000111101001111", "G");
    toRet.insert("7_8_11111111111111110001000000010000000100001111111111111111", "H");
    toRet.insert("4_8_10000001111111111111111110000001", "I");
    toRet.insert("5_8_0000000110000001100000011111111111111110", "J");
    toRet.insert("6_8_111111111111111100111100011001101100001110000001", "K");
    toRet.insert("6_8_111111111111111100000001000000010000000100000001", "L");
    toRet.insert("8_8_1111111111100000011100000011100000010000001000000111111111111111", "M");
    toRet.insert("7_8_11111111011000000011000000011000000011000000011011111111", "N");
    toRet.insert("7_8_01111110111111111000000110000001100000011111111101111110", "O");
    toRet.insert("6_8_111111111111111110001000100010001111100001110000", "P");
    toRet.insert("7_8_11111111111111111000100010001100111111100111001100000001", "R");
    toRet.insert("6_8_011100101111100110011001100110011001111101001110", "S");
    toRet.insert("8_8_1000000010000000100000001111111111111111100000001000000010000000", "T");
    toRet.insert("7_8_11111110111111110000000100000001000000011111111111111110", "U");
    toRet.insert("6_8_111000001111110000011111000111111111110011100000", "V");
    toRet.insert("0_8_11100000111111000001111100001111111110001111100000001111000111111111110011100000", "W");
    toRet.insert("6_8_110000111110011100111100001111001110011111000011", "X");
    toRet.insert("6_8_110000001111000000111111001111111111000011000000", "Y");
    toRet.insert("6_8_100001111000111110011101101110011111000111100001", "Z");
    //slash, in program it"s slash between current health and max health
    toRet.insert("11_6_110000110000011000011000001100001100001100000110000110000011000011", QChar(47));
    //backslash char but in program it"s slash above but rotated
    toRet.insert("6_11_000000000110000000111100001111100001111100001111000000011000000000", QChar(92));
    //opening bracket
    toRet.insert("4_11_00011111000011111111101110000011110000000001", QChar(40));
    //closing bracket
    toRet.insert("4_11_10000000001111000001110111111111000011111000", QChar(41));
    //char 200 it"s some weird char but here it"s symbol of healthIcon
    toRet.insert("9_9_101110000011111000111111100111111110011111111111111110111111100111111000011110000", QChar(200));
    //char 201 it"s some weird char but here it"s symbol of manaIcon
    toRet.insert("10_9_000100001001100111001111111111111110111111100111111100111110000111110000100100000100000000", QChar(201));
    //there are no empty line between letter r and next letter in those cases;
    toRet.insert("10_9_000111111000111111000010000000110000000110000000100000011111111111111111100100000100100000", "rf");
    toRet.insert("9_10_001111110000111111000001000000001100000000110000000000000001001000000110111111111011111110", "rj");
    toRet.insert("10_8_00111111001111110001000000110000001100000010000011111110111111110010000100100001", "rt");
    toRet.insert("10_9_000100000011111111111111111100100000100100000000100000011111111111111111100100000100100000", "ff");
    toRet.insert("10_9_000100000011111111111111111100100000100100000000100000011111110011111111000100001000100001", "ft");
    toRet.insert("7_10_0111111000111111110010000001001000000110100000011111111111010111111001", "Q");
    toRet.insert("2_3_111111", "'");
    return toRet;
  }
};
