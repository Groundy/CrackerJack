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
#include "qlistwidget.h"
#include "qsqlfield.h"
#include "NewProfileConfiguartor.h"

class ProfileDataBaseManager : public QObject
{
	Q_OBJECT

public:
	ProfileDataBaseManager();
	~ProfileDataBaseManager();
	int getNumberOfRecords();
	enum FieldsOfDB {
		ID,
		PROFILE_NAME,
		PROFESION,
		MANA_RESTORE_STRING,
		HEALTH_RESTORE_STRING,
		MANA_RESTORE_KEY,
		HEALTH_RESTORE_KEY,
		LAST_LOGIN,
		CREATION_TIME,
	};
	static QString translateDBFields(FieldsOfDB atr);
	static QString getValueOfCell(FieldsOfDB atr, QString profileName);
	QSqlDatabase dataBase;
	const QString DATABASE_NAME = "Profiles.db";
	QDir pathToDB;
	void openDB();
	void makeDataBase();
	void getPathToDB();
	bool checkIfDbExist();
	void addRecord(QString profileName);
	void deleteRecord(QString name);
	void modifyAtribute(QString profileName ,FieldsOfDB atr, QString newValue);
	QStringList getListOfAllRecords();
	void saveProfileToDatabase(Profile* prof);
};
