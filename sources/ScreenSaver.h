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

	ScreenSaver(QObject *parent, std::shared_ptr<VariablesClass> varClass, std::shared_ptr<GameConnecter> gameConnector);
	~ScreenSaver();

private:
	int timeBetweenScreens = 550;
	void screenLoop();
	std::shared_ptr<VariablesClass> var;
	std::shared_ptr<GameConnecter> gameConnecter;	
	void sendScreenRequestToGame(Key keyCodeForScreen);
};
