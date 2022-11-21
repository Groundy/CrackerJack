#pragma once
#include <cstddef>
#include <qstring.h>
#include "qsplitter.h"
#include "qmap.h"
#include "Logger.h"
#include <qjsonobject.h>
class Key {
public:
	Key();
	Key(int keyVal);
	Key(QString keyName);
	Key(QJsonObject obj);

	int getKeyVal() { return keyVal; };
	QString getKeyName(){ return keyName; };
	static QMap<QString, int> KeysAndCodesMap;
	static QStringList getListOfAllPossibleKeys();
	QJsonObject toJson();
private:
	int keyVal;
	QString keyName;
};

