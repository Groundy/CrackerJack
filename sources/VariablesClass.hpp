#pragma once
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
  VariablesClass() = default;
  ~VariablesClass() {
    delete battleList;
    delete eq;
  };
  void log(QString msg, bool sendToDebug = true, bool sendToUserConsol = true, bool addTimeSTamp = true) {
    logger.log(msg, sendToDebug, sendToUserConsol, addTimeSTamp);
  }
  Timers& getTimers() {
    return timers;
  }
  BattleList& getBattleList() {
    return *battleList;
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
    return *eq;
  }
  Logger& getLogger() {
    return logger;
  }
  ImgEditor& getImgEditorObj() {
    return imgEditor;
  }

 private:
  Logger          logger;
  GameProcessData gameProcess;
  GameWindow      gameWindow;
  BattleList*     battleList = new BattleList(&logger, &imgEditor);
  Settings        settings;
  Timers          timers;
  Vitallity       vitalitty;
  MiniMap         minimap;
  PlayerPosition  position;
  // MutexImg fullImage;
  Equipment* eq = new Equipment(&imgEditor);
  ImgEditor  imgEditor;
};
