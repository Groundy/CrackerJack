#pragma once
#include <Profile.h>
#include <qsharedpointer.h>

#include "BattleList.hpp"
#include "Equipment.hpp"
#include "GameProcessData.hpp"
#include "GameWindow.hpp"
#include "Logger.hpp"
#include "PlayerPosition.hpp"
#include "Settings.hpp"
#include "Timers.hpp"
#include "Vitallity.hpp"

class VariablesClass {
 public:
  VariablesClass(QSharedPointer<Profile> prof) : prof(prof){};
  ~VariablesClass() = default;

  Timers& getTimers() {
    return timers;
  }
  BattleList& getBattleList() {
    return battleList;
  }
  Settings& getSettings() {
    return settings;
  }
  Vitallity& getVitalitty() {
    return vitalitty;
  }
  MiniMap& getMiniMap() {
    return minimap;
  }
  GameWindow& getGameWindow() {
    return gameWindow;
  }
  GameProcessData& getGameProcess() {
    return gameProcess;
  }
  PlayerPosition& getPosition() {
    return position;
  }
  Equipment& getEquipment() {
    return eq;
  }
  QSharedPointer<Profile> getProf() {
    return prof;
  }

  void setFullImage(const QImage& full_image) {
    full_image_.setImg(full_image);
  }

  CJ_Image getFullImage() {
    return full_image_.getImg();
  }

 private:
  QSharedPointer<Profile> prof;
  GameProcessData         gameProcess{};
  GameWindow              gameWindow{};
  BattleList              battleList{};
  Settings                settings{};
  Timers                  timers{};
  Vitallity               vitalitty{};
  MiniMap                 minimap{};
  PlayerPosition          position{};
  Equipment               eq{};

  //used for calibration
  MutexImg full_image_;
};
