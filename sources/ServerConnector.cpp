#include "ServerConnector.h"
ServerConnector::ServerConnector(){
    createMsg(ASK_FOR_NEWEST_VERSION);
    
}

bool ServerConnector::versionToStruct(QString verStr, VersionStruct& structToRet){
    //version format dd-mm-yyyy/t, t is version of a day, start from 0;
    QString versionStr = VER_FILEVERSION_STR;
    QStringList partsOfVerStr = versionStr.split("/", Qt::SkipEmptyParts);

    bool wrongFormatOfVersionStr = partsOfVerStr.size() != 2;
    if (wrongFormatOfVersionStr) {
        //todo dodaæ log
        return false; 
    }
    QStringList partsOfDate = partsOfVerStr[0].split("/", Qt::SkipEmptyParts);
    wrongFormatOfVersionStr = partsOfDate.size() != 3;
    if (wrongFormatOfVersionStr) {
        //todo dodaæ log
        return false;
    }

    structToRet.day = partsOfDate[0].toInt();
    structToRet.month = partsOfDate[1].toInt();
    structToRet.year = partsOfDate[2].toInt();
    structToRet.versionOfDay = partsOfVerStr[1].toInt();
    return true;
}

bool ServerConnector::getMACAdress(QString& MAC_Adress){
    QList<QNetworkInterface> netInterfaces = QNetworkInterface::allInterfaces();
    const auto CORRECT_FLAG = QNetworkInterface::IsLoopBack;
    for each (QNetworkInterface inte in netInterfaces) {
        // Return only the first non-loopback MAC Address
        auto currentFlag = inte.flags();
        bool correctMacAdress = !(currentFlag & CORRECT_FLAG);
        if (correctMacAdress) {
            MAC_Adress = inte.hardwareAddress();
            return true;
        }
    }
    return false;
}

bool ServerConnector::isCurrentVersion(const QString versionStrRecivedFromServer){
    VersionStruct currentVersion, serverVersion;
    bool okCurrent = versionToStruct(VER_FILEVERSION_STR, currentVersion);
    bool okServer = versionToStruct(versionStrRecivedFromServer, serverVersion);

    bool error = !(okCurrent || okServer);
    if (error) {
        //log
        return false;
    }

    bool dayOk = currentVersion.day == serverVersion.day;
    bool monthOk = currentVersion.month == serverVersion.month;
    bool yearOk = currentVersion.year == serverVersion.year;
    bool versionOfDayOk = currentVersion.versionOfDay == serverVersion.versionOfDay;

    bool isSameVersion = dayOk && monthOk && yearOk && versionOfDayOk;
    return isSameVersion;
}

QByteArray ServerConnector::getStrOfEnum_FieldsOfMsg(FIELDS_OF_MSGS field){
    QString toRet;
    switch (field)
    {
    case ServerConnector::IP_ADRESS:    toRet = "IP_ADRESS";   break;
    case ServerConnector::MAC_ADRESS:    toRet = "MAC_ADRESS";   break;
    case ServerConnector::REASON:    toRet = "REASON";   break;
    case ServerConnector::USER_ID:    toRet = "USER_ID";   break;
    default:    toRet = "UNKOWN";   break;
    }
    return toRet.toUtf8();
}

QByteArray ServerConnector::getStrOfEnum_REASON_TO_CONNECT(REASON_TO_CONNECT reason){
    QString toRet;
    switch (reason)
    {
    case ServerConnector::ASK_FOR_NEWEST_VERSION:   toRet = "GetVersion";  break;
    case ServerConnector::ASK_FOR_VALIDITY_OF_KEY:   toRet = "checkValidity";  break;
    default:   toRet = "";  break;
    }
    return toRet.toUtf8();
}

QByteArray ServerConnector::getMarkUp(FIELDS_OF_MSGS field, QByteArray markUpValue){
    QString nameOfField = getStrOfEnum_FieldsOfMsg(field);
    QString arrAsStr = QString("<") + nameOfField + QString("===") + markUpValue + QString(">") + QString("\n");
    QByteArray toRet = arrAsStr.toUtf8();
    return toRet;
}

QByteArray ServerConnector::createMsg(REASON_TO_CONNECT reson){
    Cryptography crpyto;
    int user_id;
    bool ok = crpyto.getUserIdFromFile(user_id);
    if (!ok)
        return false; //todo
    QByteArray header = getMarkUp(FIELDS_OF_MSGS::USER_ID, QByteArray::number(user_id));

    QByteArray toEncrypt, toAdd;
    QByteArray reasonTmp = getStrOfEnum_REASON_TO_CONNECT(reson);
    toAdd = getMarkUp(FIELDS_OF_MSGS::REASON, reasonTmp);
    toEncrypt.append(toAdd);

    QString macAdress;
    bool ok2 = getMACAdress(macAdress);
    if (!ok2)
        return false;//todo //tmp 

    toAdd = getMarkUp(FIELDS_OF_MSGS::MAC_ADRESS, macAdress.toUtf8());
    toEncrypt.append(toAdd);
    
    QByteArray encrypted;
    ok = crpyto.encryptUsingUserPublicKey(toEncrypt, encrypted);
    if (!ok)
        return false; //todo

    QByteArray toRet = header + encrypted; //todo
    return toRet;
}

ServerConnector::~ServerConnector()
{
}

bool ServerConnector::conectToServer(QByteArray in_dataToSend, QByteArray& out_recivedData){
    socket = new QTcpSocket(this);
    QHostAddress hostNAme(ip);
    socket->connectToHost(hostNAme, port);
    bool waited = socket->waitForConnected(timeWaitForConn);
    if (waited) {
        qDebug() << "Connected";
        socket->write(in_dataToSend);
        socket->waitForBytesWritten(timeWaitForDataWrite);
        socket->waitForReadyRead(timeWaitForDataRead);
        int byteRead = socket->bytesAvailable();
        qDebug() << "Reading bytes::" + QString::number(byteRead);
        out_recivedData = socket->readAll();
        qDebug() << out_recivedData;
        socket->close();
    }
    else
        qDebug() << " Not connected";
    return true;
}
