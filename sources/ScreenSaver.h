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
	static void sendScreenRequestToGame(Key keyCodeForScreen, VariablesClass* var);
private:
	void screenLoop();
	VariablesClass* variables;
};
