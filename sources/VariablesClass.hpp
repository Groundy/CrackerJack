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
	~VariablesClass() {
		delete battleList;
		delete eq;
	};
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
	Equipment& getEquipment() {return *eq; }
	Logger& getLogger() { return logger; }
	ImgEditor& getImgEditorObj() { return imgEditor; }
	QSound* sound;

private:
	Logger logger;
	GameProcessData gameProcess;
	GameWindow gameWindow;
	BattleList* battleList = new BattleList(&logger, &imgEditor);
	Settings settings;
	Timers timers;
	Vitallity vitalitty;
	MiniMap minimap;
	PlayerPosition position;
	//MutexImg fullImage;
	Equipment* eq = new Equipment(&imgEditor);
	ImgEditor imgEditor;
};


