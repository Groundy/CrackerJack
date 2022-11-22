#pragma once
#include <QObject>
#include "qmap.h"
#include "qimage.h"
#include <basetsd.h>
#include <atomic>
class VariablesClass{
	typedef LONG64 TIME;
public:
	struct Frames {
		QRect gameFrame;
		QRect miniMapFrame;
		QRect healthFrame;
		QRect manaFrame;
		QRect manaShieldFrame;
		QRect combinedFrame;
		int howTheyShouldBeRotated;
	};
	Frames frames;
	enum STATES { HASTE, BATTLE, PROTECTOR_ZONE, POISONED, PARALYZED, UPGRADED };
	uint var_pidOfGame;
	QString var_winTitleOfGame;

	bool HealthAndManaRestorationShouldBeActive = false;
	QImage wholeImg;
	QImage var_healthPieceImg;
	QImage var_manaPieceImg;
	QImage var_manaShieldPieceImg;
	QImage var_combinedBoxPieceImg;

	std::atomic<float> health, mana, manashield;
	int rotationNeededForPointsAbove;
	std::atomic<bool> manaFound, healthFound, manaShieldFound, combinedFound;
	bool newData;
	
	//
	//TIME lastTimeMethodeUsed_Healing[5];
	//TIME lastTimeMethodeUsed_Mana[5];
	TIME lastTimeUsed_spell_healing;
	TIME lastTimeUsed_spell_attack;
	TIME lastTimeUsed_spell_support;
	TIME lastTimeUsed_item;

	//
	QMap<QString, QRect> potionName_rectPosOnScreen_map = getMapWithRects();


	static QMap<QString, QRect> getMapWithRects();
	VariablesClass();
	~VariablesClass();




};
