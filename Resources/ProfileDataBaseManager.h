#pragma once
#include <QObject>
#include "qsqldatabase.h"
#include "qsqlquery.h"
#include "qfile.h"
#include <QDir>
#include "Utilities.h"
#include <qstring.h>
#include "Profile.h"
#include "qvariant.h"
#include "qsqlrecord.h"

class ProfileDataBaseManager : public QObject
{
	Q_OBJECT

public:
	ProfileDataBaseManager();
	~ProfileDataBaseManager();
private:
	enum FieldsOfDB {
		ID, /*ID4*/
		PROFILE_NAME,
		PROFESION,
		MANA_RESTORE_STRING,
		HEALTH_RESTORE_STRING
	};

	QSqlDatabase dataBase;
	const QString DATABASE_NAME = "Profiles.db";
	QDir pathToDB;
	void openDB();
	void makeDataBase();
	void readDB();
	void addNewProfile();
	void getPathToDB();
	bool checkIfDbExist();
	void addEmptyRecord();
	int getNumberOfRecords();
	QString getValueOfCell(FieldsOfDB atr, int profId = 1);
	void modifyAtribute(FieldsOfDB, QString newValue, int profId = 1);
	QString translateDBFields(FieldsOfDB atr);
};
