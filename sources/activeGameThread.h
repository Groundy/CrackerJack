#pragma once
#include "qdebug.h"
#include <QThread>
#include <Windows.h>
#include <psapi.h>
#include "tlhelp32.h"
#include <comdef.h>
#include "Utilities.h"
class activeGameThread : public QThread
{
	Q_OBJECT

public:
	enum gameActivityStates {NO_ACTIVE, NO_WINDOW, NO_LOGGED, NO_HANDLER, ACTIVE };
	activeGameThread(QObject *parent);
	~activeGameThread();	
	void test();
	void run();
signals:
	void GameStateChanged(gameActivityStates);
private:
	void getListOfProcess(QList<QString>& names, QList<unsigned int>& IDs);
	unsigned int getPIDofProcess(QString nameOfProcess);
	LPCWSTR getGameWindowTitile();
	unsigned int getPIDofProcess(QString nameOfProcess, QList<QString> names, QList<unsigned int> pids);
	gameActivityStates windowIsAccessible(unsigned int PID, QList<QString> names);
	gameActivityStates checkState();
};
