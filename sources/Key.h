#pragma once
#include <cstddef>
#include <qstring.h>
#include "qsplitter.h"

enum TYPE_OF_KEY { NORMAL, SHIFT, FKEY, CONTROL, ALT };

class Key {
public:
	QChar letter;
	TYPE_OF_KEY keyType;
	
	Key::Key(TYPE_OF_KEY type, QChar charr);
	Key::Key(QString str);
	QString toString();
};

