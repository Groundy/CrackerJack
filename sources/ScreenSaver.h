#pragma once
#include <QThread>
#include "Utilities.h"
class ScreenSaver : public QThread
{
	Q_OBJECT

public:
	QString pathToScreenFolder;
	void ScreenLoop();
	void run() override;
	ScreenSaver(QObject *parent);
	ScreenSaver();
	~ScreenSaver();
};
