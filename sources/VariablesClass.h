#pragma once
#include <QObject>
#include "qmap.h"
#include "qimage.h"
#include <basetsd.h>
#include <atomic>
#include <mutex>
class VariablesClass{
public:	
	typedef LONG64 TIME;
	struct Frames {
		QRect gameFrame;
		QRect miniMapFrame;
		QRect healthFrame;
		QRect manaFrame;
		QRect manaShieldFrame;
		QRect combinedFrame;
		int howTheyShouldBeRotated;
	};
	struct MutexImg {
	public :
		void getImgCopy(QImage& returnImg) {
			mutex.lock();
			returnImg = img.copy();
			mutex.unlock();
		}
		void setImg(QImage& newImg) {
			mutex.lock();
			img = std::move(newImg);
			mutex.unlock();
		}
	private:
		std::mutex mutex;
		QImage img;
	};
	enum STATES { HASTE, BATTLE, PROTECTOR_ZONE, POISONED, PARALYZED, UPGRADED };

	Frames frames;
	QString var_winTitleOfGame;
	QImage var_healthPieceImg;
	QImage var_manaPieceImg;
	QImage var_manaShieldPieceImg;
	QImage var_combinedBoxPieceImg;
	int rotationNeededForPointsAbove;
	std::atomic<bool> manaFound, healthFound, manaShieldFound, combinedFound;

	TIME lastTimeUsed_spell_healing;
	TIME lastTimeUsed_spell_attack;
	TIME lastTimeUsed_spell_support;
	TIME lastTimeUsed_item;
	QMap<QString, QRect> potionName_rectPosOnScreen_map = getMapWithRects();
	static QMap<QString, QRect> getMapWithRects();
	VariablesClass();
	~VariablesClass();

	//getters setters
	void setNewImg(QImage& newImage) {
		fullImgMutex.lock();
		this->fullImage = std::move(newImage);
		newFullImgAwaits = true;
		fullImgMutex.unlock();
	}
	void setCurrentHealthPercentage(double percentage) {this->currentHealthPercentage = percentage;}
	void setCurrentManaPercentage(double percentage) {this->currentManaPercentage = percentage;}
	void setCurrentMSPercentage(double percentage) {this->currentMsPercentage = percentage;}
	double getCurrentHealthPercentage() {return currentHealthPercentage; }
	double getCurrentManaPercentage() { return currentManaPercentage; }
	double getCurrentMSPercentage() { return currentMsPercentage; }
	void getCopyOfCurrentFullImg(QImage& img) {
		fullImgMutex.lock();
		img = fullImage.copy();
		fullImgMutex.unlock();
	}
	void changeRestoringState(bool enable) { this->keepRestoringManaAngHealth = enable; };
	bool checkRestoringState() { return keepRestoringManaAngHealth; }
	void changeTakingScreensState(bool enable) { this->keepTakingScreenShots = enable; };
	bool checkTakingScreensState() { return keepTakingScreenShots; }
	uint getPid() { return pid; }
	void setPid(uint newPid) { pid = newPid; }
	QString getNameOfGameWindow() { return nameOfGameWindow; }
	void setNameOfGameWindow(QString newNameOfGameWindow) { nameOfGameWindow = newNameOfGameWindow; }
	HWND getHandlerToGameThread() { return handlerToGameThread; }
	void setHandlerToGameThread(HWND newHandlerToGameThread) { handlerToGameThread = newHandlerToGameThread; }

private:
	std::mutex fullImgMutex;
	QImage fullImage;
	std::atomic<bool> newFullImgAwaits;
	std::atomic<double> currentHealthPercentage, currentManaPercentage, currentMsPercentage;
	std::atomic<bool> keepRestoringManaAngHealth;
	std::atomic<bool> keepTakingScreenShots;
	std::atomic<uint> pid;
	std::atomic<HWND> handlerToGameThread;
	QString nameOfGameWindow;
};
