#include "ActiveGameThread.h"

ActiveGameThread::ActiveGameThread(QSharedPointer<VariablesClass> var) : var_(var) {
  checkGameStateTimer_.setInterval(TIMER_INTERVAL);
  if (!QObject::connect(&checkGameStateTimer_, &QTimer::timeout, this, &ActiveGameThread::checkGameState, Qt::UniqueConnection)) {
    qCritical() << "Failed to connect game activity timer.";
    exit(1);
  }
  checkGameStateTimer_.start();
}

ActiveGameThread::~ActiveGameThread() {
  checkGameStateTimer_.stop();
}

void ActiveGameThread::checkGameState() {
  GameActivityStates gameStateCode = getGameState();
  if (gameStateCode != previousGameState_) {
    emit gameStateChanged(gameStateCode);
  }
}

QString ActiveGameThread::getGameWindowTitile() const {
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

    if (title.contains(GAME_BROWESER_TITLE_)) {
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
uint ActiveGameThread::getGamePid(const QMap<QString, unsigned int>& processes) const {
  auto iteratorToProcess = processes.find(GAME_PROCESS_NAME_);
  if (iteratorToProcess == processes.end()) {
    qDebug() << "Can't find Game on running processes list!";
    return 0;
  }
  return iteratorToProcess.value();
}
int ActiveGameThread::windowIsAccessible(const uint PID, const QString& windowTitle) {
  if (PID == 0) {
    return NO_ACTIVE;
  }
  if (windowTitle.isEmpty()) {
    return NO_WINDOW;
  } else if (windowTitle == "Tibia") {
    return NO_LOGGED;
  }

  LPCWSTR nameOfWindowLPCWSTR = (const wchar_t*)windowTitle.utf16();
  HWND    handler             = FindWindow(NULL, nameOfWindowLPCWSTR);
  if (handler == NULL) {
    return NO_HANDLER;
  }
  DWORD tmp     = PID;
  DWORD hThread = GetWindowThreadProcessId(handler, &tmp);
  if (hThread == NULL) {
    return NO_HANDLER;
  }
  return ACTIVE;
}

ActiveGameThread::GameActivityStates ActiveGameThread::getGameState() {
  auto             processes    = getListOfRunningProcess();
  uint             PID          = getGamePid(processes);
  QString          windowTitle  = getGameWindowTitile();
  GameProcessData& process_data = var_->getGameProcess();

  if (windowTitle.isEmpty()) {
    qWarning() << "Can't get game window title.";
    process_data.setNameOfGameWindow("");
    process_data.setPid(0);
    return NO_WINDOW;
  }
  int gameWinState = windowIsAccessible(PID, windowTitle);
  if (gameWinState == ACTIVE) {
    HWND handlerToGameThread = getHandlerToGameWindow(PID, windowTitle);

    if (process_data.getNameOfGameWindow() != windowTitle) {
      process_data.setNameOfGameWindow(windowTitle);
    }
    if (handlerToGameThread != previousGameHandler_) {
      process_data.setHandlerToGameThread(handlerToGameThread);
    }
    if (process_data.getPid() != PID) {
      process_data.setPid(PID);
    }
  }
  return GameActivityStates(gameWinState);
}
QMap<QString, unsigned int> ActiveGameThread::getListOfRunningProcess() {
  QMap<QString, unsigned int> toRet;
  HANDLE                      hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hSnapshot) {
    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnapshot, &pe32)) {
      do {
        unsigned int ID   = (unsigned int)pe32.th32ProcessID;
        WCHAR*       name = pe32.szExeFile;
        _bstr_t      b(name);
        const char*  nameCC = b;
        toRet.insert(QString(nameCC), ID);
      } while (Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
  }
  return toRet;
}
HWND ActiveGameThread::getHandlerToGameWindow(unsigned int PID, QString WindowName) {
  LPCWSTR nameOfWindowLPCWSTR = (const wchar_t*)WindowName.utf16();
  HWND    handler             = FindWindow(NULL, nameOfWindowLPCWSTR);
  if (handler == NULL) {
    // Logger::logPotenialBug("Can't get handler to window: " + WindowName,
    // "Utilities", "clickRight");
    return HWND();
  }
  DWORD tmp     = PID;
  DWORD hThread = GetWindowThreadProcessId(handler, &tmp);

  if (hThread != NULL) {
    return handler;
  } else {
    // Logger::logPotenialBug("Can't get thread PID for used handler",
    // "Utilities", "clickRight");
    return HWND();
  }
}
