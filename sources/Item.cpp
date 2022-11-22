#include "Item.h"

Item::Item(const QJsonValue& jsonValue){
	/*
	name = jsonValue["name"].toString();
	price = jsonValue["price"].toInt();
	weight = jsonValue["weight"].toDouble();
	//todo zamienic te if/elseif na mape albo cos
	if (jsonValue["sell_blue_dijn"].toBool())
		seller = Item::SELLER::BLUE_DJIN;
	else if (jsonValue["sell_green_dijn"].toBool())
		seller = Item::SELLER::GREEN_DJIN;
	else if (jsonValue["sell_oramond"].toBool())
		seller = Item::SELLER::ORAMOND;
	else if (jsonValue["sell_zao"].toBool())
		seller = Item::SELLER::ZAO;
	else if (jsonValue["sell_other"].toBool())
		seller = Item::SELLER::OTHER_SELLER;
	else if (jsonValue["sell_rashid"].toBool())
		seller = Item::SELLER::RASHID;
	else
		seller = Item::SELLER::ANY;
	QString typeOfItemStr = jsonValue["type"].toString();
	Item::TYPE_OF_ITEM typeOfItem = Item::descriptionMap.key(typeOfItemStr);
	type = typeOfItem;
	*/
}

QStringList Item::getListOfCategories(){
	return descriptionMap.values();
}

bool Item::filrItemList(QList<Item>& items, SELLER* sellerToFiltr, TYPE_OF_ITEM* itemCategoryToFiltr){
	if (sellerToFiltr == NULL && itemCategoryToFiltr == NULL || items.size() == 0) {
		items = QList<Item>();
		return false;
}

	typedef Item::SELLER Seller;
	typedef Item::TYPE_OF_ITEM Type;

	bool filtrBySeller = sellerToFiltr != NULL;
	bool filtrByType = itemCategoryToFiltr != NULL;
	QList<Item> itemsToCopy = items;
	items.clear();
	for each (Item var in itemsToCopy) {
		if (filtrBySeller) {
			bool isProperSeller = (var.seller == *sellerToFiltr);
			if (!isProperSeller)
				continue;
		}
		if (filtrByType) {
			bool isProperType = var.type == *itemCategoryToFiltr;
			if (!isProperType)
				continue;
		}
		items.push_back(var);
	}
	return true;
}

QMap<Item::TYPE_OF_ITEM, QString> Item::descriptionMap{
	{Item::TYPE_OF_ITEM::ARMOR, "armors"},
	{Item::TYPE_OF_ITEM::AMULETS, "amulets"},
	{Item::TYPE_OF_ITEM::BOOTS, "boots"},
	{Item::TYPE_OF_ITEM::CREATURE, "creature"},
	{Item::TYPE_OF_ITEM::HELMETS, "helmets"},
	{Item::TYPE_OF_ITEM::LEGS, "legs"},
	{Item::TYPE_OF_ITEM::OTHER, "other"},
	{Item::TYPE_OF_ITEM::POTIONS, "potions"},
	{Item::TYPE_OF_ITEM::RINGS, "rings"},
	{Item::TYPE_OF_ITEM::RUNES, "runes"},
	{Item::TYPE_OF_ITEM::SHIELDS, "shields"},
	{Item::TYPE_OF_ITEM::VALUABLES, "valuables"},
	{Item::TYPE_OF_ITEM::AMMO, "ammo"},
	{Item::TYPE_OF_ITEM::AXES, "axes"},
	{Item::TYPE_OF_ITEM::SWORDS, "swords"},
	{Item::TYPE_OF_ITEM::CLUBS, "clubs"},
	{Item::TYPE_OF_ITEM::DISTANCES, "distances"},
	{Item::TYPE_OF_ITEM::ROD, "rod"},
	{Item::TYPE_OF_ITEM::WANDS, "wands"}
};

QMap<Item::SELLER, QString> Item::buyerMap{
	{Item::SELLER::BLUE_DJIN, "Blue dijnn"},
	{Item::SELLER::GREEN_DJIN, "Green dijnn"},
	{Item::SELLER::OTHER_SELLER, "Other"},
	{Item::SELLER::RASHID, "Rashid"},
	{Item::SELLER::ZAO, "Esrik"},
	{Item::SELLER::ORAMOND, "Flint"},
};

