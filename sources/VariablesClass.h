#pragma once
#include <QObject>
#include "qmap.h"
#include "qimage.h"
#include <basetsd.h>
#include <atomic>
class VariablesClass{
public:
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
	LONG64 lastTimeMethodeUsed_Healing[5];
	LONG64 lastTimeMethodeUsed_Mana[5];
	LONG64 lastTimeUsed_spell_healing;
	LONG64 lastTimeUsed_spell_attack;
	LONG64 lastTimeUsed_spell_support;
	LONG64 lastTimeUsed_item;

	//
	QMap<QString, QRect> potionName_rectPosOnScreen_map = getMapWithRects();


	static QMap<QString, QRect> getMapWithRects();
	VariablesClass();
	~VariablesClass();
};
