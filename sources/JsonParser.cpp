#include "JsonParser.h"
#include "Utilities.h"
JsonParser::JsonParser()
{
}

JsonParser::~JsonParser()
{
}

bool JsonParser::openJsonFile(QJsonObject& jsonDoc, QString pathToFile){
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
	jsonDoc = doc.object();
	return true;
}

bool JsonParser::readSpellsJson(QList<Spell>& spells){
	QJsonObject obj;
	static const QString spellFilePath = "C:\\Users\\ADMIN\\source\\repos\\CrackerJackClient\\Resources\\spells.json";//todo

	bool res = openJsonFile(obj, spellFilePath);
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

bool JsonParser::getPotionsForProf(QList<Potion>& potions, Profile::PROFESSION* prof, TypeOfPotion type){
	QJsonObject obj;
	const QString itemsFilePath = "C:\\Users\\ADMIN\\source\\repos\\CrackerJackClient\\Resources\\items.json";//todo
	bool res = openJsonFile(obj, itemsFilePath);
	if (!res) {
		Logger::logPotenialBug("Problem with Json reading", "JsonParser", "getPotionsForProf");
		return false;
	}

	QJsonArray arr = obj["potions"].toArray();
	if (arr.size() == 0) {
		Logger::logPotenialBug("No potions in json file", "JsonParser", "getPotionsForProf");
		return false;
	}

	QList<Potion> potionsToRet;
	for each (QJsonValue var in arr) {
		Potion potionToAdd;
		potionToAdd.name = var["name"].toString();
		potionToAdd.type = Item::TYPE_OF_ITEM::POTIONS;
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

bool JsonParser::readItemJson(QList<Item>& items){
	QJsonObject obj;
	const QString itemsFilePath = "C:\\Users\\ADMIN\\source\\repos\\CrackerJackClient\\Resources\\items.json";//todo
	bool res = openJsonFile(obj, itemsFilePath);
	if (!res) {
		Logger::logPotenialBug("Problem with Json reading", "JsonParser", "readSpellsJson");
		return false;
	}

	QJsonArray arr;
	QStringList listOfCategoryNames = Item::getListOfCategories();
	for each (QString var in listOfCategoryNames) {
		QJsonArray toAdd = obj[var].toArray();
		for each (QJsonValue val in toAdd)
			arr.push_back(val);
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
		objToAdd.weight = val["weight"].toDouble();
		//todo zamienic te if/elseif na mape
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

		QString typeOfItemStr = val["type"].toString();
		Item::TYPE_OF_ITEM typeOfItem = Item::typesStrUsedInJson.key(typeOfItemStr);
		objToAdd.type = typeOfItem;
		items.push_back(objToAdd);
	}
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
	bool sucess = readItemJson(readItems);
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
	if (!isWritable) {
		Logger::logPotenialBug("Error, path is not writable folder", "JsonParser", "saveJsonFile");
		return false;
	}
	QString pathToFile = pathToFolder + "\\" + fileNameWithExtension;
	QFile file(pathToFile);
	bool ok = file.open(QIODevice::OpenModeFlag::WriteOnly);
	if (!ok) {
		Logger::logPotenialBug("Another Error", "JsonParser", "saveJsonFile");
		return false;
	}
	file.write(docToSave.toJson());
	file.close();

	return true;
}

