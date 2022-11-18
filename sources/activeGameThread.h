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
#include <memory>
class activeGameThread : public QThread
{
	Q_OBJECT

public:
	enum gameActivityStates {NO_ACTIVE, NO_WINDOW, NO_LOGGED, NO_HANDLER, ACTIVE };
	activeGameThread(QObject *parent, std::shared_ptr<VariablesClass> var);
	~activeGameThread();	
	void run();
	QList<int>* historyOfGameActivity = new QList<int>;
	std::shared_ptr<VariablesClass> var;
	unsigned int PidOfGame;
	QString gameWindowTitle;
	unsigned int getPIDofProcess(QMap<QString, unsigned int>& processes, QString nameOfProcessToFind);
	gameActivityStates checkState();
	void getListOfProcess(QMap<QString, unsigned int>& processes);
	QString getGameWindowTitile();
	gameActivityStates windowIsAccessible(unsigned int PID, QString windowTitle);
signals:
	void GameStateChanged(int i);
private:
};
