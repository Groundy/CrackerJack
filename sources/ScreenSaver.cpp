#include "ScreenSaver.h"

ScreenSaver::ScreenSaver(QObject* parent, std::shared_ptr<VariablesClass> var, std::shared_ptr<GameConnecter> gameConnecter, Profile* profile)
	: QThread(parent), var(var), gameConnecter(gameConnecter), profile(profile){
}

ScreenSaver::~ScreenSaver(){
	this->terminate();
}

void ScreenSaver::sendScreenRequestToGame(){
	try{
		uint pid = var->getPid();
		if (pid == 0)
			throw std::exception("Can't send screenshot key to game, there is no known pid!");
		QString winTitle = var->getNameOfGameWindow();
		if (winTitle.isEmpty())
			throw std::exception("Can't send screenshot key to game, there is no known title of window!");
		Key key = profile->getScreenShotKey();
		gameConnecter->sendKeyStrokeToProcess(key);
	}
	catch (const std::exception& e){
		qDebug() << e.what();
	}

}

void ScreenSaver::threadLoop(){
	while (true){
		msleep(SLEEP_TIME);
		if (!var->checkTakingScreensState())
			continue;
		sendScreenRequestToGame();
	}
}

void ScreenSaver::run(){
	setPriority(QThread::Priority::HighestPriority);
	threadLoop();
}