#pragma once
#include <qobject.h>
#include <qthread.h>
#include <qtimer.h>
#include <windows.h>

#include "GameConnecter.h"
class ClickDetector : public QThread {
  Q_OBJECT
 public:
  ClickDetector(QObject* parent, QSharedPointer<GameConnecter> gameConnetor) : QThread(parent), game_connetor_(gameConnetor) {
    if (!connect(&timer_, &QTimer::timeout, this, &ClickDetector::execute)) {
      qCritical() << "Connection failed. Click detector timer";
      exit(1);
    }
    timer_.setSingleShot(true);
    timer_.start();
  };
  ~ClickDetector() = default;

 private:
  QSharedPointer<GameConnecter> game_connetor_;
  const uint                    ms_between_clicks_         = 1000;
  const uint                    ms_between_trigger_checks_ = 6;
  QTimer                        timer_;

  void execute() {
    if (middleButtonPressed()) {
      game_connetor_->autoLootAroundPlayer();
      msleep(ms_between_clicks_);
    }
    timer_.start();
  }

  bool keyPressed(const int key) const {
    return (GetAsyncKeyState(key) & 0x8000 != 0) != 0;  //[TODO] move to separate windows funcs class
  }

  bool middleButtonPressed() const {
    return keyPressed(VK_MBUTTON);  //[TODO] move to separate windows funcs class
  }
};
