#pragma once
#include <qdatetime.h>
#include <qdir.h>
#include <qimage.h>
#include <qpoint.h>
#include <qrandom.h>
#include <qsize.h>
#include <qstring.h>
#include <qthread.h>

#include <QObject.h>

#include "Calibrator.h"
#include "ImgEditor.h"
#include "PathResource.h"
#include "VariablesClass.hpp"
class MinimapAnalyzer : public QThread {
  Q_OBJECT
 public:
  MinimapAnalyzer(QObject* parent, QSharedPointer<VariablesClass> var);
  ~MinimapAnalyzer();
  void run();

 signals:
  void sendPostitionsToGUI(QString, QString, QString);

 private:
  enum class MapType { REGULAR, WAYPOINT };

  QSharedPointer<VariablesClass> var;
  const int                      SLEEP_TIME_BETWEEN_LOOPS = 20, SLEEP_TIME_NO_ACTIVE = 1000;
  const int                      MIN_LAYER = -8, MAX_LAYER = 7;
  const QImage                   LAYER_SLIDER_IMG = setSliderImg();
  const int                      MIN_X_MAP_INDEX = 124, MAX_X_MAP_INDEX = 134, MIN_Y_MAP_INDEX = 121, MAX_Y_MAP_INDEX = 129;
  const QVector<QRect>           MINI_MAP_PARTS_POSITIONS = getMiniMapPartsPostions();
  const QSize                    PROPER_MINIMAP_IMG_SIZE  = QSize(106, 109);
  QPoint                         previousPosition;

  QImage         setSliderImg();
  int            getCurrentLayer(const QImage& layerImg);
  QVector<QRect> getMiniMapPartsPostions();
  QList<QImage>  splitMiniMap(const QImage& wholeMiniMap);
  QPoint         findPlayerPosition(const QImage& miniMap, const QImage* wholeMap);
  QList<QPoint>  findStartPosOfImgMap(const QImage& imgToFind, const QImage& imgToSearchWithin, QRect frameInBigWindow);
  QRect          getFrameToLookByPreviousPos();
  QPoint         getVectorToPlayerFromImgPieceIndex(int index);
};

/*
class ImgShearcher : public QThread {
        Q_OBJECT
public:
        ImgShearcher(QObject* parent, const QImage& imgSmall, const QImage&
imgBig, QPoint& foundStartPostiton, QRect frameToLookFor) : QThread(parent),
imgSmall(imgSmall), imgBig(imgBig), foundStartPostiton(foundStartPostiton),
frameToLookFor(frameToLookFor) {}; const QImage& imgSmall, & imgBig; QPoint&
foundStartPostiton; QRect frameToLookFor; void run() { foundStartPostiton =
ImgEditor::findExactStartPositionInImg(imgSmall, imgBig, frameToLookFor);
        }
};
*/
