#include "ScreenSaver.h"

void ScreenSaver::ScreenLoop(){
	while (true){
		QThread::sleep(500);

	}
}

void ScreenSaver::run(){
	//Utilities::sendKeyStroke(Key(Key::NORMAL,QChar('q')), 0);
	Utilities::test();
	//ScreenLoop();
}

ScreenSaver::ScreenSaver(QObject *parent)
	: QThread(parent)
{
}
ScreenSaver::ScreenSaver(){
}

ScreenSaver::~ScreenSaver(){
}
