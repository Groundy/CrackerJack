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
    QString createCommand = "CREATE TABLE profiles ( ";

    createCommand.append("profileName TEXT, ");
    createCommand.append("profession INTEGER, ");
    createCommand.append("manaString TEXT, ");
    createCommand.append("healthKeys TEXT, ");
    createCommand.append("manaKeys TEXT, ");
    createCommand.append("healthString TEXT, ");
    createCommand.append("ManaItems TEXT, ");
    createCommand.append("HealthItems TEXT, ");
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
    //diag /err
    QSqlQuery query;
    QString createCommand = "INSERT INTO profiles (" + translateDBFields(PROFILE_NAME) + ") VALUES('"+profileName+"')";
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
    case ProfileDataBaseManager::PROFILE_NAME:  return "profileName";
    case ProfileDataBaseManager::PROFESION: return "profession";
    case ProfileDataBaseManager::MANA_RESTORE_STRING:   return "manaString";
    case ProfileDataBaseManager::HEALTH_RESTORE_STRING:    return "healthString";
    case ProfileDataBaseManager::MANA_RESTORE_KEY:   return "manaKeys";
    case ProfileDataBaseManager::HEALTH_RESTORE_KEY:    return "healthKeys";
    case ProfileDataBaseManager::MANA_RESTORE_ITEM:    return "ManaItems";
    case ProfileDataBaseManager::HEALTH_RESTORE_ITEM:    return "HealthItems";
    case ProfileDataBaseManager::LAST_LOGIN: return "lastLogin";
    case ProfileDataBaseManager::CREATION_TIME: return "creationTime";
    default: return "";
    }
}

int ProfileDataBaseManager::getNumberOfRecords(){
    //diag //err
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
    modifyAtribute(name, ProfileDataBaseManager::MANA_RESTORE_STRING, DB_writer_ManaAndHealthRestorePercentages(prof->ManaRestoreMethodesPercentage));
    modifyAtribute(name, ProfileDataBaseManager::HEALTH_RESTORE_STRING, DB_writer_ManaAndHealthRestorePercentages(prof->healthRestorePercentages));
    modifyAtribute(name, ProfileDataBaseManager::MANA_RESTORE_KEY, DB_writer_ManaAndHealthKeys(prof->ManaKeys));
    modifyAtribute(name, ProfileDataBaseManager::HEALTH_RESTORE_KEY, DB_writer_ManaAndHealthKeys(prof->healthKeys));
    modifyAtribute(name, ProfileDataBaseManager::HEALTH_RESTORE_ITEM, DB_writer_ManaAndHealthRestoreMethhodesNames(prof->healthRestoreMethodeNames));
    modifyAtribute(name, ProfileDataBaseManager::MANA_RESTORE_ITEM, DB_writer_ManaAndHealthRestoreMethhodesNames(prof->manaRestoreMethodeNames));
    if (prof->creationDate.isEmpty()) {
        QString creation = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm");
        modifyAtribute(name, ProfileDataBaseManager::CREATION_TIME, creation);
    }
}

void ProfileDataBaseManager::readProfileFroDataBase(Profile* prof, QString name) {
    prof->profileName = getValueOfCell(FieldsOfDB::PROFILE_NAME, name);
    QString profession = getValueOfCell(FieldsOfDB::PROFESION, name);
    prof->profession = Profile::PROFESSION(profession.toInt());
    QString healthPercentages = getValueOfCell(FieldsOfDB::HEALTH_RESTORE_STRING, name);
    prof->healthRestorePercentages = DB_reader_ManaAndHealthRestorePercentages(healthPercentages);
    QString manaPercentage = getValueOfCell(FieldsOfDB::MANA_RESTORE_STRING, name);
    prof->ManaRestoreMethodesPercentage = DB_reader_ManaAndHealthRestorePercentages(manaPercentage);
    QString healthKeys = getValueOfCell(FieldsOfDB::HEALTH_RESTORE_KEY, name);
    prof->healthKeys = DB_reader_ManaAndHealthKeys(healthKeys);
    QString manaKeys = getValueOfCell(FieldsOfDB::MANA_RESTORE_KEY, name);
    prof->ManaKeys = DB_reader_ManaAndHealthKeys(manaKeys);
    QString healthRestoreNames = getValueOfCell(FieldsOfDB::HEALTH_RESTORE_ITEM, name);
    prof->healthRestoreMethodeNames = DB_reader_ManaAndHealthRestoreMethhodesNames(healthRestoreNames);
    QString manaRestoreNames = getValueOfCell(FieldsOfDB::MANA_RESTORE_ITEM, name);
    prof->manaRestoreMethodeNames = DB_reader_ManaAndHealthRestoreMethhodesNames(manaRestoreNames);
    prof->creationDate = getValueOfCell(FieldsOfDB::CREATION_TIME, name);
}

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

QString ProfileDataBaseManager::DB_writer_ManaAndHealthKeys(QList<Key> keylist) {
    if (keylist.size() == 0)
        return QString("#");

    QString toRet = "#";
    for each (Key key in keylist)
        toRet.append(QString::number(key.number)+"#");

    return toRet;
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

QString ProfileDataBaseManager::DB_writer_ManaAndHealthRestoreMethhodesNames(QList<QString> restorationMethodesNames) {
    QString toRet = "";
    for each (QString RestorationName in restorationMethodesNames)
        toRet.append(RestorationName +"#");

    return toRet;
}

QList<QString> ProfileDataBaseManager::DB_reader_ManaAndHealthRestoreMethhodesNames(QString str){
    QList<QString> listOfMethodes;

    QStringList list = str.split("#", Qt::SplitBehaviorFlags::SkipEmptyParts);
    for each (QString var in list)
        listOfMethodes.push_back(var);

    return listOfMethodes;
}
