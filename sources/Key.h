#pragma once
#include <cstddef>
#include <qstring.h>
#include "qsplitter.h"


class Key {
public:
	enum TYPE_OF_KEY { NORMAL, SHIFT, FKEY, CONTROL, ALT };
	QChar letter;
	TYPE_OF_KEY keyType;
	
	Key::Key(TYPE_OF_KEY type, QChar charr);
	Key::Key(QString str);
	QString toString();
};

