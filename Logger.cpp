#include "Logger.h"
#include "qdatetime.h"
Logger::Logger(QObject *parent)
	: QObject(parent){

}

Logger::~Logger(){
	
}

void Logger::logPotenialBug(const QString textToSave, const QString className, const QString functionName){
	//TODO dodac jakas sensowna sciezka do patha a nie pliki projeku
	//TODO dodac wykoanie tej funkcji jedynie dla trybu debug
	QString onlyDate = QDateTime::currentDateTime().toString("dd_MM_yyyy");
	QString pathToFile = QDir::current().absoluteFilePath(onlyDate + ".txt");
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
		QString toWrite = firstPart + textToSave + "\n";
		QTextStream stream(&file);
		stream << toWrite;
		file.close();
	}
}

void Logger::logPotenialBug(const QString textToSave){
	logPotenialBug(textToSave, "", "");
}

QString Logger::getTimeStamp(){
	QDateTime time = QDateTime::currentDateTime();
	return "[" + time.toString("dd/MM/yyyy hh:mm:ss:zzz")+ "] ";
}

