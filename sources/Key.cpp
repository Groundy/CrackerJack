#include "Key.h"

Key::Key(QString keyNumber){
	TYPE_OF_KEY keyType;
	QChar letter;
	QStringList parts = keyNumber.split("+");
	if (parts.size() < 1 || parts.size() > 2)
		;//diag err
	else if(parts.size() == 1){//it's simple letter or f-key
		QString onlyOne = parts[0];
		QChar firstLetter = onlyOne[0];
		if (firstLetter == QChar('F'))
			keyType = FKEY;
		else
			keyType = NORMAL;
		if (onlyOne.size() == 2)
			letter = onlyOne[1];
		else
			;//diag //err
	}
	else if (parts.size() == 2) {
		QString first = parts[0];
		QString second = parts[1];
		if (first == "Ctrl")
			keyType = CONTROL;
		else if (first == "Alt")
			keyType = ALT;
		else if (first == "Shift")
			keyType = SHIFT;
		else
			;//diag //err
		letter = second[0];
	}
}	

QString Key::toString() {
	QString toRet = "";
	switch (keyType)
	{
	case TYPE_OF_KEY::CONTROL: {
		toRet.append("Ctrl+");
		break; }
	case TYPE_OF_KEY::SHIFT: {
		toRet.append("Shift+");
		break; }
	case TYPE_OF_KEY::ALT: {
		toRet.append("Alt+");
		break; }
	case TYPE_OF_KEY::FKEY: {
		toRet.append("F");
		//diag err(if Qletter is number)
		break; }
	case TYPE_OF_KEY::NORMAL: {
		toRet.append("");
		break; }
	default: {
		toRet.append("");
		//diag err
		break; }
	}
	toRet.append(QString(letter));
	return toRet;
}

Key::Key(TYPE_OF_KEY typeOfKey, QChar charr) {
	this->keyType = typeOfKey;
	this->letter = charr;
}