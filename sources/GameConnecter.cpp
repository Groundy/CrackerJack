#include "GameConnecter.h"

GameConnecter::GameConnecter(QSharedPointer<VariablesClass> var)
    : var_(var),
      playing_on_small_monitor_(var->getSettings().getPlayingOnSmallMonitor()),
      auto_loot_setting_(var->getProf()->getAutoLoot()) {}

GameConnecter::~GameConnecter() {
  setShiftPressed(false);
}

void GameConnecter::clickLeft(QPoint pt) {
  if (playing_on_small_monitor_) {
    pt = QPoint(pt.x() / 0.8, pt.y() / 0.8);
  }
  HWND         gameThreadHandler = var_->getGameProcess().getHandlerToGameThread();
  LPARAM       lParam            = (pt.y() << 16) + pt.x();
  QMutexLocker locker(&sender_mutex_);
  PostMessage(gameThreadHandler, WM_LBUTTONDOWN, 1, lParam);
  PostMessage(gameThreadHandler, WM_LBUTTONUP, 1, lParam);
}
void GameConnecter::clickRight(const QPoint pt) {
  const QPoint to_click          = (playing_on_small_monitor_) ? QPoint(pt.x() / 0.8, pt.y() / 0.8) : pt;
  const HWND   gameThreadHandler = var_->getGameProcess().getHandlerToGameThread();
  const LPARAM lParam            = (to_click.y() << 16) + to_click.x();
  QMutexLocker locker(&sender_mutex_);
  PostMessage(gameThreadHandler, WM_RBUTTONDOWN, 0, lParam);
  PostMessage(gameThreadHandler, WM_RBUTTONUP, 0, lParam);
}
void GameConnecter::clickRightWithShift(const QVector<QPoint>& pts, const uint sleepTimeBetweenClicks) {
  HWND gameThreadHandler = var_->getGameProcess().getHandlerToGameThread();

  QMutexLocker locker(&sender_mutex_);
  setShiftPressed(true);
  foreach (auto pt, pts) {
    if (playing_on_small_monitor_) {
      pt = QPoint(pt.x() / 0.8, pt.y() / 0.8);
    }
    LPARAM lParam = (pt.y() << 16) + pt.x();
    msleep(sleepTimeBetweenClicks);
    PostMessage(gameThreadHandler, WM_RBUTTONDOWN, 0, lParam);
    msleep(sleepTimeBetweenClicks);
    PostMessage(gameThreadHandler, WM_RBUTTONUP, 0, lParam);
    msleep(sleepTimeBetweenClicks);
  }
  setShiftPressed(false);
}
bool GameConnecter::sendKeyStrokeToProcess(const Key key, const uint sleepTime) {
  const HWND   gameThreadHandler = var_->getGameProcess().getHandlerToGameThread();
  QMutexLocker locker(&sender_mutex_);
  PostMessage(gameThreadHandler, WM_KEYDOWN, key.getKeyVal(), windows_post_param_);
  msleep(sleepTime);
  PostMessage(gameThreadHandler, WM_KEYUP, key.getKeyVal(), windows_post_param_);
  return true;
}

bool GameConnecter::sendKeyStrokeToProcess(const int virtualKey, const uint sleepTime) {
  const HWND   gameThreadHandler = var_->getGameProcess().getHandlerToGameThread();
  QMutexLocker locker(&sender_mutex_);
  PostMessage(gameThreadHandler, WM_KEYDOWN, virtualKey, windows_post_param_);
  msleep(sleepTime);
  PostMessage(gameThreadHandler, WM_KEYUP, virtualKey, windows_post_param_);
  return true;
}

void GameConnecter::sendStringToGame(const QString& str) {
  HWND gameThreadHandler = var_->getGameProcess().getHandlerToGameThread();
  for each (QChar charToSend in str) {
    sendCharToGame(charToSend, gameThreadHandler);
    msleep(2);
  }
}

void GameConnecter::useRestorationMethode(const RestorationMethode& methode, const uint additionalTime) {
  Timers& timers = var_->getTimers();
  if (methode.isSpell()) {
    timers.setTimeLastSpellUsageHealing();
    timers.setTimeLastSpellUsed(methode.getName());
  } else if (methode.isPotion()) {
    timers.setTimeLastItemUsageGeneral();
    timers.setTimeLastItemUsed(methode.getName(), additionalTime);
  }

  sendKeyStrokeToProcess(methode.getKey());
  logger_.log(methode.getName(), false, true, true);
}

void GameConnecter::sendCharToGame(const QChar charToSend, const HWND& gameThreadHandler) {
  QMutexLocker locker(&sender_mutex_);
  if (charToSend.isLetter()) {
    const WPARAM wParam = charToSend.toUpper().unicode();
    PostMessage(gameThreadHandler, WM_KEYDOWN, wParam, 1);
  } else if (charToSend.isSpace()) {
    const WPARAM wParam = 0x20;
    PostMessage(gameThreadHandler, WM_KEYDOWN, wParam, 1);
  } else if (charToSend.isDigit()) {
    uint         value  = charToSend.unicode() - 48;
    const WPARAM wParam = charToSend.unicode();
    const LPARAM lParam = ((value + 1) << 16) + 1;
    PostMessage(gameThreadHandler, WM_KEYDOWN, wParam, lParam);
  } else if (charToSend.unicode() == 39) {
    // apostrophe mark
    const WPARAM wParam = 0xDE;
    const LPARAM lParam = 0x00280001;
    PostMessage(gameThreadHandler, WM_KEYDOWN, wParam, lParam);
  } else if (charToSend.unicode() == 45) {
    // dash mark
    const WPARAM wParam = 0xBD;
    const LPARAM lParam = 0x000C001;
    PostMessage(gameThreadHandler, WM_KEYDOWN, wParam, lParam);
  }
}
void GameConnecter::autoLootAroundPlayer() {
  const auto     potinsToClick             = var_->getGameWindow().getPointsOfFieldsAroundPlayer();
  constexpr uint sleep_time_between_clicks = 120;

  if (auto_loot_setting_ == Profile::RIGHT_MOUSE_BUTTON) {
    foreach (const QPoint& pt, potinsToClick) {
      clickRight(pt);
      msleep(sleep_time_between_clicks);
    }
  } else if (auto_loot_setting_ == Profile::SHIFT_RIGHT) {
    clickRightWithShift(potinsToClick, sleep_time_between_clicks * 0.33);
    setShiftPressed(false);
  } else if (auto_loot_setting_ == Profile::LEFT_MOUSE_BUTTON) {
    foreach (const QPoint& pt, potinsToClick) {
      clickLeft(pt);
      msleep(sleep_time_between_clicks);
    }
  }

  logger_.log("auto loot.", false, true, true);
}
void GameConnecter::setShiftPressed(const bool pressed) {
  INPUT ip{};
  ip.type           = INPUT_KEYBOARD;  // Set up a generic keyboard event.
  ip.ki.wScan       = 0;               // hardware scan code for key
  ip.ki.time        = 0;
  ip.ki.dwExtraInfo = 0;
  ip.ki.wVk         = VK_SHIFT;  // virtual-key code for the "a" key
  ip.ki.dwFlags     = pressed ? 0 : KEYEVENTF_KEYUP;
  SendInput(1, &ip, sizeof(INPUT));
};
