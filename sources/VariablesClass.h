#pragma once
#include <QObject>
#include "qmap.h"
class VariablesClass : public QObject
{
	Q_OBJECT
public:
	uint var_pidOfGame;
	QString var_winTitleOfGame;
	QImage* var_img;
	QImage* var_imgHealth;
	bool var_imgHealthManaStates_readyToRead;
	QImage* var_imgMana;
	QImage* var_imgStates;

	void initQmap();
	VariablesClass();
	~VariablesClass();
};
