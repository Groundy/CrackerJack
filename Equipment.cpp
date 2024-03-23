#include "Equipment.h"
void Equipment::setStoreRect(const QRect& storeRectToSet) {
  storeRect.setRect(storeRectToSet);
  QPoint storeRectStart = storeRectToSet.topLeft();
}

QRect Equipment::getStoreRect() {
  return storeRect.getRect();
}

QVector<Equipment::State> Equipment::getCurrentStates(bool clearImg = true) {
  CJ_Image stateBar = statesBarImg_.getImg();
  if (stateBar.isNull()) {
    return {};
  }
  if (clearImg) {
    statesBarImg_.clear();
  }
  stateBar.toBlackAndWhiteOneColor(to_black_white_threshold_);
  QList<CJ_Image> imgs  = stateBar.toImageListWithSingleLetters();
  QVector<State>  toRet = {};
  foreach (CJ_Image img, imgs) {
    img.cutBlackBorders();
    QString code = img.binaryLetterImgToCode();
    if (!codeStateMap.contains(code)) {
      continue;
    }
    toRet.append(codeStateMap.value(code));
  }
  return toRet;
}
QString Equipment::getEqRectBottomText(EqRect eqRect) {
  CJ_Image bottomStrImg = getImg(eqRect);
  if (bottomStrImg.isNull()) {
    return QString();
  }
  bottomStrImg.toBlackAndWhiteExactColor(grey_col_eq_strings_);
  return bottomStrImg.toString();
}
QRect Equipment::getRect(EqRect eqRect) {
  const QPoint EQ_FRAME_TOPLEFT = storeRect.getRect().topLeft() - QPoint(74, 0);
  switch (eqRect) {
    case EqRect::Armor: {
      QPoint start = EQ_FRAME_TOPLEFT + QPoint(38, 37);
      return QRect(start, eq_fields_size_);
    }
    case EqRect::Legs: {
      QPoint start = EQ_FRAME_TOPLEFT + QPoint(38, 73);
      return QRect(start, eq_fields_size_);
    }
    case EqRect::Helmet: {
      QPoint start = EQ_FRAME_TOPLEFT + QPoint(38, 1);
      return QRect(start, eq_fields_size_);
    }
    case EqRect::Boots: {
      QPoint start = EQ_FRAME_TOPLEFT + QPoint(38, 109);
      return QRect(start, eq_fields_size_);
    }
    case EqRect::Ring: {
      QPoint start = EQ_FRAME_TOPLEFT + QPoint(1, 87);
      return QRect(start, eq_fields_size_);
    }
    case EqRect::Amulet: {
      QPoint start = EQ_FRAME_TOPLEFT + QPoint(1, 15);
      return QRect(start, eq_fields_size_);
    }
    case EqRect::Weapon: {
      QPoint start = EQ_FRAME_TOPLEFT + QPoint(1, 51);
      return QRect(start, eq_fields_size_);
    }
    case EqRect::Shield: {
      QPoint start = EQ_FRAME_TOPLEFT + QPoint(75, 51);
      return QRect(start, eq_fields_size_);
    }
    case EqRect::Backpack: {
      QPoint start = EQ_FRAME_TOPLEFT + QPoint(75, 15);
      return QRect(start, eq_fields_size_);
    }
    case EqRect::Torch: {
      QPoint start = EQ_FRAME_TOPLEFT + QPoint(75, 87);
      return QRect(start, eq_fields_size_);
    }

    case EqRect::StateBar: {
      QPoint      start           = EQ_FRAME_TOPLEFT + QPoint(1, 145);
      const QSize STATUS_BAR_SIZE = QSize(106, 11);
      return QRect(start, STATUS_BAR_SIZE);
    }
    case EqRect::SoulPoints: {
      QPoint      start                = EQ_FRAME_TOPLEFT + QPoint(1, 132);
      const QSize SOUL_AMOUNT_BAR_SIZE = QSize(32, 9);  // not whole sqare but only amount!!!
      return QRect(start, SOUL_AMOUNT_BAR_SIZE);
    }
    case EqRect::Capacity: {
      QPoint      start               = EQ_FRAME_TOPLEFT + QPoint(75, 132);
      const QSize CAP_AMOUNT_BAR_SIZE = QSize(32, 9);  // not whole sqare but only amount!!!
      return QRect(start, CAP_AMOUNT_BAR_SIZE);
    }
    default:
      return QRect();
  }
}
void Equipment::setImg(EqRect eqRect, const QImage& imgToSet) {
  switch (eqRect) {
    case EqRect::StateBar:
      statesBarImg_.setImg(imgToSet);
      return;
    case EqRect::SoulPoints:
      soulPtsImg_.setImg(imgToSet);
      return;
    case EqRect::Capacity:
      capImg_.setImg(imgToSet);
      return;
    case EqRect::Helmet:
      helmetCap_.setImg(imgToSet);
      return;
    case EqRect::Armor:
      armorImg_.setImg(imgToSet);
      return;
    case EqRect::Legs:
      legsImg_.setImg(imgToSet);
      return;
    case EqRect::Boots:
      bootsImg_.setImg(imgToSet);
      return;
    case EqRect::Ring:
      ringImg_.setImg(imgToSet);
      return;
    case EqRect::Amulet:
      amuletImg_.setImg(imgToSet);
      return;
    case EqRect::Weapon:
      weaponImg_.setImg(imgToSet);
      return;
    case EqRect::Shield:
      shieldImg_.setImg(imgToSet);
      return;
    case EqRect::Backpack:
      backpackImg_.setImg(imgToSet);
      return;
    case EqRect::Torch:
      torchImg_.setImg(imgToSet);
      return;
    default:
      return;
  }
}
QImage Equipment::getImg(EqRect eqRect) {
  switch (eqRect) {
    case EqRect::StateBar:
      return statesBarImg_.getImg();
    case EqRect::SoulPoints:
      return soulPtsImg_.getImg();
    case EqRect::Capacity:
      return capImg_.getImg();
    case EqRect::Helmet:
      return helmetCap_.getImg();
    case EqRect::Armor:
      return armorImg_.getImg();
    case EqRect::Legs:
      return legsImg_.getImg();
    case EqRect::Boots:
      return bootsImg_.getImg();
    case EqRect::Ring:
      return ringImg_.getImg();
    case EqRect::Amulet:
      return amuletImg_.getImg();
    case EqRect::Weapon:
      return weaponImg_.getImg();
    case EqRect::Shield:
      return shieldImg_.getImg();
    case EqRect::Backpack:
      return backpackImg_.getImg();
    case EqRect::Torch:
      return torchImg_.getImg();
    default:
      return QImage();
  }
}

QString Equipment::getStateCode(const QString& fileName) const {
  CJ_Image img(QImage(":/statesIcons/" + fileName));
  img.toBlackAndWhite(to_black_white_threshold_);
  img.cutBlackBorders();
  return img.binaryLetterImgToCode();
}
