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
  Key() = default;
  Key(const int keyVal) {
    bool valueAvaible = keys_codes_map_.values().contains(keyVal);
    if (!valueAvaible) {
      key_val_  = 0;
      key_name_ = QString();
      return;
    }

    key_val_  = keyVal;
    key_name_ = keys_codes_map_.key(key_val_, QString());
  };
  Key(const QString& keyName) {
    const bool name_available = keys_codes_map_.keys().contains(keyName);
    if (!name_available) {
      key_val_  = 0;
      key_name_ = QString();
      return;
    }

    key_val_  = keys_codes_map_.value(keyName, 0);
    key_name_ = keyName;
  };
  Key(const QJsonObject& obj) {
    if (!obj.contains("keyVal") || !obj.value("keyVal").isDouble() || !obj.contains("keyName") || !obj.value("keyName").isString()) {
      qWarning() << "Wrong json object source to create Key object";
      return;
    }
    key_val_  = obj.value("keyVal").toInt();
    key_name_ = obj.value("keyName").toString();
  };

  static QStringList getListOfAllPossibleKeys() {
    return keys_codes_map_.keys();
  };
  int getKeyVal() const {
    return key_val_;
  };
  QString getKeyName() const {
    return key_name_;
  };
  QJsonObject toJson() const {
    return QJsonObject{{"keyVal", static_cast<int>(key_val_)}, {"keyName", key_name_}};
  };
  bool isValid() const {
    return !key_name_.isEmpty() && key_val_ > 0;
  }

 private:
  uint                                    key_val_  = 0;
  QString                                 key_name_ = QString();
  static inline const QMap<QString, uint> keys_codes_map_{
      {"F1", 0x70},       {"F2", 0x71}, {"F3", 0x72}, {"F4", 0x73},   {"F5", 0x74},     {"F6", 0x75},
      {"F7", 0x76},       {"F8", 0x77}, {"F9", 0x78}, {"F10", 0x79},  {"F11", 0x7A},    {"F12", 0x7B},
      {"0", 48},          {"1", 49},    {"2", 50},    {"3", 51},      {"4", 52},        {"5", 53},
      {"6", 54},          {"7", 55},    {"8", 56},    {"9", 57},      {"ESC", 27},      {"INSERT", 45},
      {"DELETE", 46},     {"HOME", 36}, {"END", 35},  {"PAGEUP", 33}, {"PAGEDOWN", 34}, {"ScrollLock", 91},
      {"PauseBreak", 19}, {"/", 111},   {"*", 106},   {"-", 100},     {"+", 107},       {"`", 0xC0}};
};
}  // namespace CJ
