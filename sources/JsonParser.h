#pragma once
#include <qfile.h>
#include <qfileinfo.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>

#include <QObject>

#include "Item.h"
#include "PathResource.h"
#include "Potion.h"
#include "Profession.hpp"
#include "Profile.h"
#include "Route.h"
#include "Spell.h"
#include "Utilities.hpp"
class JsonParser {
 public:
  static bool               openJsonFile(QJsonObject& jsonDoc, QString pathToFile);
  static bool               readItemJson(QList<Item>& items);
  static bool               getManaRestoreMethodes(QStringList potionNameList, QList<Potion>& potionToReturn);
  static bool               getItemsFromCategory(QList<Item>& readItems, Item::TYPE_OF_ITEM type);
  static QMap<QString, int> readAvaibleKeys();
  static QStringList        readNamesOfAllSavedProfiles();
  static bool               readSpellsJson(QList<Spell>& spells, Spell::SpellType* type, Profession* profession);
  static QStringList        readRunesNames();
  static bool               readPotions(QList<Potion>& potions, Profession* prof, Potion::TypeOfPotion* filterType);
  static void               saveProfile(Profile* prof);
  static Profile            loadProfile(QString profileName);
  static void               deleteProfileFile(QString profileName);
  static QStringList        getNamesOManaPotsForProf(Profession profession);
  static QStringList        getNamesOfHealingPotsAndSpellsForProf(Profession profession);
  static bool               saveJsonFile(const QString& pathToFolder, const QString& fileNameWithoutExtension, const QJsonObject& jsonObj);
  static bool               readRoute(Route& route, QString routeName);

 private:
  JsonParser() = default;
};
