#include "ServerConnector.h"
ServerConnector::ServerConnector(){
    getMACAdress();
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

QString ServerConnector::getMACAdress(){
    QString macAdressToRet;
    QList<QNetworkInterface> netInterfaces = QNetworkInterface::allInterfaces();
    const auto CORRECT_FLAG = QNetworkInterface::IsLoopBack;
    for each (QNetworkInterface inte in netInterfaces) {
        // Return only the first non-loopback MAC Address
        auto currentFlag = inte.flags();
        bool correctMacAdress = !(currentFlag & CORRECT_FLAG);
        if (correctMacAdress)
            macAdressToRet = inte.hardwareAddress();
    }
    return macAdressToRet;
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

ServerConnector::~ServerConnector()
{
}
