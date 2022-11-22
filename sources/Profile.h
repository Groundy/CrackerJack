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

class Profile{
public:
	struct Frames {
		QRect gameFrame;
		QRect miniMapFrame;
		QRect healthFrame;
		QRect manaFrame;
		QRect manaShieldFrame;
		QRect combinedFrame;
		int howTheyShouldBeRotated;
	};
	enum AutoLootKey { RIGHT_MOUSE_BUTTON, SHIFT_RIGHT, LEFT_MOUSE_BUTTON };//enable only if controls
	enum Controls { CLSSIC_CONTROLS, REGULARCONTROLS, LEFT_SMART_CLICK };

	Profile();
	Profile(QJsonObject obj);
	Profile(const Profile& profile);
	Frames frames;

	//funcs()
	void clearProfile();
	QJsonObject toJson();
	QStringList getManaRestorationNames();
	QStringList getHealthRestorationNames();
	

	//getters setters
	void setName(QString name) { this->profileName = name; }
	QString getName() const { return profileName; }
	void setProfession(Profession profession) { this->profession = profession; }
	Profession getProfession() const { return profession; }
	QList<RestorationMethode> getRestMethodesHealth() const { return healthRestorations; };
	QList<RestorationMethode> getRestMethodesMana() const { return manaRestorations; };
	void setRestMethodesHealth(QList<RestorationMethode> healthRestMethodes) { this->healthRestorations = healthRestMethodes; }
	void setRestMethodesMana(QList<RestorationMethode> manaRestMethodes) { this->manaRestorations = manaRestMethodes; }
	Key getScreenShotKey() const { return screenShotKey; }
	void setScreenShotKey(Key key) { this->screenShotKey = key; }
private:	
	QList<RestorationMethode> manaRestorations;
	QList<RestorationMethode> healthRestorations;
	Profession profession;
	QString profileName;
	AutoLootKey lootKey;
	Controls controls;
	Key screenShotKey;
};
