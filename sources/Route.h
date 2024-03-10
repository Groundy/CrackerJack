#pragma once
#include <qdir.h>
#include <qjsonarray.h>
#include <qmap.h>

#include "PathResource.h"
#include "Point3D.h"
#include "RoutePoint.h"
class Route : JsonClass {
 public:
  Route() = default;
  Route(QJsonObject obj);
  ~Route() = default;

  QStringList toStringList();
  void        addPoint(RoutePoint point);
  bool        removePoint(int index);
  bool        movePointUp(int index);
  bool        movePointDown(int index);
  int         size();
  void        clear();
  bool        checkRouteCorectness(QString& errorTextToDisplay);
  void        setName(QString name) {
    routeName_ = name;
  };
  QString getName() const {
    return routeName_;
  };
  RoutePoint  getPoint(int index);
  QJsonObject toJson() const;
  bool        isValid() const;
  bool        checkIfPositionIsOnListOnIndex(Point3D toCheck, int indexToCheck);
  Route&      operator=(const Route& newRoute) {
    this->route_     = newRoute.route_;
    this->routeName_ = newRoute.getName();
    return *this;
  }

 private:
  QList<RoutePoint>                    route_;
  QString                              routeName_;
  QMap<RoutePoint::FieldType, QString> pointTypeNameMap_ = RoutePoint::getPointTypeNameMap();
};
