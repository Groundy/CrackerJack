#include "activeGameThread.h"


activeGameThread::activeGameThread(QObject *parent)
	: QThread(parent){
	bool good = connect(this, SIGNAL(GameStateChanged(gameActivityStates)), parent, SLOT(onGameStateChanged(gameActivityStates)));

}

activeGameThread::~activeGameThread(){
	qDebug() << "end";
}

void activeGameThread::run(){
	test();
}

void activeGameThread::test(){
    while (true) {
        gameActivityStates tt = checkState();
        qDebug() << QString::number(tt);
        msleep(1500);
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

LPCWSTR activeGameThread::getGameWindowTitile(){
    QString toRet;

    return (const wchar_t*)toRet.utf16();
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
    LPCWSTR nameOfWindowLPCWSTR = getGameWindowTitile();
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

