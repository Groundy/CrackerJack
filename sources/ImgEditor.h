#pragma once

#include <QObject>
#include <qimage.h>
#include "Utilities.h"
class ImgEditor  : public QObject
{
	Q_OBJECT

public:
	ImgEditor(QObject *parent);
	~ImgEditor();
	static void imgToBlackAndWhiteAllColors(QImage& img, int threshold);	
	static void imgToBlackAndWhiteOneColor(QImage& img, int threshold);
	static void cutBlackBordersOfImg(QImage& img);
	static QString letterImgToLetterCodeStr(QImage* SingleLetterImg);

	static QString imgWithStrToStr(QImage& img);
	static QImage fromCharToImg(QChar CharToImg);
	static void rotateImgToRight(QImage& imgToRotate, int timesToRotateRight);
	static bool isItPixelFromFrame(const uint& color, const int& minValueAcceptable, const int& maxValueAcceptable, bool requireSameValuesOfRGB);
	static QImage getImageFromAdvancedCode(QString codeOfImg);
	static void cutImgWithLettersToSingleLettersImgList(QImage& img, QList<QImage>& list);
	static QList<QPoint> findStartPositionInImg_mulitpeImgs(QList<QImage*> imgsToFind, QImage& imgToShareWithin);
	static QList<QPoint> findStartPositionInImg(const QImage& imgToFind, const QImage& imgToShareWithin);
	static void saveMultipleImgs(const QImage& fullImg, QList<QRect> frames, QString pathToFolder, QString fileNameWithoutExtension);
};
