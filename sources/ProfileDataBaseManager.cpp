#include "ProfileDataBaseManager.h"

ProfileDataBaseManager::ProfileDataBaseManager(){
    getPathToDB();
    openDB();
}

ProfileDataBaseManager::~ProfileDataBaseManager()
{
    if (dataBase.isOpen())
        dataBase.close();
}

void ProfileDataBaseManager::openDB() {
    //opens a DB from path, if not DB found user is informed that new DB will be created
    dataBase = QSqlDatabase::addDatabase("QSQLITE");
    dataBase.setDatabaseName(DATABASE_NAME);
    bool DBExist = checkIfDbExist();
    if (!DBExist) {
        QString textToDisplay = "Database with profiles not found in " + pathToDB.absolutePath();
        textToDisplay.append("\nNew and empty database will be create.");
        QString windowTitle = "Problem occured!";
        Utilities::showMessageBox(windowTitle, textToDisplay,QMessageBox::Ok);  
        dataBase.open(); 
        makeDataBase(); 
    }
    else
        dataBase.open();
    //[DIAG]
    //[ERR]
}

void ProfileDataBaseManager::makeDataBase(){
    QSqlQuery query;
    QString createCommand = "CREATE TABLE profiles (id INTEGER NOT NULL, ";
    createCommand.append("profileName TEXT, ");
    createCommand.append("profession INTEGER, ");
    createCommand.append("manaString TEXT, ");
    createCommand.append("healthKeys TEXT, ");
    createCommand.append("manaKeys TEXT, ");
    createCommand.append("healthString TEXT, ");
    createCommand.append("lastLogin TEXT, ");
    createCommand.append("creationTime TEXT ");

    createCommand.append(")");
    query.exec(createCommand);
}

void ProfileDataBaseManager::modifyAtribute(QString profileName,FieldsOfDB atr, QString newValue) {
    //[DIAG] //[ERR]
    QSqlQuery query;
    QString createCommand = "UPDATE profiles set " + translateDBFields(atr) + " = '" + newValue + "' where profileName = '" + profileName + "'";
    query.exec(createCommand);
}

void ProfileDataBaseManager::addRecord(QString profileName) {
    QSqlQuery query;
    QString createCommand = "INSERT INTO profiles (" + translateDBFields(PROFILE_NAME) + ",ID) VALUES('"+profileName+"',0)";
    query.exec(createCommand);
}

void ProfileDataBaseManager::getPathToDB() {
    //[TMP]
    pathToDB = QDir::current();
}

bool ProfileDataBaseManager::checkIfDbExist() {
    QString fullPath = pathToDB.absolutePath() + "\\" + DATABASE_NAME;
    QFileInfo info(fullPath);
    if (info.exists() && info.isFile())
        return true;
    else
        return false;
}

QString ProfileDataBaseManager::translateDBFields(FieldsOfDB atr){
    //works as resources of strings with db Fields
    switch (atr)
    {
    case ProfileDataBaseManager::ID:    return "ID";
    case ProfileDataBaseManager::PROFILE_NAME:  return "profileName";
    case ProfileDataBaseManager::PROFESION: return "profession";
    case ProfileDataBaseManager::MANA_RESTORE_STRING:   return "manaString";
    case ProfileDataBaseManager::HEALTH_RESTORE_STRING:    return "healthString";
    case ProfileDataBaseManager::MANA_RESTORE_KEY:   return "manaKeys";
    case ProfileDataBaseManager::HEALTH_RESTORE_KEY:    return "healthKeys";
    case ProfileDataBaseManager::LAST_LOGIN: return "lastLogin";
    case ProfileDataBaseManager::CREATION_TIME: return "creationTime";
    default: return "";
    }
}

int ProfileDataBaseManager::getNumberOfRecords(){
    int toRet = 0;
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM profiles");
    if (query.first())
        toRet = query.value(0).toInt();
    return toRet;
}

QString ProfileDataBaseManager::getValueOfCell(FieldsOfDB atr, QString profileName) {
   QString dataBaseColumn = translateDBFields(atr);
   QString createCommand = "SELECT " + dataBaseColumn + " FROM profiles WHERE profileName = " + "'" + profileName + "'";
   
   QSqlQuery query(createCommand);
   int fieldNo = query.record().indexOf(dataBaseColumn);
   
   QString toRet = "";
   while (query.next())
       toRet = query.value(fieldNo).toString();
   
   return toRet;
}

void ProfileDataBaseManager::deleteRecord(QString name) {
    QSqlQuery query;
    QString queryCommand = "DELETE FROM profiles WHERE profileName = '" + name + "'";
    query.exec(queryCommand);
}

QStringList ProfileDataBaseManager::getListOfAllRecords() {
	QStringList list;
    QString cmd = "SELECT profileName FROM profiles";
    QSqlQuery query(cmd);
    int i = 0;
    while (query.next()) {
        i++;
        QString profName = query.value(0).toString();
        QString nameOfItem = "[" + QString::number(i) + "] " + profName;
        list.push_back(nameOfItem);
    }
    return list;
}

void ProfileDataBaseManager::saveProfileToDatabase(Profile* prof){
    if (prof == nullptr)
        ;//todo
    QString name = prof->profileName;
    addRecord(name);
    modifyAtribute(name, ProfileDataBaseManager::PROFESION, QString::number(prof->profession));
    modifyAtribute(name, ProfileDataBaseManager::MANA_RESTORE_STRING, Profile::VectorToString(prof->ManaRestoreMethodesPercentage));
    modifyAtribute(name, ProfileDataBaseManager::HEALTH_RESTORE_STRING, Profile::VectorToString(prof->healthRestorePercentages));
    modifyAtribute(name, ProfileDataBaseManager::MANA_RESTORE_KEY, Profile::KeyListToString(prof->ManaKeys));
    modifyAtribute(name, ProfileDataBaseManager::HEALTH_RESTORE_KEY, Profile::KeyListToString(prof->healthKeys));
}
