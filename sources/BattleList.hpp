#pragma once
#include <CJ_Image.h>
#include <memory.h>
#include <qrect.h>

#include "Logger.hpp"
#include "PathResource.h"
#include "Utilities.hpp"
namespace CJ {
class BattleList {
 public:
  BattleList()  = default;
  ~BattleList() = default;
  void setFrame(QRect battleListAreaToSet) {
    battleListArea.setRect(battleListAreaToSet);
  }
  void setImg(const QImage& battleListImgToSet) {
    battleListImg.setImg(battleListImgToSet);
  }
  QRect getFrame() {
    return battleListArea.getRect();
  }
  QImage getImg() {
    return battleListImg.getImg();
  }
  int getEnemisAmout() {
    if (qrand() % 10 == 0) checkIfBattleListIsOk();
    QImage inneBattleList = getInnerBattleList();
    if (inneBattleList.isNull()) return -1;
    const int      middleX   = inneBattleList.width() / 2;
    int            blackDots = 0;
    constexpr uint BLACK     = qRgb(0, 0, 0);
    for (int y = 0; y < inneBattleList.height(); y++) {
      if (inneBattleList.pixel(middleX, y) == BLACK) blackDots++;
    }
    return blackDots / 2;
  }
  bool firstEnemieIsInRedFrame() {
    QImage outterBattleListLImg = getImg();
    if (outterBattleListLImg.isNull()) return true;
    QRect          inneBattleListArea  = getInnerRect(outterBattleListLImg.rect());
    QImage         innerBattleListLImg = outterBattleListLImg.copy(inneBattleListArea);
    constexpr uint RED                 = qRgb(255, 0, 0);
    constexpr uint LIGHT_RED           = qRgb(255, 128, 128);
    int            redPixels           = 0;
    for (int y = 0; y < 20; y += 19) {
      for (int x = 0; x < 20; x++) {  // top//down
        uint pix = innerBattleListLImg.pixel(x, y);
        if (pix == RED || pix == LIGHT_RED) {
          redPixels++;
        }
      }
    }
    for (int x = 0; x < 20; x += 19) {  // left//right
      for (int y = 1; y < 19; y++) {
        uint pix = innerBattleListLImg.pixel(x, y);
        if (pix == RED || pix == LIGHT_RED) {
          redPixels++;
        }
      }
    }
    const int ALL_PIX_TO_CHECK = 76;
    double    redPercentage    = (redPixels * 100.0) / ALL_PIX_TO_CHECK;
    return redPercentage >= 75;
  }
  bool checkIfBattleListIsOk() {
    CJ_Image battleMark = QImage(PathResource::getPathToBattleList());
    CJ_Image battleList = getImg();
    bool     properImg  = battleList.findStartPositionInImg(battleMark).size() == 1;
    if (!properImg) setFrame(QRect());
    return properImg;
  }
  QStringList getUniqueMonstersNames() {
    QImage innerList = getInnerBattleList();
    if (innerList.isNull()) {
      logger.log("Blad czytania listy potworow.", true, true, false);
      return QStringList();
    }
    QVector<QRect> nameRect = getFramesOfMonstersNames();
    QStringList    toRet    = {};
    for (int i = 0; i < nameRect.size(); i++) {
      CJ_Image singleMonsterNameImg = innerList.copy(nameRect[i]);
      QString  monsterName          = singleMonsterNameImg.toString(180);
      if (monsterName.isEmpty()) {
        break;
      }
      toRet.append(monsterName);
    }
    toRet.removeDuplicates();
    return toRet;
  }

 private:
  Logger&          logger = Logger::instance();
  MutexRect        battleListArea;
  MutexImg         battleListImg;
  std::atomic<int> enemiesOnBattleList = 0;
  QImage           getInnerBattleList() {
    QRect  innerBattleListRect = getInnerRect(battleListArea.getRect());
    QImage fullBattleListImg   = battleListImg.getImg();
    return fullBattleListImg.copy(innerBattleListRect);
  }
  const QPoint INNER_FRAME_FIST_NAME_START = QPoint(21, 0);
  const int    NAME_HEIGHT                 = 14;
  const int    HIGH_DIFF_BETWEEN_NAMES     = 8;
  QRect        getInnerRect(QRect outerRect) {
    return QRect(4, 15, 155, outerRect.height());
  }
  QVector<QRect> getFramesOfMonstersNames() {
    QVector<QRect> toRet                     = {};
    auto           innerRect                 = getInnerRect(battleListArea.getRect());
    int            howManyNamesFitToListSize = ((innerRect.height() - NAME_HEIGHT) / (NAME_HEIGHT + HIGH_DIFF_BETWEEN_NAMES) +
                                     1);  // one name + pairs of (name+space between names)
    for (int i = 0; i < howManyNamesFitToListSize; i++) {
      QPoint startPoint = INNER_FRAME_FIST_NAME_START + QPoint(0, i * (NAME_HEIGHT + HIGH_DIFF_BETWEEN_NAMES));
      toRet.append(QRect(startPoint.x(), startPoint.y(), innerRect.width(), NAME_HEIGHT));
    }
    return toRet;
  };
};
}  // namespace CJ
