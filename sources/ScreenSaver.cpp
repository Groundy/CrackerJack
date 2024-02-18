#include "ScreenSaver.h"

ScreenSaver::ScreenSaver(QObject* parent, QSharedPointer<VariablesClass> var, QSharedPointer<GameConnecter> gameConnecter, Profile* profile)
    : QThread(parent), var(var), gameConnecter(gameConnecter), profile(profile) {
  var->getSettings().setTakingScreensState(true);
}

ScreenSaver::~ScreenSaver() {
  this->terminate();
}

void ScreenSaver::sendScreenRequestToGame() {
  try {
    uint pid = var->getGameProcess().getPid();
    if (pid == 0) throw std::exception("Can't send screenshot key to game, there is no known pid!");
    QString winTitle = var->getGameProcess().getNameOfGameWindow();
    if (winTitle.isEmpty())
      throw std::exception(
          "Can't send screenshot key to game, there is no known title of "
          "window!");
    Key key = profile->getScreenShotKey();
    gameConnecter->sendKeyStrokeToProcess(key);
  } catch (const std::exception& e) {
    qDebug() << e.what();
  }
}

void ScreenSaver::run() {
  setPriority(QThread::Priority::HighestPriority);
  while (true) {
    msleep(SLEEP_TIME);
    if (!var->getSettings().getTakingScreensState()) continue;
    sendScreenRequestToGame();
  }
}
