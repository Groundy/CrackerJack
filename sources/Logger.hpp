#pragma once
#include <qfile>
#include <QObject>
#include <qdir.h>
#include <qtextstream.h>
#include <qdebug.h>
#include <qdatetime.h>
#include <atomic>

class Logger : public QObject{
	Q_OBJECT

signals:
	void sendMsgToUserConsol(QStringList);
	void sendMsgToUserConsolRed(QString);
public:
	Logger() {};
	~Logger() {};
	void log(QString msg, bool sendToDebug = true, bool sendToUserConsol = false, bool addTimeSTamp = true) {
		if (addTimeSTamp)
			msg.push_front(getTimeStamp());

		if (sendToDebug)
			qDebug() << msg;
		if (sendToUserConsol)
			emit sendMsgToUserConsol(QStringList() << msg);
	}
	static void staticLog(QString msg) {
		qDebug() << msg;
	}

private:
	std::atomic<long long> time = QDateTime::currentDateTime().toMSecsSinceEpoch();

	QString getTimeStamp() {
		QDateTime time = QDateTime::currentDateTime();
		return time.toString("mm:ss:zzz") + " ";
	};
	
	QString getMSecTillLastCommunicate() {
		qint64 current = QDateTime::currentDateTime().toMSecsSinceEpoch();
		qint64 timeDifference = current - time;
		time = current;
		return QString::number(timeDifference) + "  ";
	}
	
	//static void logPotenialBug(const QString textToSave);
	//static void logPotenialBug(const QString textToSave, const QString className, const QString functionName);
	//static QString getPathToLogFolder();
};
