#pragma once
#include "qdebug.h"
#include <QThread>
#include <Windows.h>
#include <psapi.h>
#include "tlhelp32.h"
#include <comdef.h>
#include "Utilities.h"
#include "atlstr.h"
#include "VariablesClass.h"
class activeGameThread : public QThread
{
	Q_OBJECT

public:
	enum gameActivityStates {NO_ACTIVE, NO_WINDOW, NO_LOGGED, NO_HANDLER, ACTIVE };
	activeGameThread(QObject *parent, VariablesClass* var);
	~activeGameThread();	
	void run();
	QList<int>* historyOfGameActivity = new QList<int>;
	VariablesClass* var;
	unsigned int PidOfGame;
	QString gameWindowTitle;
signals:
	void GameStateChanged(int i);
private:
	void getListOfProcess(QList<QString>& names, QList<unsigned int>& IDs);
	unsigned int getPIDofProcess(QString nameOfProcess);
	QString getGameWindowTitile();
	unsigned int getPIDofProcess(QString nameOfProcess, QList<QString> names, QList<unsigned int> pids);
	gameActivityStates windowIsAccessible(unsigned int PID, QString windowTitle);
	gameActivityStates checkState();
};
