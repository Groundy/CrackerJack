#include "ActiveGameThread.h"

ActiveGameThread::ActiveGameThread(QObject *parent, std::shared_ptr< VariablesClass> var)
	: QThread(parent), var(var){
    connectSingalToGUI(parent);
}
ActiveGameThread::~ActiveGameThread(){
    this->terminate();
}
void ActiveGameThread::run(){
    while (true) {
        int gameStateCode = checkGameState();
        if (gameStateCode != previousGameState)
            emit GameStateChanged(gameStateCode);
        msleep(SLEEP_TIME);
    }
}
QString ActiveGameThread::getGameWindowTitile(){
    for (HWND hwnd = GetTopWindow(NULL); hwnd != NULL; hwnd = GetNextWindow(hwnd, GW_HWNDNEXT)){
        if (!IsWindowVisible(hwnd))
            continue;

        int length = GetWindowTextLength(hwnd);
        if (length == 0)
            continue;

        wchar_t* tmp = new wchar_t[length + 1];
        GetWindowText(hwnd, tmp, length + 1);
        std::wstring wStrTitle(tmp);
        //possible problems with converting wstr to str
        std::string strTitle(wStrTitle.begin(), wStrTitle.end());
        const char* titleAsChars = strTitle.c_str();
        QString title(titleAsChars);

        if (title == "Program Manager")
            continue;

        if(title.contains(GAME_BROWESER_TITLE))//browser
            continue;

        if (title.contains("Tibia - "))
            return title;

        if(title.contains("Tibia"))
            return QString("Tibia");
    }   
    return QString();
}
uint ActiveGameThread::getGamePid(QMap<QString, unsigned int>& processes){
    try{
        auto iteratorToProcess = processes.find(GAME_PROCESS_NAME);
        if (iteratorToProcess == processes.end())
            throw std::exception("Can't find Game on running processes list!");
        return iteratorToProcess.value();
    }
    catch (const std::exception& e){
        qDebug() << e.what();
        return 0;
    }

}
int ActiveGameThread::windowIsAccessible(const uint PID, QString windowTitle){
    if (PID == 0)
        return NO_ACTIVE;
    if (windowTitle.isEmpty())
        return NO_WINDOW;
    else if (windowTitle == "Tibia")
        return NO_LOGGED;

    LPCWSTR nameOfWindowLPCWSTR = (const wchar_t*)windowTitle.utf16();
    HWND handler = FindWindow(NULL, nameOfWindowLPCWSTR);
    if (handler == NULL)
        return NO_HANDLER;
    DWORD tmp = PID;
    DWORD hThread = GetWindowThreadProcessId(handler, &tmp);
    if (hThread == NULL)
        return NO_HANDLER;
    return ACTIVE;
}
int ActiveGameThread::checkGameState(){
    try {
        auto processes = getListOfRunningProcess();
        uint PID = getGamePid(processes);
        QString windowTitle = getGameWindowTitile();
        if (windowTitle.isEmpty())
            throw std::exception("Can't get GameWindowTitle");
        int gameWinState = windowIsAccessible(PID, windowTitle);
        if (gameWinState == ACTIVE) {     
            HWND handlerToGameThread = getHandlerToGameWindow(PID, windowTitle);
            if (handlerToGameThread != previousGameHandler)
                var->getGameProcess().setHandlerToGameThread(handlerToGameThread);
            if (var->getGameProcess().getNameOfGameWindow() != windowTitle)
                var->getGameProcess().setNameOfGameWindow(windowTitle);
            if (var->getGameProcess().getPid() != PID)
                var->getGameProcess().setPid(PID);
        }
        return gameWinState;
    }
    catch (const std::exception& e){
        var->getGameProcess().setNameOfGameWindow("");
        var->getGameProcess().setPid(0);
        return NO_WINDOW;
    }
}
QMap<QString, unsigned int> ActiveGameThread::getListOfRunningProcess() {
    QMap<QString, unsigned int> toRet;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot) {
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &pe32)) {
            do {
                unsigned int ID = (unsigned int)pe32.th32ProcessID;
                WCHAR* name = pe32.szExeFile;
                _bstr_t b(name);
                const char* nameCC = b;
                toRet.insert(QString(nameCC), ID);
            } while (Process32Next(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
    }
    return toRet;
}
HWND ActiveGameThread::getHandlerToGameWindow(unsigned int PID, QString WindowName) {
    LPCWSTR nameOfWindowLPCWSTR = (const wchar_t*)WindowName.utf16();
    HWND handler = FindWindow(NULL, nameOfWindowLPCWSTR);
    if (handler == NULL) {
        //Logger::logPotenialBug("Can't get handler to window: " + WindowName, "Utilities", "clickRight");
        return HWND();
    }
    DWORD tmp = PID;
    DWORD hThread = GetWindowThreadProcessId(handler, &tmp);

    if (hThread != NULL)
        return handler;
    else {
        //Logger::logPotenialBug("Can't get thread PID for used handler", "Utilities", "clickRight");
        return HWND();
    }
}
void ActiveGameThread::connectSingalToGUI(QObject* parent) {
    const char* signal = SIGNAL(GameStateChanged(int));
    const char* slot = SLOT(onGameStateChanged(int));
    bool good = connect(this, signal, parent, slot);
}

