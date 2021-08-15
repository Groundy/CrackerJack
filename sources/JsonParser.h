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
class JsonParser{

	typedef Utilities::Item Item;
	typedef Utilities::Spell Spell;
public:
	JsonParser();
	~JsonParser();
	static QMap<Item::TYPE_OF_ITEM, QString> itemType_itemStr_map;

	enum class TypeOfPotion {HEALTH, MANA, EVERYPOTION};
	const QString spellFilePath = "C:\\Users\\ADMIN\\source\\repos\\CrackerJackClient\\Resources\\spells.json";
	const QString itemsFilePath = "C:\\Users\\ADMIN\\source\\repos\\CrackerJackClient\\Resources\\items.json";//todo
	bool openJsonFile(QJsonObject* jsonDoc, QString pathToFile);
	bool readSpellsJson(QList<Spell>& spells);
	bool filtrSpells(QList<Spell>& spells, Profile::PROFESSION* prof, Spell::TYPE_OF_SPELL* type);
	bool getPotionsForProf(QList<Utilities::Potion>& potions ,Profile::PROFESSION* prof, TypeOfPotion type);
	bool readItemJson(QList<Item>* items);
	bool getHealthRestoreMethodes(QStringList incantationsAndSpellsList, QList<Utilities::RestoreMethode>& spellsAndPotionsObjects);
	bool getManaRestoreMethodes(QStringList potionNameList, QList<Item>& potionToReturn);
	bool getItemsFromCategory(QList<Item> &readItems, Item::TYPE_OF_ITEM type);
	bool saveJsonFile(QString pathToFolder, QString fileNameWithExtension, QJsonDocument& docToSave);

};
