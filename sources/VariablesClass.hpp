#pragma once
#include "Equipment.hpp"
#include "BattleList.hpp"
#include "Settings.hpp"
#include "Vitallity.hpp"	
#include "Timers.hpp"
#include "GameWindow.hpp"
#include "PlayerPosition.hpp"
#include "GameProcessData.hpp"
#include <qsound.h>
#include "Logger.hpp"
class VariablesClass{
public:	
	VariablesClass(QSound* sound) : sound(sound) {
		sound->setLoops(QSound::Infinite);
	};
	~VariablesClass() {};
	void log(QString msg, bool sendToDebug = true, bool sendToUserConsol = true, bool addTimeSTamp = true) {
		logger.log(msg, sendToDebug, sendToUserConsol, addTimeSTamp);
	}
	Timers& getTimers() {return timers;}
	BattleList& getBattleList() {return *battleList;}
	Settings& getSettings() {return settings;}
	Vitallity& getVitalitty() {return vitalitty;}
	MiniMap& getMiniMap() {return minimap;}
	GameWindow& getGameWindow() {return gameWindow;}
	GameProcessData& getGameProcess() {return gameProcess;}
	PlayerPosition& getPosition() {return position;}
	Equipment& getEquipment() {return eq; }
	Logger logger;
	bool playingOnSmallMonitor = false;
	QSound* sound;
private:
	GameProcessData gameProcess;
	GameWindow gameWindow;
	BattleList* battleList = new BattleList(&logger);
	Settings settings;
	Timers timers;
	Vitallity vitalitty;
	MiniMap minimap;
	PlayerPosition position;
	//MutexImg fullImage;
	Equipment eq;

};


