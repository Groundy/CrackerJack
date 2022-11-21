#include "JsonParser.h"
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

bool JsonParser::readSpellsJson(QList<Spell>& spells, Spell::SpellType* type, Profession* profession){
	try{
		QJsonObject obj;
		bool openCorrectly = openJsonFile(obj, spellsPath);
		if (!openCorrectly)
			throw std::exception("Can't read json file with spells");

		if(!obj.contains("spells"))
			throw std::exception("No spells field in json file with spells");

		if (!obj["spells"].isArray())
			throw std::exception("spell field in json file is not an array!");

		QJsonArray arr = obj["spells"].toArray();
		if (arr.isEmpty())
			throw std::exception("spell array in json file is empty!");

		for (size_t i = 0; i < arr.size(); i++){
			QJsonObject singleSpellJsonObj = arr.at(i).toObject();
			
			//Spell toAdd(singleSpellJsonObj);
			if (profession != NULL) {//filtr by prof
				//if (!toAdd.isForProf(*profession))
				//	continue;
			}
			if (type != NULL) {//filtr by type
			//	if (!toAdd.isSpellType(*type))
				//	continue;
			}
			//spells.push_back(toAdd);
		}
		return true;
	}
	catch (const std::exception& e){
		qDebug() << e.what();
		Utilities::showMessageBox_INFO(e.what());
		return false;

	}
}

bool JsonParser::getPotionsForProf(QList<Potion>& potions, Profession* prof, TypeOfPotion type){
	QJsonObject obj;
	bool res = openJsonFile(obj, itemPath);
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

		bool skipCauseProfCon = true;
		if (prof == NULL)
			skipCauseProfCon = false;
		else if(potionToAdd.forMage && prof->isMage())
			skipCauseProfCon = false;
		else if(potionToAdd.forEk && prof->isEK())
			skipCauseProfCon = false;
		else if(potionToAdd.forRp && prof->isRP())
			skipCauseProfCon = false;


		if (skipCauseProfCon)
			continue;

		potionsToRet.push_back(potionToAdd);
	}
	potions = potionsToRet;
	return true;
}

bool JsonParser::readItemJson(QList<Item>& items){
	items.clear();
	QJsonObject obj;
	bool res = openJsonFile(obj, itemPath);
	if (!res) {
		Logger::logPotenialBug("Problem with Json reading", "JsonParser", "readSpellsJson");
		return false;
	}

	QStringList listOfCategoryNames = Item::getListOfCategories();
	for each (QString nameOfJsonObj in listOfCategoryNames) {
		QJsonArray itemsOfOneCategory = obj[nameOfJsonObj].toArray();
		for each (QJsonValue itemAsJsonObj in itemsOfOneCategory)
			items.push_back(Item(itemAsJsonObj));
	}


	if (items.size() == 0) {
		Logger::logPotenialBug("No items in json file", "JsonParser", "readSpellsJson");
		return false;
	}
	else
		return true;
}

bool JsonParser::getHealthRestoreMethodes(QStringList incantationsAndSpellsList, QList<Utilities::RestoreMethode>& spellsAndPotionsObjects) {
	/*
	typedef Utilities::RestoreMethode RestoreMethode;
	QList<Item> potions;
	QList<Spell> spells;
	Spell::SpellType typeToFiltr = Spell::SpellType::Healing;
	bool sucess = readSpellsJson(spells, &typeToFiltr);
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
	*/
	return true;
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

QMap<QString, int> JsonParser::readAvaibleKeys(){
	QJsonObject obj;
	bool openCorrectly = openJsonFile(obj, keyPath);
	QMap<QString, int> keys;
	for each (auto var in obj.value("keys").toArray()){	
		QString keyName = var.toObject().keys().first();
		int keyVal = var.toObject().value(keyName).toInt();
		keys.insert(keyName, keyVal);
	}

	return keys;
}

QStringList JsonParser::readNamesOfAllSavedProfiles(){
	QDir profilesDir(pathToProfileFolder);
	QStringList fillters = QStringList() << "*.json";
	return profilesDir.entryList(fillters, QDir::Files);
}

void JsonParser::saveProfiles(Profile* prof){
	const QString fileName = prof->profileName + ".json";
	QJsonDocument docToSave(prof->toJson());
	saveJsonFile(pathToProfileFolder, fileName, docToSave);
}

Profile JsonParser::loadProfiles(QString profileName){
	const QString filePath = pathToProfileFolder + "\\" + profileName + ".json";
	QJsonObject profJsonObj;
	openJsonFile(profJsonObj, filePath);
	return Profile(profJsonObj);
}

