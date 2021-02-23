#pragma once
#include "qmap.h"
#include <QObject>
#include <string>
#include "qstring.h"
#include "qvector.h"
//#include <qsplitter.h>

class Profile : public QObject 
{
	Q_OBJECT

public:
	Profile(QObject *parent);
	~Profile();
private:
	QVector<int> healthRestorePercentages;//0-5;
	QVector<int> ManaRestoreMethodesPercentage;
	int profileNumber;//1-9
	QString profileName;
	
	QString VectorToString(QVector<int> vect);
	QVector<int> StringToVector(QString str);
};
