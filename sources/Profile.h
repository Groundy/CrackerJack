#pragma once
#include <qmap>
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
	AutoHuntData() = default;
	~AutoHuntData() = default;
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

	Profile() = default;
	Profile(QJsonObject obj);
	Profile(const Profile& profile);

	//funcs()
	void clearProfile();
	QJsonObject toJson() const;
	QStringList getManaRestorationNames();
	QStringList getHealthRestorationNames();
	bool isValid() const;

	//setters
	void setName(QString name);
	void setProfession(Profession profession) ;
	void setRestMethodesHealth(QVector<RestorationMethode> healthRestMethodes) ;
	void setRestMethodesMana(QVector<RestorationMethode> manaRestMethodes) ;
	void setScreenShotKey(Key key) ;
	void setBars(int left, int right) ;
	void setControls(int enumCode) ;
	void setAutoLoot(int enumCode) ;
	void setRopeKey(Key key) ;
	void setAutoAttackKey(Key key) ;
	void setShovelKey(Key key) ;
	void setAutoHuntData(AutoHuntData autoHuntData);

	//getters 
	QString getName() const;
	Profession getProfession() const;
	QVector<RestorationMethode> getRestMethodesHealth() const ;
	QVector<RestorationMethode> getRestMethodesMana() const;
	Key getScreenShotKey() const ;
	int getControls() const ;
	AutoLoot getAutoLoot() const;
	int getBarsLeft() const;
	int getBarsRight() const;
	Key getRopeKey() const ;
	Key getAutoAttackKey() const ;
	Key getShovelKey() const ;
	AutoHuntData getAutoHuntData() const;
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
