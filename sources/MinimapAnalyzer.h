#pragma once
#include <QObject.h>
#include <qdatetime.h>
#include <qdir.h>
#include <qimage.h>
#include <qpoint.h>
#include <qrandom.h>
#include <qsize.h>
#include <qstring.h>
#include <qthread.h>

#include "PathResource.h"
#include "VariablesClass.hpp"
class MinimapAnalyzer : public QThread {
  Q_OBJECT
 public:
  MinimapAnalyzer(QObject* parent, QSharedPointer<VariablesClass> var);
  ~MinimapAnalyzer();

 signals:
  void sendPostitionsToGUI(QString, QString, QString);

 public slots:
  void execute();

 private:
  enum class MapType { REGULAR, WAYPOINT };

  QSharedPointer<VariablesClass> var;
  const int                      TIMER_INTERVAL = 20;
  const int                      MIN_LAYER = -8, MAX_LAYER = 7;
  const QImage                   LAYER_SLIDER_IMG = setSliderImg();
  const int                      MIN_X_MAP_INDEX = 124, MAX_X_MAP_INDEX = 134, MIN_Y_MAP_INDEX = 121, MAX_Y_MAP_INDEX = 129;
  const QVector<QRect>           MINI_MAP_PARTS_POSITIONS = getMiniMapPartsPostions();
  const QSize                    PROPER_MINIMAP_IMG_SIZE  = QSize(106, 109);
  QPoint                         previousPosition;

  QImage          setSliderImg();
  int             getCurrentLayer(const CJ_Image& layerImg);
  QVector<QRect>  getMiniMapPartsPostions();
  QList<QImage>   splitMiniMap(const QImage& wholeMiniMap);
  QPoint          findPlayerPosition(const CJ_Image& miniMap, const CJ_Image& wholeMap);
  QVector<QPoint> findStartPosOfImgMap(const QImage& imgToFind, const QImage& imgToSearchWithin, QRect frameInBigWindow);
  QRect           getFrameToLookByPreviousPos();
  QPoint          getVectorToPlayerFromImgPieceIndex(int index);
};
