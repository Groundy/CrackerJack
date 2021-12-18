#pragma once
#include <cstddef>
#include <qstring.h>
#include "qsplitter.h"
#include "qmap.h"
#include "Logger.h"
class Key {
public:
	static QMap<QString, int> KeysAndCodesMap;
	static QStringList getListOfAllPossibleKeys();
	int number;

	Key();
	Key(int codeOfKey);
	Key(QString keyCodeAsStr);
};

