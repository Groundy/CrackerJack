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
	void clickLeft(QPoint pt, HWND handler);
	void clickRight(QPoint pt, HWND handler);
	bool sendKeyStrokeToProcess(Key key, unsigned int PID, QString WindowName);
private:
	HWND gameHandler;
	HWND getHandlerToGameWindow(unsigned int PID, QString WindowName);
};
