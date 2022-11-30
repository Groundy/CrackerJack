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
#include "JsonClass.h"

class Profile : JsonClass{
public:
	enum AutoLoot { RIGHT_MOUSE_BUTTON, SHIFT_RIGHT, LEFT_MOUSE_BUTTON };//enable only if controls
	enum Controls { CLSSIC_CONTROLS, REGULARCONTROLS, LEFT_SMART_CLICK };

	Profile();
	Profile(QJsonObject obj);
	Profile(const Profile& profile);

	//funcs()
	void clearProfile();
	QJsonObject toJson() const;
	QStringList getManaRestorationNames();
	QStringList getHealthRestorationNames();
	bool isValid() const;

	//setters
	void setName(QString name) { this->profileName = name; }
	void setProfession(Profession profession) { this->profession = profession; }
	void setRestMethodesHealth(QVector<RestorationMethode> healthRestMethodes) { this->healthRestorations = healthRestMethodes; }
	void setRestMethodesMana(QVector<RestorationMethode> manaRestMethodes) { this->manaRestorations = manaRestMethodes; }
	void setScreenShotKey(Key key) { this->screenShotKey = key; }
	void setBars(int left, int right) { barsLeft = left; barsRight = right; }
	void setControls(int enumCode) { controls = Controls(enumCode); }
	void setAutoLoot(int enumCode) { autoLoot = AutoLoot(enumCode); }

	//getters 
	QString getName() const { return profileName; }
	Profession getProfession() const { return profession; }
	QVector<RestorationMethode> getRestMethodesHealth() const { return healthRestorations; };
	QVector<RestorationMethode> getRestMethodesMana() const { return manaRestorations; };
	Key getScreenShotKey() const { return screenShotKey; }
	int getControls() const { return controls; }
	int getAutoLoot() const { return autoLoot; }
	int getBarsLeft() const { return barsLeft; }
	int getBarsRight() const { return barsRight; }
private:	
	QVector<RestorationMethode> manaRestorations;
	QVector<RestorationMethode> healthRestorations;
	Profession profession;
	QString profileName;
	AutoLoot autoLoot;
	Controls controls;
	Key screenShotKey;
	int barsLeft, barsRight;
};
