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