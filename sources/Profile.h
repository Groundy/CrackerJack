#pragma once
#include "qmap.h"
#include <QObject>
#include <string>
#include "qstring.h"
#include "qvector.h"
#include "qkeysequence.h"
#include "Key.h"
#include "Windows.h"
#include <qexception.h>
#include "Profession.h"
struct Frames {
	QRect gameFrame;
	QRect miniMapFrame;
	QRect healthFrame;
	QRect manaFrame;
	QRect manaShieldFrame;
	QRect combinedFrame;
	int howTheyShouldBeRotated;
};
class RestorationStruct {
public:
	RestorationStruct(int threshold, Key key, QString restorationName) : threshold(threshold), key(key), name(restorationName){
	}
	RestorationStruct(QJsonObject jsonObj) {
		if (!jsonObj.contains("threshold"))
			throw std::exception("There is no threshold field in RestorationStruct in json prof file");
		QJsonValue val = jsonObj.value("threshold");
		if (val.isNull() || !val.isDouble())
			throw std::exception("Invalid RestorationStruct field value!");
		int thresholdToSet = val.toInt();

		if (!jsonObj.contains("methodeName"))
			throw std::exception("There is no methodeName field in RestorationStruct in json prof file");
		val = jsonObj.value("methodeName");
		if (val.isNull() || !val.isString())
			throw std::exception("Invalid RestorationStruct field value!");
		QString methodeNameToSet = val.toString();

		if (!jsonObj.contains("key"))
			throw std::exception("There is no key field in RestorationStruct in json prof file");
		val = jsonObj.value("key");
		if (val.isNull() || !val.isObject())
			throw std::exception("Invalid RestorationStruct field value!");
		Key keyToSet = Key(val.toObject());
		
		key = keyToSet;
		name = methodeNameToSet;
		threshold = thresholdToSet;
	}

	QJsonObject toJsonObj() { 
		QJsonObject mainObj;
		mainObj.insert("threshold", threshold);
		mainObj.insert("methodeName", name);
		mainObj.insert("key", key.toJson());
		return mainObj;
	};
	QString getName() {
		return name;
	};
private:
	int threshold;
	Key key;
	QString name;
};

class Profile{
public:
	Profile();
	Profile(QJsonObject obj);
	void toJson();
	Frames frames;
	void copyFrom(Profile& profToCopyFrom);
	QStringList getHealthRestorationNames();
	QStringList getManaRestorationNames();
public:
	enum AUTO_LOOT_KEY { RIGHT_MOUSE_BUTTON, SHIFT_RIGHT, LEFT_MOUSE_BUTTON };//enable only if controls
	enum CONTROLS { CLSSIC_CONTROLS, REGULARCONTROLS, LEFT_SMART_CLICK };
	enum STATES { HASTE, BATTLE, PROTECTOR_ZONE, POISONED, PARALYZED, UPGRADED };

	QString profileName;
	Profession profession;
	QList<RestorationStruct> manaRestorations;
	QList<RestorationStruct> healthRestorations;
	AUTO_LOOT_KEY lootKey;
	CONTROLS controls;
	QList<STATES> states;

	void clearProfile();

};
