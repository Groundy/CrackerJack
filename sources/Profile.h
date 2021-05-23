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
	//Profile(Profile& prof);
	Profile();
	~Profile();

	QList<int> healthRestorePercentages;//re
	QList<int> ManaRestoreMethodesPercentage;//re
	QList<Key> healthKeys;//re
	QList<Key> ManaKeys;//re
	QList<QString> healthRestoreMethodeNames;
	QList<QString> manaRestoreMethodeNames;

	struct Frames {
		QRect gameFrame;
		QRect miniMapFrame;
		QRect healthFrame;
		QRect manaFrame;
		QRect manaShieldFrame;
		QRect combinedFrame;
		int howTheyShouldBeRotated;
	};
	Frames frames;

	PROFESSION profession;//re
	AUTO_LOOT_KEY lootKey;
	CONTROLS controls;
	QString profileName;//re
	long long lastLoginSeconds;
	long long creationTimeSeconds;//TODO tu powinny byæ LONG64 ale nie wiem jak header daæ
	long long secondsSpentSeconds;
	void clearProfile();
	void getDataFromOtherProf(Profile& profToCopyFrom);

};
