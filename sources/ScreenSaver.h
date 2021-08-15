#pragma once
#include <QThread>
#include "activeGameThread.h"
#include "VariablesClass.h"
class ScreenSaver : public QThread{
public:
	bool enableScreenCapture = true;
	void run();
	int timeBetweenScreens = 550;
	ScreenSaver(QObject *parent, VariablesClass *varClass);
	~ScreenSaver();
private:
	void sendScreenRequestToGame(Key keyCodeForScreen);
	void screenLoop();
	VariablesClass* var;
};
