#pragma once
#include <qjsonobject.h>
#include <qstring.h>

#include <cstddef>

#include "JsonClass.hpp"
#include "qmap.h"
#include "qsplitter.h"
namespace CJ {
class Key : JsonClass {
 public:
  static QMap<QString, int> KeysAndCodesMap;
  static QStringList        getListOfAllPossibleKeys();

  Key();
  Key(int keyVal);
  Key(QString keyName);
  Key(QJsonObject obj);

  int getKeyVal() const {
    return keyVal;
  };
  QString getKeyName() const {
    return keyName;
  };
  QJsonObject toJson() const;
  bool        isValid() const {
    return !keyName.isEmpty() && keyVal > 0;
  }

 private:
  int     keyVal;
  QString keyName;
};
}  // namespace CJ
