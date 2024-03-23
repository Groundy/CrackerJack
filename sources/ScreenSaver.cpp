#include "ScreenSaver.h"
namespace CJ {
ScreenSaver::ScreenSaver(QObject* parent, QSharedPointer<VariablesClass> var, QSharedPointer<GameConnecter> gameConnecter)
    : QThread(parent), gameConnecter(gameConnecter), screenshot_key_(var->getProf()->getScreenShotKey()) {
  settings_          = var->getSettings();
  game_process_data_ = var->getGameProcess();
  settings_->setTakingScreensState(true);
}

ScreenSaver::~ScreenSaver() {
  this->terminate();
}

void ScreenSaver::sendScreenRequestToGame() {
  const uint pid = game_process_data_->getPid();
  if (pid == 0) {
    qWarning() << "Can't send screenshot key to game, there is no known pid!";
    return;
  }
  const QString winTitle = game_process_data_->getNameOfGameWindow();
  if (winTitle.isEmpty()) {
    qWarning() << "Can't send screenshot key to game, there is no known title ofwindow";
    return;
  }
  gameConnecter->sendKeyStrokeToProcess(screenshot_key_);
}

void ScreenSaver::run() {
  setPriority(QThread::Priority::HighestPriority);
  while (true) {
    msleep(SLEEP_TIME);
    if (!settings_->getTakingScreensState()) {
      continue;
    }
    sendScreenRequestToGame();
  }
}
}  // namespace CJ
