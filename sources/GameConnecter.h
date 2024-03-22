#pragma once
#include <comdef.h>
#include <qmutex.h>
#include <qsharedpointer.h>
#include <qthread.h>

#include "Key.h"
#include "Profile.h"
#include "RestorationMethode.hpp"
#include "VariablesClass.hpp"
class GameConnecter : public QThread {
  Q_OBJECT
 public:
  GameConnecter(QSharedPointer<VariablesClass> var);
  ~GameConnecter();
  void run(){};

  void clickLeft(QPoint pt);
  void clickRight(QPoint pt);
  void sendStringToGame(QString str);
  bool sendKeyStrokeToProcess(Key key, int sleepTime = 20);
  bool sendKeyStrokeToProcess(int virtualKey, int sleepTime = 20);
  void useRestorationMethode(const RestorationMethode& methode, int additionalTime = 0);
  void autoLootAroundPlayer();

 private:
  void sendCharToGame(const QChar charToSend, const HWND& gameThreadHandler);
  void clickRightWithShift(QVector<QPoint> pts, int SLEEP_TIME_BETWEEN_LOOT_CLICK);
  void setShiftPressed(const bool pressed);

  QMutex                         sender_mutex_;
  const Profile::AutoLoot        auto_loot_setting_;
  QSharedPointer<VariablesClass> var_;
  Logger&                        logger_                   = Logger::instance();
  const bool                     playing_on_small_monitor_ = false;  //[TODO] move that!
};
