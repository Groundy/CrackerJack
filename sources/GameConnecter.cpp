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
void GameConnecter::clickLeft(QPoint pt) {
	LPARAM lParam = (pt.y() << 16) + pt.x();
	PostMessage(gameHandler, WM_LBUTTONDOWN, 5, lParam);
	PostMessage(gameHandler, WM_LBUTTONUP, 4, lParam);
}
void GameConnecter::clickRight(QPoint pt) {
	LPARAM lParam = (pt.y() << 16) + pt.x();
	PostMessage(gameHandler, WM_RBUTTONDOWN, 5, lParam);
	PostMessage(gameHandler, WM_RBUTTONUP, 4, lParam);
}
bool GameConnecter::sendKeyStrokeToProcess(Key key) {
	/*
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
		PostMessage(handler, WM_KEYDOWN, key.getKeyVal(), g);
		PostMessage(handler, WM_KEYUP, key.getKeyVal(), g);
		return true;
	}
	else {
		Logger::logPotenialBug("Can't get thread PID for used handler", "Utilities", "sendKeyStrokeToProcess");
		return false;
	}
	*/
	int g = 0x1470001;
	PostMessage(gameHandler, WM_KEYDOWN, key.getKeyVal(), g);
	PostMessage(gameHandler, WM_KEYUP, key.getKeyVal(), g);
	return true;
}
void GameConnecter::sendStringToGame(QString str) {
	for each (QChar var in str) {
		if (var.isLetter()) {
			WPARAM wParam = var.toUpper().unicode();
			PostMessage(gameHandler, WM_KEYDOWN, wParam, 1);
		}
		else if (var.isSpace()) {
			WPARAM wParam = 0x20;
			PostMessage(gameHandler, WM_KEYDOWN, wParam, 1);
		}
		else if (var.isDigit()) {
			uint value = var.unicode() - 48;
			WPARAM wParam = var.unicode();
			LPARAM lParam = ((value + 1) << 16) + 1;
			PostMessage(gameHandler, WM_KEYDOWN, wParam, lParam);
		}
		else if (var.unicode() == 39) {
			//apostrophe mark
			WPARAM wParam = 0xDE;
			LPARAM lParam = 0x00280001;
			PostMessage(gameHandler, WM_KEYDOWN, wParam, lParam);
		}
		else if (var.unicode() == 45) {
			//dash mark
			WPARAM wParam = 0xBD;
			LPARAM lParam = 0x000C001;
			PostMessage(gameHandler, WM_KEYDOWN, wParam, lParam);
		}
		Sleep(2);
	}
}
