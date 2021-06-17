#include "ServerConnector.h"
ServerConnector::ServerConnector(){
	test();
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

bool ServerConnector::getMACAdress(QByteArray& MAC_Adress){
	QList<QNetworkInterface> netInterfaces = QNetworkInterface::allInterfaces();
	const auto CORRECT_FLAG = QNetworkInterface::IsLoopBack;
	for each (QNetworkInterface inte in netInterfaces) {
		// Return only the first non-loopback MAC Address
		auto currentFlag = inte.flags();
		bool correctMacAdress = !(currentFlag & CORRECT_FLAG);
		if (correctMacAdress) {
			MAC_Adress = inte.hardwareAddress().toUtf8();
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

bool ServerConnector::getUserInternetData(QByteArray& userDataMarkUps, REASON_TO_CONNECT_TO_SERVER reason){
	QByteArray toRet;

	QByteArray reasonTmp = ClientServerApi::reasonMap[reason];
	QByteArray toAdd = ClientServerApi::getMarkUp(FIELDS_OF_MSGS::REASON, reasonTmp);
	toRet.append(toAdd);

	QByteArray macAdress;
	bool ok2 = getMACAdress(macAdress);
	if (!ok2)
		return false;//todo //tmp 

	toAdd = ClientServerApi::getMarkUp(FIELDS_OF_MSGS::MAC_ADRESS, macAdress);
	toRet.append(toAdd);

	userDataMarkUps = toRet;
	return true;
}

QByteArray ServerConnector::createMsg(REASON_TO_CONNECT_TO_SERVER reason){
	QByteArray toRet;
	bool ok = getUserInternetData(toRet, reason);
	if (!ok);//todo info

	return toRet;
}

bool ServerConnector::encryptAndAddHeaderToMsg(QByteArray& msg){
	Cryptography crypto;
	bool ok1 = crypto.encryptUsingUserPublicKey(msg);
	int user_ID;
	bool ok2 = crypto.getUserIdFromFile(user_ID);
	QByteArray header = ClientServerApi::getMarkUp(FIELDS_OF_MSGS::USER_ID, QByteArray::number(user_ID));
	msg.push_front(header);
	return ok1 && ok2;
}

void ServerConnector::test(){
	QByteArray msg = createMsg(REASON_TO_CONNECT_TO_SERVER::ASK_FOR_NEWEST_VERSION);
	encryptAndAddHeaderToMsg(msg);
	QByteArray out;
	conectToServer(msg, out);
	int g = 5;
}

ServerConnector::~ServerConnector()
{
}

bool ServerConnector::conectToServer(QByteArray encryptedDataToSend, QByteArray& encryptedRecivedData){
	socket = new QTcpSocket(this);
	QHostAddress hostNAme(ip);
	socket->connectToHost(hostNAme, port);
	bool waited = socket->waitForConnected(timeWaitForConn);
	if (waited) {
		qDebug() << "Connected";
		socket->write(encryptedDataToSend);
		socket->waitForBytesWritten(timeWaitForDataWrite);
		socket->waitForReadyRead(timeWaitForDataRead);
		encryptedRecivedData = socket->readAll();
		socket->close();
		qDebug() << "Recived bytes:" + QString::number(encryptedRecivedData.size());
		qDebug() << encryptedRecivedData;
		return true;
	}
	else {
		qDebug() << " Not connected";
		return false;
	}
}
