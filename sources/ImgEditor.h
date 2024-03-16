#pragma once
#include <qimage.h>

#include "Logger.hpp"
#include "RGBstruct.h"
class ImgEditor {
 public:
  ImgEditor();
  ~ImgEditor();

  static QVector<QPoint> findStartPositionInImg_mulitpeImgs(QList<QImage*> imgsToFind, QImage& imgToShareWithin);
  static void   saveMultipleImgs(const QImage& fullImg, QVector<QRect> frames, QString pathToFolder, QString fileNameWithoutExtension);
  static void   saveMultipleImgs(const QList<QImage>& fullImg, QString pathToFolder, QString fileNameWithoutExtension);
  static void   saveImgWitinLoop(const QImage& img) {
    img.save(QString("C:\\Users\\szczy\\Desktop\\tmps\\%1.png").arg(QString::number(QDateTime::currentMSecsSinceEpoch())));
  }

 private:
  static const uint      BLACK            = qRgb(0, 0, 0);
  static const uint      WHITE            = qRgb(255, 255, 255);
  
};
