#include "activeGameThread.h"


activeGameThread::activeGameThread(QObject *parent)
	: QThread(parent){
    
    const char* signal = SIGNAL(GameStateChanged(int));
    const char* slot = SLOT(onGameStateChanged(int));
    bool good = connect(this, signal, parent, slot);
}

activeGameThread::~activeGameThread(){
	qDebug() << "end";
}

void activeGameThread::run(){
    while (true) {
        int stateCode = (int)checkState();
        emit GameStateChanged(stateCode);
        msleep(1234);
    }
}

unsigned int activeGameThread::getPIDofProcess(QString nameOfProcess) {
    QList<QString> names;
    QList<unsigned int> ids;
    unsigned int toRet;
    getListOfProcess(names, ids);
    for (int i = 0; i < names.size(); i++) {
        if (names[i].contains(nameOfProcess)) {
            if (ids.size() >= i)
                return ids[i];
            else
                return 0;
        }
    }
    return 0;
}

QString activeGameThread::getGameWindowTitile(){
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

        if(title.contains(partOfBrowserTitle))//broweser
            continue;

        if (title.contains("Tibia - "))
            return title;

        if(title.contains("Tibia"))
            return QString("Tibia");
    }   
    return QString();
}

unsigned int activeGameThread::getPIDofProcess(QString nameOfProcess, QList<QString> names, QList<unsigned int> pids){
    for (int i = 0; i < names.size(); i++) {
        if (names[i].contains(nameOfProcess)) {
            if (pids.size() >= i)
                return pids[i];
            else
                return 0;
        }
    }
    return 0;
}

activeGameThread::gameActivityStates activeGameThread::windowIsAccessible(unsigned int PID, QList<QString> names){
    QString winName = getGameWindowTitile();
    if (winName.isEmpty())
        return NO_WINDOW;
    else if (winName == "Tibia")
        return NO_LOGGED;
    LPCWSTR nameOfWindowLPCWSTR = (const wchar_t*)winName.utf16();
    HWND handler = FindWindow(NULL, nameOfWindowLPCWSTR);
    if (handler == NULL)
        return NO_HANDLER;
    DWORD tmp = PID;
    DWORD hThread = GetWindowThreadProcessId(handler, &tmp);
    if (hThread == NULL)
        return NO_HANDLER;
    return ACTIVE;
}

activeGameThread::gameActivityStates activeGameThread::checkState(){
    QList<QString> names;
    QList<unsigned int> pids;
    getListOfProcess(names, pids);
    unsigned int PID = getPIDofProcess("client.exe", names, pids);
    if (PID == 0)
        return NO_ACTIVE;
    gameActivityStates gameWinState = windowIsAccessible(PID, names);
        return gameWinState;
}

void activeGameThread::getListOfProcess(QList<QString>& names, QList<unsigned int>& IDs) {
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
                IDs.push_back(ID);
                names.push_back(QString(nameCC));
            } while (Process32Next(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
    }
}

