#pragma once
#include <QObject>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qjsonarray.h>
#include <qfile.h>
#include <qfileinfo.h>

#include "Utilities.h"
#include "Profile.h"
#include "Item.h"
#include "Profession.h"
#include "Spell.h"
class JsonParser{
public:
	JsonParser();
	~JsonParser();

	bool openJsonFile(QJsonObject& jsonDoc, QString pathToFile);
	bool readItemJson(QList<Item>& items);
	bool getHealthRestoreMethodes(QStringList incantationsAndSpellsList, QList<Utilities::RestoreMethode>& spellsAndPotionsObjects);
	bool getManaRestoreMethodes(QStringList potionNameList, QList<Item>& potionToReturn);
	bool getItemsFromCategory(QList<Item> &readItems, Item::TYPE_OF_ITEM type);
	static bool saveJsonFile(QString pathToFolder, QString fileNameWithExtension, QJsonDocument& docToSave);
	QMap<QString, int> readAvaibleKeys();


	QStringList readNamesOfAllSavedProfiles();
	bool readSpellsJson(QList<Spell>& spells, Spell::SpellType* type, Profession* profession);
	bool readPotions(QList<Potion>& potions, Profession* prof, Potion::TypeOfPotion* filterType);
	void saveProfile(Profile* prof);
	Profile loadProfiles(QString profileName);
	void deleteProfileFile(QString profileName) {
		QString profileFileName = profileName + ".json";
		QDir(pathToProfileFolder).remove(profileFileName);
	};
	QStringList getNamesOfHealingPotsAndSpellsForProf(Profession profession) {
		QList<Spell> spells;
		auto typeOfSpell = Spell::SpellType::Healing;
		readSpellsJson(spells, &typeOfSpell, &profession);

		QList<Potion> potions;
		auto typeFilters = Potion::TypeOfPotion::HEALTH;
		readPotions(potions, &profession, &typeFilters);

		QStringList avaiableHealthRestoreMethodesNames;
		for each (auto spell in spells)
			avaiableHealthRestoreMethodesNames.push_back(spell.getIncantation());
		for each (auto var in potions)
			avaiableHealthRestoreMethodesNames.push_back(var.getName());

		return avaiableHealthRestoreMethodesNames;
	}
	QStringList getNamesOManaPotsForProf(Profession profession) {
		QList<Potion> potions;
		auto typeFilter = Potion::TypeOfPotion::MANA;
		readPotions(potions, &profession, &typeFilter);

		QStringList namesOfAvaibleManaRestoreMethodes;
		for each (auto var in potions)
			namesOfAvaibleManaRestoreMethodes.push_back(var.getName());

		return namesOfAvaibleManaRestoreMethodes;
	}
private:
	QString spellsPath = "C:\\Moje\\pliki\\repos\\CrackerJackClient\\ResourcesUsing\\spells.json";//todo
	QString itemPath = "C:\\Moje\\pliki\\repos\\CrackerJackClient\\ResourcesUsing\\items.json"; //todo
	QString potionsPath = "C:\\Moje\\pliki\\repos\\CrackerJackClient\\ResourcesUsing\\potions.json"; //todo
	QString keyPath = "C:\\Moje\\pliki\\repos\\CrackerJackClient\\ResourcesUsing\\keys.json";//todo
	QString pathToProfileFolder = "C:\\Moje\\pliki\\repos\\CrackerJackClient\\Profiles\\";
};
