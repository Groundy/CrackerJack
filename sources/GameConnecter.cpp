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
	const int weirdConst = 0x1470001;
	HWND gameThreadHandler = var->getHandlerToGameThread();
	PostMessage(gameThreadHandler, WM_KEYDOWN, key.getKeyVal(), weirdConst);
	PostMessage(gameThreadHandler, WM_KEYUP, key.getKeyVal(), weirdConst);
	return true;
	
}
void GameConnecter::sendStringToGame(QString str) {
	HWND gameThreadHandler = var->getHandlerToGameThread();
	for each (QChar charToSend in str) {
		sendCharToGame(charToSend, gameThreadHandler);
		Sleep(2);
	}
}
void GameConnecter::useRestorationMethode(const RestorationMethode& methode) {
	if (methode.isSpell()) {
		var->setTimeLastSpellUsageHealing();
		var->setTimeLastSpellUsed(methode.getName());
	}
	else if (methode.isPotion())
		var->setTimeLastItemUsage();

	Key key = methode.getKey();
	//sendKeyStrokeToProcess(key);

	QString msg = QString("Used %1").arg(methode.getName());
	var->log(msg, true, true, true);
}
void GameConnecter::sendCharToGame(const QChar charToSend, const HWND& gameThreadHandler) {
	if (charToSend.isLetter()) {
		WPARAM wParam = charToSend.toUpper().unicode();
		PostMessage(gameThreadHandler, WM_KEYDOWN, wParam, 1);
	}
	else if (charToSend.isSpace()) {
		WPARAM wParam = 0x20;
		PostMessage(gameThreadHandler, WM_KEYDOWN, wParam, 1);
	}
	else if (charToSend.isDigit()) {
		uint value = charToSend.unicode() - 48;
		WPARAM wParam = charToSend.unicode();
		LPARAM lParam = ((value + 1) << 16) + 1;
		PostMessage(gameThreadHandler, WM_KEYDOWN, wParam, lParam);
	}
	else if (charToSend.unicode() == 39) {
		//apostrophe mark
		WPARAM wParam = 0xDE;
		LPARAM lParam = 0x00280001;
		PostMessage(gameThreadHandler, WM_KEYDOWN, wParam, lParam);
	}
	else if (charToSend.unicode() == 45) {
		//dash mark
		WPARAM wParam = 0xBD;
		LPARAM lParam = 0x000C001;
		PostMessage(gameThreadHandler, WM_KEYDOWN, wParam, lParam);
	}
}