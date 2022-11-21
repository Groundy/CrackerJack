#pragma once
#include <qstring.h>

#include "Key.h"

class RestorationMethode
{
public:
	RestorationMethode(int threshold, Key key, QString restorationName) : threshold(threshold), key(key), name(restorationName){
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
		
		key = keyToSet;
		name = methodeNameToSet;
		threshold = thresholdToSet;
	}

	QJsonObject toJsonObj() { 
		QJsonObject mainObj;
		mainObj.insert("threshold", threshold);
		mainObj.insert("methodeName", name);
		mainObj.insert("key", key.toJson());
		return mainObj;
	};
	QString getName() {
		return name;
	};
	int getThreshold() {
		return threshold;
	};
	QString getKeyName() {
		return key.getKeyName();
	}
private:
	int threshold;
	Key key;
	QString name;
};
