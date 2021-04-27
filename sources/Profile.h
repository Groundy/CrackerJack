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
	enum KEY_ITEM { POTION, SPELL, CUPCAKE, OTHER };

	Profile(QObject *parent);
	Profile(const Profile&);
	Profile();
	~Profile();

	QList<int> healthRestorePercentages;//done
	QList<int> ManaRestoreMethodesPercentage;//done
	QList<Key> healthKeys;//done
	QList<Key> ManaKeys;//done
	QList<KEY_ITEM> healthItems;//done
	QList<KEY_ITEM> manaItems;//done

	PROFESSION profession;//done
	AUTO_LOOT_KEY lootKey;
	CONTROLS controls;
	QString profileName;//done
	QString lastLoginDate;
	QString creationDate;
	QString secondsSpent;
	Profile operator=(const Profile prof) { return this; };
	void clearProfile();
};
