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
	Key(int codeOfKey);
	Key(QString keyCodeAsStr);
	Key(QJsonObject obj);

	int getKeyVal() { return keyVal; };
	static QMap<QString, int> KeysAndCodesMap;
	static QStringList getListOfAllPossibleKeys();
	QJsonObject toJson();
private:
	int keyVal;
	QString keyName;
};

