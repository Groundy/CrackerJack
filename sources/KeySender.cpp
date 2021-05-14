#include "KeySender.h"

KeySender::KeySender(QObject *parent, Profile* profile, VariablesClass* variableClass)
	: QThread(parent){
	var = variableClass;
}

KeySender::~KeySender(){
}

void KeySender::run(){
	mainLoop();
}

void KeySender::getKeyToSend(Key keyToSend){
	unsigned int pidOfGame = var->var_pidOfGame;
	QString titleOfGameWindow = var->var_winTitleOfGame;
	Utilities::sendKeyStrokeToProcess(keyToSend, pidOfGame, titleOfGameWindow);
}

void KeySender::mainLoop(){
	while (true) {
		Sleep(10);

	}
}

void KeySender::sendKeyToGame(Key keyToSend){
	unsigned int pidOfGame = var->var_pidOfGame;
	QString titleOfGameWindow = var->var_winTitleOfGame;
	Utilities::sendKeyStrokeToProcess(keyToSend, pidOfGame, titleOfGameWindow);
}


