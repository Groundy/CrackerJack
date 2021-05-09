#pragma once
#include <QObject>
#include <qjsondocument.h>
#include "qjsonobject.h"
#include "qjsonvalue.h"
#include "qjsonarray.h"
#include "qfile.h"
#include "Utilities.h"
#include "Profile.h"
class JsonParser : public QObject
{
	Q_OBJECT
public:
	JsonParser();
	~JsonParser();
	enum class TypeOfPotion {HEALTH, MANA, EVERYPOTION};
	const QString spellFilePath = "C:\\Users\\ADMIN\\source\\repos\\CrackerJackClient\\Resources\\spells.json";
	const QString itemsFilePath = "C:\\Users\\ADMIN\\source\\repos\\CrackerJackClient\\Resources\\items.json";
	QString getItemCategoryName(Utilities::Item::TYPE_OF_ITEM typeOfItem);
	bool openJsonFile(QJsonObject* jsonDoc, QString pathToFile);
	bool readSpellsJson(QList<Utilities::Spell>& spells);
	bool filtrSpells(QList<Utilities::Spell>& spells, Profile::PROFESSION* prof, Utilities::Spell::TYPE_OF_SPELL* type);
	bool getPotionsForProf(QList<Utilities::Potion>& potions ,Profile::PROFESSION* prof, TypeOfPotion type);
	bool readItemJson(QList<Utilities::Item>* items);
	bool getHealthRestoreMethodes(QStringList incantationsAndSpellsList, QList<Utilities::RestoreMethode>& spellsAndPotionsObjects);
	bool getManaRestoreMethodes(QStringList potionNameList, QList<Utilities::Item>& potionToReturn);
	bool getItemsFromCategory(QList<Utilities::Item> &readItems, Utilities::Item::TYPE_OF_ITEM type);

};
