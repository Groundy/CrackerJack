#pragma once
#include <qstring.h>

#include "Key.h"
#include "JsonClass.h"
#include "Spell.h"
#include "Potion.h"

class RestorationMethode : JsonClass
{
public:	
	enum class Type { POTION, SPELL };

	RestorationMethode(int threshold, Key key, QString restorationName);
	RestorationMethode();
	RestorationMethode(QJsonObject jsonObj);
	QJsonObject toJson() const;

	//getters
	int getCd() const { return cd; };
	int getCdGroup() const { return cdGroup; };
	bool isValid() const;
	QString getName() const  { return name; };
	int getThreshold() const { return threshold; };
	QString getKeyName() const { return key.getKeyName(); }
	Key getKey() const { return key; }
	int getMana() const { return manaNeeded; }
	Type getType() const { return type; }

	//setters
	void fillDataDetails(const Spell& spell) {
		cd = spell.getCd();
		cdGroup = spell.getCdGroup();
		manaNeeded = spell.getManaNeeded();
		type = Type::SPELL;
	};
	void fillDataDetails(const Potion& potion) {
		cd = 0;
		cdGroup = 1;
		manaNeeded = 1;
		type = Type::POTION;
	};

	//funcs
	bool isPotion() const { return type == Type::POTION; };
	bool isSpell() const { return type == Type::SPELL; };
	bool isType(RestorationMethode::Type typeToCheck) const { return type == typeToCheck; }
private:
	int threshold;
	Key key;
	QString name;
	int manaNeeded, cd, cdGroup;
	Type type;
};

inline RestorationMethode::RestorationMethode(int threshold, Key key, QString restorationName) :
	threshold(threshold), key(key), name(restorationName) {
}
inline RestorationMethode::RestorationMethode() : threshold(0), key(Key()), name("") {};
inline RestorationMethode::RestorationMethode(QJsonObject jsonObj) {
	try {
		QStringList fields = QStringList() << "threshold" << "methodeName" << "key" << "manaNeeded" << "cd" << "cdGroup" << "type";
		for each (QString field in fields) {
			if (!jsonObj.contains(field)) {
				QString msg = QString("There is no %1 field in RestorationMethode in json prof file").arg(field);
				throw std::exception(msg.toStdString().c_str());
			}
		}

		QJsonValue value = jsonObj["threshold"];
		int thresholdToSet = value.isDouble() ? value.toInt() : throw std::exception("Json Rest methode error with field threshold");

		value = jsonObj["methodeName"];
		QString methodeNameToSet = value.isString() ? value.toString() : throw std::exception("Json Rest methode error with field methodeName");

		value = jsonObj["key"];
		Key keyToSet = value.isObject() ? Key(value.toObject()) : throw std::exception("Json Rest methode error with field key");

		value = jsonObj["manaNeeded"];
		int manaNeededToSet = value.isDouble() ? value.toInt() : throw std::exception("Json Rest methode error with field manaNeeded");

		value = jsonObj["cd"];
		int cdToSet = value.isDouble() ? value.toInt() : throw std::exception("Json Rest methode error with field cd");

		value = jsonObj["cdGroup"];
		int cdGroupToSet = value.isDouble() ? value.toInt() : throw std::exception("Json Rest methode error with field cdGroup");

		value = jsonObj["type"];
		QString typeStrToSet = value.isString() ? value.toString() : throw std::exception("Json Rest methode error with field type");
		Type typeToSet = (typeStrToSet == "potion") ? Type::POTION : Type::SPELL;

		threshold = thresholdToSet;
		name = methodeNameToSet;
		key = keyToSet;
		manaNeeded = manaNeededToSet;
		cd = cdToSet;
		cdGroup = cdGroupToSet;
		type = typeToSet;
	}
	catch (const std::exception& e) {
		qDebug() << e.what();
	}
}
inline QJsonObject RestorationMethode::toJson() const {
	QJsonObject mainObj;
	mainObj.insert("threshold", threshold);
	mainObj.insert("methodeName", name);
	mainObj.insert("key", key.toJson());
	mainObj.insert("manaNeeded", manaNeeded);
	mainObj.insert("cd", cd);
	mainObj.insert("cdGroup", cdGroup);
	QString typeStr = isPotion() ? "potion" : "spell";
	mainObj.insert("type", typeStr);
	return mainObj;
};
inline bool RestorationMethode::isValid() const {
	return
		threshold > 0 &&
		threshold <= 100 &&
		!name.isEmpty() &&
		key.isValid();
}