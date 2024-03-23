#include "Key.h"
namespace CJ {
QStringList Key::getListOfAllPossibleKeys() {
  return KeysAndCodesMap.keys();
}

QJsonObject Key::toJson() const {
  QJsonObject json;
  json.insert("keyVal", keyVal);
  json.insert("keyName", keyName);
  return json;
}

Key::Key(int keyValue) {
  bool valueAvaible = KeysAndCodesMap.values().contains(keyValue);
  if (!valueAvaible) {
    keyVal  = -1;
    keyName = QString();
    return;
  }

  keyVal  = keyValue;
  keyName = KeysAndCodesMap.key(keyVal, QString());
}

Key::Key(QString keyNameToSet) {
  bool NameAvaible = KeysAndCodesMap.keys().contains(keyNameToSet);
  if (!NameAvaible) {
    keyVal  = -1;
    keyName = QString();
    return;
  }

  keyVal  = KeysAndCodesMap.value(keyNameToSet, -1);
  keyName = keyNameToSet;
}

Key::Key(QJsonObject obj) {
  if (!obj.contains("keyVal") || !obj.value("keyVal").isDouble() || !obj.contains("keyName") || !obj.value("keyName").isString()) {
    qWarning() << "Wrong json object source to create Key object";
    return;
  }
  keyVal  = obj.value("keyVal").toInt();
  keyName = obj.value("keyName").toString();
}

Key::Key() {
  this->keyVal = -1;
}

QMap<QString, int> Key::KeysAndCodesMap = {
    {"F1", 0x70},   {"F2", 0x71},  {"F3", 0x72},  {"F4", 0x73},   {"F5", 0x74},     {"F6", 0x75},       {"F7", 0x76},       {"F8", 0x77},
    {"F9", 0x78},   {"F10", 0x79}, {"F11", 0x7A}, {"F12", 0x7B},  {"0", 48},        {"1", 49},          {"2", 50},          {"3", 51},
    {"4", 52},      {"5", 53},     {"6", 54},     {"7", 55},      {"8", 56},        {"9", 57},          {"ESC", 27},        {"INSERT", 45},
    {"DELETE", 46}, {"HOME", 36},  {"END", 35},   {"PAGEUP", 33}, {"PAGEDOWN", 34}, {"ScrollLock", 91}, {"PauseBreak", 19}, {"/", 111},
    {"*", 106},     {"-", 100},    {"+", 107},    {"`", 0xC0}};
}  // namespace CJ
