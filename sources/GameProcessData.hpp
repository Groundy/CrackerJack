#pragma once
#include <qstring.h>
#include <basetsd.h>
#include <Windows.h>
class GameProcessData
{
public:
	GameProcessData() {};
	~GameProcessData() {};
	uint getPid() { return pid; }
	void setPid(uint newPid) { pid = newPid; }
	QString getNameOfGameWindow() { return nameOfGameWindow; }
	void setNameOfGameWindow(QString newNameOfGameWindow) { nameOfGameWindow = newNameOfGameWindow; }
	HWND getHandlerToGameThread() { return handlerToGameThread; }
	void setHandlerToGameThread(HWND newHandlerToGameThread) { handlerToGameThread = newHandlerToGameThread; }
private:
	std::atomic<uint> pid;
	std::atomic<HWND> handlerToGameThread;
	QString nameOfGameWindow;
};