#include "GameConnecter.h"

GameConnecter::GameConnecter(QObject *parent, std::shared_ptr<VariablesClass> var)
	: QObject(parent), var(var){
}

GameConnecter::~GameConnecter()
{}

void GameConnecter::clickLeft(QPoint pt) {
	LPARAM lParam = (pt.y() << 16) + pt.x();
	HWND gameThreadHandler = var->getHandlerToGameThread();
	PostMessage(gameThreadHandler, WM_LBUTTONDOWN, 5, lParam);
	PostMessage(gameThreadHandler, WM_LBUTTONUP, 4, lParam);
}
void GameConnecter::clickRight(QPoint pt) {
	HWND gameThreadHandler = var->getHandlerToGameThread();
	LPARAM lParam = (pt.y() << 16) + pt.x();
	PostMessage(gameThreadHandler, WM_RBUTTONDOWN, 5, lParam);
	PostMessage(gameThreadHandler, WM_RBUTTONUP, 4, lParam);
}
bool GameConnecter::sendKeyStrokeToProcess(Key key) {
	/*
	LPCWSTR nameOfWindowLPCWSTR = (const wchar_t*)var->getNameOfGameWindow().utf16();
	HWND handler = FindWindow(NULL, nameOfWindowLPCWSTR);
	if (handler == NULL) {
		Logger::logPotenialBug("Can't get handler to window: " + var->getNameOfGameWindow(), "Utilities", "sendKeyStrokeToProcess");
		return false;
	}
	DWORD tmp = var->getPid();
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
	/*
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
	
	const int weirdConst = 0x1470001;
	HWND gameThreadHandler = var->getHandlerToGameThread();
	PostMessage(gameThreadHandler, WM_KEYDOWN, key.getKeyVal(), weirdConst);
	PostMessage(gameThreadHandler, WM_KEYUP, key.getKeyVal(), weirdConst);
	return true;
	
}
void GameConnecter::sendStringToGame(QString str) {
	HWND gameThreadHandler = var->getHandlerToGameThread();
	for each (QChar var in str) {
		if (var.isLetter()) {
			WPARAM wParam = var.toUpper().unicode();
			PostMessage(gameThreadHandler, WM_KEYDOWN, wParam, 1);
		}
		else if (var.isSpace()) {
			WPARAM wParam = 0x20;
			PostMessage(gameThreadHandler, WM_KEYDOWN, wParam, 1);
		}
		else if (var.isDigit()) {
			uint value = var.unicode() - 48;
			WPARAM wParam = var.unicode();
			LPARAM lParam = ((value + 1) << 16) + 1;
			PostMessage(gameThreadHandler, WM_KEYDOWN, wParam, lParam);
		}
		else if (var.unicode() == 39) {
			//apostrophe mark
			WPARAM wParam = 0xDE;
			LPARAM lParam = 0x00280001;
			PostMessage(gameThreadHandler, WM_KEYDOWN, wParam, lParam);
		}
		else if (var.unicode() == 45) {
			//dash mark
			WPARAM wParam = 0xBD;
			LPARAM lParam = 0x000C001;
			PostMessage(gameThreadHandler, WM_KEYDOWN, wParam, lParam);
		}
		Sleep(2);
	}
}
