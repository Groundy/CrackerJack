#pragma once
#include <QObject>
#include "qnetworkinterface.h"
#include "version.h"
#include "qtcpsocket.h"
#include "Cryptography.h"
#include "ClientServerApi.h"

class ServerConnector : public QObject
{

	Q_OBJECT
public:
	ServerConnector();
	~ServerConnector();

	bool conectToServer(QByteArray in_dataToSend, QByteArray& out_recivedData);
	struct VersionStruct {
		int day, month, year, versionOfDay;
	};
private:

	const uint port = 4002;
	const QString ip = "127.0.0.1";// "192.168.1.14";
	const int timeWaitForConn = 3000;
	const int timeWaitForDataWrite = 1000;
	const int timeWaitForDataRead = 3000;
	QTcpSocket* socket;

	bool versionToStruct(QString verStr, VersionStruct& structToRet);
	bool getMACAdress(QByteArray& MAC_Adress);
	bool isCurrentVersion(const QString versionStrRecivedFromServer);
	bool getUserInternetData(QByteArray& userDataMarkUps, REASON_TO_CONNECT_TO_SERVER reason);
	QByteArray createMsg(REASON_TO_CONNECT_TO_SERVER reason);

	bool encryptAndAddHeaderToMsg(QByteArray& msg);
	void test();

};
