#include "Profile.h"
#include "ProfileDataBaseManager.h"
Profile::Profile(QObject *parent) : QObject(parent){

}

Profile::Profile()
{
}

Profile::Profile(bool isTestProf){
	//this->testProf = isTestProf;
	//randomValues [TODO]
	if (isTestProf) {
		/*
		ProfileDataBaseManager db;
		QList<int> randomNumberVector{13, 25, 38 , 66, 96};
		this->healthRestorePercentages = db.DB_reader_ManaAndHealthRestorePercentages(randomNumberVector)
		this->ManaRestoreMethodesPercentage = profileToCopyFrom.ManaRestoreMethodesPercentage;
		this->healthKeys = profileToCopyFrom.healthKeys;
		this->ManaKeys = profileToCopyFrom.ManaKeys;
		this->healthRestoreMethodeNames = profileToCopyFrom.healthRestoreMethodeNames;
		this->manaRestoreMethodeNames = profileToCopyFrom.manaRestoreMethodeNames;
		this->frames = profileToCopyFrom.frames;
		this->profession = PROFESSION::EK;
		this->lootKey = profileToCopyFrom.lootKey;
		this->controls = profileToCopyFrom.controls;
		this->profileName = profileToCopyFrom.profileName;
		this->lastLoginDate = profileToCopyFrom.lastLoginDate;
		this->creationDate = profileToCopyFrom.creationDate;
		this->secondsSpent = profileToCopyFrom.secondsSpent;
		*/
	}
}


Profile::~Profile(){
	if (!this->testProf) {
		ProfileDataBaseManager db;
		db.saveProfileToDataBase(*this);
	}
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
	this->lastLoginDate = profileToCopyFrom.lastLoginDate;
	this->creationDate = profileToCopyFrom.creationDate;
	this->secondsSpent = profileToCopyFrom.secondsSpent;
}
