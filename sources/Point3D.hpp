#pragma once
#include <qjsonobject.h>
#include <qstring.h>
#include <qstringlist.h>

#include "JsonClass.hpp"
#include "Logger.hpp"
namespace CJ {
struct Point3D : JsonClass {
 public:
  Point3D() = default;
  Point3D(int x, int y, int f) : x_(x), y_(y), f_(f){};
  Point3D(const QString& str) {
    QStringList parts = str.split(",", Qt::SplitBehaviorFlags::SkipEmptyParts);
    if (parts.size() != 3) {
      QString text = QString("String pass to create Point3D has wrong structure. Input=[%1]").arg(str);
      //Logger::logPotenialBug(text, "Point3D", "Point3D"); //[TODO]
      return;
    }

    x_ = parts[0].right(parts[0].size() - 2).toInt();
    y_ = parts[1].right(parts[1].size() - 2).toInt();
    f_ = parts[2].right(parts[2].size() - 2).toInt();

    if (!isValid()) {
      QString text = QString("String pass to create Point3D has wrong values. Input=[%1]").arg(str);
      //Logger::logPotenialBug(text, "Point3D", "Point3D");
      return;
    }
  };
  Point3D(const QJsonObject& obj) {
    f_ = obj.value("f").toInt();
    y_ = obj.value("y").toInt();
    x_ = obj.value("x").toInt();
  };
  ~Point3D() = default;

  QString toString() const {
    return QString("X=%1,Y=%2,F=%3").arg(x_).arg(y_).arg(f_);
  };
  bool operator==(const Point3D& pt) {
    return (x_ == pt.x_) && (y_ == pt.y_) && (f_ == pt.f_);
  };
  QPoint getXY() {
    return QPoint(x_, y_);
  }
  bool isValid() const {
    return x_ > 0 && x_ <= 2560 && y_ > 0 && y_ < 2560 && f_ >= -8 && f_ <= 7;
  };
  QJsonObject toJson() const {
    return QJsonObject{{"x", x_}, {"y", y_}, {"f", f_}};
  }
  //thais temple cordinates
  int x_ = 625;
  int y_ = 1265;
  int f_ = 0;
};
}  // namespace CJ
