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
namespace CJ {
class JsonParser {
 public:
  static QVector<Item>   getItemsFromCategory(const ItemType&& type);
  static Route           readRoute(const QString& routeName);
  static QStringList     readNamesOfAllSavedProfiles();
  static QVector<Spell>  readSpellsJson(const Spell::SpellType&& type, const Profession&& profession);
  static QStringList     getNamesOfHealingPotsAndSpellsForProf(const Profession profession);
  static QVector<Potion> readPotions(const Profession&& prof, const Potion::TypeOfPotion&& filterType);
  static QStringList     readRunesNames();
  static void            saveProfileToJson(const Profile& prof);
  static Profile         loadProfileFromJson(const QString& profileName);
  static void            deleteProfileFile(const QString& profileName);
  static QStringList     getNamesOManaPotsForProf(const Profession profession);
  static void            saveJsonFile(const QString& fileNameWithoutExtension, const QJsonObject& jsonObj);
  static void            saveRouteToJson(const QString& name, const Route& route);

 private:
  JsonParser() = default;
  static QVector<Item> readItemsFromJson();
  static QJsonObject   getJsonFileObject(const QString& pathToFile);
  //static bool               getManaRestoreMethodes(QStringList potionNameList, QVector<Potion>& potionToReturn);
  //static QMap<QString, int> readAvailableKeys();
};
}  // namespace CJ
