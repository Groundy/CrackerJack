#pragma once
#include <QThread>

#include "GameActivityChecker.h"
#include "GameConnecter.h"
#include "Profile.h"
#include "VariablesClass.hpp"
class ScreenSaver : public QThread {
 public:
  void run();
  ScreenSaver(QObject* parent, QSharedPointer<VariablesClass> var, QSharedPointer<GameConnecter> gameConnector);
  ~ScreenSaver();

 private:
  const uint                    SLEEP_TIME = 985;
  const Key                     screenshot_key_;
  QSharedPointer<GameConnecter> gameConnecter;

  QSharedPointer<Settings>        settings_;
  QSharedPointer<GameProcessData> game_process_data_;

  void sendScreenRequestToGame();
};
