#pragma once
#include "Utilities.h"
#include <QObject>
#include "qjsonvalue.h"

class Item{
public:
	enum TYPE_OF_ITEM { ARMOR, AMULETS, BOOTS, CREATURE, HELMETS, LEGS, POTIONS, RINGS, RUNES, SHIELDS, VALUABLES, AMMO, AXES, SWORDS, CLUBS, DISTANCES, ROD, WANDS, OTHER, ANY};
	enum class SELLER { BLUE_DJIN, GREEN_DJIN, ORAMOND, ZAO, OTHER_SELLER, RASHID, ANY };

	QString name;
	int price;
	double weight;
	TYPE_OF_ITEM type;
	SELLER seller;
	Utilities::RestoreMethode toRestoreMethode();
	Item();
	Item(QJsonValue jsonValue);

	static QStringList getListOfCategories();
	static bool filrItemList(QList<Item>& items, SELLER* sellerToFiltr, TYPE_OF_ITEM* itemCategoryToFiltr);
	static QMap<TYPE_OF_ITEM, QString> descriptionMap;
	static QMap<SELLER, QString> buyerMap;
};

class Potion : public Item {
public:
	int manaReg, healthReg;
	bool forMage, forRp, forEk;
};
