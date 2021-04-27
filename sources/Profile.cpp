#include "Profile.h"

Profile::Profile(QObject *parent) : QObject(parent)
{
	
}

Profile::Profile()
{
}

Profile::Profile(const Profile &profile){
}

Profile::~Profile()
{
}

void Profile::clearProfile(){
	healthRestorePercentages.clear();
	ManaRestoreMethodesPercentage.clear();
	healthKeys.clear();
	ManaKeys.clear();
	healthItems.clear();
	manaItems.clear();

	PROFESSION profession = PROFESSION(0);//done
	QString profileName = "";//done
}
