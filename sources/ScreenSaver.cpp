#include "ScreenSaver.h"

ScreenSaver::ScreenSaver(QObject* parent, QSharedPointer<VariablesClass> var, QSharedPointer<GameConnecter> gameConnecter, Profile* profile)
    : QThread(parent), var(var), gameConnecter(gameConnecter), profile(profile) {
  var->getSettings().setTakingScreensState(true);
}

ScreenSaver::~ScreenSaver() {
  this->terminate();
}

void ScreenSaver::sendScreenRequestToGame() {
  const uint pid = var->getGameProcess().getPid();
  if (pid == 0) {
    qWarning() << "Can't send screenshot key to game, there is no known pid!";
    return;
  }
  const QString winTitle = var->getGameProcess().getNameOfGameWindow();
  if (winTitle.isEmpty()) {
    qWarning() << "Can't send screenshot key to game, there is no known title ofwindow";
    return;
  }
  const Key key = profile->getScreenShotKey();
  gameConnecter->sendKeyStrokeToProcess(key);
}

void ScreenSaver::run() {
  setPriority(QThread::Priority::HighestPriority);
  while (true) {
    msleep(SLEEP_TIME);
    if (!var->getSettings().getTakingScreensState()) {
      continue;
    }
    sendScreenRequestToGame();
  }
}
