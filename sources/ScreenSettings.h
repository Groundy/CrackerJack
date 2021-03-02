#pragma once

#include <QObject>

enum AUTO_LOOT_KEY { RIGHT, SHIFT_RIGHT, LEFT };
enum CONTROLS { CLSSIC_CONTROLS, REGULARCONTROLS, LEFT_SMART_CLICK };
enum ARCS_STYLE { DEFAULT, COMPACT, PARALLER, LARGE };
enum POSITION { LEFT, RIGHT, BOTTOM, TOP };


 class ScreenSettings : public QObject
{
	Q_OBJECT

public:
	ScreenSettings(QObject *parent);
	~ScreenSettings();
};
