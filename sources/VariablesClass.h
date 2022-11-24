#pragma once
#include <QObject>
#include <qmap.h>
#include <qimage.h>
#include <basetsd.h>
#include <atomic>
#include <mutex>

#include "Logger.h"
#include "RestorationMethode.h"
class VariablesClass : QObject{
	Q_OBJECT
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

	QMap<QString, QRect> potionName_rectPosOnScreen_map = getMapWithRects();
	static QMap<QString, QRect> getMapWithRects();
	VariablesClass();
	~VariablesClass();

	void log(QString msg, bool sendToDebug = true, bool sendToUserConsol = true, bool addTimeSTamp = true) {
		logger.log(msg, sendToDebug, sendToUserConsol, addTimeSTamp);
	}
	bool framesAreValid() {
		bool manaHealthFramesOk =
			!getHealthArea().isEmpty() &&
			(!getCombinedArea().isEmpty() || !getManaArea().isEmpty());
		return manaHealthFramesOk;
	}

	//getters setters
	void setNewImg(QImage& newImage) {
		fullImgMutex.lock();
		this->fullImage = std::move(newImage);
		newFullImgAwaits = true;
		fullImgMutex.unlock();
	}
	void setCurrentPercentage(double health, double mana, double manaShield) {
		this->currentHealthPercentage = health;
		this->currentManaPercentage = mana;
		this->currentMsPercentage = manaShield;
	}
	void setCurrentRawValues(int health, int mana, int manaShield) {
		this->currentHealthRaw = health;
		this->currentManaRaw = mana;
		this->currentMSRaw = manaShield;
	}
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

	//health, mana, combined, shield frames
	void setRotation(int rotation) { healthManaFrames.howTheyShouldBeRotated = rotation; }
	void setHealthArea(QRect toSet) { healthManaFrames.healthFrame = toSet; }
	void setManaArea(QRect toSet) { healthManaFrames.manaFrame = toSet; }
	void setMSArea(QRect toSet) { healthManaFrames.manaShieldFrame = toSet; }
	void setCombinedArea(QRect toSet) { healthManaFrames.combinedFrame = toSet; }
	int getRotation() { return healthManaFrames.howTheyShouldBeRotated; }
	QRect getHealthArea() { return healthManaFrames.healthFrame; }
	QRect getManaArea() { return healthManaFrames.manaFrame; }
	QRect getMSArea() { return healthManaFrames.manaShieldFrame; }
	QRect getCombinedArea() { return healthManaFrames.combinedFrame; }

	//otherFrames
	void setMiniMapArea(QRect toSet) { otherFrames.miniMapFrame = toSet; }
	QRect getMiniMapArea() { return otherFrames.miniMapFrame; }
	void setMainArea(QRect toSet) { otherFrames.gameFrame = toSet; }
	QRect getMainArea() { return otherFrames.gameFrame; }

	//health, mana, combined, shield IMG piececs
	void setImageHealth(QImage& img) { healthImg.setImg(img);}
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

	//timers
	qint64 getTimeLastItemUsage() const { return timeLastItemUsage; }
	qint64 getTimeLastSpellUsageAttack() const { return timeLastSpellAttack; }
	qint64 getTimeLastSpellUsageHealing() const { return timeLastSpellHealing; }
	qint64 getTimeLastSpellUsageSupport() const { return timeLastSpellSupport; }
	qint64 getTimeLastSpellUsed(QString spellName) { 
		lastTimeUsagesMutex.lock();
		return lastTimeSpellUsagesMap.value(spellName, 0);
		lastTimeUsagesMutex.unlock();
	}
	void setTimeLastItemUsage() { timeLastItemUsage = now(); }
	void setTimeLastSpellUsageAttack() { timeLastSpellAttack = now(); }
	void setTimeLastSpellUsageHealing() { timeLastSpellHealing = now(); }
	void setTimeLastSpellUsageSupport() { timeLastSpellSupport = now(); }
	void setTimeLastSpellUsed(QString spellName) {
		lastTimeUsagesMutex.lock();
		bool alreadyOnList = lastTimeSpellUsagesMap.contains(spellName);
		if (alreadyOnList)
			lastTimeSpellUsagesMap[spellName] = now();
		else 
			lastTimeSpellUsagesMap.insert(spellName, now());
	}
	bool restMethodeCanBeUsed(const RestorationMethode& restMethode) {
		const qint64 now = this->now();
		if (restMethode.isSpell()) {
			if (currentManaRaw < restMethode.getMana())
				return false;
			if (now < timeLastSpellHealing +  (1000 * restMethode.getCdGroup()))
				return false;
			if (now < getTimeLastSpellUsed(restMethode.getName()) + (1000 * restMethode.getCd()))
				return false;

			return true;
		}
		else if (restMethode.isPotion()) {
			if(now < timeLastItemUsage)
				return false;
			//later should be added checking if char has proper pot!
			return true;
		}
	}


	Logger logger;
private:
	std::mutex fullImgMutex;
	QImage fullImage;
	std::atomic<bool> newFullImgAwaits;
	std::atomic<double> currentHealthPercentage, currentManaPercentage, currentMsPercentage;
	std::atomic<int> currentHealthRaw, currentManaRaw, currentMSRaw;
	std::atomic<bool> keepRestoringManaAngHealth;
	std::atomic<bool> keepTakingScreenShots;
	std::atomic<bool> keepLoadingScreenShots;
	std::atomic<uint> pid;
	std::atomic<HWND> handlerToGameThread;
	QString nameOfGameWindow;	
	HealthManaFrames healthManaFrames;
	OtherFrames otherFrames;
	MutexImg healthImg, manaImg, combinedImg, msImg;
	std::atomic<qint64> timeLastItemUsage, timeLastSpellAttack, timeLastSpellHealing, timeLastSpellSupport;
	std::mutex lastTimeUsagesMutex;
	QMap<QString, qint64> lastTimeSpellUsagesMap;

	qint64 now() { return QDateTime::currentMSecsSinceEpoch(); };
};
