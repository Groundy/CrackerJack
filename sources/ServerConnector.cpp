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

QByteArray ServerConnector::getMarkUp(FIELDS_OF_MSGS field, QByteArray markUpValue){
	QString nameOfField = QString::fromStdString(getFieldStr(field));
	QString arrAsStr = QString("<") + nameOfField + QString("===") + markUpValue + QString(">") + QString("\n");
	QByteArray toRet = arrAsStr.toUtf8();
	return toRet;
}

bool ServerConnector::getUserInternetData(QByteArray& userDataMarkUps, REASON_TO_CONNECT_TO_SERVER reason){
	QByteArray toRet;

	QByteArray reasonTmp = QByteArray::fromStdString(getConnectionReasonStr(reason));
	QByteArray toAdd = getMarkUp(FIELDS_OF_MSGS::REASON, reasonTmp);
	toRet.append(toAdd);

	QByteArray macAdress;
	bool ok2 = getMACAdress(macAdress);
	if (!ok2)
		return false;//todo //tmp 

	toAdd = getMarkUp(FIELDS_OF_MSGS::MAC_ADRESS, macAdress);
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
	QByteArray header = getMarkUp(FIELDS_OF_MSGS::USER_ID, QByteArray::number(user_ID));
	msg.push_front(header);
	return ok1 && ok2;
}

void ServerConnector::test(){
}

ServerConnector::~ServerConnector()
{
}

bool ServerConnector::conectToServer(QByteArray in_dataToSend, QByteArray& out_recivedData){
	qDebug() << "Bytes to send: "+ QString::number(in_dataToSend.size());
	socket = new QTcpSocket(this);
	QHostAddress hostNAme(ip);
	socket->connectToHost(hostNAme, port);
	bool waited = socket->waitForConnected(timeWaitForConn);
	if (waited) {
		qDebug() << "Connected";
		socket->write(in_dataToSend);
		socket->waitForBytesWritten(timeWaitForDataWrite);
		socket->waitForReadyRead(timeWaitForDataRead);
		//int byteRead = socket->bytesAvailable();
		out_recivedData = socket->readAll();
		qDebug() << "Recived bytes:" + QString::number(out_recivedData.size());
		qDebug() << out_recivedData;
		socket->close();
	}
	else
		qDebug() << " Not connected";
	return true;
}
