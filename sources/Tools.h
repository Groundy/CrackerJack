#pragma once
#include <RGBstruct.h>
#include <qdir.h>
#include <qimage.h>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include "JsonParser.h"
class Tools
{
public:
	static void saveJsonFileWithCodesOfImgsInFolder(QString inputFolderPath, QString outputFolderPath) {	
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
};
