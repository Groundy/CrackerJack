#pragma once
#include <qobject.h>
#include <qthread.h>
#include <qdebug.h>
#include <qdatetime.h>
#include <windows.h>
#include <memory>
#include "GameConnecter.h"
class ClickDetector : public QThread
{
	Q_OBJECT
public:
	ClickDetector(QObject* parent, std::shared_ptr<GameConnecter> gameConnetorPtr)
		: gameConnetorPtr(gameConnetorPtr), QThread(parent) {
	};
	~ClickDetector() {};
	void run() {
		while (true) {
			if (middleButtonPressed()) {
				gameConnetorPtr->autoLootAroundPlayer();
				msleep(1000);
			}
			msleep(minMSecBetweenClicks);
		}
	}

private:
	std::shared_ptr<GameConnecter> gameConnetorPtr;
	const int minMSecBetweenClicks = 6;
	bool keyPressed(int key) {
		return (GetAsyncKeyState(key) & 0x8000 != 0) != 0;
	}
	bool middleButtonPressed() {
		return (GetAsyncKeyState(VK_MBUTTON) & 0x8000 != 0) != 0;
	}
};
