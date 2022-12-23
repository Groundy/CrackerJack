#pragma once
#include "Equipment.hpp"
#include "BattleList.hpp"
#include "Settings.hpp"
#include "Vitallity.hpp"	
#include "Timers.hpp"
#include "GameWindow.hpp"
#include "PlayerPosition.hpp"
#include "GameProcessData.hpp"

class VariablesClass{
public:	
	VariablesClass() {};
	~VariablesClass() {};
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
	GameWindow& getGameWindow() {return gameWindow;}
	GameProcessData& getGameProcess() {return gameProcess;}
	PlayerPosition& getPosition() {return position;}
	Equipment& getEquipment() {return eq; }
	Logger logger;
	bool playingOnSmallMonitor = false;
private:
	GameProcessData gameProcess;
	GameWindow gameWindow;
	BattleList battleList;
	Settings settings;
	Timers timers;
	Vitallity vitalitty;
	MiniMap minimap;
	PlayerPosition position;
	//other
	MutexImg fullImage;
	Equipment eq;
};


