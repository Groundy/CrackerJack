#pragma once
#include <QObject>
#include "qmap.h"
#include "qimage.h"
#include <basetsd.h>
class VariablesClass : public QObject
{
	Q_OBJECT
public:
	uint var_pidOfGame;
	QString var_winTitleOfGame;

	bool HealthAndManaRestorationShouldBeActive = false;

	//Advaned health and mana shearch
	//enum class calibrationState{NOT_CALIBRATED, CALIBRATED, DURING_CALIBRATION};
	QImage wholeImg;
	QImage var_healthPieceImg;
	QImage var_manaPieceImg;
	QImage var_manaShieldPieceImg;
	QImage var_combinedBoxPieceImg;
	float health, mana, manashield;
	int rotationNeededForPointsAbove;
	bool manaFound, healthFound, manaShieldFound, combinedFound;
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
