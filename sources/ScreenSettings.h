#pragma once

#include <QObject>


enum ARCS_STYLE { DEFAULT, COMPACT, PARALLER, LARGE };
enum POSITION { LEFT, RIGHT, BOTTOM, TOP };


 class ScreenSettings : public QObject
{
	Q_OBJECT

public:
	ScreenSettings(QObject *parent);
	ScreenSettings();
	~ScreenSettings();
	int numberOfAddedBarsLeft;
	int numberOfAddedBarsRight;

};
