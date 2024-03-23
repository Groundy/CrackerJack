#pragma once
#include <comdef.h>
#include <qmutex.h>
#include <qsharedpointer.h>
#include <qthread.h>

#include "Key.h"
#include "Profile.h"
#include "RestorationMethode.hpp"
#include "VariablesClass.hpp"
namespace CJ {
class GameConnecter : public QThread {
  Q_OBJECT
 public:
  GameConnecter(QSharedPointer<VariablesClass> var);
  ~GameConnecter();
  void run(){};

  void clickLeft(const QPoint pt);
  void clickRight(const QPoint pt);
  void sendStringToGame(const QString& str);
  bool sendKeyStrokeToProcess(const Key key, const uint sleepTime = 20);
  bool sendKeyStrokeToProcess(const int virtualKey, const uint sleepTime = 20);
  void useRestorationMethode(const RestorationMethode& methode, const uint additionalTime = 0);
  void autoLootAroundPlayer();

 private:
  void sendCharToGame(const QChar charToSend, const HWND& gameThreadHandler);
  void clickRightWithShift(const QVector<QPoint>& pts, const uint sleepTimeBetweenClicks);
  void setShiftPressed(const bool pressed);

  QMutex                  sender_mutex_;
  const Profile::AutoLoot auto_loot_setting_;
  Logger&                 logger_                   = Logger::instance();
  const bool              playing_on_small_monitor_ = false;  //[TODO] move that!
  const int               windows_post_param_       = 0x1470001;

  QSharedPointer<GameProcessData> game_process_data_;
  QSharedPointer<Timers>          timers_;
  QSharedPointer<GameWindow>      game_window_;
};
}  // namespace CJ
