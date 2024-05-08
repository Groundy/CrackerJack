#pragma once
#ifdef _WIN64
#include <Windows.h>
#include <basetsd.h>
#endif

#include <qstring.h>
namespace CJ {
class GameProcessData {
 public:
  GameProcessData()  = default;
  ~GameProcessData() = default;
  uint getPid() const {
    return pid_;
  }
  void setPid(const uint newPid) {
    pid_ = newPid;
  }
  QString getNameOfGameWindow() const {
    return nameOfGameWindow_;
  }
  void setNameOfGameWindow(const QString& newNameOfGameWindow) {
    nameOfGameWindow_ = newNameOfGameWindow;
  }
  #ifdef _WIN64
  HWND getHandlerToGameThread() {
    return handlerToGameThread_;
  }
  void setHandlerToGameThread(const HWND& newHandlerToGameThread) {
    handlerToGameThread_ = newHandlerToGameThread;
  }
  #endif
 private:
  std::atomic<uint> pid_                 = 0x0;
  #ifdef _WIN64
  std::atomic<HWND> handlerToGameThread_ = HWND();
  #endif
  QString           nameOfGameWindow_    = QString();
};
}  // namespace CJ
