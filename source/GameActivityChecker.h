#pragma once
#include <QThread>
#include <qdebug.h>
#include <qtimer.h>

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
  GameActivityStates previousGameState_         = GameActivityStates::NO_ACTIVE;
  const QString      game_process_name_         = "client.exe";
  const QString      game_browser_process_name_ = "Tibia - Free Multiplayer Online Role Playing Game";
  const uint         check_interval_            = 2000;
  HWND               previousGameHandler_       = 0;
  QTimer             checkGameStateTimer_;

  QSharedPointer<GameProcessData> game_process_data_;

  uint                getGamePid() const;
  QString             getGameWindowTitile() const;
  GameActivityStates  getGameState();
  void                execute();
  QMap<QString, uint> getListOfRunningProcess_Windows() const;
  GameActivityStates  windowIsAccessible(const uint PID, const QString& windowTitle);
  HWND                getHandlerToGameWindow(const uint PID, const QString& WindowName);
};
}  // namespace CJ
