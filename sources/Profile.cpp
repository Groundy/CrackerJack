#include "Profile.h"
#include "ProfileDataBaseManager.h"
Profile::Profile(QObject *parent) : QObject(parent){

}

Profile::Profile()
{
}

Profile::Profile(const Profile &profile){}

Profile::~Profile(){
	ProfileDataBaseManager db;
	db.saveProfileToDataBase(*this);
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
