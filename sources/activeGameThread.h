#pragma once
#include <QThread.h>
#include <Windows.h>
#include <atlstr.h>
#include <comdef.h>
#include <psapi.h>
#include <qdebug.h>
#include <tlhelp32.h>

#include "VariablesClass.hpp"
class ActiveGameThread : public QThread {
  Q_OBJECT
 public:
  enum GameActivityStates { NO_ACTIVE, NO_WINDOW, NO_LOGGED, NO_HANDLER, ACTIVE };
  ActiveGameThread(QObject* parent, QSharedPointer<VariablesClass> var);
  ~ActiveGameThread();
  void run();

 signals:
  void GameStateChanged(int i);

 private:
  int                            previousGameState_   = -1;
  const QString                  GAME_PROCESS_NAME_   = "client.exe";
  const QString                  GAME_BROWESER_TITLE_ = "Tibia - Free Multiplayer Online Role Playing Game";
  const uint                     SLEEP_TIME_          = 1234;
  QSharedPointer<VariablesClass> var_;
  HWND                           previousGameHandler_ = 0;

  uint                        getGamePid(const QMap<QString, unsigned int>& processes);
  QString                     getGameWindowTitile();
  int                         checkGameState();
  QMap<QString, unsigned int> getListOfRunningProcess();
  int                         windowIsAccessible(const uint PID, const QString& windowTitle);
  HWND                        getHandlerToGameWindow(unsigned int PID, QString WindowName);
};
