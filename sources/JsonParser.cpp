#include "JsonParser.h"
#include "Utilities.h"
JsonParser::JsonParser()
{
}

JsonParser::~JsonParser()
{
}

QMap<Utilities::Item::TYPE_OF_ITEM, QString> JsonParser::itemType_itemStr_map = {
	{Utilities::Item::TYPE_OF_ITEM::ARMOR, "armors"},
	{Utilities::Item::TYPE_OF_ITEM::AMULETS, "amulets"},
	{Utilities::Item::TYPE_OF_ITEM::BOOTS, "boots"},
	{Utilities::Item::TYPE_OF_ITEM::CREATURE, "creature"},
	{Utilities::Item::TYPE_OF_ITEM::HELMETS, "helmets"},
	{Utilities::Item::TYPE_OF_ITEM::LEGS, "legs"},
	{Utilities::Item::TYPE_OF_ITEM::OTHER, "other"},
	{Utilities::Item::TYPE_OF_ITEM::POTIONS, "potions"},
	{Utilities::Item::TYPE_OF_ITEM::RINGS, "rings"},
	{Utilities::Item::TYPE_OF_ITEM::RUNES, "runes"},
	{Utilities::Item::TYPE_OF_ITEM::SHIELDS, "shields"},
	{Utilities::Item::TYPE_OF_ITEM::VALUABLES, "valuables"},
	{Utilities::Item::TYPE_OF_ITEM::AMMO, "ammo"},
	{Utilities::Item::TYPE_OF_ITEM::AXES, "axes"},
	{Utilities::Item::TYPE_OF_ITEM::SWORDS, "swords"},
	{Utilities::Item::TYPE_OF_ITEM::CLUBS, "clubs"},
	{Utilities::Item::TYPE_OF_ITEM::DISTANCES, "distance"},
	{Utilities::Item::TYPE_OF_ITEM::ROD, "rod"},
	{Utilities::Item::TYPE_OF_ITEM::WANDS, "wands"}
};

bool JsonParser::openJsonFile(QJsonObject* jsonDoc, QString pathToFile){
	QFile file;
	file.setFileName(pathToFile);
	bool sucess = file.open(QIODevice::ReadOnly | QIODevice::Text);
	if (!sucess) {
		Logger::logPotenialBug("Can't open that json file, probably it doesnt exist", "JsonParser", "openJsonFile");
		return false;
	}
	QString val = file.readAll();
	if (val.size() == 0) {
		Logger::logPotenialBug("opened but can't be read that json file, probably no rights to read/it's empty/ it not vaild", "JsonParser", "openJsonFile");
		return false;
	}
	file.close();
	QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
	*jsonDoc = doc.object();
	return true;
}

bool JsonParser::readSpellsJson(QList<Spell>& spells){
	QJsonObject obj;
	bool res = openJsonFile(&obj, spellFilePath);
	if (!res) {
		Logger::logPotenialBug("Problem with Json reading", "JsonParser", "readSpellsJson");
		return false;
	}

	QJsonArray arr = obj["spells"].toArray();
	if (arr.size() == 0) {
		Logger::logPotenialBug("No spells in json file", "JsonParser", "readSpellsJson");
		return false;
	}

	QList<Spell> spellsToRet;
	for (size_t i = 0; i < arr.size(); i++){
		QJsonValue val = arr.at(i);
		Spell objToAdd;
		objToAdd.name = val["name"].toString();
		objToAdd.incantations = val["incantantion"].toString();
		objToAdd.EK = val["EK"].toBool();
		objToAdd.RP = val["RP"].toBool();
		objToAdd.MS = val["MS"].toBool();
		objToAdd.ED = val["ED"].toBool();
		objToAdd.mana = val["mana"].toInt();
		objToAdd.cd = val["CD"].toInt();
		objToAdd.cdGroup = val["CDGROUP"].toInt();
		objToAdd.soulPoints = val["SoulPoints"].toInt();
		QString typeAsStr = val["TYPE"].toString();
		Spell::TYPE_OF_SPELL type;
		if (typeAsStr == "HEALING")
			type = Spell::TYPE_OF_SPELL::HEALING;
		else if (typeAsStr == "SUPPORT")
			type = Spell::TYPE_OF_SPELL::SUPPORT;
		else if (typeAsStr == "ATTACK")
			type = Spell::TYPE_OF_SPELL::ATTACK;
		else {
			Logger::logPotenialBug("Type value read from json file is other than 3 allowed types", "JsonParser", "readSpellsJson");
			return false;
		}
		objToAdd.typeOfSpell = type;
		spellsToRet.push_back(objToAdd);
	}
	spells = spellsToRet;
	return true;
}

bool JsonParser::filtrSpells(QList<Spell>& spells, Profile::PROFESSION* prof, Spell::TYPE_OF_SPELL* type){
	if (prof == NULL && type == NULL)
		return false;
	typedef Utilities::Spell Spell;

	bool filtrByProf = prof != NULL;
	bool filtrByType = type != NULL;
	QList<Spell> spellsCopy = spells;      
	QList<Spell> spelsToRet;
	for each (Spell var in spellsCopy) {

		if (filtrByProf) {
			bool isProperProf = false;
			if (*prof == Profile::ED)
				isProperProf = var.ED;
			else if (*prof == Profile::EK)
				isProperProf = var.EK;
			else if (*prof == Profile::MS)
				isProperProf = var.MS;
			else if (*prof == Profile::RP)
				isProperProf = var.RP;

			if (!isProperProf)
				continue;
		}
		if (filtrByType) {
			bool isProperType = var.typeOfSpell == *type;
			if (!isProperType)
				continue;
		}
		spelsToRet.push_back(var);
	}
	spells = spelsToRet;
	return true;
}

bool JsonParser::getPotionsForProf(QList<Utilities::Potion>& potions, Profile::PROFESSION* prof, TypeOfPotion type){
	typedef Utilities::Potion Potion;
	QJsonObject obj;
	bool res = openJsonFile(&obj, itemsFilePath);
	if (!res) {
		Logger::logPotenialBug("Problem with Json reading", "JsonParser", "getPotionsForProf");
		return false;
	}

	QJsonArray arr = obj["potions"].toArray();
	if (arr.size() == 0) {
		Logger::logPotenialBug("No spells in json file", "JsonParser", "getPotionsForProf");
		return false;
	}

	QList<Potion> potionsToRet;
	for each (QJsonValue var in arr) {
		Potion potionToAdd;
		potionToAdd.name = var["name"].toString();
		potionToAdd.type = Utilities::Item::TYPE_OF_ITEM::POTIONS;
		potionToAdd.manaReg = var["mana"].toInt();
		potionToAdd.healthReg = var["health"].toInt();
		potionToAdd.forMage = var["for_mage"].toBool();
		potionToAdd.forRp = var["for_RP"].toBool();
		potionToAdd.forEk = var["for_EK"].toBool();

		bool healthPotionsWanted = (type == TypeOfPotion::EVERYPOTION || type == TypeOfPotion::HEALTH);
		bool manaPotionsWanted = (type == TypeOfPotion::EVERYPOTION || type == TypeOfPotion::MANA);

		bool skipCauseHealthCondition = healthPotionsWanted && potionToAdd.healthReg <= 0;
		bool skipCauseManaCondition = manaPotionsWanted && potionToAdd.manaReg <= 0;
		if (skipCauseHealthCondition || skipCauseManaCondition)
			continue;

		bool isMage, isRP, isEK;
		if (prof != NULL) {
			isMage = *prof == Profile::PROFESSION::ED || *prof == Profile::PROFESSION::MS;
			isRP = *prof == Profile::PROFESSION::RP;
			isEK = *prof == Profile::PROFESSION::EK;
		}

		bool skipCauseProfCon = true;
		if (prof == NULL)
			skipCauseProfCon = false;
		else if(potionToAdd.forMage && isMage)
			skipCauseProfCon = false;
		else if(potionToAdd.forEk && isEK)
			skipCauseProfCon = false;
		else if(potionToAdd.forRp && isRP)
			skipCauseProfCon = false;


		if (skipCauseProfCon)
			continue;

		potionsToRet.push_back(potionToAdd);
	}
	potions = potionsToRet;
	return true;
}

bool JsonParser::readItemJson(QList<Utilities::Item>* items){
	typedef Utilities::Item Item;
	QJsonObject obj;
	bool res = openJsonFile(&obj, itemsFilePath);
	if (!res) {
		Logger::logPotenialBug("Problem with Json reading", "JsonParser", "readSpellsJson");
		return false;
	}

	QJsonArray arr;
	QStringList listOfCategoryNames = { "armors","amulets","boots","creature","helmets",
		"legs","other","potions","rings","runes","shields","valuables","ammo","axes","swords",
		"clubs","distance","rod","wands"
	};
	for each (QString var in listOfCategoryNames) {
		QJsonArray toAdd = obj[var].toArray();
		for each (QJsonValue val in toAdd){
			arr.push_back(val);
		}
	}

	if (arr.size() == 0) {
		Logger::logPotenialBug("No items in json file", "JsonParser", "readSpellsJson");
		return false;
	}

	QList<Item> itemToRet;
	for (size_t i = 0; i < arr.size(); i++) {
		QJsonValue val = arr[i];
		Item objToAdd;
		objToAdd.name = val["name"].toString();
		objToAdd.price = val["price"].toInt();
		objToAdd.weight = val["weight"].toInt();
		if (val["sell_blue_dijn"].toBool())
			objToAdd.seller = Item::SELLER::BLUE_DJIN;
		else if (val["sell_green_dijn"].toBool())
			objToAdd.seller = Item::SELLER::GREEN_DJIN;
		else if (val["sell_yasir"].toBool())
			objToAdd.seller = Item::SELLER::YASIR;
		else if (val["sell_zao"].toBool())
			objToAdd.seller = Item::SELLER::ZAO;
		else if (val["sell_other"].toBool())
			objToAdd.seller = Item::SELLER::OTHER_SELLER;
		else if (val["sell_rashid"].toBool())
			objToAdd.seller = Item::SELLER::RASHID;
		else {
			Logger::logPotenialBug("Seller value read from json file is other than allowed and defined sellers", "JsonParser", "readItemJson");
			return false;
		}

		QString typeOfItem = val["type"].toString();
		
		if (typeOfItem == QString("ARMOR"))
			objToAdd.type = Item::TYPE_OF_ITEM::ARMOR;
		else if (typeOfItem == QString("AMULET"))
			objToAdd.type = Item::TYPE_OF_ITEM::AMULETS;
		else if (typeOfItem == QString("BOOTS"))
			objToAdd.type = Item::TYPE_OF_ITEM::BOOTS;
		else if (typeOfItem == QString("CREATURE"))
			objToAdd.type = Item::TYPE_OF_ITEM::CREATURE;
		else if (typeOfItem == QString("HELMET"))
			objToAdd.type = Item::TYPE_OF_ITEM::HELMETS;
		else if (typeOfItem == QString("LEGS"))
			objToAdd.type = Item::TYPE_OF_ITEM::LEGS;
		else if (typeOfItem == QString("OTHER"))
			objToAdd.type = Item::TYPE_OF_ITEM::OTHER;
		else if (typeOfItem == QString("POTION"))
			objToAdd.type = Item::TYPE_OF_ITEM::POTIONS;
		else if (typeOfItem == QString("RING"))
			objToAdd.type = Item::TYPE_OF_ITEM::RINGS;
		else if (typeOfItem == QString("RUNE"))
			objToAdd.type = Item::TYPE_OF_ITEM::RUNES;
		else if (typeOfItem == QString("SHIELD"))
			objToAdd.type = Item::TYPE_OF_ITEM::SHIELDS;
		else if (typeOfItem == QString("AMMO"))
			objToAdd.type = Item::TYPE_OF_ITEM::AMMO;
		else if (typeOfItem == QString("AXE"))
			objToAdd.type = Item::TYPE_OF_ITEM::AXES;
		else if (typeOfItem == QString("SWORD"))
			objToAdd.type = Item::TYPE_OF_ITEM::SWORDS;
		else if (typeOfItem == QString("CLUB"))
			objToAdd.type = Item::TYPE_OF_ITEM::CLUBS;
		else if (typeOfItem == QString("DISTANCE"))
			objToAdd.type = Item::TYPE_OF_ITEM::DISTANCES;
		else if (typeOfItem == QString("ROD"))
			objToAdd.type = Item::TYPE_OF_ITEM::ROD;
		else if (typeOfItem == QString("WAND"))
			objToAdd.type = Item::TYPE_OF_ITEM::WANDS;
		else
			return false;

		itemToRet.push_back(objToAdd);
	}
	*items = itemToRet;
	return true;
}

bool JsonParser::getHealthRestoreMethodes(QStringList incantationsAndSpellsList, QList<Utilities::RestoreMethode>& spellsAndPotionsObjects) {
	typedef Utilities::RestoreMethode RestoreMethode;
	QList<Item> potions;
	QList<Spell> spells;
	bool sucess = readSpellsJson(spells);
	Spell::TYPE_OF_SPELL typeToFiltr = Spell::TYPE_OF_SPELL::HEALING;
	bool sucess2 = filtrSpells(spells, NULL, &typeToFiltr);
	getItemsFromCategory(potions, Item::TYPE_OF_ITEM::POTIONS);

	bool sucess3 = !potions.isEmpty() && !spells.isEmpty();
	bool failure = !(sucess && sucess2 && sucess3);
	if (failure) {
		Logger::logPotenialBug("Problem in obtaining health spells and pots", "JsonParser", "getHealthRestoreMethodes");
		return false;
	}

	QList<RestoreMethode> restoreMethodesToRet;
	for each(QString nameOfMethodes in incantationsAndSpellsList){

		bool skipLookingInPotions = false;
		for each (Spell spell in spells){
			bool isProperSpellToReturn = spell.incantations == nameOfMethodes;
			if (isProperSpellToReturn) {
				skipLookingInPotions = true;
				RestoreMethode toAdd = spell.toRestoreMethode();
				restoreMethodesToRet.push_back(toAdd);
				break;
			}
		}
		if (skipLookingInPotions)
			continue;
		for each (Item item in potions){
			bool isProperPotionToReturn = item.name == nameOfMethodes;
			if (isProperPotionToReturn) {
				RestoreMethode toAdd = item.toRestoreMethode();
				restoreMethodesToRet.push_back(toAdd);
				break;
			}
		}
	}
	spellsAndPotionsObjects = restoreMethodesToRet;
	bool allWentGood = incantationsAndSpellsList.size() == restoreMethodesToRet.size();
	return allWentGood;
}

bool JsonParser::getManaRestoreMethodes(QStringList potionNameToBeFound, QList<Item>& potionToReturn){
	QList<Item> allExistingPotions;
	getItemsFromCategory(allExistingPotions, Item::TYPE_OF_ITEM::POTIONS);

	if (allExistingPotions.isEmpty())
		return false;

	QList<Item> foundPotions;
	for each (QString nameOfMethodes in potionNameToBeFound) {
		for each (Item item in allExistingPotions) {
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
}

bool JsonParser::getItemsFromCategory(QList<Item>& itemsToRet, Item::TYPE_OF_ITEM type){
	QList<Item> readItems, itemsTmp;
	bool sucess = readItemJson(&readItems);
	if (!sucess)
		return false;

	for each (Item var in readItems){
		bool isProperCategory = type == var.type;
		if (isProperCategory)
			itemsTmp.push_back(var);
	}

	itemsToRet = itemsTmp;
	return true;
}

bool JsonParser::saveJsonFile(QString pathToFolder, QString fileNameWithExtension, QJsonDocument& docToSave){
	QFileInfo folderInfo = QFileInfo(pathToFolder);
	bool isDir = folderInfo.isDir();
	if (!isDir){
		Logger::logPotenialBug("Error, path is not folder", "JsonParser", "saveJsonFile");
		return false;
	}
	bool isWritable = folderInfo.isWritable();
	if (!isDir) {
		Logger::logPotenialBug("Error, path is writable folder", "JsonParser", "saveJsonFile");
		return false;
	}

	QFile file(fileNameWithExtension);
	bool ok = file.open(QIODevice::OpenModeFlag::WriteOnly);
	if (!ok) {
		Logger::logPotenialBug("Another Error", "JsonParser", "saveJsonFile");
		return false;
	}
	file.write(docToSave.toJson());
	file.close();

	return true;
}

