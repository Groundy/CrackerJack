#pragma once
#include "qmap.h"
#include <QObject>
#include <string>
#include "qstring.h"
#include "qvector.h"
#include "qkeysequence.h"
#include "Key.h"

class Profile : public QObject{
	Q_OBJECT

public:
	enum PROFESSION { RP, EK, ED, MS};
	enum AUTO_LOOT_KEY { RIGHT_MOUSE_BUTTON, SHIFT_RIGHT, LEFT_MOUSE_BUTTON };//enable only if controls
	enum CONTROLS { CLSSIC_CONTROLS, REGULARCONTROLS, LEFT_SMART_CLICK };

	Profile(QObject *parent);
	Profile(const Profile&);
	Profile();
	~Profile();

	QList<int> healthRestorePercentages;//re
	QList<int> ManaRestoreMethodesPercentage;//re
	QList<Key> healthKeys;//re
	QList<Key> ManaKeys;//re
	QList<QString> healthRestoreMethodeNames;
	QList<QString> manaRestoreMethodeNames;

	PROFESSION profession;//re
	AUTO_LOOT_KEY lootKey;
	CONTROLS controls;
	QString profileName;//re
	QString lastLoginDate;
	QString creationDate;
	QString secondsSpent;
	void clearProfile();

};
