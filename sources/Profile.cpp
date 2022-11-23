#include "Profile.h"

Profile::Profile(){
}
Profile::Profile(QJsonObject obj) {
	try{
		QJsonValue value;

		value = obj["profileName"];
		if(value.isUndefined() || !value.isString())
			throw std::exception("Error in reading profile from json file, problem with field profileName");
		this->profileName = value.toString();

		value = obj["profession"];
		if (value.isUndefined() || !value.isString())
			throw std::exception("Error in reading profile from json file, problem with field profession");
		this->profession = Profession(value.toString());

		value = obj.contains("healthRestorations");
		if(value.isUndefined() || !value.isArray())
			throw std::exception("Error in reading profile from json file, problem with field healthRestorations");
		auto healRestorationArr = value.toArray();
		for each (auto var in healRestorationArr){
			RestorationMethode resStr(var.toObject());
			this->healthRestorations.push_back(resStr);
		}

		value = obj.contains("manaRestorations");
		if (value.isUndefined() || !value.isArray())
			throw std::exception("Error in reading profile from json file, problem with field manaRestorations");
		auto manaRestorationArr = value.toArray();
		for each (auto var in manaRestorationArr) {
			RestorationMethode resStr(var.toObject());
			this->manaRestorations.push_back(resStr);
		}

		value = obj["lootKey"];
		if (value.isUndefined() || !value.isDouble())
			throw std::exception("Error in reading profile from json file, problem with field lootKey");
		this->autoLoot = AutoLoot(value.toInt());

		value = obj["controls"];
		if (value.isUndefined() || !value.isDouble())
			throw std::exception("Error in reading profile from json file, problem with field controls");
		this->controls = Controls(value.toInt());

		value = obj["screenShotKey"];
		if (value.isUndefined() || !value.isObject())
			throw std::exception("Error in reading profile from json file, problem with field screenShotKey");
		this->screenShotKey = Key(value.toObject());

		value = obj["leftBars"];
		if (value.isUndefined() || !value.isDouble())
			throw std::exception("Error in reading profile from json file, problem with field leftBars");
		this->barsLeft = value.toInt();

		value = obj["rightBars"];
		if (value.isUndefined() || !value.isDouble())
			throw std::exception("Error in reading profile from json file, problem with field rightBars");
		this->barsRight = value.toInt();
	}
	catch (const std::exception& e){
		qDebug() << e.what() << endl;
		Utilities::showMessageBox_INFO(e.what());
	}
}
Profile::Profile(const Profile& profile) {
	this->healthRestorations = profile.healthRestorations;
	this->manaRestorations = profile.manaRestorations;
	this->profession = profile.profession;
	this->autoLoot = profile.autoLoot;
	this->controls = profile.controls;
	this->profileName = profile.profileName;
	this->screenShotKey = profile.screenShotKey;
	this->barsLeft = profile.barsLeft;
	this->barsRight = profile.barsRight;
}

QJsonObject Profile::toJson() const{
	QJsonObject mainObj;
	QJsonArray healthArray, manaArray;
	for each (auto var in healthRestorations)
		healthArray.push_back(var.toJson());
	for each (auto var in manaRestorations)
		manaArray.push_back(var.toJson());
	
	mainObj.insert("profileName", profileName);
	mainObj.insert("profession", profession.getTypeName());
	mainObj.insert("healthRestorations", healthArray);
	mainObj.insert("manaRestorations", manaArray);
	mainObj.insert("screenShotKey", screenShotKey.toJson());
	mainObj.insert("lootKey", autoLoot);
	mainObj.insert("controls", controls);
	mainObj.insert("leftBars", barsLeft);
	mainObj.insert("rightBars", barsRight);
	return mainObj;
}

void Profile::clearProfile(){
	healthRestorations.clear();
	manaRestorations.clear();
	profession = Profession();
	profileName.clear();
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