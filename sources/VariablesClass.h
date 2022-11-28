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
	struct MutexImg {
	public :
		void getImgCopy(QImage& returnImg) {
			mutex.lock();
			if (!img.isNull());
				returnImg = img.copy();
			mutex.unlock();
		}
		void setImg(const QImage& newImg) {
			mutex.lock();
			img = newImg.copy();
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

	//QMap<QString, QRect> potionName_rectPosOnScreen_map = getMapWithRects();
	//static QMap<QString, QRect> getMapWithRects();
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
	qint64 now() { return QDateTime::currentMSecsSinceEpoch(); };


	//unasigned gettersSetters
	void setNewImg(const QImage& newImage) {
		fullImage.setImg(newImage);
		newFullImgAwaits = true;
	}
	void getCopyOfCurrentFullImg(QImage& img) {
		fullImage.getImgCopy(img);
	}

	//game process
	uint getPid() { return pid; }
	void setPid(uint newPid) { pid = newPid; }
	QString getNameOfGameWindow() { return nameOfGameWindow; }
	void setNameOfGameWindow(QString newNameOfGameWindow) { nameOfGameWindow = newNameOfGameWindow; }
	HWND getHandlerToGameThread() { return handlerToGameThread; }
	void setHandlerToGameThread(HWND newHandlerToGameThread) { handlerToGameThread = newHandlerToGameThread; }


	//health mana manashield
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
	double getCurrentHealthPercentage() const {return currentHealthPercentage; }
	double getCurrentManaPercentage() const { return currentManaPercentage; }
	double getCurrentMSPercentage() const { return currentMsPercentage; }
	int getCurrentRawManaVal() const { return currentManaRaw; }

	//options
	void setSettingRestoringState(bool enable) { this->keepRestoringManaAngHealth = enable; };
	void setSettingTakingScreensState(bool enable) { this->keepTakingScreenShots = enable; };
	void setSettingLoadingState(bool enable) { this->keepLoadingScreenShots = enable; };
	bool getSettingTakingScreensState() const { return keepTakingScreenShots; }
	bool getSettingRestoringState() const { return keepRestoringManaAngHealth; }
	bool getSettingLoadingState() const { return keepLoadingScreenShots; }

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
		qint64 value = lastTimeSpellUsagesMap.value(spellName, 0);
		lastTimeUsagesMutex.unlock();
		return value;
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
		lastTimeUsagesMutex.unlock();
	}

	//minimap
	void setSettingKeepAnalyzeMiniMap(bool enable) { this->keepAnalyzingMiniMap = enable; };
	bool getSettingKeepAnalyzeMiniMap() const { return keepAnalyzingMiniMap; }
	void getImgMiniMap(QImage& imgToRet) { 
		this->miniMap.getImgCopy(imgToRet); 
		this->miniMap.clear();
	}
	void setImgMiniMap(const QImage& miniMapImg) { this->miniMap.setImg(miniMapImg); }
	QRect getFrameMiniMap() {
		miniMapFrameMutex.lock();
		QRect miniMapFrame = this->miniMapFrame;
		miniMapFrameMutex.unlock();
		return miniMapFrame;
	}
	void setFrameMiniMap(const QRect& miniMapFrame) { 
		miniMapFrameMutex.lock();
		this->miniMapFrame = miniMapFrame;
		miniMapFrameMutex.unlock();
	}
	void getImgMiniMapLayer(QImage& imgToRet) { 
		this->minMapLayer.getImgCopy(imgToRet); 
		this->minMapLayer.clear();
	}
	void setImgMiniMapLayer(const QImage& minMapLayerImg) { this->minMapLayer.setImg(minMapLayerImg); }

	//gameFrame mainGameWindow
	void setSettingKeepAnalyzeMainGameWindow(bool enable) { this->keepAnalyzingMainGameWindow = enable; };
	bool getSettingKeepAnalyzeMainGameWindow() const { return keepAnalyzingMainGameWindow; }
	void getImgMainGameWindow(QImage& imgToRet) { this->gameWindow.getImgCopy(imgToRet); }
	void setImgMainGameWindow(const QImage& mainGameWindow) { this->gameWindow.setImg(mainGameWindow); }
	QRect getFrameMainGameWindow() {
		gameWindowFrameMutex.lock();
		QRect mainGameFrame = this->gameWindowFrame;
		gameWindowFrameMutex.unlock();
		return mainGameFrame;
	}
	void setFrameMainGameWindow(const QRect& gameWindowFrame) {
		gameWindowFrameMutex.lock();
		this->gameWindowFrame = gameWindowFrame;
		gameWindowFrameMutex.unlock();
	}


	Logger logger;
private:
	//game
	std::atomic<uint> pid;
	std::atomic<HWND> handlerToGameThread;
	QString nameOfGameWindow;	

	//health mana
	std::atomic<double> currentHealthPercentage = 100.0, currentManaPercentage = 100.0, currentMsPercentage = 100.0;
	std::atomic<int> currentHealthRaw = 100, currentManaRaw = 100, currentMSRaw = 100;
	HealthManaFrames healthManaFrames;
	MutexImg healthImg, manaImg, combinedImg, msImg;

	//settings
	std::atomic<bool> newFullImgAwaits;
	std::atomic<bool> keepRestoringManaAngHealth = true;
	std::atomic<bool> keepTakingScreenShots = true;
	std::atomic<bool> keepLoadingScreenShots  = true;
	std::atomic<bool> keepAnalyzingMiniMap = false;
	std::atomic<bool> keepAnalyzingMainGameWindow;

	//timers
	std::atomic<qint64> timeLastItemUsage, timeLastSpellAttack, timeLastSpellHealing, timeLastSpellSupport;
	std::mutex lastTimeUsagesMutex;
	QMap<QString, qint64> lastTimeSpellUsagesMap;

	//MiniMap
	MutexImg miniMap;
	MutexImg minMapLayer;
	QRect miniMapFrame;
	std::mutex miniMapFrameMutex;

	//GameFrameMap
	MutexImg gameWindow;
	QRect gameWindowFrame;
	std::mutex gameWindowFrameMutex;

	//other
	MutexImg fullImage;		
	//QRect gameFrame;

};
