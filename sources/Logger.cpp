#include "Logger.h"
#include "qdatetime.h"
Logger::Logger(){
	

}

Logger::~Logger(){
	
}

void Logger::logPotenialBug(const QString textToSave, const QString className, const QString functionName){
	#ifndef _DEBUG
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
	const QString CURRENT_PATH = QDir::currentPath();
	QDir dir = QDir::tempPath();
	dir.cdUp();
	QString pathToTmpFolder = dir.path();

	QString pathToCrackerJackFolder = pathToTmpFolder + "/CrackerJack";
	bool crackerJackFolderExist = QDir(pathToCrackerJackFolder).exists();
	if (!crackerJackFolderExist) {
		bool sucess = QDir(pathToTmpFolder).mkpath(pathToCrackerJackFolder);
		if (!sucess)
			return CURRENT_PATH;
	}


	QString pathToLogFolder = pathToCrackerJackFolder + "/Logs";
	bool LogDirExist = QDir(pathToLogFolder).exists();
	if (!LogDirExist){
		bool sucess = QDir(pathToTmpFolder).mkpath(pathToLogFolder);
		if (!sucess)
			return CURRENT_PATH;
	}
	return pathToLogFolder;
}

void Logger::logPotenialBug(const QString textToSave){
	logPotenialBug(textToSave, "", "");
}

QString Logger::getTimeStamp(){
	QDateTime time = QDateTime::currentDateTime();
	return "[" + time.toString("hh:mm:ss:zzz")+ "] ";
}

