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
class ActiveGameThread : public QThread
{
	Q_OBJECT

public:
	enum GameActivityStates {NO_ACTIVE, NO_WINDOW, NO_LOGGED, NO_HANDLER, ACTIVE };
	ActiveGameThread(QObject *parent, std::shared_ptr<VariablesClass> var);
	~ActiveGameThread();
	void run();
	QList<int>* historyOfGameActivity = new QList<int>;
	std::shared_ptr<VariablesClass> var;
	unsigned int PidOfGame;
	QString gameWindowTitle;
	unsigned int getPIDofProcess(QMap<QString, unsigned int>& processes, QString nameOfProcessToFind);
	GameActivityStates checkState();
	void getListOfProcess(QMap<QString, unsigned int>& processes);
	QString getGameWindowTitile();
	GameActivityStates windowIsAccessible(unsigned int PID, QString windowTitle);
signals:
	void GameStateChanged(int i);
private:
};
