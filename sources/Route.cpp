﻿#include "Route.h"

Route::Route(QJsonObject obj) {
  QJsonArray pointsArray = obj["points"].toArray();
  if (pointsArray.isEmpty()) {
    qWarning() << "No points fields in route file";
    return;
  }

  QList<RoutePoint> tmpRoute;
  for each (QJsonValue val in pointsArray) {
    RoutePoint toAdd(val.toObject());
    tmpRoute.append(toAdd);
  }
  if (tmpRoute.size() < 2) {
    qWarning() << "not enough points in route";
    return;
  }
  route_     = tmpRoute;
  routeName_ = obj.value("routeName").toString();
}

QStringList Route::toStringList() {
  if (route_.isEmpty()) {
    return QStringList();
  }

  QStringList ToRet;
  for (int i = 0; i < route_.size(); i++) {
    QString index = QString::number(i);
    QString pt    = route_[i].getPosition().toString();
    QString type  = pointTypeNameMap_.value(route_[i].getFieldType());
    ToRet << QString("[%1]  %2,  %3").arg(index, pt, type);
  }
  return ToRet;
}
void Route::addPoint(RoutePoint routePointToAdd) {
  RoutePoint toAdd(routePointToAdd);
  route_.push_back(toAdd);
}
bool Route::removePoint(int index) {
  bool indexInRange = index < route_.size();
  if (!indexInRange) {
    return false;
  }
  route_.removeAt(index);
  return true;
}
bool Route::movePointUp(int index) {
  bool itFirst = index == 0;
  if (itFirst) {
    return false;
  }
  route_.swap(index, index - 1);
  return true;
}
bool Route::movePointDown(int index) {
  bool isLast = index == route_.size() - 1;
  if (isLast) {
    return false;
  }

  route_.swap(index, index + 1);
  return true;
}
int Route::size() {
  return route_.size();
}
void Route::clear() {
  route_.clear();
}
QJsonObject Route::toJson() const {
  QJsonArray arr;
  for each (RoutePoint pt in route_) {
    arr.append(pt.toJson());
  }
  QJsonObject mainObj;
  mainObj.insert("routeName", routeName_);
  mainObj.insert("points", QJsonValue(arr));
  return mainObj;
};
bool Route::checkRouteCorectness(QString& errorTextToDisplay) {
  typedef RoutePoint::FieldType FieldType;
  if (route_.size() < 2) {
    qWarning() << "Route is too short";
    return false;
  }

  bool samePoint = route_.first() == route_.last();
  if (!samePoint) {
    qWarning() << "Route should start and end in the same point";
    return false;
  }

  QVector<FieldType> typesGoingDown = RoutePoint::getGoingDownTypes();
  QVector<FieldType> typesGoingSame = RoutePoint::getStayingSameLevelTypes();
  QVector<FieldType> typesGoingUp   = RoutePoint::getGoingUpTypes();

  enum class FLOOR_CHANGE { UP, SAME, DOWN };
  FLOOR_CHANGE flChange;

  for (int i = 0; i < route_.size() - 1; i++) {
    Point3D   current     = route_[i].getPosition();
    Point3D   next        = route_[i + 1].getPosition();
    FieldType currentType = route_[i].getFieldType();

    bool      canGoToNextPoint;
    const int currentF = current.getFloor();
    const int nextF    = next.getFloor();
    if (currentF < nextF) {
      canGoToNextPoint = typesGoingUp.contains(currentType);
      flChange         = FLOOR_CHANGE::UP;
    } else if (currentF == nextF) {
      canGoToNextPoint = typesGoingSame.contains(currentType);
      flChange         = FLOOR_CHANGE::SAME;
    } else if (currentF > nextF) {
      canGoToNextPoint = typesGoingDown.contains(currentType);
      flChange         = FLOOR_CHANGE::DOWN;
    }
    if (canGoToNextPoint) {
      continue;
    }
    qWarning() << "Can't find a way from point" << i << "to point" << QString::number(i + 1) << ", because type"
               << pointTypeNameMap_.value(currentType) << ", can't move to " << (flChange == FLOOR_CHANGE::DOWN ? "down" : "up")
               << "floor;";
    return false;
  }
  return true;
}
RoutePoint Route::getPoint(int index) {
  if (index < route_.size()) {
    return route_[index];
  } else if (index == route_.size()) {
    return route_[0];
  } else {
    return RoutePoint();
  }
}
bool Route::isValid() const {
  return (route_.size() > 2) && (route_.first().getPosition() == route_.last().getPosition());
}
bool Route::checkIfPositionIsOnListOnIndex(Point3D toCheck, int index) {
  if (index < 0 || index >= route_.size()) {
    return false;
  }

  int  currentX = toCheck.getX();
  int  currentY = toCheck.getY();
  int  x        = route_[index].getPosition().getX();
  bool properX  = currentX >= x - 1 && currentX <= x + 1;
  if (!properX) {
    return false;
  }
  int  y       = route_[index].getPosition().getY();
  bool properY = currentY >= y - 1 && currentY <= y + 1;
  if (!properY) {
    return false;
  }
  return true;
}
