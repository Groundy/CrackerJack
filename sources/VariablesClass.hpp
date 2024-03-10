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
  ImgEditor& getImgEditorObj() {
    return imgEditor;
  }
  QSharedPointer<Profile> getProf() {
    return prof;
  }

 private:
  QSharedPointer<Profile> prof;
  GameProcessData         gameProcess;
  GameWindow              gameWindow;
  BattleList              battleList = BattleList(&imgEditor);
  Settings                settings;
  Timers                  timers;
  Vitallity               vitalitty;
  MiniMap                 minimap;
  PlayerPosition          position;
  // MutexImg fullImage;
  Equipment eq = Equipment(&imgEditor);
  ImgEditor imgEditor;
};
