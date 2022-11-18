#pragma once
#include <QObject>
#include <qjsondocument.h>
#include "qjsonobject.h"
#include "qjsonvalue.h"
#include "qjsonarray.h"
#include "qfile.h"
#include "Utilities.h"
#include "Profile.h"
#include "qfileinfo.h"
#include "Item.h"
class JsonParser{
	typedef Utilities::Spell Spell;
public:
	JsonParser();
	~JsonParser();

	enum class TypeOfPotion {HEALTH, MANA, EVERYPOTION};
	static bool openJsonFile(QJsonObject& jsonDoc, QString pathToFile);
	bool readSpellsJson(QList<Spell>& spells);
	bool filtrSpells(QList<Spell>& spells, Profile::PROFESSION* prof, Spell::TYPE_OF_SPELL* type);
	bool getPotionsForProf(QList<Potion>& potions ,Profile::PROFESSION* prof, TypeOfPotion type);
	static bool readItemJson(QList<Item>& items);
	bool getHealthRestoreMethodes(QStringList incantationsAndSpellsList, QList<Utilities::RestoreMethode>& spellsAndPotionsObjects);
	bool getManaRestoreMethodes(QStringList potionNameList, QList<Item>& potionToReturn);
	bool getItemsFromCategory(QList<Item> &readItems, Item::TYPE_OF_ITEM type);
	static bool saveJsonFile(QString pathToFolder, QString fileNameWithExtension, QJsonDocument& docToSave);
	QMap<QString, int> readAvaibleKeys();
};
