#pragma once
#include <qstring.h>

#include "Key.h"
#include "JsonClass.h"
class RestorationObject : JsonClass {
public:
	enum class Type { POTION, SPELL };
	RestorationObject(QJsonObject obj) {
		try
		{
			int manaNeededTmp = obj["manaNeeded"].toInt();
			int cdTmp = obj["cd"].toInt();
			int cdGroupTmp = obj["cdGroup"].toInt();
			QString typeStr = obj["type"].toString();
			Type typeTmp;
			if (typeStr == "spell")
				typeTmp = Type::SPELL;
			else if(typeStr == "potion")
				typeTmp = Type::POTION;
			else
				throw std::exception("wrong value of type in Restoration Object");

			this->manaNeeded = manaNeededTmp;
			this->cd = cdTmp;
			this->cdGroup = cdGroupTmp;
			this->type = typeTmp;
		}
		catch (const std::exception& e){
			qDebug() << e.what();
		}
	};
	RestorationObject() : manaNeeded(0), cd(0), cdGroup(0){};
	RestorationObject(int manaNeeded, int cd, int cdGroup, Type type)
		: manaNeeded(manaNeeded), cd(cd), cdGroup(cdGroup), type(type) {};


	QJsonObject toJson() const {
		QJsonObject obj;
		obj.insert( "manaNeeded", manaNeeded);
		obj.insert( "cd", cd);
		obj.insert( "cdGroup", cdGroup);
		QString typeStr;
		if (type == Type::POTION)
			typeStr == "potion";
		else if (type == Type::SPELL)
			typeStr = "spell";
		else
			typeStr = "";

		obj.insert("type", typeStr);
		return obj;
	};
	bool isValid() const { return cd > 0; };
	int getCd() const { return cd; };
	int getCdGroup() const { return cdGroup; };
	Type getType() const { return type; };

	RestorationObject& operator= (const RestorationObject& restObject) {
		this->cd = restObject.cd;
		this->cdGroup = restObject.cdGroup;
		this->manaNeeded = restObject.manaNeeded;
		this->type = restObject.type;
		return *this;
	}
private:
	int manaNeeded, cd, cdGroup;
	Type type;
};

class RestorationMethode : JsonClass
{
public:
	RestorationMethode(int threshold, Key key, QString restorationName) : 
		threshold(threshold), key(key), name(restorationName){
	}
	RestorationMethode() : threshold(0), key(Key()), name("") {};
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

		if (!jsonObj.contains("restorationObj"))
			throw std::exception("There is no restorationObj field in RestorationMethode in json prof file");
		auto  t = jsonObj["restorationObj"].toObject().size();
		RestorationObject entityTmp(jsonObj["restorationObj"].toObject());
		
		key = keyToSet;
		name = methodeNameToSet;
		threshold = thresholdToSet;
		restorationObj = entityTmp;
	}

	QJsonObject toJson() const { 
		QJsonObject mainObj;
		mainObj.insert("threshold", threshold);
		mainObj.insert("methodeName", name);
		mainObj.insert("key", key.toJson());
		mainObj.insert("restorationObj", restorationObj.toJson());
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
	void setRestorationObject(const RestorationObject& restObject) {
		this->restorationObj = restObject;
	}
	bool restObjSet() const {
		return restorationObj.isValid();
	}
	bool isValid() const {
		return
			threshold > 0 &&
			threshold <= 100 &&
			!name.isEmpty() &&
			key.isValid() &&
			restorationObj.isValid();
	}
	int getCd() const { return restorationObj.getCd(); };
	int getCdGroup() const { return restorationObj.getCdGroup(); };
	bool isPotion() const {return restorationObj.getType() == Resto };
private:
	int threshold;
	Key key;
	QString name;
	RestorationObject restorationObj;
};
