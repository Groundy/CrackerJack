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
#include "qdatetime.h"
class ProfileDataBaseManager : public QObject
{
	Q_OBJECT

public:
	ProfileDataBaseManager();
	~ProfileDataBaseManager();
	int getNumberOfRecords();
	enum FieldsOfDB {
		PROFILE_NAME,
		PROFESION,
		MANA_RESTORE_STRING,
		HEALTH_RESTORE_STRING,
		MANA_RESTORE_KEY,
		HEALTH_RESTORE_KEY,
		MANA_RESTORE_ITEM,
		HEALTH_RESTORE_ITEM,
		LAST_LOGIN,
		CREATION_TIME,
		POS_HP,		POS_SHP,		POS_GHP,		POS_UHP,		POS_SPHP,
		POS_GSP,		POS_USP,
		POS_MP,		POS_SMP,		POS_GMP,		POS_UMP,


		END
	};
	QMap <FieldsOfDB, QString> getMapOfDBFields();
	QMap <FieldsOfDB, QString> field_exactNameInDb_map = getMapOfDBFields();
	

	QString getValueOfCell(FieldsOfDB atr, QString profileName);
	QSqlDatabase dataBase;
	const QString DATABASE_NAME = "Profiles.db";
	QDir pathToDB;
	void openDB();
	void makeDataBase();
	void getPathToDB();
	bool checkIfDbExist();
	void addRecord(QString profileName);
	void deleteRecord(QString name);
	bool modifyAtribute(QString profileName ,FieldsOfDB atr, QString newValue);
	QStringList getListOfAllRecords();
	void saveProfileToDatabase(Profile* prof);
	void readProfileFroDataBase(Profile* prof, QString profileName);
	
	QString DB_writer_ManaAndHealthRestorePercentages(QList<int> vect);
	QList<int> DB_reader_ManaAndHealthRestorePercentages(QString str);

	QString DB_writer_ManaAndHealthKeys(QList<Key> keylist);
	QList<Key> DB_reader_ManaAndHealthKeys(QString str);

	QString DB_writer_ManaAndHealthRestoreMethhodesNames(QStringList keylist);
	QList<QString> DB_reader_ManaAndHealthRestoreMethhodesNames(QString str);

	QString DB_writer_PosOfItem(QRect rect);
	QRect DB_reader_PosOfItem(QString str);
	static void writeItemPosToDb(QString profileName, FieldsOfDB dbField, QRect rectToSave);
	static void readItemPosToDb(QString profileName, FieldsOfDB dbField, QRect& rectToRead);
};
