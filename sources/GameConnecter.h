#pragma once
#include <QObject>
#include <comdef.h> 
#include "Profile.h"
#include "RestorationMethode.h"
#include "VariablesClass.h"
#include "Key.h"
class GameConnecter  : public QObject
{
	Q_OBJECT
public:
	GameConnecter(QObject* parent, std::shared_ptr<VariablesClass> var, Profile* profile);
	~GameConnecter();
	void clickLeft(QPoint pt);
	void clickRight(QPoint pt);
	void sendStringToGame(QString str);
	bool sendKeyStrokeToProcess(Key key, int sleepTime = 20);
    bool sendKeyStrokeToProcess(int virtualKey, int sleepTime = 20);
	void useRestorationMethode(const RestorationMethode& methode);
	void autoLootAroundPlayer();
private:
	Profile::AutoLoot autoLootSetting;
	std::shared_ptr<VariablesClass> var;
	void sendCharToGame(const QChar charToSend, const HWND& gameThreadHandler);
	void clickRightWithShift(QVector<QPoint> pts, int SLEEP_TIME_BETWEEN_LOOT_CLICK);
    void pressShift() {
        INPUT ip;
        ip.type = INPUT_KEYBOARD; // Set up a generic keyboard event.
        ip.ki.wScan = 0; // hardware scan code for key
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;

        ip.ki.wVk = VK_SHIFT; // virtual-key code for the "a" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    }
    void releaseShift() {
        INPUT ip;
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0; 
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;
        ip.ki.wVk = VK_SHIFT;
        ip.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &ip, sizeof(INPUT));
    }
};
