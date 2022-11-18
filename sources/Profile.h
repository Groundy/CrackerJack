#pragma once
#include "qmap.h"
#include <QObject>
#include <string>
#include "qstring.h"
#include "qvector.h"
#include "qkeysequence.h"
#include "Key.h"
#include "Windows.h"


class Profile{
public:
	enum PROFESSION { RP, EK, ED, MS};
	enum AUTO_LOOT_KEY { RIGHT_MOUSE_BUTTON, SHIFT_RIGHT, LEFT_MOUSE_BUTTON };//enable only if controls
	enum CONTROLS { CLSSIC_CONTROLS, REGULARCONTROLS, LEFT_SMART_CLICK };
	struct Frames {
		QRect gameFrame;
		QRect miniMapFrame;
		QRect healthFrame;
		QRect manaFrame;
		QRect manaShieldFrame;
		QRect combinedFrame;
		int howTheyShouldBeRotated;
	};
	//enum STATES {HASTE, BATTLE, PROTECTOR_ZONE, POISONED, PARALYZED, UPGRADED}
	Profile();
	~Profile();

	QList<int> healthRestorePercentages;//re
	QList<int> ManaRestoreMethodesPercentage;//re
	QList<Key> healthKeys;//re
	QList<Key> ManaKeys;//re
	QList<QString> healthRestoreMethodeNames;
	QList<QString> manaRestoreMethodeNames;


	Frames frames;

	PROFESSION profession;//re
	AUTO_LOOT_KEY lootKey;
	CONTROLS controls;
	QString profileName;//re
	LONG64 lastLoginSeconds;
	LONG64 creationTimeSeconds;
	LONG64 secondsSpentSeconds;
	void clearProfile();
	void getDataFromOtherProf(Profile& profToCopyFrom);

};
