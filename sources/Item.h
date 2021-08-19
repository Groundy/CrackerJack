#pragma once
#include "Utilities.h"
#include <QObject>

class Item{
public:
	static enum TYPE_OF_ITEM { ARMOR, AMULETS, BOOTS, CREATURE, HELMETS, LEGS, POTIONS, RINGS, RUNES, SHIELDS, VALUABLES, AMMO, AXES, SWORDS, CLUBS, DISTANCES, ROD, WANDS, OTHER };
	static enum class SELLER { BLUE_DJIN, GREEN_DJIN, YASIR, ZAO, OTHER_SELLER, RASHID };

	QString name;
	int price, weight;
	TYPE_OF_ITEM type;
	SELLER seller;
	Utilities::RestoreMethode toRestoreMethode();
	
	static QMap<TYPE_OF_ITEM, QString> descriptionMap;
};

class Potion : public Item {
public:
	int manaReg, healthReg;
	bool forMage, forRp, forEk;
};
