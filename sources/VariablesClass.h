#pragma once
#include <QObject>
#include "qmap.h"
#include "qimage.h"
class VariablesClass : public QObject
{
	Q_OBJECT
public:
	uint var_pidOfGame;
	QString var_winTitleOfGame;

	//Advaned health and mana shearch
	enum calibrationState{NOT_CALIBRATED, CALIBRATED, DURING_CALIBRATION};
	QImage var_healthPieceImg;
	QImage var_manaPieceImg;
	QImage var_manaShieldPieceImg;
	QImage var_combinedBoxPieceImg;
	float health, mana, manashield;
	int rotationNeededForPointsAbove;
	bool manaFound, healthFound, manaShieldFound, combinedFound;
	bool newData;
	calibrationState caliState;

	static QString outPutFolderPath();
	VariablesClass();
	~VariablesClass();
};
