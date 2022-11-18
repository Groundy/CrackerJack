#include "ActiveGameThread.h"

ActiveGameThread::ActiveGameThread(QObject *parent, std::shared_ptr< VariablesClass> var)
	: QThread(parent), var(var){
    const char* signal = SIGNAL(GameStateChanged(int));
    const char* slot = SLOT(onGameStateChanged(int));
    bool good = connect(this, signal, parent, slot);
}

ActiveGameThread::~ActiveGameThread(){
    this->terminate();
    delete historyOfGameActivity;
}

void ActiveGameThread::run(){
    while (true) {
        int stateCode = (int)checkState();
        //That loops stores history of gamestates, signal is emited only if gameStateWasChanged
        if (historyOfGameActivity->size() < 10) {
            historyOfGameActivity->push_back(stateCode);
            emit GameStateChanged(stateCode);
            continue;
        }
        else if (historyOfGameActivity->size() == 10){
            historyOfGameActivity->removeFirst();
            historyOfGameActivity->push_back(stateCode);
        }
        for each (int var in *historyOfGameActivity){
            if (var != historyOfGameActivity->first()){
                emit GameStateChanged(stateCode);
                break;
            }
        }
        msleep(1234);
    }
}

QString ActiveGameThread::getGameWindowTitile(){
    const QString partOfBrowserTitle = "Tibia - Free Multiplayer Online Role Playing Game";
  
    for (HWND hwnd = GetTopWindow(NULL); hwnd != NULL; hwnd = GetNextWindow(hwnd, GW_HWNDNEXT))
    {
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

        if(title.contains(partOfBrowserTitle))//browser
            continue;

        if (title.contains("Tibia - "))
            return title;

        if(title.contains("Tibia"))
            return QString("Tibia");
    }   
    return QString();
}

unsigned int ActiveGameThread::getPIDofProcess(QMap<QString, unsigned int>& processes, QString nameOfProcessToFind){
    auto iteratorToProcess = processes.find(nameOfProcessToFind);
    if (iteratorToProcess == processes.end())
        return 0;

    return iteratorToProcess.value();
}

ActiveGameThread::GameActivityStates ActiveGameThread::windowIsAccessible(const unsigned int PID, QString windowTitle){
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

ActiveGameThread::GameActivityStates ActiveGameThread::checkState(){
    QMap<QString, unsigned int> processes;
    getListOfProcess(processes);
    unsigned int PID = getPIDofProcess(processes, "client.exe");
    QString windowTitle = getGameWindowTitile();
    GameActivityStates gameWinState = windowIsAccessible(PID, windowTitle);
    if (gameWinState == ACTIVE) {
        if (var->var_winTitleOfGame != windowTitle)
            var->var_winTitleOfGame = windowTitle;
        if (var->var_pidOfGame != PID)
            var->var_pidOfGame = PID;
    }
    else{
        if (!gameWindowTitle.isEmpty())
            gameWindowTitle = QString();
        if (PidOfGame != 0)
            PidOfGame = 0;
    }
    return gameWinState;
}

void ActiveGameThread::getListOfProcess(QMap<QString, unsigned int>& processes) {
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
                processes.insert(QString(nameCC), ID);
            } while (Process32Next(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
    }
}

