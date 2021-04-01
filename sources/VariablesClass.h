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
	bool useAdvancedShearch = true;

	//Advaned health and mana shearch
	enum calibrationState{CALIBRATED, NOT_CALIBRATED , DURING_CALIBRATION};
	QImage var_healthPieceImg;
	QImage var_manaPieceImg;
	QImage var_manaShieldPieceImg;
	bool isManaShieldActive;
	calibrationState caliState;
	int rotationNeededForPointsAbove;

	//simply health and mana shearch
	QImage healthBarImg;
	QImage manaBarImg;

	static QString outPutFolderPath();
	VariablesClass();
	~VariablesClass();
};
