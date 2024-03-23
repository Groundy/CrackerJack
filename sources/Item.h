#pragma once
#include <qjsonvalue.h>

#include <QObject>

#include "Profession.hpp"
#include "RestorationMethode.hpp"
namespace CJ {
enum class ItemType {
  ARMOR,
  AMULETS,
  BOOTS,
  CREATURE,
  HELMETS,
  LEGS,
  POTIONS,
  RINGS,
  RUNES,
  SHIELDS,
  VALUABLES,
  AMMO,
  AXES,
  SWORDS,
  CLUBS,
  DISTANCES,
  ROD,
  WANDS,
  OTHER,
  ANY
};
enum class ItemSeller { BLUE_DJIN, GREEN_DJIN, ORAMOND, ZAO, OTHER_SELLER, RASHID, ANY };

static const QMap<ItemType, QString> descriptionMap = {
    {ItemType::ARMOR, "armors"},         //
    {ItemType::AMULETS, "amulets"},      //
    {ItemType::BOOTS, "boots"},          //
    {ItemType::CREATURE, "creature"},    //
    {ItemType::HELMETS, "helmets"},      //
    {ItemType::LEGS, "legs"},            //
    {ItemType::OTHER, "other"},          //
    {ItemType::POTIONS, "potions"},      //
    {ItemType::RINGS, "rings"},          //
    {ItemType::RUNES, "runes"},          //
    {ItemType::SHIELDS, "shields"},      //
    {ItemType::VALUABLES, "valuables"},  //
    {ItemType::AMMO, "ammo"},            //
    {ItemType::AXES, "axes"},            //
    {ItemType::SWORDS, "swords"},        //
    {ItemType::CLUBS, "clubs"},          //
    {ItemType::DISTANCES, "distances"},  //
    {ItemType::ROD, "rod"},              //
    {ItemType::WANDS, "wands"},          //
};

static const QMap<ItemSeller, QString> buyer_map = {
    {ItemSeller::BLUE_DJIN, "Blue dijnn"},    //
    {ItemSeller::GREEN_DJIN, "Green dijnn"},  //
    {ItemSeller::OTHER_SELLER, "Other"},      //
    {ItemSeller::RASHID, "Rashid"},           //
    {ItemSeller::ZAO, "Esrik"},               //
    {ItemSeller::ORAMOND, "Flint"}            //
};

class Item {
 public:
  QString    name_;
  int        price_;
  double     weight_;
  ItemType   type_;
  ItemSeller seller_;
  Item() = default;
  Item(const QJsonValue& jsonValue);

  static QStringList getListOfCategories();
  static void        filrItemList(QVector<Item>& items, ItemSeller&& sellerToFiltr);
  static void        filrItemList(QVector<Item>& items, ItemType&& itemCategoryToFiltr);
};
}  // namespace CJ
