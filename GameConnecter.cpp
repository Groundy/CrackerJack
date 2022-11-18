#include "GameConnecter.h"

GameConnecter::GameConnecter(QObject *parent)
	: QObject(parent){
}

GameConnecter::~GameConnecter()
{}

HWND GameConnecter::getHandlerToGameWindow(unsigned int PID, QString WindowName) {
	LPCWSTR nameOfWindowLPCWSTR = (const wchar_t*)WindowName.utf16();
	HWND handler = FindWindow(NULL, nameOfWindowLPCWSTR);
	if (handler == NULL) {
		Logger::logPotenialBug("Can't get handler to window: " + WindowName, "Utilities", "clickRight");
		return HWND();
	}
	DWORD tmp = PID;
	DWORD hThread = GetWindowThreadProcessId(handler, &tmp);

	if (hThread != NULL)
		return handler;
	else {
		Logger::logPotenialBug("Can't get thread PID for used handler", "Utilities", "clickRight");
		return HWND();
	}
}

void GameConnecter::clickLeft(QPoint pt, HWND handler) {
	LPARAM lParam = (pt.y() << 16) + pt.x();
	PostMessage(handler, WM_LBUTTONDOWN, 5, lParam);
	PostMessage(handler, WM_LBUTTONUP, 4, lParam);
}

void GameConnecter::clickRight(QPoint pt, HWND handler) {
	LPARAM lParam = (pt.y() << 16) + pt.x();
	PostMessage(handler, WM_RBUTTONDOWN, 5, lParam);
	PostMessage(handler, WM_RBUTTONUP, 4, lParam);
}

bool GameConnecter::sendKeyStrokeToProcess(Key key, unsigned int PID, QString nameOfWindow) {
	LPCWSTR nameOfWindowLPCWSTR = (const wchar_t*)nameOfWindow.utf16();
	HWND handler = FindWindow(NULL, nameOfWindowLPCWSTR);
	if (handler == NULL) {
		Logger::logPotenialBug("Can't get handler to window: " + nameOfWindow, "Utilities", "sendKeyStrokeToProcess");
		return false;
	}
	DWORD tmp = PID;
	DWORD hThread = GetWindowThreadProcessId(handler, &tmp);
	if (hThread != NULL) {
		int g = 0x1470001;
		PostMessage(handler, WM_KEYDOWN, key.number, g);
		PostMessage(handler, WM_KEYUP, key.number, g);
		return true;
	}
	else {
		Logger::logPotenialBug("Can't get thread PID for used handler", "Utilities", "sendKeyStrokeToProcess");
		return false;
	}
}