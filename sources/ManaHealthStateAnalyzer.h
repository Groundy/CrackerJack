#pragma once

#include <QThread>

class ManaHealthStateAnalyzer : public QThread
{
	Q_OBJECT

public:
	ManaHealthStateAnalyzer(QObject *parent);
	~ManaHealthStateAnalyzer();
};
