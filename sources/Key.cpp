#include "Key.h"

QStringList Key::getListOfAllPossibleKeys(){
	return KeysAndCodesMap.keys();
}

Key::Key(int codeOfKey){
	bool thereIsSuchCode = KeysAndCodesMap.values().contains(codeOfKey);
	this->number = thereIsSuchCode ? codeOfKey : -1;
}

Key::Key(QString keyCodeAsStr){
	this->number = KeysAndCodesMap.value(keyCodeAsStr, -1);
}

Key::Key(){
	this->number = -1;
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
