#pragma once
#include <qstring.h>

#include "Key.h"

class RestoreActionEntity {
public:
	enum class EnitityType { POTION, SPELL };
	QJsonObject toJsonObj();
	RestoreActionEntity(QJsonObject obj) {};
	RestoreActionEntity() {};
	RestoreActionEntity(int manaNeeded, int cd, int cdGroup, EnitityType type)
		: manaNeeded(manaNeeded), cd(cd), cdGroup(cdGroup), type(type) {};
private:
	int manaNeeded, cd, cdGroup;
	EnitityType type;
};

class RestorationMethode
{
public:
	RestorationMethode(int threshold, Key key, QString restorationName) : 
		threshold(threshold), key(key), name(restorationName){
	}
	RestorationMethode(QJsonObject jsonObj) {
		if (!jsonObj.contains("threshold"))
			throw std::exception("There is no threshold field in RestorationMethode in json prof file");
		QJsonValue val = jsonObj.value("threshold");
		if (val.isNull() || !val.isDouble())
			throw std::exception("Invalid RestorationMethode field value!");
		int thresholdToSet = val.toInt();

		if (!jsonObj.contains("methodeName"))
			throw std::exception("There is no methodeName field in RestorationMethode in json prof file");
		val = jsonObj.value("methodeName");
		if (val.isNull() || !val.isString())
			throw std::exception("Invalid RestorationMethode field value!");
		QString methodeNameToSet = val.toString();

		if (!jsonObj.contains("key"))
			throw std::exception("There is no key field in RestorationMethode in json prof file");
		val = jsonObj.value("key");
		if (val.isNull() || !val.isObject())
			throw std::exception("Invalid RestorationMethode field value!");
		Key keyToSet = Key(val.toObject());
		/*
		if (!jsonObj.contains("actionEntity"))
			throw std::exception("There is no actionEntity field in RestorationMethode in json prof file");
		RestoreActionEntity entityTmp(val["actionEntity"].toObject());
		*/
		key = keyToSet;
		name = methodeNameToSet;
		threshold = thresholdToSet;
		//actionEntity = std::move(entityTmp);
	}
	QJsonObject toJsonObj() const { 
		QJsonObject mainObj;
		mainObj.insert("threshold", threshold);
		mainObj.insert("methodeName", name);
		mainObj.insert("key", key.toJson());
		//mainObj.insert("actionEntity", actionEntity.toJsonObj());
		return mainObj;
	};
	QString getName() const  {
		return name;
	};
	int getThreshold() const {
		return threshold;
	};
	QString getKeyName() const {
		return key.getKeyName();
	}
private:
	int threshold;
	Key key;
	QString name;
	RestoreActionEntity actionEntity;
};
