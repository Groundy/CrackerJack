#pragma once
#include <qpoint.h>

#include "Point3D.hpp"
namespace CJ {

//[TODO ] this class is useless!!! replace it with SharedPoiner to Point 3D in Variable class later!
class PlayerPosition {
 public:
  void setPlayerPos(const QPoint position, int floorToSet) {
    QMutexLocker locker(&mutex);
    pos_ = Point3D(position.x(), position.y(), floorToSet);
  }
  void setPlayerPos(const Point3D position) {
    QMutexLocker locker(&mutex);
    pos_ = position;
  }
  void setPlayerPos(const int x, const int y, const int f) {
    QMutexLocker locker(&mutex);
    pos_ = Point3D(x, y, f);
  }
  Point3D getPlayerPos() {
    QMutexLocker locker(&mutex);
    return pos_;
  }
  void clear() {
    QMutexLocker locker(&mutex);
    pos_ = Point3D{};
  }
  int getFloor() {
    QMutexLocker locker(&mutex);
    return pos_.f_;
  }
  QPoint getXY() {
    QMutexLocker locker(&mutex);
    return pos_.getXY();
  }

 private:
  QMutex  mutex;
  Point3D pos_{};
};
}  // namespace CJ
