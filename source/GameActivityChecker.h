#pragma once
#include <qdebug.h>
#include <qtimer.h>

#include <QThread>

#include "VariablesClass.hpp"

#ifdef _WIN64
#include <Windows.h>
#include <atlstr.h>
#include <comdef.h>
#include <psapi.h>
#include <tlhelp32.h>
#endif

namespace CJ {
enum class GameActivityStates { NO_ACTIVE, NO_WINDOW, NO_LOGGED, NO_HANDLER, ACTIVE };

class GameActivityChecker : public QObject {
  Q_OBJECT
 public:
  GameActivityChecker(QSharedPointer<VariablesClass> var);
  ~GameActivityChecker();

 signals:
  void gameStateChanged(GameActivityStates state);

 private:
  GameActivityStates previousGameState_ = GameActivityStates::NO_ACTIVE;

  const uint check_interval_ = 2000;
#ifdef _WIN64
  HWND          previousGameHandler_       = 0;
  const QString game_process_name_         = "client.exe";
  const QString game_browser_process_name_ = "Tibia - Free Multiplayer Online Role Playing Game";
#else
  uint previousGameHandler_ = 0;
#endif

  QTimer                          checkGameStateTimer_;
  QSharedPointer<GameProcessData> game_process_data_;
  uint                            getGamePid() const;
  GameActivityStates              getGameState();
  void                            execute();

  GameActivityStates windowIsAccessible(const uint PID, const QString& windowTitle);
#ifdef _WIN64
  HWND                getHandlerToGameWindow(const uint PID, const QString& WindowName);
  QMap<QString, uint> getListOfRunningProcess_Windows() const;
  QString             getGameWindowTitile() const;
#else
  uint getHandlerToGameWindow(const uint PID);
#endif
};
}  // namespace CJ
