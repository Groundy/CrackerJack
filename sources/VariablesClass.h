#pragma once
#include <QObject>

class VariablesClass : public QObject
{
	Q_OBJECT
public:
	uint var_pidOfGame;
	QString var_winTitleOfGame;
	QImage* var_img;
	QImage* var_img_health;
	QImage* var_img_mana;


	VariablesClass();
	~VariablesClass();
};
