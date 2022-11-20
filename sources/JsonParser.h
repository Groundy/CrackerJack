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
#include "Profession.h"
class JsonParser{
	typedef Utilities::Spell Spell;
public:
	JsonParser();
	~JsonParser();

	enum class TypeOfPotion {HEALTH, MANA, EVERYPOTION};
	bool openJsonFile(QJsonObject& jsonDoc, QString pathToFile);
	bool readSpellsJson(QList<Spell>& spells);
	bool filtrSpells(QList<Spell>& spells, Profession* profession, Spell::TYPE_OF_SPELL* type);
	bool getPotionsForProf(QList<Potion>& potions, Profession* profession, TypeOfPotion type);
	bool readItemJson(QList<Item>& items);
	bool getHealthRestoreMethodes(QStringList incantationsAndSpellsList, QList<Utilities::RestoreMethode>& spellsAndPotionsObjects);
	bool getManaRestoreMethodes(QStringList potionNameList, QList<Item>& potionToReturn);
	bool getItemsFromCategory(QList<Item> &readItems, Item::TYPE_OF_ITEM type);
	static bool saveJsonFile(QString pathToFolder, QString fileNameWithExtension, QJsonDocument& docToSave);
	QMap<QString, int> readAvaibleKeys();

	void saveProfiles(Profile* prof);
	Profile loadProfiles(QString profileName);
	void deleteProfileFile(QString profileName) {};
	QStringList getNamesOfHealingPotsAndSpellsForProf(Profession profession) {
		QList<Potion> potions;
		QList<Utilities::Spell> spells;
		readSpellsJson(spells);
		auto typeOfSpell = Utilities::Spell::TYPE_OF_SPELL::HEALING;
		filtrSpells(spells, &profession, &typeOfSpell);
		getPotionsForProf(potions, &profession, JsonParser::TypeOfPotion::HEALTH);

		QStringList namesOfAvaibleHealthRestoreMethodes;
		for each (auto var in spells)
			namesOfAvaibleHealthRestoreMethodes.push_back(var.incantations);
		for each (auto var in potions)
			namesOfAvaibleHealthRestoreMethodes.push_back(var.name);

		return namesOfAvaibleHealthRestoreMethodes;
	}
	QStringList getNamesOManaPotsForProf(Profession profession) {
		QList<Potion> potions;
		getPotionsForProf(potions, &profession, JsonParser::TypeOfPotion::MANA);

		QStringList namesOfAvaibleManaRestoreMethodes;
		for each (auto var in potions)
			namesOfAvaibleManaRestoreMethodes.push_back(var.name);

		return namesOfAvaibleManaRestoreMethodes;
	}
private:
	QString spellsPath = "C:\\Moje\\pliki\\repos\\CrackerJackClient\\ResourcesUsing\\spells.json";//todo
	QString itemPath = "C:\\Users\\ADMIN\\source\\repos\\CrackerJackClient\\Resources\\items.json"; //todo
	QString keyPath = "C:\\Moje\\pliki\\repos\\CrackerJackClient\\ResourcesUsing\\keys.json";//todo
	QString pathToProfileFolder = "C:\\Moje\\pliki\\repos\\CrackerJackClient\\Profiles\\";
};
