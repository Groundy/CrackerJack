#include "Profile.h"
#include "ProfileDataBaseManager.h"
Profile::Profile(QObject *parent) : QObject(parent){

}

Profile::Profile(){
	qDebug() << "New profile created";
}

Profile::~Profile(){
	qDebug() << "Profile: " + profileName + " freed from memory";
}

void Profile::clearProfile(){
	healthRestorePercentages.clear();
	ManaRestoreMethodesPercentage.clear();
	healthKeys.clear();
	ManaKeys.clear();
	healthRestoreMethodeNames.clear();
	manaRestoreMethodeNames.clear();

	PROFESSION profession = PROFESSION(0);//done
	QString profileName = "";//done
}

void Profile::getDataFromOtherProf(Profile& profileToCopyFrom){
	this->healthRestorePercentages = profileToCopyFrom.healthRestorePercentages;
	this->ManaRestoreMethodesPercentage = profileToCopyFrom.ManaRestoreMethodesPercentage;
	this->healthKeys = profileToCopyFrom.healthKeys;
	this->ManaKeys = profileToCopyFrom.ManaKeys;
	this->healthRestoreMethodeNames = profileToCopyFrom.healthRestoreMethodeNames;
	this->manaRestoreMethodeNames = profileToCopyFrom.manaRestoreMethodeNames;
	this->frames = profileToCopyFrom.frames;
	this->profession = profileToCopyFrom.profession;
	this->lootKey = profileToCopyFrom.lootKey;
	this->controls = profileToCopyFrom.controls;
	this->profileName = profileToCopyFrom.profileName;
	this->lastLoginSeconds = profileToCopyFrom.lastLoginSeconds;
	this->creationTimeSeconds = profileToCopyFrom.creationTimeSeconds;
	this->secondsSpentSeconds = profileToCopyFrom.secondsSpentSeconds;
}
