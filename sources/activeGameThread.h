#pragma once
#include <qdebug.h>
#include <QThread>
#include <Windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <atlstr.h>
#include <comdef.h>
#include <memory>

#include "Utilities.h"
#include "VariablesClass.h"
class ActiveGameThread : public QThread
{
	Q_OBJECT
public:
	enum GameActivityStates {NO_ACTIVE, NO_WINDOW, NO_LOGGED, NO_HANDLER, ACTIVE };
	ActiveGameThread(QObject *parent, std::shared_ptr<VariablesClass> var);
	~ActiveGameThread();
	void run();

signals:
	void GameStateChanged(int i);

private:
	int previousGameState = -1;
	const QString GAME_PROCESS_NAME = "client.exe";
	const QString GAME_BROWESER_TITLE = "Tibia - Free Multiplayer Online Role Playing Game";
	const uint SLEEP_TIME = 1234;
	std::shared_ptr<VariablesClass> var;

	uint getGamePid(QMap<QString, unsigned int>& processes);
	QString getGameWindowTitile();
	int checkGameState();
	QMap<QString, unsigned int> getListOfRunningProcess();
	void connectSingalToGUI(QObject* parent) {
		const char* signal = SIGNAL(GameStateChanged(int));
		const char* slot = SLOT(onGameStateChanged(int));
		bool good = connect(this, signal, parent, slot);
	}
	int windowIsAccessible(unsigned int PID, QString windowTitle);
};
