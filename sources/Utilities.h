#pragma once
#include <qstring.h>
#include <qmessagebox.h>
#include <qprocess.h>
#include <Windows.h>
#include <psapi.h>
#include <qrgb.h>
#include <qdatetime.h>
#include <iostream>
#include <qlist.h>
#include <qdir.h>
#include <qdebug.h>
#include <tlhelp32.h>
#include <QRgb>
#include <QtCore/qsettings.h>

//#include "Calibrator.h"
#include "RGBstruct.h"
//#include "VariablesClass.h"
#include "Key.h"


class Utilities {
public:
	static QString StrCodeToLetter(QString code);
	static QMap<QString, QString> getQmapWithCodes();
	static void showMessageBox_INFO(QString text);
	static bool showMessageBox_NO_YES(QString text);
	static int showMessageBox(QString title, QString text, QFlags<QMessageBox::StandardButton> buttons);
	/*
	enum class FOLDERS_OF_TMP_FOLDER {Logs, Profiles, TradeReports, Main, Routes, MarketLists};
	enum class FieldsOfIniFile {
		LANGUAGE,
		LAST_USED_LOGIN,
		LAST_USED_PROFILE_NAME,
	};


	static qint64 getCurrentTimeInMiliSeconds();
	static QDir getDirWithCrackerJackTmpFolder(FOLDERS_OF_TMP_FOLDER folderType);
	static QMap<FieldsOfIniFile, QString> get_Field_NamesFromIni_map();
	static QString readFromIniFile(FieldsOfIniFile nameOfField);
	static void writeIniFile(FieldsOfIniFile nameOfField, QString value);
	static QStringList TOOL_getCodesOfAllInFolder_regular(QString pathToInputFolder, QString pathToOutputFolder);
	static QStringList TOOL_getCodesOfAllInFolder_bottom(QString pathToInputFolder);
	static void TOOL_saveImgToOutPutFolder(QImage& img, QString extraName);
	static QString getPathToSettingsFile();

	static void TOOL_manaSit(int pid, QString winTitle, std::shared_ptr<GameConnecter> gameConnecter){
		int i = 0;
		while (true) {
			Key key;
			if (i == 0)
				key = Key("F7");
			else if (i == 1)
				key = Key("END");
			else
				key = Key("F11");
			gameConnecter->sendKeyStrokeToProcess(key);
			Sleep(125 * 100);
			i++;
			i = i % 3;
		} 
	}
	static void UNSUED_findBoredersOfFrames(QImage fullScreen);
	static void UNUSED_imgToOneColor(QImage& img, QRgb minimalColorValues, QRgb maxColorValues, QRgb colorToSet, bool allOfThem);
	static void UNUSED_imgAvoideOneColor(QImage& img, QRgb minimalColorValues, QRgb maxColorValues, bool allOfThem);
	static void UNUSED_changeGreyPixelsToBlack(QImage& img, int minGreyVal, int maxGreyVal);
	static int UNUSED_modifyBit(int numberToEdit, int postition, int zeroOrOne);
	*/
};
