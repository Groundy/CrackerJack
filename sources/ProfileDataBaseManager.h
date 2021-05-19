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
#include "qstring.h"
class ProfileDataBaseManager : public QObject
{
	Q_OBJECT

public:
	ProfileDataBaseManager();
	~ProfileDataBaseManager();
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
	
	QString pathToFolderWithProfiles;
	QStringList getAllNamesOfProfFiles();
	bool getPathToProfileFile(QString profName, QString& path);
	bool modifyFieldValue(QString profName, FieldsOfDB field, QString newValue);
	bool readFieldValue(QString profName, FieldsOfDB field, QString& readValue);
	bool addNewProfileToFolder(QString profName);
	bool deleteProfile(QString profName);
	bool getNamesOfAllFilesInFormToDisplay(QStringList& namesOfProfiles);
	bool saveProfileToDataBase(Profile& profileToSave);
	bool readProfileFromDataBase(QString profileName, Profile& profileToSave);





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
