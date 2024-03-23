#include "Item.h"
namespace CJ {
Item::Item(const QJsonValue& jsonValue) {
  //[TODO]
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

QStringList Item::getItemCategoriesNames() {
  return descriptionMap.values();
}

void Item::filrItems(QVector<Item>& items, const ItemSeller&& sellerToFiltr) {
  std::vector<Item> itemsToCopy = items.toStdVector();
  std::vector<Item> to_ret;
  auto              filtr_lamda = [sellerToFiltr](const Item& item) { return item.seller_ == sellerToFiltr; };
  std::copy_if(begin(itemsToCopy), end(itemsToCopy), std::back_inserter(to_ret), filtr_lamda);
  items = QVector<Item>(begin(to_ret), end(to_ret));
}

void Item::filrItems(QVector<Item>& items, const ItemType&& itemCategoryToFiltr) {
  std::vector<Item> itemsToCopy = items.toStdVector();
  std::vector<Item> to_ret;
  auto              filtr_lamda = [itemCategoryToFiltr](const Item& item) { return item.type_ == itemCategoryToFiltr; };
  std::copy_if(begin(itemsToCopy), end(itemsToCopy), std::back_inserter(to_ret), filtr_lamda);
  items = QVector<Item>(begin(to_ret), end(to_ret));
}
}  // namespace CJ
