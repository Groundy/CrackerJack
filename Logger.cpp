#include "Logger.h"
#include "qdatetime.h"
Logger::Logger(QObject *parent)
	: QObject(parent){

}

Logger::~Logger(){
	
}

void Logger::logPotenialBug(const QString textToSave, const QString className, const QString functionName){
	QString firstPart = getTimeStamp() + "[" + className + "] [" + functionName + "] ";
	QString onlyDate = QDateTime::currentDateTime().toString("dd_MM_yyyy");
	//TODO dodac jakas sensowna sciezka do patha a nie pliki projeku
	QString pathToFile = QDir::current().absoluteFilePath(onlyDate + ".txt");
	QString toWrite = firstPart + textToSave + "\n";
	QFile file(pathToFile);
	file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
	QTextStream stream(&file);
	stream << toWrite;
	file.close();
}

void Logger::logPotenialBug(const QString textToSave){
	logPotenialBug(textToSave, "", "");
}

QString Logger::getTimeStamp(){
	QDateTime time = QDateTime::currentDateTime();
	return "[" + time.toString("dd/MM/yyyy hh:mm:ss:zzz")+ "] ";
}

