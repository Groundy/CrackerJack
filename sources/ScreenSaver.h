#pragma once
#include <QThread>
#include <memory>

#include "ActiveGameThread.h"
#include "VariablesClass.h"
#include "GameConnecter.h"
#include "Profile.h"
class ScreenSaver : public QThread{
public:
	void run();
	ScreenSaver(QObject *parent, std::shared_ptr<VariablesClass> var, std::shared_ptr<GameConnecter> gameConnector, Profile* prof);
	~ScreenSaver();

private:
	const uint SLEEP_TIME = 550;
	std::shared_ptr<VariablesClass> var;
	std::shared_ptr<GameConnecter> gameConnecter;	
	Profile* profile;

	void threadLoop();
	void sendScreenRequestToGame();
};
