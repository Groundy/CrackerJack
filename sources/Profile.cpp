#include "Profile.h"

Profile::Profile(){
}
Profile::Profile(QJsonObject obj) {
	try{
		if (!obj.contains("profileName"))
			throw std::exception("No profileName field in profil json file!");
		this->profileName = obj.value("profileName").toString();

		if (!obj.contains("profession"))
			throw std::exception("No profileName field in profil json file!");
		this->profession = PROFESSION(obj.value("profession").toInt());

		if (!obj.contains("healthRestorations"))
			throw std::exception("No healthRestorations field in profil json file!");
		auto healRestorationArr = obj.value("healthRestorations").toArray();
		for each (auto var in healRestorationArr){
			RestorationStruct resStr(var.toObject());
			this->healthRestorations.push_back(resStr);
		}

		if (!obj.contains("manaRestorations"))
			throw std::exception("No manaRestorations field in profil json file!");
		auto manaRestorationArr = obj.value("manaRestorations").toArray();
		for each (auto var in manaRestorationArr) {
			RestorationStruct resStr(var.toObject());
			this->manaRestorations.push_back(resStr);
		}

		if (!obj.contains("lootKey"))
			throw std::exception("No lootKey field in profil json file!");
		this->lootKey = AUTO_LOOT_KEY(obj.value("lootkey").toInt());
		
		if (!obj.contains("controls"))
			throw std::exception("No controls field in profil json file!");
		this->controls = CONTROLS(obj.value("controls").toInt());
	}
	catch (const std::exception& e){
		qDebug() << e.what() << endl;
	}
}

void Profile::toJson(){
	QJsonObject mainObj;
	QString professionName;
	switch (profession) {
	case Profile::RP:
		professionName = "RP";
		break;
	case Profile::EK:
		professionName = "EK";
		break;
	case Profile::ED:
		professionName = "ED";
		break;
	case Profile::MS:
		professionName = "MS";
		break;
	default:
		professionName = "RP";
		break;
	};

	QJsonArray healthArray, manaArray;
	for each (auto var in healthRestorations)
		healthArray.push_back(var.toJsonObj());
	for each (auto var in manaRestorations)
		manaArray.push_back(var.toJsonObj());
	
	mainObj.insert("profileName", profileName);
	mainObj.insert("profession", professionName);
	mainObj.insert("healthRestorations", healthArray);
	mainObj.insert("manaRestorations", healthArray);
	mainObj.insert("lootKey", lootKey);
	mainObj.insert("controls", controls);
}

void Profile::clearProfile(){
	healthRestorations.clear();
	manaRestorations.clear();
	profession = PROFESSION(0);//done
	profileName.clear();
}

void Profile::copyFrom(Profile& profileToCopyFrom){
	this->healthRestorations = profileToCopyFrom.healthRestorations;
	this->manaRestorations = profileToCopyFrom.manaRestorations;
	this->frames = profileToCopyFrom.frames;
	this->profession = profileToCopyFrom.profession;
	this->lootKey = profileToCopyFrom.lootKey;
	this->controls = profileToCopyFrom.controls;
	this->profileName = profileToCopyFrom.profileName;
}

QStringList Profile::getHealthRestorationNames(){
	QStringList toRet;
	for each (auto var in healthRestorations)
		toRet.push_back(var.getName());
	return toRet;
}

QStringList Profile::getManaRestorationNames(){
	QStringList toRet;
	for each (auto var in manaRestorations)
		toRet.push_back(var.getName());
	return toRet;
}
