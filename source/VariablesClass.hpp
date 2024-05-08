#pragma once
#include "Equipment.h"
#include "Profile.h"
#include <qsharedpointer.h>

#include "BattleList.hpp"
#include "Equipment.h"
#include "GameProcessData.hpp"
#include "GameWindow.hpp"
#include "Logger.hpp"
#include "PlayerPosition.hpp"
#include "Settings.hpp"
#include "Timers.hpp"
#include "Vitallity.hpp"
namespace CJ {
class VariablesClass {
 public:
  VariablesClass(QSharedPointer<Profile> prof) : prof(prof){};
  ~VariablesClass() = default;

  QSharedPointer<Timers> getTimers() {
    return timers;
  }
  QSharedPointer<BattleList> getBattleList() {
    return battleList;
  }
  QSharedPointer<Settings> getSettings() {
    return settings;
  }
  QSharedPointer<Vitallity> getVitalitty() {
    return vitalitty;
  }
  QSharedPointer<MiniMap> getMiniMap() {
    return minimap;
  }
  QSharedPointer<GameWindow> getGameWindow() {
    return gameWindow;
  }
  QSharedPointer<GameProcessData> getGameProcess() {
    return gameProcess;
  }
  QSharedPointer<PlayerPosition> getPosition() {
    return position;
  }
  QSharedPointer<Equipment> getEquipment() {
    return eq;
  }
  QSharedPointer<Profile> getProf() {
    return prof;
  }

 private:
  QSharedPointer<Profile>         prof;
  QSharedPointer<GameProcessData> gameProcess = QSharedPointer<GameProcessData>(new GameProcessData());
  QSharedPointer<GameWindow>      gameWindow  = QSharedPointer<GameWindow>(new GameWindow());
  QSharedPointer<BattleList>      battleList  = QSharedPointer<BattleList>(new BattleList());
  QSharedPointer<Settings>        settings    = QSharedPointer<Settings>(new Settings());
  QSharedPointer<Timers>          timers      = QSharedPointer<Timers>(new Timers());
  QSharedPointer<Vitallity>       vitalitty   = QSharedPointer<Vitallity>(new Vitallity());
  QSharedPointer<MiniMap>         minimap     = QSharedPointer<MiniMap>(new MiniMap());
  QSharedPointer<PlayerPosition>  position    = QSharedPointer<PlayerPosition>(new PlayerPosition());
  QSharedPointer<Equipment>       eq          = QSharedPointer<Equipment>(new Equipment());
};
}  // namespace CJ
