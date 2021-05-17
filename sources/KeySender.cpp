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

void KeySender::mainLoop(){
	while (true) {
		Sleep(10);

	}
}
