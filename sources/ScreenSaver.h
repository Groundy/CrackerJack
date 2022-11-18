#pragma once
#include <QThread>
#include "activeGameThread.h"
#include "VariablesClass.h"
#include "GameConnecter.h"
#include <memory>
class ScreenSaver : public QThread{
public:
	bool enableScreenCapture = true;
	void run();
	int timeBetweenScreens = 550;
	ScreenSaver(QObject *parent, VariablesClass *varClass, std::shared_ptr<GameConnecter> gameConnector);
	~ScreenSaver();
	void sendScreenRequestToGame(Key keyCodeForScreen, VariablesClass* var);
private:
	void screenLoop();
	VariablesClass* variables;
	std::shared_ptr<GameConnecter> gameConnecter;
};
