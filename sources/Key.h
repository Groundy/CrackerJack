#pragma once
#include <cstddef>
#include <qstring.h>
#include "qsplitter.h"
class Key {
private:
	int number;
	bool control;
	bool fkey;
	char letter;

public:
	enum type { FKEY, CONTROL, LETTER };
	Key::Key(type type, int numberArg, bool controlArg, bool fkeyArg, char letterArg);
	QString toString();
};

