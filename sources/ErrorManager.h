#pragma once
#include "qmap.h"
#include <QObject>

class ErrorManager : public QObject
{
	Q_OBJECT

public:

	ErrorManager();
	~ErrorManager();
};
