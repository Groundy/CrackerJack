#pragma once
#include "qmap.h"
#include <QObject>

class ErrorManager : public QObject
{
	Q_OBJECT
public:
	enum screenReading {
		OK = 0,
		CANT_GET_SCREEN_NAME_FROM_SCREENSHOT_FOLDER = 2,
		CANT_LOAD_SCREEN_FROM_SCREENSHOT_FOLDER = 4,
	};
	ErrorManager();
	~ErrorManager();
};
