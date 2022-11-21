#pragma once
#include <qmap.h>
#include <QObject>
#include <string>
#include <qjsonarray.h>
#include <qstring.h>
#include <qvector.h>
#include <qkeysequence.h>
#include <Windows.h>
#include <qlist.h>

#include "Key.h"
#include "Profession.h"
#include "RestorationMethode.h"
#include "Utilities.h"
struct Frames {
	QRect gameFrame;
	QRect miniMapFrame;
	QRect healthFrame;
	QRect manaFrame;
	QRect manaShieldFrame;
	QRect combinedFrame;
	int howTheyShouldBeRotated;
};
class Profile{
public:
	Profile();
	Profile(QJsonObject obj);
	QJsonObject toJson();
	Frames frames;
	void copyFrom(Profile& profToCopyFrom);
	QStringList getHealthRestorationNames();
	QStringList getManaRestorationNames();

	enum AUTO_LOOT_KEY { RIGHT_MOUSE_BUTTON, SHIFT_RIGHT, LEFT_MOUSE_BUTTON };//enable only if controls
	enum CONTROLS { CLSSIC_CONTROLS, REGULARCONTROLS, LEFT_SMART_CLICK };
	enum STATES { HASTE, BATTLE, PROTECTOR_ZONE, POISONED, PARALYZED, UPGRADED };

	QString profileName;
	Profession profession;
	QList<RestorationMethode> manaRestorations;
	QList<RestorationMethode> healthRestorations;
	AUTO_LOOT_KEY lootKey;
	CONTROLS controls;
	QList<STATES> states;
	Key screenShotKey;
	void clearProfile();

};
