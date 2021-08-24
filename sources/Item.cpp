#include "Item.h"

Utilities::RestoreMethode Item::toRestoreMethode() {
	bool isPotion = this->type == TYPE_OF_ITEM::POTIONS;
	Utilities::RestoreMethode toRet;
	if (isPotion) {
		toRet.name = this->name;
		toRet.mana = 0;//manaNeededToUsePotion
		toRet.cd = 1;//potions have 1 sec cooldown, for all potions
		toRet.cdGroup = 1;
		toRet.type = Utilities::RestoreMethode::TypeOfMethode::POTION;
	}
	else
		Logger::logPotenialBug("Tried to convert object _Item_ to _RestoreMethode_ but it's not a potion!, returning empty _RestoreMethode_", "Utilites/Item", "toRestoreMethode()");
	return toRet;
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
	{Item::TYPE_OF_ITEM::DISTANCES, "distance"},
	{Item::TYPE_OF_ITEM::ROD, "rod"},
	{Item::TYPE_OF_ITEM::WANDS, "wands"}
};

QMap<Item::SELLER, QString> Item::buyerMap{
	{Item::SELLER::BLUE_DJIN, "Blue dijnn"},
	{Item::SELLER::GREEN_DJIN, "Green dijnn"},
	{Item::SELLER::OTHER_SELLER, "Other"},
	{Item::SELLER::RASHID, "Rashid"},
	{Item::SELLER::ZAO, "Esrik"},
	{Item::SELLER::YASIR, "Yasir"},
};

QMap<Item::TYPE_OF_ITEM, QString> Item::typesStrUsedInJson{
	{Item::TYPE_OF_ITEM::ARMOR, "armor"},
	{Item::TYPE_OF_ITEM::AMULETS, "amulet"},
	{Item::TYPE_OF_ITEM::BOOTS, "boots"},
	{Item::TYPE_OF_ITEM::CREATURE, "creature"},
	{Item::TYPE_OF_ITEM::HELMETS, "helmet"},
	{Item::TYPE_OF_ITEM::LEGS, "legs"},
	{Item::TYPE_OF_ITEM::OTHER, "other"},
	{Item::TYPE_OF_ITEM::POTIONS, "potion"},
	{Item::TYPE_OF_ITEM::RINGS, "ring"},
	{Item::TYPE_OF_ITEM::RUNES, "rune"},
	{Item::TYPE_OF_ITEM::SHIELDS, "shield"},
	{Item::TYPE_OF_ITEM::VALUABLES, "valuable"},
	{Item::TYPE_OF_ITEM::AMMO, "ammo"},
	{Item::TYPE_OF_ITEM::AXES, "axe"},
	{Item::TYPE_OF_ITEM::SWORDS, "sword"},
	{Item::TYPE_OF_ITEM::CLUBS, "club"},
	{Item::TYPE_OF_ITEM::DISTANCES, "distance"},
	{Item::TYPE_OF_ITEM::ROD, "rod"},
	{Item::TYPE_OF_ITEM::WANDS, "wand"}
};
