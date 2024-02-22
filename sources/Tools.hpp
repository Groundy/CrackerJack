#pragma once
#include <RGBstruct.h>
#include <qdir.h>
#include <qimage.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qstring.h>

#include "JsonParser.h"
class Tools {
 public:
  static void saveJsonFileWithCodesOfImgsInFolderColor(QString inputFolderPath, QString outputFolderPath) {
    // patern width_height_digits
    QDir        directory(inputFolderPath);
    QStringList litOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
    QJsonArray  arr;
    for each (QString fileName in litOfFIles) {
      QString   pathToFile = QString("%1\\%2").arg(inputFolderPath, fileName);
      QImage    img(pathToFile);
      const int WIDTH       = img.width();
      const int HEIGHT      = img.height();
      QString   strWithCode = QString("%1#%2#").arg(QString::number(WIDTH), QString::number(HEIGHT));
      for (size_t x = 0; x < WIDTH; x++) {
        for (size_t y = 0; y < HEIGHT; y++) {
          uint    pixVal      = (uint)img.pixel(x, y);
          QString strToAppend = RGBstruct(pixVal).toString();
          strWithCode.append(strToAppend);
        }
      }
      const QString fileNameWithoutExtension = fileName.left(fileName.size() - 4);
      QJsonObject   obj;
      obj.insert("name", fileNameWithoutExtension);
      obj.insert("value", strWithCode);
      arr.push_back(obj);
      QString toDisplay = fileName + "----->" + strWithCode;
      qDebug() << toDisplay;
    }
    QJsonObject toSave;
    toSave.insert("codes", arr);
    JsonParser::saveJsonFile(outputFolderPath, "imgsCodes.json", toSave);
  }
  static void saveJsonFileWithCodesOfImgsInFolderBinary(QString inputFolderPath, QString outputFolderPath) {
    // patern width_height_digits
    QDir        directory(inputFolderPath);
    QStringList litOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
    QJsonArray  arr;
    for each (QString fileName in litOfFIles) {
      QString   pathToFile = QString("%1\\%2").arg(inputFolderPath, fileName);
      QImage    img(pathToFile);
      const int THRESHOLD = 200;
      ImgEditor::imgToBlackAndWhiteOneColor(img, THRESHOLD);
      QString       imgPixelsValues          = ImgEditor::binaryLetterImgToLetterStr(img);
      const QString fileNameWithoutExtension = fileName.left(fileName.size() - 4);
      QJsonObject   obj;
      obj.insert("name", fileNameWithoutExtension);
      obj.insert("value", imgPixelsValues);
      arr.push_back(obj);
      QString toDisplay = fileName + "----->" + imgPixelsValues;
      qDebug() << toDisplay;
    }
    QJsonObject toSave;
    toSave.insert("codes", arr);
    JsonParser::saveJsonFile(outputFolderPath, "imgsCodes.json", toSave);
  }
  static void mergedMaps(QString inputPath, QString outputPath) {
    const int   MIN_X_MAP_INDEX = 124, MAX_X_MAP_INDEX = 134, MIN_Y_MAP_INDEX = 121, MAX_Y_MAP_INDEX = 129;
    const int   WHOLE_IMG_WIDTH  = 256 * (MAX_X_MAP_INDEX - MIN_X_MAP_INDEX);
    const int   WHOLE_IMG_HEIGHT = 256 * (MAX_Y_MAP_INDEX - MIN_Y_MAP_INDEX);
    QStringList types            = QStringList() << "Color"
                                      << "WaypointCost";
    for each (QString type in types) {
      for (size_t floorImgName = 0; floorImgName <= 15; floorImgName++) {
        QImage* wholeImg = new QImage(WHOLE_IMG_WIDTH, WHOLE_IMG_HEIGHT, QImage::Format::Format_ARGB32);
        for (size_t xIndex = MIN_X_MAP_INDEX; xIndex <= MAX_X_MAP_INDEX; xIndex++) {
          for (size_t yIndex = MIN_Y_MAP_INDEX; yIndex <= MAX_Y_MAP_INDEX; yIndex++) {
            QString xValStr = QString::number(xIndex * 256);
            QString yValStr = QString::number(yIndex * 256);
            QString mapPieceFileName =
                QString("%1\\Minimap_%2_%3_%4_%5.png").arg(inputPath, type, xValStr, yValStr, QString::number(floorImgName));
            QImage mapPiece = QImage(mapPieceFileName);
            if (mapPiece.isNull()) continue;

            const int xOffset = (xIndex - MIN_X_MAP_INDEX) * 256;
            const int yOffset = (yIndex - MIN_Y_MAP_INDEX) * 256;
            for (size_t x = 0; x < 256; x++) {
              for (size_t y = 0; y < 256; y++) {
                const int xPositionInBigImg = x + xOffset;
                const int yPositionInBigImg = y + yOffset;
                QPoint    pt                = QPoint(xPositionInBigImg, yPositionInBigImg);
                uint      rgb               = mapPiece.pixel(x, y);
                wholeImg->setPixel(pt, rgb);
              }
            }
          }
        }
        int     realFloor      = -floorImgName + 7;
        QString outPutFileName = QString("%1\\%2_%3.png").arg(outputPath, QString::number(realFloor), type);
        wholeImg->save(outPutFileName);
        delete wholeImg;
      }
    }
  }
  static void getAllColorsUsedInWalkableMaps(QString inputPath, QString outputPath) {
    QDir               directory(inputPath);
    QStringList        litOfFIles = directory.entryList(QStringList() << "*_Color*.png", QDir::Files);
    QVector<RGBstruct> colorSet;
    for each (QString fileName in litOfFIles) {
      QString pathToFile = inputPath + "\\" + fileName;
      QImage  img        = QImage(pathToFile);
      for (size_t x = 0; x < img.width(); x++) {
        for (size_t y = 0; y < img.height(); y++) {
          RGBstruct rgb(img.pixel(x, y));
          if (!colorSet.contains(rgb)) colorSet.push_back(rgb);
        }
      }
    }
    QJsonArray arr;
    for each (RGBstruct var in colorSet) {
      arr.push_back(var.toJson());
    }
    QJsonObject objToSave;
    objToSave.insert("allPossibleColors", arr);
    JsonParser::saveJsonFile(outputPath, "allPossibleMapColors", objToSave);
  }
  static void convertMapsToStrings(QString inputPath, QString outputPath) {
    QMap<RGBstruct, QChar> colorCharMap;
    colorCharMap.insert(RGBstruct(153, 153, 153), '0');
    colorCharMap.insert(RGBstruct(0, 0, 0), '1');
    colorCharMap.insert(RGBstruct(0, 51, 255), '2');
    colorCharMap.insert(RGBstruct(102, 102, 102), '3');
    colorCharMap.insert(RGBstruct(0, 240, 0), '4');
    colorCharMap.insert(RGBstruct(0, 255, 255), '5');
    colorCharMap.insert(RGBstruct(153, 153, 153), '6');
    colorCharMap.insert(RGBstruct(255, 255, 255), '7');
    colorCharMap.insert(RGBstruct(255, 255, 204), '8');
    colorCharMap.insert(RGBstruct(0, 102, 0), '9');
    colorCharMap.insert(RGBstruct(102, 255, 153), 'A');
    colorCharMap.insert(RGBstruct(153, 102, 51), 'B');
    colorCharMap.insert(RGBstruct(51, 102, 153), 'C');
    colorCharMap.insert(RGBstruct(153, 204, 255), 'D');
    colorCharMap.insert(RGBstruct(0, 102, 255), 'E');
    colorCharMap.insert(RGBstruct(0, 51, 153), 'F');

    QDir        directory(inputPath);
    QStringList litOfFIles = directory.entryList(QStringList() << "*_Color*.png", QDir::Files);
    for each (QString fileName in litOfFIles) {
      QString    pathToFile = inputPath + "\\" + fileName;
      QString    floorName  = fileName.split("_").first();
      QImage     img        = QImage(pathToFile);
      QJsonArray mapAsChars;
      for (size_t y = 0; y < img.height(); y++) {
        QString lineOfMapStr = QString();
        for (size_t x = 0; x < img.width(); x++) {
          RGBstruct pixVal  = RGBstruct(img.pixel(x, y));
          QChar     tmpChar = colorCharMap.value(pixVal, 'x');
          lineOfMapStr.append(tmpChar);
        }
        mapAsChars.append(lineOfMapStr);
      }
      QJsonObject toSave;
      toSave.insert("chars", mapAsChars);
      JsonParser::saveJsonFile(outputPath, floorName, toSave);
    }
  }
};
