#pragma once
#include <qjsonobject.h>

#include "JsonClass.hpp"
#include "Point3D.h"
namespace CJ {
class RoutePoint : JsonClass {
 public:
  enum FieldType {
    REGULAR,
    STAIRS_UP,
    STAIRS_DOWN,
    ROPE_FIELD,
    SHOVEL_HOLE_ALWAYS_OPEN,
    SHOVEL_HOLE_NEED_SHOVEL,
    LADDER_UP,
    LADDER_DOWN,
    TELEPORT,
    EXIT_POINT
  };
  RoutePoint();
  RoutePoint(Point3D position, FieldType fieldType);
  RoutePoint(QJsonObject jsonObj);

  static QMap<FieldType, QString> getPointTypeNameMap();
  static QVector<FieldType>       getGoingUpTypes();
  static QVector<FieldType>       getGoingDownTypes();
  static QVector<FieldType>       getStayingSameLevelTypes();

  bool        operator==(const RoutePoint& point);
  RoutePoint& operator=(const RoutePoint& point);
  QJsonObject toJson() const;
  bool        isValid() const;
  Point3D     getPosition() const {
    return position;
  }
  FieldType getFieldType() const {
    return fieldType;
  }
  void setPosition(Point3D point3d) {
    position = point3d;
  }
  void setFieldType(FieldType type) {
    fieldType = type;
  }

 private:
  FieldType fieldType;
  Point3D   position;
};
}  // namespace CJ
