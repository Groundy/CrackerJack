#pragma once
#include <QThread>

#include "ActiveGameThread.h"
#include "GameConnecter.h"
#include "Profile.h"
#include "VariablesClass.hpp"
class ScreenSaver : public QThread {
 public:
  void run();
  ScreenSaver(QObject* parent, QSharedPointer<VariablesClass> var, QSharedPointer<GameConnecter> gameConnector);
  ~ScreenSaver();

 private:
  const uint                     SLEEP_TIME = 985;
  QSharedPointer<VariablesClass> var;
  QSharedPointer<GameConnecter>  gameConnecter;

  void sendScreenRequestToGame();
};
