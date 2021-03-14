#pragma once
#include <QObject>

class VariablesClass : public QObject
{
	Q_OBJECT
public:
	uint var_pidOfGame;
	QString var_winTitleOfGame;
	QImage* var_img;


	VariablesClass();
	~VariablesClass();
};
