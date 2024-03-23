#pragma once
#include <qpoint.h>

#include "Point3D.h"
namespace CJ {
class PlayerPosition {
 public:
  void setPlayerPos(const QPoint position, int floorToSet) {
    x_     = position.x();
    y_     = position.y();
    floor_ = floorToSet;
  }
  void setPlayerPos(const Point3D position) {
    x_     = position.getX();
    y_     = position.getY();
    floor_ = position.getFloor();
  }
  void setPlayerPos(const int x, const int y, const int f) {
    x_     = x;
    y_     = y;
    floor_ = f;
  }
  Point3D getPlayerPos() {
    Point3D toRet = Point3D(x_, y_, floor_);
    return toRet;
  }
  void clear() {
    x_     = -1;
    y_     = -1;
    floor_ = -20;
  }
  int getFloor() {
    return floor_;
  }
  QPoint getXY() {
    return QPoint(x_, y_);
  }

 private:
  std::atomic<int> floor_ = 0;
  std::atomic<int> x_;
  std::atomic<int> y_;
};
}  // namespace CJ
