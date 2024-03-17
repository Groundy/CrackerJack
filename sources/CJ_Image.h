#pragma once
#include <qimage.h>

#include "PathResource.h"
#include "RGBstruct.h"
class CJ_Image : public QImage {
 public:
  CJ_Image(const QImage& img) : QImage(img){};
  CJ_Image() = default;

  void toBlackAndWhite(const int threshold);

  void toBlackAndWhiteOneColor(const int threshold);

  void toBlackAndWhiteExactColor(const QRgb COLOR);

  void toBlackAndWhiteAllColors(const int threshold);

  void cutBlackBorders();

  QList<CJ_Image> toImageListWithSingleLetters() const;

  QString binaryLetterImgToCode() const;

  QString toString(int threshold = 240);

  QVector<QPoint> findStartPositionInImg(const QImage& imgToFind, QRect frameInBigWindow = QRect()) const;

  QPoint findExactStartPositionInImg(const QImage& imgToFind, QRect frameInBigWindow = QRect()) const;

  void rotateImgToRight(const int timesToRotateRight);

  QVector<QPoint> findStartPositionInImg_mulitpeImgs(const QList<QImage>& imgsToFind) const;

  void saveWithCurrentTimeStamp() const;

  void saveFrames(const QImage& fullImg, const QVector<QRect>& frames, const QString& fileNameWithoutExtension) const;

  static void saveListOfImgs(const QList<QImage>& imgs, const QString& fileNameWithoutExtension);

  static QImage getColorImageFromCode(const QString& codeOfImg);

  static CJ_Image fromCharToImg(QChar CharToImg);

  static bool isItPixelFromFrame(const uint& color, const int& minValueAcceptable, const int& maxValueAcceptable,
                                 bool requireSameValuesOfRGB);

 private:
  static const uint                          BLACK = qRgb(0, 0, 0);
  static const uint                          WHITE = qRgb(255, 255, 255);
  inline static const QMap<QString, QString> standardLetters{
      {"6_8_011111101111111110000001100000011111111101111110", "0"},
      {"4_8_01000001111111111111111100000001", "1"},
      {"6_8_010000111100011110001101100110011111000101100001", "2"},
      {"6_8_010000101100001110010001100100011111111101101110", "3"},
      {"6_8_000011000001010000100100011111111111111100000100", "4"},
      {"6_8_000000101111001111110001100100011001111110001110", "5"},
      {"6_8_001111100111111111010001100100011001111100001110", "6"},
      {"6_8_100000001000001110001111101111001111000011000000", "7"},
      {"6_8_011011101111111110010001100100011111111101101110", "8"},
      {"6_8_011100001111100110001001100010111111111001111100", "9"},
      {"6_6_000110101111101001101001111111011111", "a"},
      {"6_9_111111111111111111000100001000100001000111111000011110", "b"},
      {"5_6_011110111111100001100001100001", "c"},
      {"6_9_000011110000111111000100001000100001111111111111111111", "d"},
      {"6_6_011110111111101001101001111001011010", "e"},
      {"5_9_000100000011111111111111111100100000100100000", "f"},
      {"6_8_011110001111110110000101100001011111111111111110", "g"},
      {"6_9_111111111111111111000100000000100000000111111000011111", "h"},
      {"2_8_1011111110111111", "i"},
      {"4_10_0000000001001000000110111111111011111110", "j"},
      {"6_9_111111111111111111000001100000011110000110011000100001", "k"},
      {"2_9_111111111111111111", "l"},
      {"10_6_111111111111100000100000111111011111100000100000111111011111", "m"},
      {"6_6_111111111111100000100000111111011111", "n"},
      {"6_6_011110111111100001100001111111011110", "o"},
      {"6_8_111111111111111110000100100001001111110001111000", "p"},
      {"5_6_111111111111010000110000110000", "r"},
      {"5_6_011001111101101101101111100110", "s"},
      {"5_8_0010000011111110111111110010000100100001", "t"},
      {"6_6_111110111111000001000001111111111111", "u"},
      {"6_6_111000111110000111000111111110111000", "v"},
      {"8_6_111100111111000011111100111100000011111111111100", "w"},
      {"6_6_110011111111001100001100111111110011", "x"},
      {"6_8_110000001111001100111111001111001111000011000000", "y"},
      {"5_6_100011100111101101111001110001", "z"},
      {"7_8_00000111001111111111110011000100111111000011111100000111", "A"},
      {"6_8_111111111111111110010001100100011111111101101110", "B"},
      {"6_8_011111101111111110000001100000011000000101000010", "C"},
      {"7_8_11111111111111111000000110000001110000110111111000111100", "D"},
      {"6_8_111111111111111110010001100100011001000110010001", "E"},
      {"6_8_111111111111111110010000100100001001000010010000", "F"},
      {"7_8_01111110111111111000000110000001100010011000111101001111", "G"},
      {"7_8_11111111111111110001000000010000000100001111111111111111", "H"},
      {"4_8_10000001111111111111111110000001", "I"},
      {"5_8_0000000110000001100000011111111111111110", "J"},
      {"6_8_111111111111111100111100011001101100001110000001", "K"},
      {"6_8_111111111111111100000001000000010000000100000001", "L"},
      {"8_8_1111111111100000011100000011100000010000001000000111111111111111", "M"},
      {"7_8_11111111011000000011000000011000000011000000011011111111", "N"},
      {"7_8_01111110111111111000000110000001100000011111111101111110", "O"},
      {"6_8_111111111111111110001000100010001111100001110000", "P"},
      {"7_8_11111111111111111000100010001100111111100111001100000001", "R"},
      {"6_8_011100101111100110011001100110011001111101001110", "S"},
      {"8_8_1000000010000000100000001111111111111111100000001000000010000000", "T"},
      {"7_8_11111110111111110000000100000001000000011111111111111110", "U"},
      {"6_8_111000001111110000011111000111111111110011100000", "V"},
      {"0_8_11100000111111000001111100001111111110001111100000001111000111111111110011100000", "W"},
      {"6_8_110000111110011100111100001111001110011111000011", "X"},
      {"6_8_110000001111000000111111001111111111000011000000", "Y"},
      {"6_8_100001111000111110011101101110011111000111100001", "Z"},
      //slash, in program it"s slash between current health and max health
      {"11_6_110000110000011000011000001100001100001100000110000110000011000011", QChar(47)},
      //backslash char but in program it"s slash above but rotated
      {"6_11_000000000110000000111100001111100001111100001111000000011000000000", QChar(92)},
      //opening bracket
      {"4_11_00011111000011111111101110000011110000000001", QChar(40)},
      //closing bracket
      {"4_11_10000000001111000001110111111111000011111000", QChar(41)},
      //char 200 it"s some weird char but here it"s symbol of healthIcon
      {"9_9_101110000011111000111111100111111110011111111111111110111111100111111000011110000", QChar(200)},
      //char 201 it"s some weird char but here it"s symbol of manaIcon
      {"10_9_000100001001100111001111111111111110111111100111111100111110000111110000100100000100000000", QChar(201)},
      //there are no empty line between letter r and next letter in those cases;
      {"10_9_000111111000111111000010000000110000000110000000100000011111111111111111100100000100100000", "rf"},
      {"9_10_001111110000111111000001000000001100000000110000000000000001001000000110111111111011111110", "rj"},
      {"10_8_00111111001111110001000000110000001100000010000011111110111111110010000100100001", "rt"},
      {"10_9_000100000011111111111111111100100000100100000000100000011111111111111111100100000100100000", "ff"},
      {"10_9_000100000011111111111111111100100000100100000000100000011111110011111111000100001000100001", "ft"},
      {"7_10_0111111000111111110010000001001000000110100000011111111111010111111001", "Q"},
      {"2_3_111111", "'"}};
};