#pragma once
#include <qimage.h>
#include <qrect.h>

#include "Utilities.hpp"
struct HealthManaFrames {
  QRect healthFrame;
  QRect manaFrame;
  QRect manaShieldFrame;
  QRect combinedFrame;
  int   howTheyShouldBeRotated = 0;
};
class Vitallity {
 public:
  Vitallity(){};
  ~Vitallity(){};
  void setCurrentPercentage(double health, double mana, double manaShield) {
    this->currentHealthPercentage = health;
    this->currentManaPercentage   = mana;
    this->currentMsPercentage     = manaShield;
  }
  void setCurrentRawValues(int health, int mana, int manaShield) {
    this->currentHealthRaw = health;
    this->currentManaRaw   = mana;
    this->currentMSRaw     = manaShield;
  }
  double getCurrentHealthPercentage() const {
    return currentHealthPercentage;
  }
  double getCurrentManaPercentage() const {
    return currentManaPercentage;
  }
  double getCurrentMSPercentage() const {
    return currentMsPercentage;
  }
  int getCurrentRawManaVal() const {
    return currentManaRaw;
  }
  void setRotation(int rotation) {
    healthManaFrames.howTheyShouldBeRotated = rotation;
  }
  void setHealthArea(QRect toSet) {
    healthManaFrames.healthFrame = toSet;
  }
  void setManaArea(QRect toSet) {
    healthManaFrames.manaFrame = toSet;
  }
  void setMSArea(QRect toSet) {
    healthManaFrames.manaShieldFrame = toSet;
  }
  void setCombinedArea(QRect toSet) {
    healthManaFrames.combinedFrame = toSet;
  }
  int getRotation() {
    return healthManaFrames.howTheyShouldBeRotated;
  }
  QRect getHealthArea() {
    return healthManaFrames.healthFrame;
  }
  QRect getManaArea() {
    return healthManaFrames.manaFrame;
  }
  QRect getMSArea() {
    return healthManaFrames.manaShieldFrame;
  }
  QRect getCombinedArea() {
    return healthManaFrames.combinedFrame;
  }
  void setImageHealth(QImage& img) {
    healthImg.setImg(img);
  }
  void setImageMana(QImage& img) {
    manaImg.setImg(img);
  }
  void setImageMS(QImage& img) {
    msImg.setImg(img);
  }
  void setImageCombined(QImage& img) {
    combinedImg.setImg(img);
  }
  void getImageHealth(QImage& img, bool clear = false) {
    img = healthImg.getImgCopy();
    if (clear) healthImg.clear();
  }
  void getImageMana(QImage& img, bool clear = false) {
    img = manaImg.getImgCopy();
    if (clear) manaImg.clear();
  }
  void getImageMS(QImage& img, bool clear = false) {
    img = msImg.getImgCopy();
    if (clear) msImg.clear();
  }
  void getImageCombined(QImage& img, bool clear = false) {
    img = combinedImg.getImgCopy();
    if (clear) combinedImg.clear();
  }
  bool needCalibration() {
    if (getHealthArea().isEmpty()) return true;
    if (getCombinedArea().isEmpty() && getManaArea().isEmpty()) return true;
    return false;
  }
  void clearHealth() {
    currentHealthPercentage = 100.0;
    currentHealthRaw        = 50000;
  }
  void clearMana() {
    currentManaPercentage = 100.0;
    currentManaRaw        = 100000;
  }

 private:
  std::atomic<double> currentHealthPercentage = 100.0, currentManaPercentage = 100.0, currentMsPercentage = 100.0;
  std::atomic<int>    currentHealthRaw = 100, currentManaRaw = 100, currentMSRaw = 100;
  HealthManaFrames    healthManaFrames;
  MutexImg            healthImg, manaImg, combinedImg, msImg;
};

class MiniMap {
 public:
  MiniMap(){};
  ~MiniMap(){};
  void getImgMiniMap(QImage& imgToRet) {
    imgToRet = this->miniMap.getImgCopy();
    this->miniMap.clear();
  }
  void setImgMiniMap(const QImage& miniMapImg) {
    this->miniMap.setImg(miniMapImg);
  }
  QRect getFrameMiniMap() {
    return miniMapFrame.getRect();
  }
  void setFrameMiniMap(const QRect& newRect) {
    miniMapFrame.setRect(newRect);
  }
  void getImgMiniMapLayer(QImage& imgToRet) {
    imgToRet = this->minMapLayer.getImgCopy();
    this->minMapLayer.clear();
  }
  QPoint getCompasMiddlePoint() {
    QPoint miniMapTopLeft = miniMapFrame.getRect().topLeft();
    if (miniMapTopLeft.isNull()) return QPoint();
    return miniMapTopLeft + vectorToCompassMiddle;
  }
  void setImgMiniMapLayer(const QImage& minMapLayerImg) {
    this->minMapLayer.setImg(minMapLayerImg);
  }

 private:
  MutexImg     miniMap;
  MutexImg     minMapLayer;
  MutexRect    miniMapFrame;
  const QPoint vectorToCompassMiddle = QPoint(144, 22);
};
