#include "JsonParser.h"

bool JsonParser::openJsonFile(QJsonObject& jsonDoc, QString pathToFile){
	QFile file;
	file.setFileName(pathToFile);
	bool sucess = file.open(QIODevice::ReadOnly | QIODevice::Text);
	if (!sucess) {
		//Logger::logPotenialBug("Can't open that json file, probably it doesnt exist", "JsonParser", "openJsonFile");
		return false;
	}
	QString val = file.readAll();
	if (val.size() == 0) {
		//Logger::logPotenialBug("opened but can't be read that json file, probably no rights to read/it's empty/ it not vaild", "JsonParser", "openJsonFile");
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
		bool openCorrectly = openJsonFile(obj, PathResource::getPathToSpellJsonFile());
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
			
			Spell toAdd(singleSpellJsonObj);
			if (profession != NULL) {//filtr by prof
				if (!toAdd.isForProf(*profession))
					continue;
			}
			if (type != NULL) {//filtr by type
				if (!toAdd.isSpellType(*type))
					continue;
			}
			spells.push_back(toAdd);
		}
		return true;
	}
	catch (const std::exception& e){
		qDebug() << e.what();
		Utilities::showMessageBox_INFO(e.what());
		return false;

	}
}
bool JsonParser::readPotions(QList<Potion>& potions, Profession* prof, Potion::TypeOfPotion* filterType){
	try{
		potions.clear();
		QJsonObject obj;
		bool openCorrectly = openJsonFile(obj, PathResource::getPathToPotionsJsonFile());
		if (!openCorrectly)
			throw std::exception("Can't open items json file!");

		if(!obj.contains("potions"))
			throw std::exception("No potions field in items json file!");

		if(!obj["potions"].isArray())
			throw std::exception("potions field in items json file is not array type!");

		QJsonArray arr = obj["potions"].toArray();
		for each (QJsonValue potionJsonVal in arr) {
			Potion potionToAdd(potionJsonVal.toObject());
			if (prof != NULL) {
				if(!potionToAdd.isForProf(*prof))
					continue;
			}
			if (filterType != NULL) {
				if (!potionToAdd.isType(*filterType))
					continue;
			}
			potions.push_back(potionToAdd);
		}
		return true;
	}
	catch (const std::exception& e){
		qDebug() << e.what();
		Utilities::showMessageBox_INFO(e.what());
		return false;
	}
}
bool JsonParser::readItemJson(QList<Item>& items){
	items.clear();
	QJsonObject obj;
	bool res = openJsonFile(obj, PathResource::getPathToItemJsonFile());
	if (!res) {
		//Logger::logPotenialBug("Problem with Json reading", "JsonParser", "readSpellsJson");
		return false;
	}

	QStringList listOfCategoryNames = Item::getListOfCategories();
	for each (QString nameOfJsonObj in listOfCategoryNames) {
		QJsonArray itemsOfOneCategory = obj[nameOfJsonObj].toArray();
		for each (QJsonValue itemAsJsonObj in itemsOfOneCategory)
			items.push_back(Item(itemAsJsonObj));
	}


	if (items.size() == 0) {
		//Logger::logPotenialBug("No items in json file", "JsonParser", "readSpellsJson");
		return false;
	}
	else
		return true;
}
bool JsonParser::getManaRestoreMethodes(QStringList potionNameToBeFound, QList<Potion>& potionToReturn){
	/*
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
	*/ return true;
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
bool JsonParser::saveJsonFile(QString pathToFolder, QString fileNameWithOutExtension, QJsonObject jsonObj){
	try{
		QFileInfo folderInfo = QFileInfo(pathToFolder);
		bool isDir = folderInfo.isDir();
		if (!isDir)
			throw("Error in saving json file, given pathToFolder is not really path to folder!");
		bool isWritable = folderInfo.isWritable();
		if (!isWritable) 
			throw("Error in saving json file, given folder is not writtable!");

		QString filePath;
		if(pathToFolder.right(1) == "\\")
			filePath = QString("%1%2.json").arg(pathToFolder, fileNameWithOutExtension);
		else
			filePath = QString("%1\\%2.json").arg(pathToFolder, fileNameWithOutExtension);
		QFile file(filePath);
		bool ok = file.open(QIODevice::OpenModeFlag::WriteOnly);
		if (!ok)
			throw("Error in saving json file!");
		file.write(QJsonDocument(jsonObj).toJson());
		file.close();
		return true;
	}
	catch (const std::exception& e){
		Logger::staticLog(e.what());
		return false;
	}
}
QMap<QString, int> JsonParser::readAvaibleKeys(){
	QJsonObject obj;
	bool openCorrectly = openJsonFile(obj, PathResource::getPathToKeysJsonFile());
	QMap<QString, int> keys;
	for each (auto var in obj.value("keys").toArray()){	
		QString keyName = var.toObject().keys().first();
		int keyVal = var.toObject().value(keyName).toInt();
		keys.insert(keyName, keyVal);
	}

	return keys;
}
QStringList JsonParser::readNamesOfAllSavedProfiles(){
	QDir profilesDir(PathResource::getPathToProfileFolder());
	QStringList fillters = QStringList() << "*.json";
	return profilesDir.entryList(fillters, QDir::Files);
}
void JsonParser::saveProfile(Profile* prof){
	saveJsonFile(PathResource::getPathToProfileFolder(), prof->getName(), prof->toJson());
}
Profile JsonParser::loadProfile(QString profileName){
	const QString filePath = PathResource::getPathToProfileFile(profileName);
	QJsonObject profJsonObj;
	openJsonFile(profJsonObj, filePath);
	return Profile(profJsonObj);
}
void JsonParser::deleteProfileFile(QString profileName) {
	QString profileFileName = profileName + ".json";
	QDir(PathResource::getPathToProfileFolder()).remove(profileFileName);
};
QStringList JsonParser::getNamesOManaPotsForProf(Profession profession) {
	QList<Potion> potions;
	auto typeFilter = Potion::TypeOfPotion::MANA;
	readPotions(potions, &profession, &typeFilter);

	QStringList namesOfAvaibleManaRestoreMethodes;
	for each (auto var in potions)
		namesOfAvaibleManaRestoreMethodes.push_back(var.getName());

	return namesOfAvaibleManaRestoreMethodes;
}
QStringList JsonParser::getNamesOfHealingPotsAndSpellsForProf(Profession profession) {
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
