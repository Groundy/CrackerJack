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
	toRet.insert(ProfileDataBaseManager::PROFILE_NAME, "PROFILE_NAME");
	toRet.insert(ProfileDataBaseManager::PROFESION, "PROFESION");
	toRet.insert(ProfileDataBaseManager::MANA_RESTORE_STRING, "MANA_RESTORE_STRING");
	toRet.insert(ProfileDataBaseManager::HEALTH_RESTORE_STRING, "HEALTH_RESTORE_STRING");
	toRet.insert(ProfileDataBaseManager::MANA_RESTORE_KEY, "MANA_RESTORE_KEY");
	toRet.insert(ProfileDataBaseManager::HEALTH_RESTORE_KEY, "HEALTH_RESTORE_KEY");
	toRet.insert(ProfileDataBaseManager::MANA_RESTORE_ITEM, "MANA_RESTORE_ITEM");
	toRet.insert(ProfileDataBaseManager::HEALTH_RESTORE_ITEM, "HEALTH_RESTORE_ITEM");
	toRet.insert(ProfileDataBaseManager::LAST_LOGIN, "LAST_LOGIN");
	toRet.insert(ProfileDataBaseManager::CREATION_TIME, "CREATION_TIME");
	toRet.insert(ProfileDataBaseManager::SECONDS_SPENT, "SECONDS_SPENT");

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
	//if (!fileExist)
	//	return false;//TODO zastanowic sie czy powinno sie stworzyc pusty plik i go modyfikowac czy tylko zwrocic return
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
	QStringList namesOfAllFiles = getAllNamesOfProfFiles();
	for each (QString var in namesOfAllFiles){
		bool fileAlreadyExist = var == nameOfFile;
		if (fileAlreadyExist)
			return true;

	}

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

	QString mainGameFrameStr = DB_writer_rectangleWithPositionInImg(profileToSave.frames.gameFrame);
	ok = modifyFieldValue(profileName, Field::POS_LAST_GAME_FRAME, mainGameFrameStr);
	allReadTries.push_back(ok);


	QString miniMapFrameStr = DB_writer_rectangleWithPositionInImg(profileToSave.frames.gameFrame);
	ok = modifyFieldValue(profileName, Field::POS_LAST_MINIMAP, miniMapFrameStr);
	allReadTries.push_back(ok);


	QString healthBarStr = DB_writer_rectangleWithPositionInImg(profileToSave.frames.gameFrame);
	ok = modifyFieldValue(profileName, Field::POS_LAST_HEALTH_BAR, healthBarStr);
	allReadTries.push_back(ok);


	QString manaBarStr = DB_writer_rectangleWithPositionInImg(profileToSave.frames.gameFrame);
	ok = modifyFieldValue(profileName, Field::POS_LAST_MANA_BAR, manaBarStr);
	allReadTries.push_back(ok);


	QString comboBoxBarStr = DB_writer_rectangleWithPositionInImg(profileToSave.frames.gameFrame);
	ok = modifyFieldValue(profileName, Field::POS_LAST_COMBOBOX_BAR, comboBoxBarStr);
	allReadTries.push_back(ok);


	QString manaShieldBarStr = DB_writer_rectangleWithPositionInImg(profileToSave.frames.gameFrame);
	ok = modifyFieldValue(profileName, Field::POS_LAST_MANA_SHIELD_BAR, manaShieldBarStr);
	allReadTries.push_back(ok);


	QString RotationStr;
	int nr = profileToSave.frames.howTheyShouldBeRotated;
	bool notDeifned = nr < -1 || nr > 1;
	RotationStr = notDeifned ? QString::number(0) : QString::number(nr);
	ok = modifyFieldValue(profileName, Field::ROTATION, RotationStr);
	allReadTries.push_back(ok);
	
	if (profileToSave.lastLoginSeconds > 0) {
		LONG64 currentTime = (Utilities::getCurrentTimeInMiliSeconds() / 1000);
		LONG64 secondsSpentOdThisSession = currentTime - profileToSave.lastLoginSeconds;
		LONG64 allSecondsSpentOnThisProf = profileToSave.secondsSpentSeconds + secondsSpentOdThisSession;
		QString secondsSpendStr = QString::number(allSecondsSpentOnThisProf);
		ok = modifyFieldValue(profileName, Field::SECONDS_SPENT, secondsSpendStr);
		allReadTries.push_back(ok);
	}

	if (profileToSave.creationTimeSeconds < 1) {
		LONG64 currentTime = Utilities::getCurrentTimeInMiliSeconds() / 1000;
		QString creationTimeStrToSet = QString::number(currentTime);
		ok = modifyFieldValue(profileName, Field::CREATION_TIME, creationTimeStrToSet);
		allReadTries.push_back(ok);
	}

	//Last login time is saved during reading profile from file

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
	bool ok;
	QList<bool> allRight;

	QString profesionToSet;
	ok = readFieldValue(NAME, Field::PROFESION, profesionToSet);
	profileToBeRead.profession = Profile::PROFESSION(profesionToSet.toInt());
	allRight.push_back(ok);

	QString health_PercentageStr;
	ok = readFieldValue(NAME, Field::HEALTH_RESTORE_STRING, health_PercentageStr);
	QList<int> health_Percentages = DB_reader_ManaAndHealthRestorePercentages(health_PercentageStr);
	profileToBeRead.healthRestorePercentages = health_Percentages;
	allRight.push_back(ok);

	QString mana_PercentageStr;
	ok = readFieldValue(NAME, Field::MANA_RESTORE_STRING, mana_PercentageStr);
	QList<int> Mana_Percentages = DB_reader_ManaAndHealthRestorePercentages(mana_PercentageStr);
	profileToBeRead.ManaRestoreMethodesPercentage = Mana_Percentages;
	allRight.push_back(ok);

	QString health_keysStr;
	ok = readFieldValue(NAME, Field::HEALTH_RESTORE_KEY, health_keysStr);
	QList<Key> Health_keys = DB_reader_ManaAndHealthKeys(health_keysStr);
	profileToBeRead.healthKeys = Health_keys;
	allRight.push_back(ok);

	QString mana_keysStr;
	ok = readFieldValue(NAME, Field::MANA_RESTORE_KEY, mana_keysStr);
	QList<Key> Mana_keys = DB_reader_ManaAndHealthKeys(mana_keysStr);
	profileToBeRead.ManaKeys = Mana_keys;
	allRight.push_back(ok);

	QString health_MethodesStr;
	ok = readFieldValue(NAME, Field::HEALTH_RESTORE_ITEM, health_MethodesStr);
	QStringList healthMethodes = DB_reader_ManaAndHealthRestoreMethhodesNames(health_MethodesStr);
	profileToBeRead.healthRestoreMethodeNames = healthMethodes;
	allRight.push_back(ok);

	QString mana_MethodesStr;
	ok = readFieldValue(NAME, Field::MANA_RESTORE_ITEM, mana_MethodesStr);
	QStringList mana_Methodes = DB_reader_ManaAndHealthRestoreMethhodesNames(mana_MethodesStr);
	profileToBeRead.manaRestoreMethodeNames = mana_Methodes;
	allRight.push_back(ok);

	QString mainGameFrameStr, miniMapFrameStr, healthBarStr, manaBarStr, comboBoxBarStr, manaShieldBarStr, RotationStr;
	QRect frame;

	ok = readFieldValue(NAME, Field::POS_LAST_GAME_FRAME, mainGameFrameStr);
	allRight.push_back(ok);
	frame = DB_reader_rectangleWithPositionInImg(mainGameFrameStr);
	profileToBeRead.frames.gameFrame = frame;

	ok = readFieldValue(NAME, Field::POS_LAST_MINIMAP, miniMapFrameStr);
	allRight.push_back(ok);
	frame = DB_reader_rectangleWithPositionInImg(miniMapFrameStr);
	profileToBeRead.frames.miniMapFrame = frame;

	ok = readFieldValue(NAME, Field::POS_LAST_HEALTH_BAR, healthBarStr);
	allRight.push_back(ok); 
	frame = DB_reader_rectangleWithPositionInImg(healthBarStr);
	profileToBeRead.frames.healthFrame = frame;

	ok = readFieldValue(NAME, Field::POS_LAST_MANA_BAR, manaBarStr);
	allRight.push_back(ok); 
	frame = DB_reader_rectangleWithPositionInImg(manaBarStr);
	profileToBeRead.frames.manaFrame = frame;

	ok = readFieldValue(NAME, Field::POS_LAST_COMBOBOX_BAR, comboBoxBarStr);
	allRight.push_back(ok); 
	frame = DB_reader_rectangleWithPositionInImg(comboBoxBarStr);
	profileToBeRead.frames.combinedFrame = frame;

	ok = readFieldValue(NAME, Field::POS_LAST_MANA_SHIELD_BAR, manaShieldBarStr);
	allRight.push_back(ok); 
	frame = DB_reader_rectangleWithPositionInImg(manaShieldBarStr);
	profileToBeRead.frames.manaShieldFrame = frame;

	ok = readFieldValue(NAME, Field::ROTATION, RotationStr);
	allRight.push_back(ok);
	profileToBeRead.frames.howTheyShouldBeRotated = RotationStr.toInt();

	QString secondsSpendStr;
	ok = readFieldValue(NAME, Field::SECONDS_SPENT, secondsSpendStr);
	allRight.push_back(ok);
	profileToBeRead.secondsSpentSeconds = secondsSpendStr.toLongLong();

	QString creationTimeStrToSet;
	ok = readFieldValue(profileName, Field::CREATION_TIME, creationTimeStrToSet);
	allRight.push_back(ok);
	profileToBeRead.creationTimeSeconds = creationTimeStrToSet.toLongLong();

	QString lasLoginTimeStrToSet;
	ok = readFieldValue(profileName, Field::LAST_LOGIN, lasLoginTimeStrToSet);
	allRight.push_back(ok);
	profileToBeRead.lastLoginSeconds = lasLoginTimeStrToSet.toLongLong();

	for each (bool var in allRight) {
		if (!var) {
			return false;
		}
	}
	return true;
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
