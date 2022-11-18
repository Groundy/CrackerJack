#include "Key.h"

QStringList Key::getListOfAllPossibleKeys(){
	return KeysAndCodesMap.keys();
}

QJsonObject Key::toJson() {
	QJsonObject json;
	json.insert("keyVal", keyVal);
	json.insert("keyName", keyName);
	return json;
}

Key::Key(int codeOfKey){
	bool thereIsSuchCode = KeysAndCodesMap.values().contains(codeOfKey);
	this->keyVal = thereIsSuchCode ? codeOfKey : -1;
}

Key::Key(QString keyCodeAsStr){
	this->keyVal = KeysAndCodesMap.value(keyCodeAsStr, -1);
}

Key::Key(QJsonObject obj){
	if (!obj.contains("keyVal"))
		throw std::exception("no keyVal field in key object in json file");
	if(!obj.value("keyVal").isDouble())
		throw std::exception("invalid keyVal field in key object in json file");

	if (!obj.contains("keyName"))
		throw std::exception("no keyName field in key object in json file");
	if (!obj.value("keyName").isString())
		throw std::exception("invalid keyName field in key object in json file");

	keyVal = obj.value("keyVal").toInt();
	keyName = obj.value("keyName").toString();
}

Key::Key(){
	this->keyVal = -1;
}

QMap<QString, int> Key::KeysAndCodesMap = {
	{"F1", 0x70},
	{"F2", 0x71},
	{"F3", 0x72},
	{"F4", 0x73},
	{"F5", 0x74},
	{"F6", 0x75},
	{"F7", 0x76},
	{"F8", 0x77},
	{"F9", 0x78},
	{"F10", 0x79},
	{"F11", 0x7A},
	{"F12", 0x7B},
	{"0", 48},
	{"1", 49},
	{"2", 50},
	{"3", 51},
	{"4", 52},
	{"5", 53},
	{"6", 54},
	{"7", 55},
	{"8", 56},
	{"9", 57},
	{"ESC", 27},
	{"INSERT", 45},
	{"DELETE", 46},
	{"HOME", 36},
	{"END", 35},
	{"PAGEUP", 33},
	{"PAGEDOWN", 34},
	{"ScrollLock", 91},
	{"PauseBreak", 19},
	{"/", 111},
	{"*", 106},
	{"-", 100},
	{"+", 107}
};
