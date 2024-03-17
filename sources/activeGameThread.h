#pragma once
#include <QThread.h>
#include <Windows.h>
#include <atlstr.h>
#include <comdef.h>
#include <psapi.h>
#include <qdebug.h>
#include <qtimer.h>
#include <tlhelp32.h>

#include "VariablesClass.hpp"
class ActiveGameThread : public QObject {
  Q_OBJECT
 public:
  enum GameActivityStates { NO_ACTIVE, NO_WINDOW, NO_LOGGED, NO_HANDLER, ACTIVE };
  ActiveGameThread(QSharedPointer<VariablesClass> var);
  ~ActiveGameThread();

 signals:
  void gameStateChanged(int i);

 private:
  GameActivityStates             previousGameState_   = NO_ACTIVE;
  const QString                  GAME_PROCESS_NAME_   = "client.exe";
  const QString                  GAME_BROWESER_TITLE_ = "Tibia - Free Multiplayer Online Role Playing Game";
  const uint                     TIMER_INTERVAL       = 2000;
  QSharedPointer<VariablesClass> var_;
  HWND                           previousGameHandler_ = 0;
  QTimer                         checkGameStateTimer_;

  uint                        getGamePid(const QMap<QString, unsigned int>& processes) const;
  QString                     getGameWindowTitile() const;
  GameActivityStates          getGameState();
  void                        checkGameState();
  QMap<QString, unsigned int> getListOfRunningProcess();
  int                         windowIsAccessible(const uint PID, const QString& windowTitle);
  HWND                        getHandlerToGameWindow(unsigned int PID, QString WindowName);
};
