#include "ScreenSaver.h"

ScreenSaver::ScreenSaver(QObject* parent, VariablesClass* varClass)
	: QThread(parent){
	setPriority(QThread::Priority::HighestPriority);
	var = varClass;
}

ScreenSaver::~ScreenSaver(){
	this->terminate();
}

void ScreenSaver::sendScreenRequestToGame(Key keyCodeForScreen){
	uint pid = var->var_pidOfGame;
	QString winTitle = var->var_winTitleOfGame;
	bool wrongParameters = pid == 0 || winTitle.isEmpty();
	if (wrongParameters) {
		Logger::logPotenialBug("Wrong parameters of game(pid or game window title", "ScreenSaver", "sendScreenRequestToGame");
		return;
	}
	Key key(keyCodeForScreen);
	Utilities::sendKeyStrokeToProcess(key, pid, winTitle);
}

void ScreenSaver::screenLoop(){
	while (true){
		if (!enableScreenCapture) {
			msleep(5 * timeBetweenScreens);
			continue;
		}
		sendScreenRequestToGame(Key::HOME);
		msleep(timeBetweenScreens);
	}
}

void ScreenSaver::run(){
	screenLoop();
}