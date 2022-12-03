#include "GameConnecter.h"

GameConnecter::GameConnecter(QObject *parent, std::shared_ptr<VariablesClass> var, Profile* prof)
	: QObject(parent), var(var){
	autoLootSetting = prof->getAutoLoot();
}
GameConnecter::~GameConnecter(){
	releaseShift();
}

void GameConnecter::clickLeft(QPoint pt) {
	LPARAM lParam = (pt.y() << 16) + pt.x();
	HWND gameThreadHandler = var->getGameProcess().getHandlerToGameThread();
	PostMessage(gameThreadHandler, WM_LBUTTONDOWN, 5, lParam);
	PostMessage(gameThreadHandler, WM_LBUTTONUP, 4, lParam);
}
void GameConnecter::clickRight(QPoint pt) {
	HWND gameThreadHandler = var->getGameProcess().getHandlerToGameThread();
	LPARAM lParam = (pt.y() << 16) + pt.x();
	PostMessage(gameThreadHandler, WM_RBUTTONDOWN, 0, lParam);
	PostMessage(gameThreadHandler, WM_RBUTTONUP, 0, lParam);
}
void GameConnecter::clickRightWithShift(QVector<QPoint> pts, int SLEEP_TIME_BETWEEN_LOOT_CLICK) {
	HWND gameThreadHandler = var->getGameProcess().getHandlerToGameThread();

	pressShift();
	for each (auto pt in pts){
		LPARAM lParam = (pt.y() << 16) + pt.x();
		PostMessage(gameThreadHandler, WM_RBUTTONDOWN, 6, lParam);
		Sleep(SLEEP_TIME_BETWEEN_LOOT_CLICK);
		PostMessage(gameThreadHandler, WM_RBUTTONUP, 4, lParam);
	}
	releaseShift();
}
bool GameConnecter::sendKeyStrokeToProcess(Key key, int sleepTime) {
	const int weirdConst = 0x1470001;
	HWND gameThreadHandler = var->getGameProcess().getHandlerToGameThread();
	PostMessage(gameThreadHandler, WM_KEYDOWN, key.getKeyVal(), weirdConst);
	Sleep(sleepTime);
	PostMessage(gameThreadHandler, WM_KEYUP, key.getKeyVal(), weirdConst);
	return true;
	
}
bool GameConnecter::sendKeyStrokeToProcess(int virtualKey, int sleepTime) {
	const int weirdConst = 0x1470001;
	HWND gameThreadHandler = var->getGameProcess().getHandlerToGameThread();
	PostMessage(gameThreadHandler, WM_KEYDOWN, virtualKey, weirdConst);
	Sleep(sleepTime);
	PostMessage(gameThreadHandler, WM_KEYUP, virtualKey, weirdConst);
	return true;

}
void GameConnecter::sendStringToGame(QString str) {
	HWND gameThreadHandler = var->getGameProcess().getHandlerToGameThread();
	for each (QChar charToSend in str) {
		sendCharToGame(charToSend, gameThreadHandler);
		Sleep(2);
	}
}
void GameConnecter::useRestorationMethode(const RestorationMethode& methode) {
	if (methode.isSpell()) {
		var->getTimers().setTimeLastSpellUsageHealing();
		var->getTimers().setTimeLastSpellUsed(methode.getName());
	}
	else if (methode.isPotion())
		var->getTimers().setTimeLastItemUsage();

	Key key = methode.getKey();
	sendKeyStrokeToProcess(key);

	QString msg = QString("Used %1").arg(methode.getName());
	var->log(msg, false, true, true);
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
void GameConnecter::autoLootAroundPlayer() {
	auto potinsToClick = var->getMainWindow().getPointsOfFieldsAroundPlayer();
	var->log("Wykonuje auto loot wokol gracz.", false, true, true);
	const int SLEEP_TIME_BETWEEN_LOOT_CLICK = 100;
	switch (autoLootSetting){
	case Profile::RIGHT_MOUSE_BUTTON:
		for each (auto pt in potinsToClick){
			clickRight(pt);
			Sleep(SLEEP_TIME_BETWEEN_LOOT_CLICK);
		}
		break;
	case Profile::SHIFT_RIGHT:
		clickRightWithShift(potinsToClick, SLEEP_TIME_BETWEEN_LOOT_CLICK);
		break;
	case Profile::LEFT_MOUSE_BUTTON:
		for each (auto pt in potinsToClick) {
			clickLeft(pt);
			Sleep(SLEEP_TIME_BETWEEN_LOOT_CLICK);
		}
		break;
	default:
		break;
	}
}

