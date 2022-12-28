#include "GameConnecter.h"

GameConnecter::GameConnecter(QSharedPointer<VariablesClass> var, Profile* prof)
		: var(var){
	autoLootSetting = prof->getAutoLoot();
	playingOnSmallMonitor = var->getSettings().getPlayingOnSmallMonitor();
}
GameConnecter::~GameConnecter(){
	releaseShift();
}

void GameConnecter::clickLeft(QPoint pt) {
	if(playingOnSmallMonitor)
		pt = QPoint(pt.x() / 0.8, pt.y() / 0.8);
	HWND gameThreadHandler = var->getGameProcess().getHandlerToGameThread();
	LPARAM lParam = (pt.y() << 16) + pt.x();
	senderMutex.lock();
	PostMessage(gameThreadHandler, WM_LBUTTONDOWN, 1, lParam);
	PostMessage(gameThreadHandler, WM_LBUTTONUP, 1, lParam);
	senderMutex.unlock();
}
void GameConnecter::clickRight(QPoint pt) {
	if (playingOnSmallMonitor)
		pt = QPoint(pt.x() / 0.8, pt.y() / 0.8);
	HWND gameThreadHandler = var->getGameProcess().getHandlerToGameThread();
	LPARAM lParam = (pt.y() << 16) + pt.x();
	senderMutex.lock();
	PostMessage(gameThreadHandler, WM_RBUTTONDOWN, 0, lParam);
	PostMessage(gameThreadHandler, WM_RBUTTONUP, 0, lParam);
	senderMutex.unlock();
}
void GameConnecter::clickRightWithShift(QVector<QPoint> pts, int SLEEP_TIME_BETWEEN_LOOT_CLICK) {
	HWND gameThreadHandler = var->getGameProcess().getHandlerToGameThread();

	senderMutex.lock();
	pressShift();
	SLEEP_TIME_BETWEEN_LOOT_CLICK /= 3;
	for each (auto pt in pts){
		if (playingOnSmallMonitor)
			pt = QPoint(pt.x() / 0.8, pt.y() / 0.8);
		LPARAM lParam = (pt.y() << 16) + pt.x();
		Sleep(SLEEP_TIME_BETWEEN_LOOT_CLICK);
		PostMessage(gameThreadHandler, WM_RBUTTONDOWN, 0, lParam);
		Sleep(SLEEP_TIME_BETWEEN_LOOT_CLICK);
		PostMessage(gameThreadHandler, WM_RBUTTONUP, 0, lParam);
		Sleep(SLEEP_TIME_BETWEEN_LOOT_CLICK);
	}
	releaseShift();
	senderMutex.unlock();
}
bool GameConnecter::sendKeyStrokeToProcess(Key key, int sleepTime) {
	const int weirdConst = 0x1470001;
	HWND gameThreadHandler = var->getGameProcess().getHandlerToGameThread();
	senderMutex.lock();
	PostMessage(gameThreadHandler, WM_KEYDOWN, key.getKeyVal(), weirdConst);
	Sleep(sleepTime);
	PostMessage(gameThreadHandler, WM_KEYUP, key.getKeyVal(), weirdConst);
	senderMutex.unlock();
	return true;
	
}
bool GameConnecter::sendKeyStrokeToProcess(int virtualKey, int sleepTime) {
	const int weirdConst = 0x1470001;
	HWND gameThreadHandler = var->getGameProcess().getHandlerToGameThread();
	senderMutex.lock();
	PostMessage(gameThreadHandler, WM_KEYDOWN, virtualKey, weirdConst);
	Sleep(sleepTime);
	PostMessage(gameThreadHandler, WM_KEYUP, virtualKey, weirdConst);
	senderMutex.unlock();
	return true;

}
void GameConnecter::sendStringToGame(QString str) {
	HWND gameThreadHandler = var->getGameProcess().getHandlerToGameThread();
	for each (QChar charToSend in str) {
		sendCharToGame(charToSend, gameThreadHandler);
		Sleep(2);
	}
}
void GameConnecter::useRestorationMethode(const RestorationMethode& methode, int additionalTime) {
	Timers& timers = var->getTimers();
	if (methode.isSpell()) {
		timers.setTimeLastSpellUsageHealing();
		timers.setTimeLastSpellUsed(methode.getName());
	}
	else if (methode.isPotion()) {
		timers.setTimeLastItemUsageGeneral();
		timers.setTimeLastItemUsed(methode.getName(), additionalTime);
	}

	Key key = methode.getKey();
	sendKeyStrokeToProcess(key);
	var->log(methode.getName(), false, true, true);
}
void GameConnecter::sendCharToGame(const QChar charToSend, const HWND& gameThreadHandler) {
	senderMutex.lock();
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
	senderMutex.unlock();
}
void GameConnecter::autoLootAroundPlayer() {
	auto potinsToClick = var->getGameWindow().getPointsOfFieldsAroundPlayer();
	const int SLEEP_TIME_BETWEEN_LOOT_CLICK = 111;
	switch (autoLootSetting){
	case Profile::RIGHT_MOUSE_BUTTON:
		for each (auto pt in potinsToClick){
			clickRight(pt);
			Sleep(SLEEP_TIME_BETWEEN_LOOT_CLICK);
		}
		break;
	case Profile::SHIFT_RIGHT:
		clickRightWithShift(potinsToClick, SLEEP_TIME_BETWEEN_LOOT_CLICK);
		this->releaseShift();
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
	var->log("auto loot.", false, true, true);

}

