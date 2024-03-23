#pragma once
#include <qpoint.h>

#include "Point3D.h"
namespace CJ {
class PlayerPosition {
 public:
  void setPlayerPos(const QPoint xyToSet, int floorToSet) {
    QMutexLocker locker(&positionMutex);
    xy    = xyToSet;
    floor = floorToSet;
  }
  void setPlayerPos(const Point3D toSet) {
    QMutexLocker locker(&positionMutex);
    xy    = QPoint(toSet.getX(), toSet.getY());
    floor = toSet.getFloor();
  }
  void setPlayerPos(const int x, const int y, const int f) {
    QMutexLocker locker(&positionMutex);
    xy    = QPoint(x, y);
    floor = f;
  }
  Point3D getPlayerPos() {
    QMutexLocker locker(&positionMutex);
    Point3D      toRet = Point3D(xy.x(), xy.y(), floor);
    return toRet;
  }
  void clear() {
    QMutexLocker locker(&positionMutex);
    xy    = QPoint(-1, -1);
    floor = -20;
  }
  int getFloor() {
    QMutexLocker locker(&positionMutex);
    return floor;
  }
  QPoint getXY() {
    QMutexLocker locker(&positionMutex);
    QPoint       toRet = xy;
    return xy;
  }

 private:
  QMutex           positionMutex;
  std::atomic<int> floor = 0;
  QPoint           xy;
};
}  // namespace CJ
