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
class Utilities {
public:

	static int showMessageBox(QString title, QString text, QFlags<QMessageBox::StandardButton> buttons);
	static bool showMessageBox_NO_YES(QString title, QString text);
	static bool sendKeyStrokeToProcess(Key key, unsigned int PID, QString WindowName);
	static void saveImgToOutPutFolder(QImage* img, QString* extraName);
	static LPCWSTR convert_StrToLPCWSTR(QString str);
	static void cutBlackBordersOfImg(QImage& img);
	static void imgToBlackAndWhiteOneColor(QImage& img, int threshold);
	static void imgToBlackAndWhiteAllColors(QImage& img, int threshold);
	static void cutImgWithLettersToSingleLettersImgList(QImage& img, QList<QImage>& list);
	static QString imgWithStrToStr(QImage* img);
	static QString letterImgToLetterCodeStr(QImage* SingleLetterImg);
	static QChar StrCodeToQChar(QString code);
	static QMap<QString, QChar> getQmapWithCodes();
	static QImage fromCharToImg(QChar CharToImg);
	static void rotateImgToRight(QImage* imgToRotate, int timesToRotateRight);
	static bool isItPixelFromFrame(uint color, int minValueAcceptable, int maxValueAcceptable, bool requireSameValuesOfRGB);
	static LONG64 getCurrentTimeInMiliSeconds();
	static QImage getImageFromAdvancedCode(QString codeOfImg);
	static void getMapWithNumbersFromBottomBar(QMap<QString, int>& lightToRet, QMap<QString, int>& darkToRet);
	static int getNumberFromBottomBar(QImage& bottomBar);
	//
	enum class FieldsOfIniFile {
		LANGUAGE,
		LAST_USED_LOGIN,
		LAST_USED_PROFILE_NAME
	};
	static QMap<FieldsOfIniFile, QString> get_Field_NamesFromIni_map();
	//static QMap<FieldsOfIniFile, QString> Field_NamesFromIni_map;
	static QString readFromIniFile(FieldsOfIniFile nameOfField);
	static void writeIniFile(FieldsOfIniFile nameOfField, QString value);
	//
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
	class Item {
	public:
		enum class TYPE_OF_ITEM { ARMOR, AMULETS, BOOTS, CREATURE, HELMETS, LEGS, OTHER, POTIONS, RINGS, RUNES, SHIELDS, VALUABLES, AMMO, AXES, SWORDS, CLUBS, DISTANCES, ROD, WANDS };
		enum class SELLER {BLUE_DJIN, GREEN_DJIN, YASIR, ZAO, OTHER_SELLER, RASHID};
		QString name;
		int price, weight;
		TYPE_OF_ITEM type;
		SELLER seller;
		RestoreMethode toRestoreMethode() {
			bool isPotion = this->type == TYPE_OF_ITEM::POTIONS;
			if (isPotion) {
				RestoreMethode toRet;
				toRet.name = this->name;
				toRet.mana = 0;//manaNeededToUsePotion
				toRet.cd = 1;//potions have 1 sec cooldown, for all potions
				toRet.cdGroup = 1;
				toRet.type = RestoreMethode::TypeOfMethode::POTION;
				return toRet;
			}
			else
				return RestoreMethode();//diag err
		}
	};
	class Potion : public Item {
	public:
		int manaReg, healthReg;
		bool forMage, forRp, forEk;
	};
	//
	static QStringList TOOL_getCodesOfAllInFolder_regular(QString pathToInputFolder, QString pathToOutputFolder);
	static QStringList TOOL_getCodesOfAllInFolder_bottom(QString pathToInputFolder);
	/*
	static void UNSUED_findBoredersOfFrames(QImage fullScreen);
	static void UNUSED_imgToOneColor(QImage& img, QRgb minimalColorValues, QRgb maxColorValues, QRgb colorToSet, bool allOfThem);
	static void UNUSED_imgAvoideOneColor(QImage& img, QRgb minimalColorValues, QRgb maxColorValues, bool allOfThem);
	static void UNUSED_changeGreyPixelsToBlack(QImage& img, int minGreyVal, int maxGreyVal);
	static int UNUSED_modifyBit(int numberToEdit, int postition, int zeroOrOne);
	*/
};
