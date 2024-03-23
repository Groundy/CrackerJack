#pragma once
#include <qpoint.h>
#include <qrect.h>

#include "CJ_Image.h"
#include "Utilities.hpp"
class Equipment {
 public:
  enum STATES { HASTE, BATTLE, PROTECTOR_ZONE, POISONED, PARALYZED, UPGRADED, HUNGER, DRUNKEN };
  enum class EqRect { StateBar, SoulPoints, Capacity, Helmet, Armor, Legs, Boots, Ring, Amulet, Weapon, Shield, Backpack, Torch };

  Equipment()  = default;
  ~Equipment() = default;

  void            setStoreRect(const QRect& storeRectToSet);
  QRect           getStoreRect();
  QVector<STATES> getCurrentStates(bool clearImg = true);
  QString         getEqRectBottomText(EqRect eqRect);
  QRect           getRect(EqRect eqRect);
  void            setImg(EqRect eqRect, const QImage& imgToSet);
  QImage          getImg(EqRect eqRect);

 private:
  MutexImg statesBarImg_, soulPtsImg_, capImg_, helmetCap_, armorImg_, legsImg_, bootsImg_, ringImg_, amuletImg_, weaponImg_, shieldImg_,
      backpackImg_, torchImg_;
  MutexRect   storeRect;
  const int   to_black_white_threshold_ = 100;
  const QRgb  grey_col_eq_strings_      = qRgb(191, 191, 191);
  const QSize eq_fields_size_           = QSize(32, 32);

  QString                     getStateCode(const QString& fileName) const;
  const QMap<QString, STATES> codeStateMap = {            //
      {getStateCode("battleIco"), STATES::BATTLE},        //
      {getStateCode("hungerIco"), STATES::HUNGER},        //
      {getStateCode("drunkenIco"), STATES::DRUNKEN},      //
      {getStateCode("hastedIco"), STATES::HASTE},         //
      {getStateCode("paralyzedIco"), STATES::PARALYZED},  //
      {getStateCode("pzIco"), STATES::PROTECTOR_ZONE},    //
      {getStateCode("upgradedIco"), STATES::UPGRADED}};
};
