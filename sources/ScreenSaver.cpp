#include "ScreenSaver.h"

ScreenSaver::ScreenSaver(QObject* parent, std::shared_ptr<VariablesClass> var, std::shared_ptr<GameConnecter> gameConnecter)
	: QThread(parent), var(var), gameConnecter(gameConnecter){
}

ScreenSaver::~ScreenSaver(){
	this->terminate();
}


void ScreenSaver::sendScreenRequestToGame(Key keyCodeForScreen){
	uint pid = var->var_pidOfGame;
	QString winTitle = var->var_winTitleOfGame;
	bool wrongParameters = pid == 0 || winTitle.isEmpty();
	if (wrongParameters) {
		Logger::logPotenialBug("Wrong parameters of game(pid or game window title)", "ScreenSaver", "sendScreenRequestToGame");
		return;
	}
	Key key(keyCodeForScreen);
	gameConnecter->sendKeyStrokeToProcess(key, pid, winTitle);
}

void ScreenSaver::screenLoop(){
	while (true){
		if (!enableScreenCapture) {
			msleep(5 * timeBetweenScreens);
			continue;
		}
		sendScreenRequestToGame(Key("HOME"));
		msleep(timeBetweenScreens);
	}
}

void ScreenSaver::run(){
	setPriority(QThread::Priority::HighestPriority);
	screenLoop();
}