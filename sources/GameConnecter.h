#pragma once
#include <QObject>
//#include "tlhelp32.h"
#include <comdef.h> 
#include "Logger.h"
#include "Key.h"
class GameConnecter  : public QObject
{
	Q_OBJECT
public:
	GameConnecter(QObject *parent);
	~GameConnecter();
	void clickLeft(QPoint pt);
	void clickRight(QPoint pt);
	void sendStringToGame(QString str);
	bool sendKeyStrokeToProcess(Key key);
private:
	HWND gameHandler;
	unsigned int PID;
	HWND getHandlerToGameWindow(unsigned int PID, QString WindowName);

};
