#pragma once
#include <qfile>
#include <QObject>
#include <qdir.h>
#include <qtextstream.h>
class Logger{
public:
	Logger();
	static void logPotenialBug(const QString textToSave);
	static void logPotenialBug(const QString textToSave, const QString className, const QString functionName);
	static QString getPathToLogFolder();
	static QString getTimeStamp();
	~Logger();
};
