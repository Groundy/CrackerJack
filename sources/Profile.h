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
#include "AttackMethode.hpp"
#include "Profession.hpp"
#include "RestorationMethode.hpp"
#include "Utilities.hpp"
#include "JsonClass.hpp"

class AutoHuntData : JsonClass {
public:
	AutoHuntData() {};
	~AutoHuntData() {};
	bool isValid() const { return true; }
	QJsonObject toJson() const {
		QJsonObject obj;
		obj.insert("minEnemiesToStop", minEnemiesToStop);
		obj.insert("minEnemiesToContinue", minEnemiesToContinue);
		QJsonArray attackMethodesArr;
		for each (auto methode in attackMethodes){
			attackMethodesArr.append(methode.toJson());
		}
		obj.insert("attackMethodes", attackMethodesArr);
		return obj;
	} 
	AutoHuntData(QJsonObject obj){
		minEnemiesToContinue = obj["minEnemiesToContinue"].toInt();
		minEnemiesToStop = obj["minEnemiesToStop"].toInt();
		QJsonArray attackMethodesArr = obj["attackMethodes"].toArray();
		for each (auto methode in attackMethodesArr) 
			this->attackMethodes.append(AttackMethode(methode.toObject()));
	}
	int getMinMonToContinue() const { return minEnemiesToContinue; }
	int getMinMonToStop() const { return minEnemiesToStop; }
	QVector<AttackMethode> getAttacks() const { return attackMethodes; }
	void setMinMonToContinue(int toSet) { this->minEnemiesToContinue = toSet; }
	void setMinMonToStop(int toSet) { this->minEnemiesToStop = toSet; }
	void setAttacks(QVector<AttackMethode> toSet) { this->attackMethodes = toSet; }
	AutoHuntData& operator = (const AutoHuntData& data) {
		this->setMinMonToContinue(data.getMinMonToContinue());
		this->setMinMonToStop(data.getMinMonToStop());
		this->setAttacks(data.getAttacks());
		return *this;
	}
private:
	int minEnemiesToStop = 0, minEnemiesToContinue = 0;
	QVector<AttackMethode> attackMethodes = {};
};

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
	void setRopeKey(Key key) { this->ropeKey = key; }
	void setAutoAttackKey(Key key) { this->autoAttackKey = key; }
	void setShovelKey(Key key) { this->shovelKey = key; }
	void setAutoHuntData(AutoHuntData autoHuntData) { this->autoHuntData = autoHuntData; }

	//getters 
	QString getName() const { return profileName; }
	Profession getProfession() const { return profession; }
	QVector<RestorationMethode> getRestMethodesHealth() const { return healthRestorations; };
	QVector<RestorationMethode> getRestMethodesMana() const { return manaRestorations; };
	Key getScreenShotKey() const { return screenShotKey; }
	int getControls() const { return controls; }
	AutoLoot getAutoLoot() const { return autoLoot; }
	int getBarsLeft() const { return barsLeft; }
	int getBarsRight() const { return barsRight; }
	Key getRopeKey() const { return ropeKey; }
	Key getAutoAttackKey() const { return autoAttackKey; }
	Key getShovelKey() const { return shovelKey; }
	AutoHuntData getAutoHuntData() const { return autoHuntData; }
private:	
	QVector<RestorationMethode> manaRestorations;
	QVector<RestorationMethode> healthRestorations;
	Profession profession;
	QString profileName;
	AutoLoot autoLoot;
	Controls controls;
	Key screenShotKey, shovelKey, ropeKey, autoAttackKey;
	int barsLeft, barsRight;
	AutoHuntData autoHuntData = AutoHuntData();
};
