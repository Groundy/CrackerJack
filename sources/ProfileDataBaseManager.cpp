#include "ProfileDataBaseManager.h"
#include "StringResource.h"
ProfileDataBaseManager::ProfileDataBaseManager(){
	QDir dir = QDir::current();//TODO sprawdzic czy po zainstwalowaniu w wersji finalnej jest to Ok i czy s¹ wszelkie prawa
	bool foundFolderWithProfs = dir.cd("Profiles");
	//TODO dodac obsluge gdy folder nie zostanie znaleziony
	pathToFolderWithProfiles = dir.absolutePath();
	
}

ProfileDataBaseManager::~ProfileDataBaseManager(){

}

QMap<ProfileDataBaseManager::FieldsOfDB, QString> ProfileDataBaseManager::getMapOfDBFields(){
	QMap<ProfileDataBaseManager::FieldsOfDB, QString> toRet;
	toRet.insert(ProfileDataBaseManager::PROFILE_NAME, "profileName");
	toRet.insert(ProfileDataBaseManager::PROFESION, "profession");
	toRet.insert(ProfileDataBaseManager::MANA_RESTORE_STRING, "manaString");
	toRet.insert(ProfileDataBaseManager::HEALTH_RESTORE_STRING, "healthString");
	toRet.insert(ProfileDataBaseManager::MANA_RESTORE_KEY, "manaKeys");
	toRet.insert(ProfileDataBaseManager::HEALTH_RESTORE_KEY, "healthKeys");
	toRet.insert(ProfileDataBaseManager::MANA_RESTORE_ITEM, "ManaItems");
	toRet.insert(ProfileDataBaseManager::HEALTH_RESTORE_ITEM, "HealthItems");
	toRet.insert(ProfileDataBaseManager::LAST_LOGIN, "lastLogin");
	toRet.insert(ProfileDataBaseManager::CREATION_TIME, "creationTime");

	toRet.insert(ProfileDataBaseManager::POS_HP, "POS_HP");
	toRet.insert(ProfileDataBaseManager::POS_SHP, "POS_SHP");
	toRet.insert(ProfileDataBaseManager::POS_GHP, "POS_GHP");
	toRet.insert(ProfileDataBaseManager::POS_UHP, "POS_UHP");
	toRet.insert(ProfileDataBaseManager::POS_SPHP, "POS_SPHP");
	toRet.insert(ProfileDataBaseManager::POS_GSP, "POS_GSP");
	toRet.insert(ProfileDataBaseManager::POS_USP, "POS_USP");
	toRet.insert(ProfileDataBaseManager::POS_MP, "POS_MP");
	toRet.insert(ProfileDataBaseManager::POS_SMP, "POS_SMP");
	toRet.insert(ProfileDataBaseManager::POS_GMP, "POS_GMP");
	toRet.insert(ProfileDataBaseManager::POS_UMP, "POS_UMP");

	toRet.insert(ProfileDataBaseManager::POS_LAST_GAME_FRAME, "POS_LAST_GAME_FRAME");
	toRet.insert(ProfileDataBaseManager::POS_LAST_HEALTH_BAR, "POS_LAST_HEALTH_BAR");
	toRet.insert(ProfileDataBaseManager::POS_LAST_MANA_BAR, "POS_LAST_MANA_BAR");
	toRet.insert(ProfileDataBaseManager::POS_LAST_MANA_SHIELD_BAR, "POS_LAST_MANA_SHIELD_BAR");
	toRet.insert(ProfileDataBaseManager::POS_LAST_MINIMAP, "POS_LAST_MINIMAP");
	toRet.insert(ProfileDataBaseManager::ROTATION, "ROTATION");

	return toRet;
}

QStringList ProfileDataBaseManager::getAllNamesOfProfFiles(){
	QDir directory(pathToFolderWithProfiles);
	QStringList profileFilesNames = directory.entryList(QStringList() << "PROF___*.ini", QDir::Files);
	return profileFilesNames;
}

bool ProfileDataBaseManager::getPathToProfileFile(QString profName, QString& path){
	QStringList listOfFiles = getAllNamesOfProfFiles();
	QString fileName;
	for each (QString var in listOfFiles) {
		QString strToCompare = "PROF___" + profName + ".ini";
		bool thisIsThatFile = var == strToCompare;
		if (thisIsThatFile) {
			path = QDir(pathToFolderWithProfiles).absoluteFilePath(var);
			return true;
		}
	}
	return false;
}

bool ProfileDataBaseManager::modifyFieldValue(QString profName, FieldsOfDB field, QString newValue){
	QString pathToFile;
	bool fileExist = getPathToProfileFile(profName, pathToFile);
	if (!fileExist)
		return false;//TODO zastanowic sie czy powinno sie stworzyc pusty plik i go modyfikowac czy tylko zwrocic return
	QSettings settings(pathToFile, QSettings::IniFormat);
	bool isEableToWrite = settings.isWritable();
	if (!isEableToWrite)
		return false;//TODO poinformowac uzytkownika ze jest problem z plikiem profilu

	QString nameOfFieldAsStr = field_exactNameInDb_map[field];
	settings.setValue(nameOfFieldAsStr, newValue);

	return true;
}

bool ProfileDataBaseManager::readFieldValue(QString profName, FieldsOfDB field, QString& readValue){
	QString pathToFile;
	bool fileExist = getPathToProfileFile(profName, pathToFile);
	if (!fileExist)
		return false;

	QSettings settings(pathToFile, QSettings::IniFormat);
	QString nameOfFieldAsStr = field_exactNameInDb_map[field];
	readValue = settings.value(nameOfFieldAsStr).toString();

	return true;
}

bool ProfileDataBaseManager::addNewProfileToFolder(QString profName){
	QString nameOfFile = "PROF___" + profName + ".ini";
	QFile fileToCreate(pathToFolderWithProfiles + "\\" + nameOfFile);

	fileToCreate.open(QIODevice::ReadWrite);
	fileToCreate.resize(0);
	fileToCreate.close();

	QMap<ProfileDataBaseManager::FieldsOfDB, QString> copy_map = field_exactNameInDb_map;
	modifyFieldValue(profName, FieldsOfDB::PROFILE_NAME, profName);
	copy_map.remove(FieldsOfDB::PROFILE_NAME);

	for each (FieldsOfDB field in copy_map.keys())
		modifyFieldValue(profName, field, "");
	
	return true;
}

bool ProfileDataBaseManager::deleteProfile(QString profName){
	QString nameOfFile = "PROF___" + profName + ".ini";
	bool result = QDir(pathToFolderWithProfiles).remove(nameOfFile);
	return result;
}

bool ProfileDataBaseManager::getNamesOfAllFilesInFormToDisplay(QStringList& namesOfProfiles){
	QStringList toRet;
	QStringList namesOfFiles = getAllNamesOfProfFiles();

	for each (QString var in namesOfFiles){
		QStringList list_1 = var.split("___", Qt::SkipEmptyParts);
		if (list_1.size() >= 2) {
			QStringList list_2 = list_1[1].split(".ini", Qt::SkipEmptyParts);
			if (!list_2.isEmpty()) {
				QString profileName = list_2[0];
				QString size = QString::number(toRet.size() + 1);
				QString finalStr = QString("[") + size + QString("] ") + profileName;
				toRet.push_back(finalStr);
			}
		}
	}
	namesOfProfiles = toRet;
	return true;
}

bool ProfileDataBaseManager::saveProfileToDataBase(Profile& profileToSave){
	typedef ProfileDataBaseManager::FieldsOfDB Field;
	addNewProfileToFolder(profileToSave.profileName);
	QList<bool> allReadTries;
	bool ok;

	QString profileName = profileToSave.profileName;
	ok = modifyFieldValue(profileName, Field::PROFILE_NAME, profileName);
	allReadTries.push_back(ok);

	QString profesionToSet = QString::number(profileToSave.profession);
	ok = modifyFieldValue(profileName, Field::PROFESION, profesionToSet);
	allReadTries.push_back(ok);

	QString health_Percentages = DB_writer_ManaAndHealthRestorePercentages(profileToSave.healthRestorePercentages);
	ok = modifyFieldValue(profileName, Field::HEALTH_RESTORE_STRING, health_Percentages);
	allReadTries.push_back(ok);

	QString mana_Percentages = DB_writer_ManaAndHealthRestorePercentages(profileToSave.ManaRestoreMethodesPercentage);
	ok = modifyFieldValue(profileName, Field::MANA_RESTORE_STRING, mana_Percentages);
	allReadTries.push_back(ok);

	QString health_Keys = DB_writer_ManaAndHealthKeys(profileToSave.healthKeys);
	ok = modifyFieldValue(profileName, Field::HEALTH_RESTORE_KEY, health_Keys);
	allReadTries.push_back(ok);

	QString mana_Keys = DB_writer_ManaAndHealthKeys(profileToSave.ManaKeys);
	ok = modifyFieldValue(profileName, Field::MANA_RESTORE_KEY, mana_Keys);
	allReadTries.push_back(ok);

	QString health_MethodesNames = DB_writer_ManaAndHealthRestoreMethhodesNames(profileToSave.healthRestoreMethodeNames);
	ok = modifyFieldValue(profileName, Field::HEALTH_RESTORE_ITEM, health_MethodesNames);
	allReadTries.push_back(ok);

	QString mana_MethodesNames = DB_writer_ManaAndHealthRestoreMethhodesNames(profileToSave.manaRestoreMethodeNames);
	ok = modifyFieldValue(profileName, Field::MANA_RESTORE_ITEM, mana_MethodesNames);
	allReadTries.push_back(ok);

	bool toRet = true;
	for each (bool var in allReadTries){
		if (!var) {
			toRet = false;
			break;
		}
	}
	return toRet;
}

bool ProfileDataBaseManager::readProfileFromDataBase(QString profileName, Profile& profileToBeRead){
	typedef ProfileDataBaseManager::FieldsOfDB Field;
	const QString NAME = profileName;
	profileToBeRead.profileName = profileName;

	QString profesionToSet;
	bool ok2 = readFieldValue(NAME, Field::PROFESION, profesionToSet);
	profileToBeRead.profession = Profile::PROFESSION(profesionToSet.toInt());

	QString health_PercentageStr;
	bool ok3 = readFieldValue(NAME, Field::HEALTH_RESTORE_STRING, health_PercentageStr);
	QList<int> health_Percentages = DB_reader_ManaAndHealthRestorePercentages(health_PercentageStr);
	profileToBeRead.healthRestorePercentages = health_Percentages;

	QString mana_PercentageStr;
	bool ok4 = readFieldValue(NAME, Field::MANA_RESTORE_STRING, mana_PercentageStr);
	QList<int> Mana_Percentages = DB_reader_ManaAndHealthRestorePercentages(mana_PercentageStr);
	profileToBeRead.ManaRestoreMethodesPercentage = Mana_Percentages;

	QString health_keysStr;
	bool ok5 = readFieldValue(NAME, Field::HEALTH_RESTORE_KEY, health_keysStr);
	QList<Key> Health_keys = DB_reader_ManaAndHealthKeys(health_keysStr);
	profileToBeRead.healthKeys = Health_keys;

	QString mana_keysStr;
	bool ok6 = readFieldValue(NAME, Field::MANA_RESTORE_KEY, mana_keysStr);
	QList<Key> Mana_keys = DB_reader_ManaAndHealthKeys(mana_PercentageStr);
	profileToBeRead.ManaKeys = Mana_keys;

	QString health_MethodesStr;
	bool ok7 = readFieldValue(NAME, Field::HEALTH_RESTORE_ITEM, health_MethodesStr);
	QStringList healthMethodes = DB_reader_ManaAndHealthRestoreMethhodesNames(health_MethodesStr);
	profileToBeRead.healthRestoreMethodeNames = healthMethodes;

	QString mana_MethodesStr;
	bool ok8 = readFieldValue(NAME, Field::MANA_RESTORE_ITEM, mana_MethodesStr);
	QStringList mana_Methodes = DB_reader_ManaAndHealthRestoreMethhodesNames(mana_MethodesStr);
	profileToBeRead.manaRestoreMethodeNames = mana_Methodes;

	bool toRet = ok2 && ok3 && ok4 && ok5 && ok6 && ok7 && ok8;
	return toRet;
}








//writers
QString ProfileDataBaseManager::DB_writer_ManaAndHealthRestorePercentages(QList<int> vect) {
	//ERR //DIAG
	int size = vect.size();
	if (size == 0)
		return QString("#");

	QString toRet =QString("#");
	for (int i = 0; i < size; i++)
		toRet.append(QString::number(vect[i]) + "-");

	return toRet;
}

QString ProfileDataBaseManager::DB_writer_ManaAndHealthKeys(QList<Key> keylist) {
	if (keylist.size() == 0)
		return QString("#");

	QString toRet = "#";
	for each (Key key in keylist)
		toRet.append(QString::number(key.number)+"#");

	return toRet;
}

QString ProfileDataBaseManager::DB_writer_ManaAndHealthRestoreMethhodesNames(QStringList restorationMethodesNames) {
	QString toRet = "";
	for each (QString RestorationName in restorationMethodesNames)
		toRet.append(RestorationName +"#");

	return toRet;
}

QString ProfileDataBaseManager::DB_writer_rectangleWithPositionInImg(QRect rect){
	QString toRet = "";
	if (rect.isEmpty())
		return toRet;

	QString start_y = QString::number(rect.y());
	QString start_x = QString::number(rect.x());
	QString width = QString::number(rect.width());
	QString height = QString::number(rect.height());
	QString mark = QString("_");
	toRet = start_x + mark + start_y + mark + width + mark + height;
	return toRet;
}
//readers
QList<int> ProfileDataBaseManager::DB_reader_ManaAndHealthRestorePercentages(QString str) {
	QList<int> vectWithThreshold;

	QStringList list = str.split("#",Qt::SplitBehaviorFlags::SkipEmptyParts);
	if (list.size() != 1)
		return vectWithThreshold;        //DIAG //ERROR
	QStringList thresholds = list[0].split("-",Qt::SplitBehaviorFlags::SkipEmptyParts);

	for (int i = 0; i < thresholds.size(); i++)
		vectWithThreshold.push_back(thresholds[i].toInt());

	return vectWithThreshold;
}

QList<Key> ProfileDataBaseManager::DB_reader_ManaAndHealthKeys(QString str) {
	QList<Key> keyList;
	QString tmp = str.remove(0, 1);
	if(tmp.size() == 0)
		return keyList;
	str = tmp;

	QStringList list = str.split("#", Qt::SplitBehaviorFlags::SkipEmptyParts);
	for (int i = 0; i < list.size(); i++)
		keyList.push_back(Key(list[i].toInt()));

	return keyList;
}

QStringList ProfileDataBaseManager::DB_reader_ManaAndHealthRestoreMethhodesNames(QString str){
	QList<QString> listOfMethodes;

	QStringList list = str.split("#", Qt::SplitBehaviorFlags::SkipEmptyParts);
	for each (QString var in list)
		listOfMethodes.push_back(var);

	return listOfMethodes;
}

QRect ProfileDataBaseManager::DB_reader_rectangleWithPositionInImg(QString str){
	QString mark = QString("_");
	QStringList values = str.split(mark, Qt::SkipEmptyParts);
	QRect toRet;

	if (values.size() == 4) {
		int start_y = values[0].toInt();
		int start_x = values[1].toInt();
		int width = values[2].toInt();
		int height = values[3].toInt();
		toRet = QRect(start_x, start_y, width, height);
	}
	
	return toRet;
}

bool ProfileDataBaseManager::writeRectToDb(QString profileName, FieldsOfDB dbField, QRect rectToSave){
	int nubmerOfFieldInEnum = dbField;
	int stratPositionInEnum = POS_HP;
	int endPositionInEnum = POS_LAST_MINIMAP;
	bool isGoodField = dbField >= stratPositionInEnum && dbField <= endPositionInEnum;
	if (!isGoodField)
		return false;

	QString noImportant;
	bool profExist = getPathToProfileFile(profileName, noImportant);
	bool allRight = (isGoodField) && profExist;
	if (!allRight)
		return false;

	QString newValue = DB_writer_rectangleWithPositionInImg(rectToSave);
	bool res = modifyFieldValue(profileName, dbField, newValue);
	return res;
}

bool ProfileDataBaseManager::readRectFromDb(QString profileName, FieldsOfDB dbField, QRect& rectToRead){
	int nubmerOfFieldInEnum = dbField;
	int stratPositionInEnum = POS_HP;
	int endPositionInEnum = POS_LAST_MINIMAP;
	bool isGoodField = dbField >= stratPositionInEnum && dbField <= endPositionInEnum;

	QString noImportant;
	bool profExist = getPathToProfileFile(profileName, noImportant);
	bool allRight = (isGoodField) && profExist;
	if (!allRight)
		return false;

	QString toBeRead;
	rectToRead = DB_reader_rectangleWithPositionInImg(toBeRead);
	return true;
}
