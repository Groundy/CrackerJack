#include "JsonParser.h"
namespace CJ {
QJsonObject JsonParser::getJsonFileObject(const QString& pathToFile) {
  QFile      file(pathToFile);
  const bool sucess = file.open(QIODevice::ReadOnly | QIODevice::Text);
  if (!sucess) {
    //Logger::logPotenialBug("Can't open that json file, probably it doesnt exist", "JsonParser", "openJsonFile"); //[TODO]
    return QJsonObject();
  }
  const QString content = file.readAll();
  if (content.isEmpty()) {
    //Logger::logPotenialBug("opened but can't be read that json file, probably no rights to read/it's empty/ it not vaild", "JsonParser", "openJsonFile"); //[TODO]
    return QJsonObject();
  }
  file.close();
  return QJsonDocument::fromJson(content.toUtf8()).object();
}
QVector<Spell> JsonParser::readSpellsJson(const Spell::SpellType&& type, const Profession&& profession) {
  const QJsonObject obj         = getJsonFileObject(PathResource::getPathToSpellJsonFile());
  const QJsonArray  spell_jsons = obj["spells"].toArray();
  QVector<Spell>    spells;
  foreach (const QJsonValue& spell_json, spell_jsons) {
    Spell spell(spell_json.toObject());
    if (spell.isForProf(profession) && spell.isSpellType(type)) {
      spells.append(spell);
    }
  }
  return spells;
}
QStringList JsonParser::readRunesNames() {
  const QJsonArray runes_array = getJsonFileObject(PathResource::getPathToRunesFile()).value("runes").toArray();
  QStringList      toRet;
  foreach (const QJsonValue& arrObj, runes_array) {
    toRet.append(arrObj.toObject().value("name").toString());
  }
  return toRet;
}
QVector<Potion> JsonParser::readPotions(const Profession&& prof, const Potion::TypeOfPotion&& filterType) {
  const QJsonObject obj = getJsonFileObject(PathResource::getPathToPotionsJsonFile());
  if (!obj.contains("potions")) {
    qWarning() << "No potions field in items json file!";
    Utilities::showMessageBox_INFO("No potions field in items json file!");
    return QVector<Potion>();
  }
  if (!obj["potions"].isArray()) {
    qWarning() << "potions field in items json file is not array type!";
    Utilities::showMessageBox_INFO("potions field in items json file is not array type!");
    return QVector<Potion>();
  }

  QVector<Potion> potions;
  foreach (QJsonValue potionJsonVal, obj["potions"].toArray()) {
    Potion potionToAdd(potionJsonVal.toObject());
    if (potionToAdd.isForProf(prof) && potionToAdd.isType(filterType)) {
      potions.push_back(potionToAdd);
    }
  }
  return potions;
}
QVector<Item> JsonParser::readItemsFromJson() {
  QJsonObject obj = getJsonFileObject(PathResource::getPathToItemJsonFile());
  if (!obj.isEmpty()) {
    //Logger::logPotenialBug("Problem with Json reading", "JsonParser", "readSpellsJson"); //[TODO]
    return QVector<Item>();
  }

  QVector<Item> to_ret;
  foreach (const QString& nameOfJsonObj, Item::getItemCategoriesNames()) {
    foreach (const QJsonValue& itemAsJsonObj, obj[nameOfJsonObj].toArray()) {
      to_ret.push_back(Item(itemAsJsonObj));
    }
  }

  if (to_ret.isEmpty()) {
    //Logger::logPotenialBug("No items in json file", "JsonParser", "readSpellsJson"); //[TODO]
  }
  return to_ret;
}

QVector<Item> JsonParser::getItemsFromCategory(const ItemType&& type) {
  QVector<Item> all_items = readItemsFromJson();
  Item::filrItems(all_items, static_cast<const ItemType&&>(type));
  return all_items;
}

void JsonParser::saveJsonFile(const QString& fileNameWithoutExtension, const QJsonObject& jsonObj) {
  const QString path_to_dir      = PathResource::getPathToTmpJsonDir();
  const QString abs_path_to_file = path_to_dir + fileNameWithoutExtension + ".json";
  QFile         file(abs_path_to_file);
  if (!file.open(QIODevice::OpenModeFlag::WriteOnly)) {
    qWarning() << "Error, saving json file";
    return;
  }
  file.write(QJsonDocument(jsonObj).toJson());
  file.close();
}

QStringList JsonParser::readNamesOfAllSavedProfiles() {
  QDir        profiles_dir = PathResource::getProfileFolder();
  QStringList fillters     = QStringList() << "*.json";
  return profiles_dir.entryList(fillters, QDir::Files);
}

void JsonParser::saveProfileToJson(const Profile& prof) {
  const QString abs_path_to_file = PathResource::getProfileFolder().path() + "//" + prof.getName() + ".json";
  QFile         file(abs_path_to_file);
  if (!file.open(QIODevice::OpenModeFlag::WriteOnly)) {
    qWarning() << "Error, saving json file";
    return;
  }
  file.write(QJsonDocument(prof.toJson()).toJson());
  file.close();
}

Profile JsonParser::loadProfileFromJson(const QString& profileName) {
  QJsonObject profJsonObj = getJsonFileObject(PathResource::getPathToProfileFile(profileName));
  return Profile(profJsonObj);
}
void JsonParser::deleteProfileFile(const QString& profileName) {
  QDir profilesDir = PathResource::getProfileFolder();
  profilesDir.remove(profileName + ".json");
};
QStringList JsonParser::getNamesOManaPotsForProf(const Profession profession) {
  const QVector<Potion> potions = readPotions(static_cast<const Profession&&>(profession), Potion::TypeOfPotion::MANA);

  QStringList namesOfAvaibleManaRestoreMethodes;
  foreach (auto var, potions) {
    namesOfAvaibleManaRestoreMethodes.push_back(var.getName());
  }

  return namesOfAvaibleManaRestoreMethodes;
}
QStringList JsonParser::getNamesOfHealingPotsAndSpellsForProf(const Profession profession) {
  const QVector<Spell>  spells  = readSpellsJson(Spell::SpellType::Healing, static_cast<const Profession&&>(profession));
  const QVector<Potion> potions = readPotions(static_cast<const Profession&&>(profession), Potion::TypeOfPotion::HEALTH);

  QStringList avaiableHealthRestoreMethodesNames;
  foreach (auto spell, spells) {
    avaiableHealthRestoreMethodesNames.push_back(spell.getIncantation());
  }
  foreach (auto var, potions) {
    avaiableHealthRestoreMethodesNames.push_back(var.getName());
  }

  return avaiableHealthRestoreMethodesNames;
}

Route JsonParser::readRoute(const QString& routeName) {
  const QJsonObject obj = getJsonFileObject(PathResource::getPathToRouteFile(routeName));
  const Route       route(obj);
  return route.isValid() ? route : Route();
}

void JsonParser::saveRouteToJson(const QString& name, const Route& route) {
  const QString abs_path_to_file = PathResource::getRouteFolder().path() + "//" + name + ".json";
  QFile         file(abs_path_to_file);
  if (!file.open(QIODevice::OpenModeFlag::WriteOnly)) {
    qWarning() << "Error, saving json file";
    return;
  }
  file.write(QJsonDocument(route.toJson()).toJson());
  file.close();
}

}  // namespace CJ

/*
bool JsonParser::getManaRestoreMethodes(QStringList potionNameToBeFound, QVector<Potion>& potionToReturn) {
  
	QList<Item> allExistingPotions;
	getItemsFromCategory(allExistingPotions, Item::TYPE_OF_ITEM::POTIONS);

	if (allExistingPotions.isEmpty())
		return false;

	QList<Item> foundPotions;
	foreach (QString nameOfMethodes, potionNameToBeFound) {
		foreach (Item item, allExistingPotions) {
			bool isProperPotionToReturn = item.name == nameOfMethodes;
			if (isProperPotionToReturn) {
				foundPotions.push_back(item);
				break;
			}
		}
	}
	potionToReturn = foundPotions;
	bool allWentGood = foundPotions.size() == potionNameToBeFound.size();
	return allWentGood;

  return true;
}*/

/*
QMap<QString, int> JsonParser::readAvailableKeys() {
  const QJsonArray   keys_from_json = getJsonFileObject(PathResource::getPathToKeysJsonFile()).value("keys").toArray();
  QMap<QString, int> keys;
  foreach (const QJsonValue& var, keys_from_json) {
    const QJsonObject obj     = var.toObject();
    const QString     keyName = obj.keys().first();
    const int         keyVal  = obj.value(keyName).toInt();
    keys.insert(keyName, keyVal);
  }

  return keys;
}
*/
