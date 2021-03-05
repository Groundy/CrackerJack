#pragma once

#include <QObject>

class miniMapAnalyser : public QObject
{
	Q_OBJECT
	

public:

	miniMapAnalyser(QObject *parent);
	~miniMapAnalyser();
};
