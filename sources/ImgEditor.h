#pragma once
#include <qimage.h>

#include "Logger.hpp"
#include "RGBstruct.h"
class ImgEditor {
 public:
  ImgEditor();
  ~ImgEditor();

  QString imgWithStrToStr(QImage& img, int threshold = 240);
  QImage  fromCharToImg(QChar CharToImg);

  static void imgToBlackAndWhiteAllColors(QImage& img, int threshold);
  static void imgToBlackAndWhiteOneColor(QImage& img, int threshold);
  static void imgToBlackAndWhiteExactColor(QImage& img, const QRgb COLOR) {
    for (int x = 0; x < img.height(); x++) {
      for (int y = 0; y < img.width(); y++) {
        bool setWhite = img.pixel(x, y) == COLOR;
        img.setPixel(x, y, setWhite ? WHITE : BLACK);
      }
    }
  }
  static void          cutBlackBordersOfImg(QImage& img);
  static QString       binaryLetterImgToCode(QImage& singleLetterImg);
  static void          rotateImgToRight(QImage& imgToRotate, int timesToRotateRight);
  static bool          isItPixelFromFrame(const uint& color, const int& minValueAcceptable, const int& maxValueAcceptable,
                                          bool requireSameValuesOfRGB);
  static QImage        getColorImageFromCode(const QString& codeOfImg);
  static QList<QImage> cutImgWithLettersToSingleLettersImgList(QImage& img);
  static QVector<QPoint> findStartPositionInImg_mulitpeImgs(QList<QImage*> imgsToFind, QImage& imgToShareWithin);
  static QVector<QPoint> findStartPositionInImg(const QImage& imgToFind, const QImage& imgToShareWithin, QRect frameInBigWindow = QRect());
  static QPoint findExactStartPositionInImg(const QImage& imgToFind, const QImage& imgToShareWithin, QRect frameInBigWindow = QRect());
  static void   saveMultipleImgs(const QImage& fullImg, QVector<QRect> frames, QString pathToFolder, QString fileNameWithoutExtension);
  static void   saveMultipleImgs(const QList<QImage>& fullImg, QString pathToFolder, QString fileNameWithoutExtension);
  static void   saveImgWitinLoop(const QImage& img) {
    img.save(QString("C:\\Users\\szczy\\Desktop\\tmps\\%1.png").arg(QString::number(QDateTime::currentMSecsSinceEpoch())));
  }

 private:
  static const uint      BLACK            = qRgb(0, 0, 0);
  static const uint      WHITE            = qRgb(255, 255, 255);
  QMap<QString, QString> normalLettersMap = getCodesMap();
  QMap<QString, QString> getCodesMap() {
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
