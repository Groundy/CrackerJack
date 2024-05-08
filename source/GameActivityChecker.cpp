#include "GameActivityChecker.h"
#include <QProcess>
namespace CJ {
GameActivityChecker::GameActivityChecker(QSharedPointer<VariablesClass> var) {
  game_process_data_ = var->getGameProcess();

  checkGameStateTimer_.setInterval(check_interval_);
  execute();
  if (!QObject::connect(&checkGameStateTimer_, &QTimer::timeout, this, &GameActivityChecker::execute, Qt::UniqueConnection)) {
    qCritical() << "Failed to connect game activity timer.";
    exit(1);
  }
  checkGameStateTimer_.start();
}

GameActivityChecker::~GameActivityChecker() {
  checkGameStateTimer_.stop();
}

void GameActivityChecker::execute() {
  GameActivityStates gameStateCode = getGameState();
  if (gameStateCode != previousGameState_) {
    emit gameStateChanged(gameStateCode);
  }
}

QString GameActivityChecker::getGameWindowTitile() const {
  for (HWND hwnd = GetTopWindow(NULL); hwnd != NULL; hwnd = GetNextWindow(hwnd, GW_HWNDNEXT)) {
    if (!IsWindowVisible(hwnd)) {
      continue;
    }
    const uint length = GetWindowTextLength(hwnd);
    if (length == 0) {
      continue;
    }

    wchar_t* tmp = new wchar_t[length + 1];
    GetWindowText(hwnd, tmp, length + 1);
    QString title = QString::fromWCharArray(tmp);
    delete[] tmp;

    if (title == "Program Manager") {
      continue;
    }

    if (title.contains(game_browser_process_name_)) {
      continue;
    }

    if (title.contains("Tibia - ")) {
      return title;
    }

    if (title.contains("Tibia")) {
      return QString("Tibia");
    }
  }
  return QString();
}

uint GameActivityChecker::getGamePid() const {
  const QMap<QString, uint> processes         = getListOfRunningProcess();
  auto                      iteratorToProcess = processes.find(game_process_name_);
  if (iteratorToProcess == processes.end()) {
    qDebug() << "Can't find Game on running processes list!";
    return 0;
  }
  return iteratorToProcess.value();
}

GameActivityStates GameActivityChecker::windowIsAccessible(const uint PID, const QString& windowTitle) {
  if (PID == 0) {
    return GameActivityStates::NO_ACTIVE;
  }
  if (windowTitle.isEmpty()) {
    return GameActivityStates::NO_WINDOW;
  } else if (windowTitle == "Tibia") {
    return GameActivityStates::NO_LOGGED;
  }

  LPCWSTR nameOfWindowLPCWSTR = (const wchar_t*)windowTitle.utf16();
  HWND    handler             = FindWindow(NULL, nameOfWindowLPCWSTR);
  if (handler == NULL) {
    return GameActivityStates::NO_HANDLER;
  }
  DWORD tmp     = PID;
  DWORD hThread = GetWindowThreadProcessId(handler, &tmp);
  if (hThread == NULL) {
    return GameActivityStates::NO_HANDLER;
  }
  return GameActivityStates::ACTIVE;
}

GameActivityStates GameActivityChecker::getGameState() {
  const uint    PID         = getGamePid();
  const QString windowTitle = getGameWindowTitile();

  if (windowTitle.isEmpty()) {
    qWarning() << "Can't get game window title.";
    game_process_data_->setNameOfGameWindow("");
    game_process_data_->setPid(0);
    return GameActivityStates::NO_WINDOW;
  }
  GameActivityStates gameWinState = windowIsAccessible(PID, windowTitle);
  if (gameWinState == GameActivityStates::ACTIVE) {
    HWND handlerToGameThread = getHandlerToGameWindow(PID, windowTitle);

    if (game_process_data_->getNameOfGameWindow() != windowTitle) {
      game_process_data_->setNameOfGameWindow(windowTitle);
    }
    if (handlerToGameThread != previousGameHandler_) {
      game_process_data_->setHandlerToGameThread(handlerToGameThread);
    }
    if (game_process_data_->getPid() != PID) {
      game_process_data_->setPid(PID);
    }
  }
  return GameActivityStates(gameWinState);
}
QMap<QString, uint> GameActivityChecker::getListOfRunningProcess_Windows() const {
  #ifdef _WIN64
  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (!hSnapshot) {
    return QMap<QString, uint>();
  }

  PROCESSENTRY32 pe32{};
  pe32.dwSize = sizeof(PROCESSENTRY32);
  if (!Process32First(hSnapshot, &pe32)) {
    CloseHandle(hSnapshot);
    return QMap<QString, uint>();
  }

  QMap<QString, uint> toRet;
  do {
    unsigned int ID   = (unsigned int)pe32.th32ProcessID;
    WCHAR*       name = pe32.szExeFile;
    _bstr_t      b(name);
    const char*  nameCC = b;
    toRet.insert(QString(nameCC), ID);
  } while (Process32Next(hSnapshot, &pe32));
  CloseHandle(hSnapshot);
  return toRet;
  #endif


  return QMap<QString, uint>();
}

HWND GameActivityChecker::getHandlerToGameWindow(const uint PID, const QString& windowName) {
  LPCWSTR nameOfWindowLPCWSTR = (const wchar_t*)windowName.utf16();
  HWND    handler             = FindWindow(NULL, nameOfWindowLPCWSTR);
  if (handler == NULL) {
    // Logger::logPotenialBug("Can't get handler to window: " + WindowName,
    // "Utilities", "clickRight");
    return HWND();
  }
  DWORD tmp     = PID;
  DWORD hThread = GetWindowThreadProcessId(handler, &tmp);

  if (hThread == NULL) {
    // Logger::logPotenialBug("Can't get thread PID for used handler",
    // "Utilities", "clickRight");
    return HWND();
  }

  return handler;
}
}  // namespace CJ
