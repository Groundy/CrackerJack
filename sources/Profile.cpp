#include "Profile.h"

Profile::Profile(){
}
Profile::Profile(QJsonObject obj) {
	try {

		QString field = "profileName";
		QJsonValue value = obj[field];
		if (value.isUndefined() || !value.isString())
			throw std::exception(field.toStdString().c_str());
		this->profileName = value.toString();

		field = "profession";
		value = obj[field];
		if (value.isUndefined() || !value.isString())
			throw std::exception(field.toStdString().c_str());
		this->profession = Profession(value.toString());

		field = "healthRestorations";
		value = obj[field];
		if(value.isUndefined() || !value.isArray())
			throw std::exception(field.toStdString().c_str());
		auto healRestorationArr = value.toArray();
		for each (auto var in healRestorationArr){
			RestorationMethode resStr(var.toObject());
			this->healthRestorations.push_back(resStr);
		}

		field = "manaRestorations";
		value = obj[field];
		if (value.isUndefined() || !value.isArray())
			throw std::exception(field.toStdString().c_str());
		auto manaRestorationArr = value.toArray();
		for each (auto var in manaRestorationArr) {
			RestorationMethode resStr(var.toObject());
			this->manaRestorations.push_back(resStr);
		}

		field = "lootKey";
		value = obj[field];
		if (value.isUndefined() || !value.isDouble())
			throw std::exception(field.toStdString().c_str());
		this->autoLoot = AutoLoot(value.toInt());

		field = "controls";
		value = obj[field];
		if (value.isUndefined() || !value.isDouble())
			throw std::exception(field.toStdString().c_str());
		this->controls = Controls(value.toInt());

		field = "screenShotKey";
		value = obj["screenShotKey"];
		if (value.isUndefined() || !value.isObject())
			throw std::exception(field.toStdString().c_str());
		this->screenShotKey = Key(value.toObject());

		field = "leftBars";
		value = obj[field];
		if (value.isUndefined() || !value.isDouble())
			throw std::exception(field.toStdString().c_str());
		this->barsLeft = value.toInt();
	
		field = "rightBars";
		value = obj[field];
		if (value.isUndefined() || !value.isDouble())
			throw std::exception(field.toStdString().c_str());
		this->barsRight = value.toInt();

		field = "shovelKey";
		value = obj[field];
		if (value.isUndefined() || !value.isObject())
			throw std::exception(field.toStdString().c_str());
		this->shovelKey = Key(value.toObject());

		field = "autoAttackKey";
		value = obj[field];
		if (value.isUndefined() || !value.isObject())
			throw std::exception(field.toStdString().c_str());
		this->autoAttackKey = Key(value.toObject());

		field = "ropeKey";
		value = obj[field];
		if (value.isUndefined() || !value.isObject())
			throw std::exception(field.toStdString().c_str());
		this->ropeKey = Key(value.toObject());

		field = "autoHunt";
		value = obj[field];
		if (value.isUndefined() || !value.isObject())
			throw std::exception(field.toStdString().c_str());
		this->autoHuntData = AutoHuntData(value.toObject());
	}
	catch (const std::exception& e){
		QString msg = QString("Error in reading profile from json file, problem with field %1").arg(e.what());
		qDebug() << msg << endl;
		Utilities::showMessageBox_INFO(msg);
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
	QJsonArray healthArray, manaArray;
	for each (auto var in healthRestorations)
		healthArray.push_back(var.toJson());
	for each (auto var in manaRestorations)
		manaArray.push_back(var.toJson());

	QJsonObject mainObj;
	mainObj.insert("profileName", profileName);
	mainObj.insert("profession", profession.getTypeName());
	mainObj.insert("healthRestorations", healthArray);
	mainObj.insert("manaRestorations", manaArray);
	mainObj.insert("screenShotKey", screenShotKey.toJson());
	mainObj.insert("lootKey", autoLoot);
	mainObj.insert("controls", controls);
	mainObj.insert("leftBars", barsLeft);
	mainObj.insert("rightBars", barsRight);

	mainObj.insert("shovelKey", shovelKey.toJson());
	mainObj.insert("ropeKey", ropeKey.toJson());
	mainObj.insert("autoAttackKey", autoAttackKey.toJson());
	mainObj.insert("autoHunt", autoHuntData.toJson());
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
bool Profile::isValid() const {
	return
		!profileName.isEmpty() &&
		screenShotKey.isValid();
}