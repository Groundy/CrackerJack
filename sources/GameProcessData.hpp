#pragma once
#include <Windows.h>
#include <basetsd.h>
#include <qstring.h>
class GameProcessData {
 public:
  GameProcessData()  = default;
  ~GameProcessData() = default;
  uint getPid() const {
    return pid;
  }
  void setPid(const uint newPid) {
    pid = newPid;
  }
  QString getNameOfGameWindow() const {
    return nameOfGameWindow;
  }
  void setNameOfGameWindow(const QString& newNameOfGameWindow) {
    nameOfGameWindow = newNameOfGameWindow;
  }
  HWND getHandlerToGameThread() {
    return handlerToGameThread;
  }
  void setHandlerToGameThread(const HWND newHandlerToGameThread) {
    handlerToGameThread = newHandlerToGameThread;
  }

 private:
  std::atomic<uint> pid;
  std::atomic<HWND> handlerToGameThread;
  QString           nameOfGameWindow;
};
