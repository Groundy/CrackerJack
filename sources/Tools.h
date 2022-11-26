#pragma once
#include <RGBstruct.h>
#include <qdir.h>
#include <qimage.h>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include "JsonParser.h"
#include <qstring.h>
class Tools
{
public:
	static void saveJsonFileWithCodesOfImgsInFolderColor(QString inputFolderPath, QString outputFolderPath) {	
		// patern width_height_digits
		QDir directory(inputFolderPath);
		QStringList litOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
		QJsonArray arr;
		for each (QString fileName in litOfFIles){
			QString pathToFile = QString("%1\\%2").arg(inputFolderPath, fileName);
			QImage img(pathToFile);
			const int WIDTH = img.width();
			const int HEIGHT = img.height();
			QString strWithCode = QString("%1#%2#").arg(QString::number(WIDTH), QString::number(HEIGHT));
			for (size_t x = 0; x < WIDTH; x++) {
				for (size_t y = 0; y < HEIGHT; y++) {
					uint pixVal = (uint)img.pixel(x, y);
					QString strToAppend = RGBstruct(pixVal).toString();
					strWithCode.append(strToAppend);
				}
			}
			const QString fileNameWithoutExtension = fileName.left(fileName.size() - 4);
			QJsonObject obj;
			obj.insert("name", fileNameWithoutExtension);
			obj.insert("value", strWithCode);
			arr.push_back(obj);
			QString toDisplay = fileName + "----->" + strWithCode;
			qDebug() << toDisplay;
		}
		QJsonDocument docToSave(arr);
		JsonParser().saveJsonFile(outputFolderPath, "imgsCodes.json", docToSave);
	}
	static void saveJsonFileWithCodesOfImgsInFolderBinary(QString inputFolderPath, QString outputFolderPath) {
		// patern width_height_digits
		QDir directory(inputFolderPath);
		QStringList litOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
		QJsonArray arr;
		for each (QString fileName in litOfFIles) {
			QString pathToFile = QString("%1\\%2").arg(inputFolderPath, fileName);
			QImage img(pathToFile);
			const int THRESHOLD = 200;
			ImgEditor::imgToBlackAndWhiteOneColor(img, THRESHOLD);
			QString imgPixelsValues = ImgEditor::binaryLetterImgToLetterStr(img);
			const QString fileNameWithoutExtension = fileName.left(fileName.size() - 4);
			QJsonObject obj;
			obj.insert("name", fileNameWithoutExtension);
			obj.insert("value", imgPixelsValues);
			arr.push_back(obj);
			QString toDisplay = fileName + "----->" + imgPixelsValues;
			qDebug() << toDisplay;
		}
		QJsonDocument docToSave(arr);
		JsonParser().saveJsonFile(outputFolderPath, "imgsCodes.json", docToSave);
	}
	static void mergedMaps(QString inputPath, QString outputPath) {
		QDir directory(inputPath);
		QStringList litOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
		const int MIN_X_MAP_INDEX = 124, MAX_X_MAP_INDEX = 134, MIN_Y_MAP_INDEX = 121, MAX_Y_MAP_INDEX = 129;

		const int WHOLE_IMG_WIDTH = 256 * (MAX_X_MAP_INDEX - MIN_X_MAP_INDEX); 
		const int WHOLE_IMG_HEIGHT = 256 * (MAX_Y_MAP_INDEX - MIN_Y_MAP_INDEX);
		QStringList types = QStringList() << "Color" << "WaypointCost";
		for each (QString type in types){
			for (size_t floor = 0; floor <= 15; floor++){
				QImage* wholeImg = new QImage(WHOLE_IMG_WIDTH, WHOLE_IMG_HEIGHT, QImage::Format::Format_ARGB32);
				for (size_t xIndex = MIN_X_MAP_INDEX; xIndex <= MAX_X_MAP_INDEX; xIndex++){
					for (size_t yIndex = MIN_Y_MAP_INDEX; yIndex <= MAX_Y_MAP_INDEX; yIndex++) {
						QString xValStr = QString::number(xIndex * 256);
						QString yValStr = QString::number(yIndex * 256);
						QString mapPieceFileName = QString("%1\\Minimap_%2_%3_%4_%5.png").arg(inputPath, type, xValStr, yValStr, QString::number(floor));
						QImage mapPiece = QImage(mapPieceFileName);
						if(mapPiece.isNull())
							continue;

						for (size_t x = 0; x < 256; x++){
							for (size_t y = 0; y < 256; y++) {
								const int xPositionInBigImg = x + ((xIndex - MIN_X_MAP_INDEX) * 256);
								const int yPositionInBigImg = y + ((yIndex - MIN_Y_MAP_INDEX) * 256);
								QPoint pt = QPoint(xPositionInBigImg, yPositionInBigImg);
								uint rgb = mapPiece.pixel(x, y);
								wholeImg->setPixel(pt, rgb);
							}
						}
					}
				}
				QString outPutFileName = QString("%1\\%2_%3.png").arg(outputPath, QString::number(floor), type);
				wholeImg->save(outPutFileName);
				delete wholeImg;
			}
		}
		int a = 4;
	}
};
