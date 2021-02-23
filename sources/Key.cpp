#include "Key.h"

static Key toKey(QString keyNumber) {
	QStringList listKey = keyNumber.split('-', Qt::SkipEmptyParts);
	//there will be 4 parts numer,ctrl-key, f-key, char
	if (listKey.size() != 4);
	//[DIAG] [ERROR]
	int number = static_cast<int>(listKey[0].toInt());
	bool ckey = static_cast<bool>(listKey[1].toInt());
	bool fkey = static_cast<bool>(listKey[2].toInt());
	char keyChar = static_cast<char>(listKey[3].toInt());
}	

QString Key::toString() {
		QString strToConvert = "";
		if (this->number < 0 || this->number >9);
		//[DIAG] //[ERR]
		strToConvert.append(this->number + "-");

		int toAdd = this->control ? 1 : 0;
		strToConvert.append(toAdd + "-");
		toAdd = this->fkey ? 1 : 0;
		strToConvert.append(toAdd + "-");
		if (this->letter == NULL)
			strToConvert.append(" ");
		else
			strToConvert.append(this->letter);

		int toRet = strToConvert.toInt();
		return toRet;
	}

Key::Key(type type, int numberArg, bool controlArg, bool fkeyArg, char letterArg) {
	switch (type)
	{
	case Key::FKEY:
		fkey = true;
		control = false;
		letter = NULL;
		number = numberArg;
		break;
	case Key::CONTROL:
		fkey = false;
		control = true;
		letter = NULL;
		number = numberArg;
		break;
	case Key::LETTER:
		fkey = false;
		control = false;
		letter = letterArg;
		number = numberArg;
		break;
	default:
		break;
	}
}