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
