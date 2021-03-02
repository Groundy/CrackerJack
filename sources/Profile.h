#pragma once
#include "qmap.h"
#include <QObject>
#include <string>
#include "qstring.h"
#include "qvector.h"
//#include <qsplitter.h>
#include "qkeysequence.h"

enum PROFESSION { RP, EK, ED, MS};

class Profile : public QObject 
{
	Q_OBJECT

public:
	Profile(QObject *parent);
	Profile();
	~Profile();

	QList<int> healthRestorePercentages;//0-5;
	QList<int> ManaRestoreMethodesPercentage;
	QList<QKeySequence> healthKeys;
	QList<QKeySequence> ManaKeys;

	QString profileName;
	PROFESSION profession;
	QString lastLoginDate;
	QString creationDate;


	static QString VectorToString(QList<int> vect);
	static QList<int> StringToVector(QString str);

	static QString KeyListToString(QList<QKeySequence> keylist);
	static QList<QKeySequence> StringToKeyList(QString str);

	static int ProfToInt(PROFESSION prof);
	static PROFESSION ProfToInt(int profInt);
};
