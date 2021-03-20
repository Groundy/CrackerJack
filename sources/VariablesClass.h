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
	enum calibrationState{NOT_CALIBRATED = (1024*1024*1024), DURING_CALIBRATION, CALIBRATED};
	QImage* var_healthPieceImg;
	QImage* var_manaPieceImg;
	QImage* var_manaShieldPieceImg;
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
