#pragma once
#include <QThread>

#include "GameActivityChecker.h"
#include "GameConnecter.h"
#include "Profile.h"
#include "VariablesClass.hpp"
namespace CJ {
class ScreenSaver : public QThread {
 public:
  void run(){};
  ScreenSaver(QObject* parent, QSharedPointer<VariablesClass> var, QSharedPointer<GameConnecter> gameConnector);
  ~ScreenSaver();

 private:
  const uint                    sleep_time_ = 985;
  const Key                     screenshot_key_;
  QSharedPointer<GameConnecter> game_connecter_;
  QTimer                        timer_;

  QSharedPointer<Settings>        settings_;
  QSharedPointer<GameProcessData> game_process_data_;

  void sendScreenRequestToGame();
  void execute();
};
}  // namespace CJ
