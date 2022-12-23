#pragma once
#include <QObject>
#include <qmap.h>
#include <qimage.h>
#include <basetsd.h>
#include <atomic>
#include <mutex>

#include "Logger.h"
#include "RestorationMethode.h"
#include "Point3D.h"
#include "ImgEditor.h"
#include "PathResource.h"
#include "Equipment.hpp"
class BattleList
{
public:
	BattleList() {};
	~BattleList() {};
	void setFrame(QRect battleListAreaToSet) {
		battleListArea.setRect(battleListAreaToSet);
	};
	void setImg(const QImage& battleListImgToSet) {
		battleListImg.setImg(battleListImgToSet);
	}
	QRect getFrame() {
		return battleListArea.getRect();
	};
	QImage getImg() {
		//ImgEditor::saveImgWitinLoop(battleListImg.getImgCopy());
		return battleListImg.getImgCopy();
	}
	int getEnemisAmout() {
		if (qrand() % 10 == 0)
			checkIfBattleListIsOk();
		QImage inneBattleList = getInnerBattleList();
		if (inneBattleList.isNull())
			return -1;
		const int middleX = inneBattleList.width() / 2;
		int blackDots = 0;
		const uint BLACK = qRgb(0, 0, 0);
		for (int y = 0; y < inneBattleList.height(); y++) {
			if (inneBattleList.pixel(middleX, y) == BLACK)
				blackDots++;
		}
		return blackDots / 2;
	}
	bool firstEnemieIsInRedFrame() {
		QImage outterBattleListLImg = getImg();
		if (outterBattleListLImg.isNull())
			return true;
		QRect inneBattleListArea = getInnerRect(outterBattleListLImg.rect());
		QImage innerBattleListLImg = outterBattleListLImg.copy(inneBattleListArea);
		constexpr uint RED = qRgb(255, 0, 0);
		constexpr uint LIGHT_RED = qRgb(255, 128, 128);
		int redPixels = 0;
		for (int y = 0; y < 20; y += 19) {
			for (int x = 0; x < 20; x++){//top//down
				uint pix = innerBattleListLImg.pixel(x, y);
				if (pix == RED || pix == LIGHT_RED)
					redPixels++;
			}
		}
		for (int x = 0; x < 20; x += 19) {//left//right
			for (int y = 1; y < 19; y++) {
				uint pix = innerBattleListLImg.pixel(x, y);
				if (pix == RED || pix == LIGHT_RED)
					redPixels++;
			}
		}
		const int ALL_PIX_TO_CHECK = 76;
		double redPercentage = (redPixels * 100.0) / ALL_PIX_TO_CHECK;
		return redPercentage >= 75;
	}
	bool checkIfBattleListIsOk() {
		QImage battleMark = QImage(PathResource::getPathToBattleList());
		QImage battleList = getImg();
		bool properImg = ImgEditor::findStartPositionInImg(battleMark, battleList).size() == 1;
		if (!properImg)
			setFrame(QRect());
		return properImg;
	}
	QStringList getUniqueMonstersNames() {
		QImage innerList = getInnerBattleList();
		if (innerList.isNull())
			return QStringList() << "Blad czytania battle listy.";
		QVector<QRect> nameRect = getFramesOfMonstersNames();
		QStringList toRet = {};
		for (int i = 0; i < nameRect.size(); i++){
			QImage singleMonsterNameImg = innerList.copy(nameRect[i]);
			QString monsterName = ImgEditor::imgWithStrToStr(singleMonsterNameImg, 180);
			if (monsterName.isEmpty())
				break;
			toRet.append(monsterName);
		}
		toRet.removeDuplicates();
		return toRet;
	}
private:
	MutexRect battleListArea;
	MutexImg battleListImg;
	std::atomic<int> enemiesOnBattleList = 0;
	QImage getInnerBattleList() {
		QRect innerBattleListRect = getInnerRect(battleListArea.getRect());
		QImage fullBattleListImg = battleListImg.getImgCopy();
		return fullBattleListImg.copy(innerBattleListRect);
	}
	const QPoint INNER_FRAME_FIST_NAME_START = QPoint(21,0);
	const int NAME_HEIGHT = 14;
	const int HIGH_DIFF_BETWEEN_NAMES = 8;
	QRect getInnerRect(QRect outerRect) {
		return QRect(4, 15, 155, outerRect.height());
	}
	QVector<QRect> getFramesOfMonstersNames() {
		QVector<QRect> toRet = {};
		auto innerRect = getInnerRect(battleListArea.getRect());
		int howManyNamesFitToListSize = ((innerRect.height() - NAME_HEIGHT) / (NAME_HEIGHT + HIGH_DIFF_BETWEEN_NAMES) + 1);//one name + pairs of (name+space between names)
		for (int i = 0; i < howManyNamesFitToListSize; i++){
			QPoint startPoint = INNER_FRAME_FIST_NAME_START + QPoint(0, i * (NAME_HEIGHT + HIGH_DIFF_BETWEEN_NAMES));
			toRet.append(QRect(startPoint.x(), startPoint.y(), innerRect.width(), NAME_HEIGHT));
		}
		return toRet;
	};
};
class Settings{
public:
	Settings() {};
	~Settings() {};
	void setRestoringState(bool enable) { this->keepRestoringManaAngHealth = enable; };
	void setTakingScreensState(bool enable) { this->keepTakingScreenShots = enable; };
	void setLoadingState(bool enable) { this->keepLoadingScreenShots = enable; };
	void setKeepAnalyzeMiniMap(bool enable) { this->keepAnalyzingMiniMap = enable; };
	void setKeepAnalyzeMainGameWindow(bool enable) { this->keepAnalyzingMainGameWindow = enable; };
	void setKeepHuntingAutomaticly(bool enable) { this->keepHuntingAutomaticly = enable; };
	bool getTakingScreensState() const { return keepTakingScreenShots; }
	bool getRestoringState() const { return keepRestoringManaAngHealth; }
	bool getLoadingState() const { return keepLoadingScreenShots; }
	bool getKeepHuntingAutomaticly() const { return keepHuntingAutomaticly; }
	bool getKeepAnalyzeMainGameWindow() const { return keepAnalyzingMainGameWindow; }
	bool getKeepAnalyzeMiniMap() const { return keepAnalyzingMiniMap; }

	bool getKeepAnalyzeStates() const { return keepAnalyzeStates; }
	void setKeepAnalyzeStates(bool enable) { this->keepAnalyzeStates = enable; };
private:
	//std::atomic<bool> newFullImgAwaits;
	std::atomic<bool> keepRestoringManaAngHealth = true;
	std::atomic<bool> keepTakingScreenShots = true;
	std::atomic<bool> keepLoadingScreenShots = true;
	std::atomic<bool> keepAnalyzingMiniMap = true;
	std::atomic<bool> keepAnalyzingMainGameWindow;
	std::atomic<bool> keepHuntingAutomaticly = true;
	std::atomic<bool> keepAnalyzeStates = true;
};
class Timers
{
public:
	Timers() {};
	~Timers() {};
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
private:
	std::atomic<qint64> timeLastItemUsage, timeLastSpellAttack, timeLastSpellHealing, timeLastSpellSupport;
	std::mutex lastTimeUsagesMutex;
	QMap<QString, qint64> lastTimeSpellUsagesMap;
	qint64 now() { return QDateTime::currentMSecsSinceEpoch(); };
};	
struct HealthManaFrames {
	QRect healthFrame;
	QRect manaFrame;
	QRect manaShieldFrame;
	QRect combinedFrame;
	int howTheyShouldBeRotated = 0;
};
class Vitallity{
public:
	Vitallity() {};
	~Vitallity() {};
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
	double getCurrentHealthPercentage() const { return currentHealthPercentage; }
	double getCurrentManaPercentage() const { return currentManaPercentage; }
	double getCurrentMSPercentage() const { return currentMsPercentage; }
	int getCurrentRawManaVal() const { return currentManaRaw; }
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
	void setImageHealth(QImage& img) { healthImg.setImg(img); }
	void setImageMana(QImage& img) { manaImg.setImg(img); }
	void setImageMS(QImage& img) { msImg.setImg(img); }
	void setImageCombined(QImage& img) { combinedImg.setImg(img); }
	void getImageHealth(QImage& img, bool clear = false) {
		img = healthImg.getImgCopy();
		if (clear)
			healthImg.clear();
	}
	void getImageMana(QImage& img, bool clear = false) {
		img = manaImg.getImgCopy();
		if (clear)
			manaImg.clear();
	}
	void getImageMS(QImage& img, bool clear = false) {
		img = msImg.getImgCopy();
		if (clear)
			msImg.clear();
	}
	void getImageCombined(QImage& img, bool clear = false) {
		img = combinedImg.getImgCopy();
		if (clear)
			combinedImg.clear();
	}
	bool needCalibration() {
		if (getHealthArea().isEmpty())
			return true;
		if (getCombinedArea().isEmpty() && getManaArea().isEmpty())
			return true;
		return false;
	}
	void clearHealth() {
		currentHealthPercentage = 100.0;
		currentHealthRaw = 50000;
	}
	void clearMana() {
		currentManaPercentage = 100.0;
		currentManaRaw = 100000;
	}
private:
	std::atomic<double> currentHealthPercentage = 100.0, currentManaPercentage = 100.0, currentMsPercentage = 100.0;
	std::atomic<int> currentHealthRaw = 100, currentManaRaw = 100, currentMSRaw = 100;
	HealthManaFrames healthManaFrames;
	MutexImg healthImg, manaImg, combinedImg, msImg;
};
class MiniMap
{
public:
	MiniMap() {};
	~MiniMap() {};
	void getImgMiniMap(QImage& imgToRet) {
		imgToRet = this->miniMap.getImgCopy();
		this->miniMap.clear();
	}
	void setImgMiniMap(const QImage& miniMapImg) { this->miniMap.setImg(miniMapImg); }
	QRect getFrameMiniMap() {
		return miniMapFrame.getRect();
	}
	void setFrameMiniMap(const QRect& newRect) {
		miniMapFrame.setRect(newRect);
	}
	void getImgMiniMapLayer(QImage& imgToRet) {
		imgToRet = this->minMapLayer.getImgCopy();
		this->minMapLayer.clear();
	}
	void setImgMiniMapLayer(const QImage& minMapLayerImg) { this->minMapLayer.setImg(minMapLayerImg); }
private:
	MutexImg miniMap;
	MutexImg minMapLayer;
	MutexRect miniMapFrame;
};
class MainWindow{
public:
	MainWindow() {};
	~MainWindow() {};
	void getImgMainGameWindow(QImage& imgToRet) { imgToRet = this->gameWindow.getImgCopy(); }
	void setImgMainGameWindow(const QImage& mainGameWindow) { this->gameWindow.setImg(mainGameWindow); }
	QRect getFrameMainGameWindow() {
		return gameWindowFrame.getRect();
	}
	void setFrameMainGameWindow(const QRect& newGameWindowFrame) {
		gameWindowFrame.setRect(newGameWindowFrame);
	}
	QVector<QPoint> getPointsOfFieldsAroundPlayer() {
		QRect frame = getFrameMainGameWindow();
		const int TITLE_WIDTH = frame.width() / TITLES_X;
		const int TITLE_HEIGHT = frame.height() / TITLES_Y;
		QPoint middleOfTitleZeroZero = frame.topLeft() + QPoint(TITLE_WIDTH/2, TITLE_HEIGHT/2);
		QPoint middleOfPlayerTitle = middleOfTitleZeroZero + QPoint(7 * TITLE_WIDTH, 5 * TITLE_HEIGHT + 12);//that way cursor is little too high, 12 to lower in down.
		QVector<QPoint> toRet = {};
		for (int x = -1; x <= 1; x++){
			for (int y = -1; y <= 1; y++) {
				toRet.push_back(middleOfPlayerTitle + QPoint(x * TITLE_WIDTH, y * TITLE_HEIGHT));
			}
		}
		toRet.swapItemsAt(1, 4);
		return toRet;
	}
private:
	const int TITLES_X = 15, TITLES_Y = 11;
	MutexImg gameWindow;
	MutexRect gameWindowFrame;
};
class GameProcess
{
public:
	GameProcess() {};
	~GameProcess() {};
	uint getPid() { return pid; }
	void setPid(uint newPid) { pid = newPid; }
	QString getNameOfGameWindow() { return nameOfGameWindow; }
	void setNameOfGameWindow(QString newNameOfGameWindow) { nameOfGameWindow = newNameOfGameWindow; }
	HWND getHandlerToGameThread() { return handlerToGameThread; }
	void setHandlerToGameThread(HWND newHandlerToGameThread) { handlerToGameThread = newHandlerToGameThread; }
private:
	std::atomic<uint> pid;
	std::atomic<HWND> handlerToGameThread;
	QString nameOfGameWindow;
}; 
class Position
{
public:
	Position() {};
	~Position() {};
	void setPlayerPos(const QPoint xyToSet, int floorToSet) {
		positionMutex.lock();
		xy = xyToSet;
		positionMutex.unlock();
		floor = floorToSet;
	}
	void setPlayerPos(const Point3D toSet) {
		positionMutex.lock();
		xy = QPoint(toSet.getX(), toSet.getY());
		positionMutex.unlock();
		floor = toSet.getFloor();
	}
	void setPlayerPos(const int x, const int y, const int f) {
		positionMutex.lock();
		xy = QPoint(x, y);
		positionMutex.unlock();
		floor = f;
	}
	Point3D getPlayerPos() {
		positionMutex.lock();
		Point3D toRet = Point3D(xy.x(), xy.y(), floor);
		positionMutex.unlock();
		return toRet;
	}
	void clear(){
		xy = QPoint(-1, -1);
		floor = -20;
	}
	int getFloor() { return floor; }
	QPoint getXY() {
		positionMutex.lock();
		QPoint toRet = xy;
		positionMutex.unlock();
		return xy;
	}
private:
	std::mutex positionMutex;
	std::atomic<int> floor = 0;
	QPoint xy;
};
/*
class Equipment
{
public:
	enum STATES { HASTE, BATTLE, PROTECTOR_ZONE, POISONED, PARALYZED, UPGRADED, HUNGER };
	enum class EqRect { None, StateBar };
	Equipment() {};
	~Equipment() {};
	void setStoreRect(const QRect& storeRectToSet) {
		storeRect.setRect(storeRectToSet); 
		QPoint storeRectStart = storeRectToSet.topLeft();
	}
	QRect getStoreRect() { return storeRect.getRect(); }
	void setStateBarImg(const QImage& stateBarImgToSet) { statesBarImg.setImg(stateBarImgToSet); }
	QVector<STATES> getCurrentStates(bool clearImg = true) {
		QImage stateBar = statesBarImg.getImgCopy();
		if(stateBar.isNull())
			return {};
		if (clearImg)
			statesBarImg.clear();
		ImgEditor::imgToBlackAndWhiteOneColor(stateBar, toBlackAndWhiteThreshold);
		QList<QImage> imgs;
		ImgEditor::cutImgWithLettersToSingleLettersImgList(stateBar, imgs);
		QVector<STATES> toRet = {};
		for each (QImage img in imgs){
			ImgEditor::cutBlackBordersOfImg(img);
			QString code = ImgEditor::binaryLetterImgToLetterStr(img);
			if (!codeStateMap.contains(code))
				continue;
			toRet.append(codeStateMap.value(code));
		}
		return toRet;
	}


	void setRect(EqRect eqRect, const QRect rectToSet) {
		switch (eqRect)
		{
		case Equipment::EqRect::StateBar: { 
			stateBarRect.setRect(rectToSet); 
			return; 
		}
		default: break;
		}
	}
	void setImg(EqRect eqRect, const QImage& imgToSet) {
		switch (eqRect)
		{
		case Equipment::EqRect::StateBar: statesBarImg.setImg(imgToSet); return;
		default: break;
		}
	}
private:
	MutexImg statesBarImg;
	MutexRect storeRect, stateBarRect;


	const int toBlackAndWhiteThreshold = 100;
	QMap<QString, STATES> codeStateMap = populateMap();

	QMap<QString, STATES> populateMap() {
		QMap<QString, STATES> toRect = {};
		toRect.insert(getStateCode("battleIco"), STATES::BATTLE);
		toRect.insert(getStateCode("hungerIco"), STATES::HUNGER);
		return toRect;
	}
	QString getStateCode(QString fileName) {
		QImage img(":/statesIcons/" + fileName);
		ImgEditor::imgToBlackAndWhiteOneColor(img, toBlackAndWhiteThreshold);
		ImgEditor::cutBlackBordersOfImg(img);
		QString code = ImgEditor::binaryLetterImgToLetterStr(img);
		return code;
	}
	
};
*/
class VariablesClass : QObject{
	Q_OBJECT
public:	
	typedef LONG64 TIME;
	VariablesClass();
	~VariablesClass();
	void log(QString msg, bool sendToDebug = true, bool sendToUserConsol = true, bool addTimeSTamp = true) {
		logger.log(msg, sendToDebug, sendToUserConsol, addTimeSTamp);
	}
	//unasigned gettersSetters
	void setNewImg(const QImage& newImage) {
		fullImage.setImg(newImage);
	}
	void getCopyOfCurrentFullImg(QImage& img) {
		img = fullImage.getImgCopy();
	}

	Timers& getTimers() {return timers;}
	BattleList& getBattleList() {return battleList;}
	Settings& getSettings() {return settings;}
	Vitallity& getVitalitty() {return vitalitty;}
	MiniMap& getMiniMap() {return minimap;}
	MainWindow& getMainWindow() {return mainWindow;}
	GameProcess& getGameProcess() {return gameProcess;}
	Position& getPosition() {return position;}
	Equipment& getEquipment() {return eq; }
	Logger logger;
	bool playingOnSmallMonitor = false;
private:

	GameProcess gameProcess;
	MainWindow mainWindow;
	BattleList battleList;
	Settings settings;
	Timers timers;
	Vitallity vitalitty;
	MiniMap minimap;
	Position position;
	//other
	MutexImg fullImage;
	Equipment eq;
};


