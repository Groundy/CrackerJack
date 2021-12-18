#pragma once
#include "qstring.h"
#include "qmessagebox.h"
#include <Key.h>
#include "qprocess.h"
#include "Windows.h"
#include <psapi.h>
#include "qlist.h"
#include "tlhelp32.h"
#include <comdef.h> 
#include <iostream>
#include "RGBstruct.h"
#include "qdir.h"
#include "qdebug.h"
#include "VariablesClass.h"
#include "qdatetime.h"
#include "qrgb.h"
#include "Logger.h"
class Utilities {
public:
	enum class FOLDERS_OF_TMP_FOLDER {Logs, Profiles, TradeReports, Main, Routes, MarketLists};
	enum class FieldsOfIniFile {
		LANGUAGE,
		LAST_USED_LOGIN,
		LAST_USED_PROFILE_NAME,
	};

	static int showMessageBox(QString title, QString text, QFlags<QMessageBox::StandardButton> buttons);
	static void showMessageBox_INFO(QString text);
	static bool showMessageBox_NO_YES(QString text);
	static bool sendKeyStrokeToProcess(Key key, unsigned int PID, QString WindowName);
	static void imgToBlackAndWhiteAllColors(QImage& img, int threshold);
	static QString imgWithStrToStr(QImage& img);
	static QImage fromCharToImg(QChar CharToImg);
	static void rotateImgToRight(QImage& imgToRotate, int timesToRotateRight);
	static bool isItPixelFromFrame(uint color, int minValueAcceptable, int maxValueAcceptable, bool requireSameValuesOfRGB);
	static LONG64 getCurrentTimeInMiliSeconds();
	static QImage getImageFromAdvancedCode(QString codeOfImg);
	static int getNumberFromBottomBar(QImage& bottomBar);
	static QDir getDirWithCrackerJackTmpFolder(FOLDERS_OF_TMP_FOLDER folderType);
	static void clickLeft(QPoint pt, HWND handler);
	static void clickRight(QPoint pt, HWND handler);
	static void sendStringToGame(QString str, HWND handler);
	static HWND getHandlerToGameWindow(unsigned int PID, QString WindowName);
	static QMap<FieldsOfIniFile, QString> get_Field_NamesFromIni_map();
	static QString readFromIniFile(FieldsOfIniFile nameOfField);
	static void writeIniFile(FieldsOfIniFile nameOfField, QString value);

	class RestoreMethode {
	public:
		enum class TypeOfMethode{ POTION, SPELL };
		QString name;
		int mana, cd, cdGroup;
		TypeOfMethode type;
	};
	class Spell {
	public:
		enum class TYPE_OF_SPELL { HEALING, SUPPORT, ATTACK };
		QString name, incantations;
		bool EK, MS, ED, RP;
		int mana, cd, cdGroup, soulPoints;
		TYPE_OF_SPELL typeOfSpell;
		RestoreMethode toRestoreMethode() {
			RestoreMethode toRet;
			toRet.name = this->name;
			toRet.mana = this->mana;
			toRet.cd = this->cd;
			toRet.cdGroup = this->cdGroup;
			toRet.type = RestoreMethode::TypeOfMethode::SPELL;
			return toRet;
		}
	};	

	static QStringList TOOL_getCodesOfAllInFolder_regular(QString pathToInputFolder, QString pathToOutputFolder);
	static QStringList TOOL_getCodesOfAllInFolder_bottom(QString pathToInputFolder);
	static void TOOL_saveImgToOutPutFolder(QImage& img, QString extraName);
	static void cutImgWithLettersToSingleLettersImgList(QImage& img, QList<QImage>& list);
	static QMap<QString, QString> getQmapWithCodes();
	static QString getPathToSettingsFile();
	static void getMapWithNumbersFromBottomBar(QMap<QString, int>& lightToRet, QMap<QString, int>& darkToRet);
	static void imgToBlackAndWhiteOneColor(QImage& img, int threshold);
	static QString StrCodeToLetter(QString code);
	static void cutBlackBordersOfImg(QImage& img);
	static QString letterImgToLetterCodeStr(QImage* SingleLetterImg);

	static void TOOL_manaSit(int pid,QString winTitle){
		int i = 0;
		while (true) {
			Key key;
			if (i == 0)
				key = Key("F7");
			else if (i == 1)
				key = Key("END");
			else
				key = Key("F11");
			Utilities::sendKeyStrokeToProcess(key, pid, winTitle);
			Sleep(125 * 100);
			i++;
			i = i % 3;
		} 
	}
	/*
	static void UNSUED_findBoredersOfFrames(QImage fullScreen);
	static void UNUSED_imgToOneColor(QImage& img, QRgb minimalColorValues, QRgb maxColorValues, QRgb colorToSet, bool allOfThem);
	static void UNUSED_imgAvoideOneColor(QImage& img, QRgb minimalColorValues, QRgb maxColorValues, bool allOfThem);
	static void UNUSED_changeGreyPixelsToBlack(QImage& img, int minGreyVal, int maxGreyVal);
	static int UNUSED_modifyBit(int numberToEdit, int postition, int zeroOrOne);
	*/
};
