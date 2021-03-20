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
	static void sendKeyStrokeToProcess(Key key, unsigned int PID, QString WindowName);
	static int modifyBit(int numberToEdit, int postition, int zeroOrOne)
	{
		int mask = 1 << postition;
		return (numberToEdit & ~mask) | ((zeroOrOne << postition) & mask);
	}
	static void saveImgToOutPutFolder(QImage* img, QString extraName);
	static LPCWSTR convert_StrToLPCWSTR(QString str);

	static void cutBlackBordersOfImg(QImage* img);
	static void imgToBlackAndWhiteOneColor(QImage* img, int threshold);
	static void imgToBlackAndWhiteAllColors(QImage* img, int threshold);
	static void imgToOneColor(QImage* img, QRgb minimalColorValues, QRgb maxColorValues, QRgb colorToSet, bool allOfThem);
	static void changeGreyPixelsToBlack(QImage* img, int min, int max);
	static QStringList getCodesOfAllLetersInFolder(QString pathToInputFolder, QString pathToOutputFolder);
	static void cutImgWithLettersToSingleLettersImgList(QImage* img, QList<QImage>* list);
	static QString imgWithStrToStr(QImage* img);
	static QString letterImgToLetterCodeStr(QImage* SingleLetterImg);
	static QChar StrCodeToQChar(QString code);
	static QMap<QString, QChar> getQmapWithCodes();
	static QList<QPoint> findStartPositionInImg(QImage* imgToFind, QImage* imgToShareWithin);
	static QImage fromCharToImg(QChar CharToImg);
	static void rotateImgToRight(QImage* imgToRotate, int timesToRotateRight);
	//static void showQImageInWindow(QImage* img);
};