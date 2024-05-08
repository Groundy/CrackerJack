#include "ScreenSaver.h"
namespace CJ {
ScreenSaver::ScreenSaver(QObject* parent, QSharedPointer<VariablesClass> var, QSharedPointer<GameConnecter> gameConnecter)
    : QThread(parent), game_connecter_(gameConnecter), screenshot_key_(var->getProf()->getScreenShotKey()) {
  settings_          = var->getSettings();
  game_process_data_ = var->getGameProcess();
  settings_->setTakingScreensState(true);
  timer_.setInterval(sleep_time_);
  if (connect(&timer_, &QTimer::timeout, this, &ScreenSaver::execute)) {
    qCritical() << "Failed to connect. Screen saver timer";
    return;
  }
  timer_.start();
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
  game_connecter_->sendKeyStrokeToProcess(screenshot_key_);
}

void ScreenSaver::execute() {
  if (!settings_->getTakingScreensState()) {
    return;
  }
  sendScreenRequestToGame();
}
}  // namespace CJ
