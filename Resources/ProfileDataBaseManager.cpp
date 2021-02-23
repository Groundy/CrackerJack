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
        Utilities::showMessageBox(windowTitle, textToDisplay);  
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
    QString createCommand = "CREATE TABLE profiles (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, ";
    createCommand.append("profileName TEXT, ");
    createCommand.append("manaString TEXT, ");
    createCommand.append("healthString TEXT ");
    createCommand.append(")");
    query.exec(createCommand);
}

void ProfileDataBaseManager::modifyAtribute(FieldsOfDB atr, QString newValue, int profId) {
    //[DIAG] //[ERR]
    QSqlQuery query;
    QString createCommand = "UPDATE profiles set " + translateDBFields(atr) + " = '" + newValue + "' where ID = " + QString::number(profId);
    query.exec(createCommand);
}

void ProfileDataBaseManager::addEmptyRecord() {
    QSqlQuery query;
    QString createCommand = "INSERT INTO profiles (" + translateDBFields(PROFILE_NAME) + ") VALUES('')";
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
    case ProfileDataBaseManager::PROFESION: return "profesion";
    case ProfileDataBaseManager::MANA_RESTORE_STRING:   return "manaString";
    case ProfileDataBaseManager::HEALTH_RESTORE_STRING:    return "healthString";
    default: return "";
    }
}

int ProfileDataBaseManager::getNumberOfRecords(){
    int toRet;
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM profiles");
    if (query.first())
        toRet = query.value(0).toInt();
    return toRet;
}

QString ProfileDataBaseManager::getValueOfCell(FieldsOfDB atr, int profId) {
   QString dataBaseColumn = translateDBFields(atr);
   QString createCommand = "SELECT " + dataBaseColumn + " FROM profiles WHERE ID = " + QString::number(profId);
   
   QSqlQuery query(createCommand);
   int fieldNo = query.record().indexOf(dataBaseColumn);
   
   QString toRet = "";
   while (query.next())
       toRet = query.value(fieldNo).toString();
   
   return toRet;
}

