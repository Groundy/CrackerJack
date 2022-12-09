#pragma once
#include <qimage.h>

#include "Logger.h"
#include "RGBstruct.h"
#include "Utilities.h"
class ImgEditor{
public:
	static void imgToBlackAndWhiteAllColors(QImage& img, int threshold);	
	static void imgToBlackAndWhiteOneColor(QImage& img, int threshold);
	static void cutBlackBordersOfImg(QImage& img);
	static QString binaryLetterImgToLetterStr (QImage& singleLetterImg);

	static QString imgWithStrToStr(QImage& img, int threshold = 240);
	static QImage fromCharToImg(QChar CharToImg);
	static void rotateImgToRight(QImage& imgToRotate, int timesToRotateRight);
	static bool isItPixelFromFrame(const uint& color, const int& minValueAcceptable, const int& maxValueAcceptable, bool requireSameValuesOfRGB);
	static QImage getColorImageFromCode(const QString& codeOfImg);
	static void cutImgWithLettersToSingleLettersImgList(QImage& img, QList<QImage>& list);
	static QList<QPoint> findStartPositionInImg_mulitpeImgs(QList<QImage*> imgsToFind, QImage& imgToShareWithin);
	static QList<QPoint> findStartPositionInImg(const QImage& imgToFind, const QImage& imgToShareWithin, QRect frameInBigWindow = QRect());
	static QPoint findExactStartPositionInImg(const QImage& imgToFind, const QImage& imgToShareWithin, QRect frameInBigWindow = QRect());
	static void saveMultipleImgs(const QImage& fullImg, QList<QRect> frames, QString pathToFolder, QString fileNameWithoutExtension);
	static void saveMultipleImgs(const QList<QImage>& fullImg, QString pathToFolder, QString fileNameWithoutExtension);
	static void saveImgWitinLoop(const QImage& img) {
		img.save(QString("C:\\Users\\szczy\\Desktop\\tmps\\%1.png").arg(QString::number(QDateTime::currentMSecsSinceEpoch())));
	}
private:
	ImgEditor();
	~ImgEditor();
};
