#pragma once
#include <QThread>
#include "ActiveGameThread.h"
#include "VariablesClass.hpp"
#include "GameConnecter.h"
#include "Profile.h"
class ScreenSaver : public QThread{
public:
	void run();
	ScreenSaver(QObject *parent, QSharedPointer<VariablesClass> var, QSharedPointer<GameConnecter> gameConnector, Profile* prof);
	~ScreenSaver();

private:
	const uint SLEEP_TIME = 985;
	QSharedPointer<VariablesClass> var;
	QSharedPointer<GameConnecter> gameConnecter;	
	Profile* profile;

	void sendScreenRequestToGame();
};
