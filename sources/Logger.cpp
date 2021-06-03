#include "Logger.h"
#include "qdatetime.h"
Logger::Logger(QObject *parent)
	: QObject(parent){

}

Logger::~Logger(){
	
}

void Logger::logPotenialBug(const QString textToSave, const QString className, const QString functionName){
	#ifndef DEBUG
	return;
	#endif

	QString fileName = QDateTime::currentDateTime().toString("dd_MM_yyyy") + ".txt";
	QString pathToFile = getPathToLogFolder() + "/" + fileName;
	QFile file(pathToFile);
	bool openCorrectly = file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
	if (!openCorrectly) {//waiting for acess to file
		int i = 10;
		while (i > 0) {
			_sleep(1);
			i--;
			openCorrectly = file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
			if (openCorrectly)
				break;
		}
	}
	if (openCorrectly) {
		QString firstPart = getTimeStamp() + "[" + className + "] [" + functionName + "] ";
		QString toWrite = firstPart + textToSave + "\n\n";
		QTextStream stream(&file);
		stream << toWrite;
		file.close();
	}
}

QString Logger::getPathToLogFolder(){
	QDir dir = QDir::tempPath();
	dir.cdUp();
	QString pathToTmpFolder = dir.path();

	QString pathToCrackerJackFolder = pathToTmpFolder + "/CrackerJack";
	bool crackerJackFolderExist = QDir(pathToCrackerJackFolder).exists();
	if (!crackerJackFolderExist)
		QDir(pathToTmpFolder).mkpath(pathToCrackerJackFolder);

	QString pathToLogFolder = pathToCrackerJackFolder + "/Logs";
	bool LogDirExist = QDir(pathToLogFolder).exists();
	QString toRet = LogDirExist ? pathToLogFolder : QDir::currentPath();
	return toRet;
}

void Logger::logPotenialBug(const QString textToSave){
	logPotenialBug(textToSave, "", "");
}

QString Logger::getTimeStamp(){
	QDateTime time = QDateTime::currentDateTime();
	return "[" + time.toString("dd/MM/yyyy hh:mm:ss:zzz")+ "] ";
}

