#pragma once
#include <QObject>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qjsonarray.h>
#include <qfile.h>
#include <qfileinfo.h>

#include "PathResource.h"
#include "Utilities.h"
#include "Profile.h"
#include "Item.h"
#include "Profession.h"
#include "Spell.h"
#include "Potion.h"
class JsonParser{
public:
	static bool openJsonFile(QJsonObject& jsonDoc, QString pathToFile);
	static bool readItemJson(QList<Item>& items);
	static bool getManaRestoreMethodes(QStringList potionNameList, QList<Potion>& potionToReturn);
	static bool getItemsFromCategory(QList<Item> &readItems, Item::TYPE_OF_ITEM type);
	static QMap<QString, int> readAvaibleKeys();
	static QStringList readNamesOfAllSavedProfiles();
	static bool readSpellsJson(QList<Spell>& spells, Spell::SpellType* type, Profession* profession);
	static bool readPotions(QList<Potion>& potions, Profession* prof, Potion::TypeOfPotion* filterType);
	static void saveProfile(Profile* prof);
	static Profile loadProfile(QString profileName);
	static void deleteProfileFile(QString profileName);
	static QStringList getNamesOManaPotsForProf(Profession profession);
	static QStringList getNamesOfHealingPotsAndSpellsForProf(Profession profession);
	static bool saveJsonFile(QString pathToFolder, QString fileNameWithoutExtension, QJsonObject jsonObj);
private:
	JsonParser() {};
};
