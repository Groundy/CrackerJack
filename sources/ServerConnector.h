#pragma once
#include <QObject>
#include "qnetworkinterface.h"
#include "version.h"
#include "qtcpsocket.h"
#include "Cryptography.h"
class ServerConnector : public QObject
{
	Q_OBJECT

public:
	enum REASON_TO_CONNECT { ASK_FOR_NEWEST_VERSION, ASK_FOR_VALIDITY_OF_KEY };
	enum FIELDS_OF_MSGS {IP_ADRESS, MAC_ADRESS, REASON, USER_ID};
	ServerConnector();
	~ServerConnector();

	bool conectToServer(QByteArray in_dataToSend, QByteArray& out_recivedData);
private:
	struct VersionStruct {
		int day, month, year, versionOfDay;
	};

	const uint port = 4002;
	const QString ip = "192.168.1.14";
	const int timeWaitForConn = 3000;
	const int timeWaitForDataWrite = 1000;
	const int timeWaitForDataRead = 3000;
	QTcpSocket* socket;

	bool versionToStruct(QString verStr, VersionStruct& structToRet);
	bool getMACAdress(QString& MAC_Adress);
	bool isCurrentVersion(const QString versionStrRecivedFromServer);
	QByteArray getStrOfEnum_FieldsOfMsg(FIELDS_OF_MSGS field);
	QByteArray getStrOfEnum_REASON_TO_CONNECT(REASON_TO_CONNECT reason);
	QByteArray getMarkUp(FIELDS_OF_MSGS markUpField, QByteArray markUpValue);
	QByteArray createMsg(REASON_TO_CONNECT reson);

};
