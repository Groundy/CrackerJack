#include "VariablesClass.h"

VariablesClass::VariablesClass(){
}

VariablesClass::~VariablesClass(){

}
QMap<QString, QRect> VariablesClass::getMapWithRects(){
	QMap<QString, QRect> toRet;
	toRet.insert("Ultimate Spirit Potion", QRect());
	toRet.insert("Great Health Potion", QRect());
	toRet.insert("Ultimate Mana Potion", QRect());
	toRet.insert("Ultimate Health Potion", QRect());
	toRet.insert("Supreme Health Potion", QRect());
	toRet.insert("Strong Mana Potion", QRect());
	toRet.insert("Strong Health Potion", QRect());
	toRet.insert("Mana Potion", QRect());
	toRet.insert("Health Potion", QRect());
	toRet.insert("Great Mana Potion", QRect());
	toRet.insert("Great Spirit Potion", QRect());
	return toRet;
}
