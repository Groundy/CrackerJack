#include "Key.h"

bool Key::checkIfnumberIsAloowed(int keyNumber){
	for (size_t i = 0; i < 256; i++) {
		if (KEYS(i) == KEYS(keyNumber))
			return true;
	}
	return false;
}

Key::Key(){
	number = endOfEnum;
}

Key::Key(int numberToSet){
	bool isAllowed = checkIfnumberIsAloowed(numberToSet);
	numberToSet = isAllowed ? numberToSet : -1;
	this->number = numberToSet;
	if (this->number == -1)
		Logger::logPotenialBug("Empty key was created", "Key", "Key(constructor)");
}

Key::Key(QKeySequence qsec){
	QString str = qsec.toString();
	int keyValue;
	if (str == "F1") keyValue = KEYS(F1);
	else if (str == "F2") keyValue = KEYS(F2);
	else if (str == "F3") keyValue = KEYS(F3);
	else if (str == "F4") keyValue = KEYS(F4);
	else if (str == "F5") keyValue = KEYS(F5);
	else if (str == "F6") keyValue = KEYS(F6);
	else if (str == "F7") keyValue = KEYS(F7);
	else if (str == "F8") keyValue = KEYS(F8);
	else if (str == "F9") keyValue = KEYS(F9);
	else if (str == "F10") keyValue = KEYS(F10);
	else if (str == "F11") keyValue = KEYS(F11);
	else if (str == "F12") keyValue = KEYS(F12);
	else if (str == "0") keyValue = KEYS(n0);
	else if (str == "1") keyValue = KEYS(n1);
	else if (str == "2") keyValue = KEYS(n2);
	else if (str == "3") keyValue = KEYS(n3);
	else if (str == "4") keyValue = KEYS(n4);
	else if (str == "5") keyValue = KEYS(n5);
	else if (str == "6") keyValue = KEYS(n6);
	else if (str == "7") keyValue = KEYS(n7);
	else if (str == "8") keyValue = KEYS(n8);
	else if (str == "9") keyValue = KEYS(n9);
	else if (str == "Esc") keyValue = KEYS(ESC);
	else if (str == "Ins") keyValue = KEYS(INSERT);
	else if (str == "Del") keyValue = KEYS(DELETE_);
	else if (str == "Home") keyValue = KEYS(HOME);
	else if (str == "End") keyValue = KEYS(END);
	else if (str == "PgUp") keyValue = KEYS(PAGEUP);
	else if (str == "PgDown") keyValue = KEYS(PAGEDOWN);
	else if (str == "ScrollLock") keyValue = KEYS(ScrollLock);
	else if (str == "Pause") keyValue = KEYS(PauseBreak);
	else if (str == "/") keyValue = KEYS(SLASH);
	else if (str == "*") keyValue = KEYS(ASTERIX);
	else if (str == "-") keyValue = KEYS(MINUS);
	else if (str == "+") keyValue = KEYS(PLUS);
	else  keyValue = KEYS(endOfEnum);
	number = keyValue;
}

QKeySequence Key::toQKeySequence(Key key){
	int keyValue = key.number;
	QString toRetStr;
	bool isFKey = keyValue >= F1 && keyValue <= F12;
	bool isNumber = keyValue >= n0 && keyValue <= n9;
	if (isFKey) {
		toRetStr = "F";
		int fkey = keyValue - F1 + 1;
		toRetStr.append(QString::number(fkey));
	}
	else if (isNumber) {
		int key = keyValue - n0 + 1;
		const int codeOfValueZeroInDataBase = 57;
		bool zeroValueShouldBeDisplayed = keyValue == codeOfValueZeroInDataBase;
		toRetStr = zeroValueShouldBeDisplayed ? QString::number(0) : QString::number(key);
	}
	else if (keyValue == ESC) toRetStr = "Esc";
	else if (keyValue == INSERT) toRetStr = "Ins";
	else if (keyValue == DELETE_) toRetStr = "Del";
	else if (keyValue == HOME) toRetStr = "Home";
	else if (keyValue == END) toRetStr = "End";
	else if (keyValue == PAGEUP) toRetStr = "PgUp";
	else if (keyValue == PAGEDOWN) toRetStr = "PgDown";
	else if (keyValue == ScrollLock) toRetStr = "ScrollLock";
	else if (keyValue == PauseBreak) toRetStr = "Pause";
	else if (keyValue == SLASH) toRetStr = "/";
	else if (keyValue == ASTERIX) toRetStr = "*";
	else if (keyValue == MINUS) toRetStr = "-";
	else if (keyValue == PLUS) toRetStr =  "+";
	else toRetStr = "";

	return QKeySequence(toRetStr);
}
