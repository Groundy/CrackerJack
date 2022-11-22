#pragma once
#include <QObject>
#include <qmap.h>
#include <qimage.h>
#include <basetsd.h>
#include <atomic>
#include <mutex>
class VariablesClass{
public:	
	typedef LONG64 TIME;
	struct HealthManaFrames {
		QRect healthFrame;
		QRect manaFrame;
		QRect manaShieldFrame;
		QRect combinedFrame;
		int howTheyShouldBeRotated = 0;
	};
	struct OtherFrames {
		QRect gameFrame;
		QRect miniMapFrame;
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
		void clear() {
			mutex.lock();
			img = QImage();
			mutex.unlock();
		}
	private:
		std::mutex mutex;
		QImage img;
	};
	enum STATES { HASTE, BATTLE, PROTECTOR_ZONE, POISONED, PARALYZED, UPGRADED };


	QString var_winTitleOfGame;
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
	void changeLoadingState(bool enable) { this->keepLoadingScreenShots = enable; };
	bool checkLoadingState() { return keepLoadingScreenShots; }

	void setRotation(int rotation) { healthManaFrames.howTheyShouldBeRotated = rotation; }
	int getRotation() { return healthManaFrames.howTheyShouldBeRotated; }
	void setHealthArea(QRect toSet) { healthManaFrames.healthFrame = toSet; }
	QRect getHealthArea() { return healthManaFrames.healthFrame; }
	void setManaArea(QRect toSet) { healthManaFrames.manaFrame = toSet; }
	QRect getManaArea() { return healthManaFrames.manaFrame; }
	void setMSArea(QRect toSet) { healthManaFrames.manaShieldFrame = toSet; }
	QRect getMSArea() { return healthManaFrames.manaShieldFrame; }
	void setCombinedArea(QRect toSet) { healthManaFrames.combinedFrame = toSet; }
	QRect getCombinedArea() { return healthManaFrames.combinedFrame; }

	void setMiniMapArea(QRect toSet) { otherFrames.miniMapFrame = toSet; }
	QRect getMiniMapArea() { return otherFrames.miniMapFrame; }
	void setMainArea(QRect toSet) { otherFrames.gameFrame = toSet; }
	QRect getMainArea() { return otherFrames.gameFrame; }


	void setImageHealth(QImage& img) { healthImg.setImg(img); /*qDebug() << "set new img!";*/ }
	void setImageMana(QImage& img) { manaImg.setImg(img); }
	void setImageMS(QImage& img) { msImg.setImg(img); }
	void setImageCombined(QImage& img) { combinedImg.setImg(img); }
	void getImageHealth(QImage& img, bool clear = false) { 
		healthImg.getImgCopy(img);
		if (clear)
			healthImg.clear();
	}
	void getImageMana(QImage& img, bool clear = false) {
		manaImg.getImgCopy(img); 
		if (clear)
			manaImg.clear();
	}
	void getImageMS(QImage& img, bool clear = false) {
		msImg.getImgCopy(img); 
		if (clear)
			msImg.clear();
	}
	void getImageCombined(QImage& img, bool clear = false){
		combinedImg.getImgCopy(img);
		if (clear) 
			combinedImg.clear();
	}
private:
	std::mutex fullImgMutex;
	QImage fullImage;
	std::atomic<bool> newFullImgAwaits;
	std::atomic<double> currentHealthPercentage, currentManaPercentage, currentMsPercentage;
	std::atomic<bool> keepRestoringManaAngHealth;
	std::atomic<bool> keepTakingScreenShots;
	std::atomic<bool> keepLoadingScreenShots;
	std::atomic<uint> pid;
	std::atomic<HWND> handlerToGameThread;
	QString nameOfGameWindow;	
	HealthManaFrames healthManaFrames;
	OtherFrames otherFrames;
	MutexImg healthImg, manaImg, combinedImg, msImg;
};
