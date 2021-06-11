#pragma once

#include <QObject>
#include "qnetworkinterface.h"
#include "version.h"
class ServerConnector : public QObject
{
	Q_OBJECT

public:
	enum REASON_TO_CONNECT { ASK_FOR_VERSION, ASK_FOR_TIME_STAMP };
	struct VersionStruct {
		int day, month, year, versionOfDay;
	};
	ServerConnector();
	~ServerConnector();
private:
	bool versionToStruct(QString verStr, VersionStruct& structToRet);
	QString getMACAdress();
	bool isCurrentVersion(const QString versionStrRecivedFromServer);
};
