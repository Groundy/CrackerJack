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
		this->profession = Profession(obj.value("profession").toString());

		if (!obj.contains("healthRestorations"))
			throw std::exception("No healthRestorations field in profil json file!");
		auto healRestorationArr = obj.value("healthRestorations").toArray();
		for each (auto var in healRestorationArr){
			RestorationMethode resStr(var.toObject());
			this->healthRestorations.push_back(resStr);
		}

		if (!obj.contains("manaRestorations"))
			throw std::exception("No manaRestorations field in profil json file!");
		auto manaRestorationArr = obj.value("manaRestorations").toArray();
		for each (auto var in manaRestorationArr) {
			RestorationMethode resStr(var.toObject());
			this->manaRestorations.push_back(resStr);
		}

		if (!obj.contains("lootKey"))
			throw std::exception("No lootKey field in profil json file!");
		this->lootKey = AUTO_LOOT_KEY(obj.value("lootkey").toInt());
		
		if (!obj.contains("controls"))
			throw std::exception("No controls field in profil json file!");
		this->controls = CONTROLS(obj.value("controls").toInt());

		if (!obj.contains("screenShotKey"))
			throw std::exception("No screenShotKey field in profil json file!");
		Key key = Key(obj.value("screenShotKey").toObject());
		if (!key.isValid())
			throw std::exception("wrong screenShotKey field value!");
		this->screenShotKey = key;
	}
	catch (const std::exception& e){
		qDebug() << e.what() << endl;
		Utilities::showMessageBox_INFO(e.what());
	}
}

QJsonObject Profile::toJson(){
	QJsonObject mainObj;
	QJsonArray healthArray, manaArray;
	for each (auto var in healthRestorations)
		healthArray.push_back(var.toJsonObj());
	for each (auto var in manaRestorations)
		manaArray.push_back(var.toJsonObj());
	
	mainObj.insert("profileName", profileName);
	mainObj.insert("profession", profession.getTypeName());
	mainObj.insert("healthRestorations", healthArray);
	mainObj.insert("manaRestorations", manaArray);
	mainObj.insert("screenShotKey", screenShotKey.toJson());
	mainObj.insert("lootKey", lootKey);
	mainObj.insert("controls", controls);
	return mainObj;
}

void Profile::clearProfile(){
	healthRestorations.clear();
	manaRestorations.clear();
	profession = Profession();
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
