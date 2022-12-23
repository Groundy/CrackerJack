#pragma once
#include "Spell.h"
#include "Item.h"
#include "Key.h"
#include "JsonClass.hpp"
class AttackMethode : JsonClass
{
public:
	AttackMethode(int cd, int cdGroup, int requiredMana, QString name, Key key, int monsersToUse)
	: cd(cd), cdGroup(cdGroup), requiredMana(requiredMana), name(name), key(key), minMonstersToUse(monsersToUse){
	}
	AttackMethode(const Spell& spell, Key key, int monsersToUse) {
		this->cd = spell.getCd();
		this->cdGroup = spell.getCdGroup();
		this->requiredMana = spell.getManaNeeded();
		this->name = spell.getIncantation();
		this->key = key;
		this->minMonstersToUse = monsersToUse;
	};
	AttackMethode(const QString runeName, Key key, int monsersToUse) {
		this->cd = 2;
		this->cdGroup = 2;
		this->requiredMana = 0;
		this->name = runeName;
		this->minMonstersToUse = monsersToUse;
		this->key = key;
	};
	AttackMethode(const QJsonObject obj) {
		this->cd = obj["cd"].toInt();
		this->cdGroup = obj["cdGroup"].toInt();
		this->requiredMana = obj["requiredMana"].toInt();
		this->name = obj["name"].toString();
		this->minMonstersToUse = obj["minMonstersToUse"].toInt();
		this->key = Key(obj["key"].toObject());
	};
	bool isValid() const {
		return cd >= 1 && cdGroup >= 1 && requiredMana >= 0 && name.size() > 5 && key.isValid() && minMonstersToUse >= 1;
	}
	QJsonObject toJson() const {
		QJsonObject obj;
		obj.insert("cd", cd);
		obj.insert("cdGroup", cdGroup);
		obj.insert("requiredMana", requiredMana);
		obj.insert("name", name);
		obj.insert("key", key.toJson());
		obj.insert("minMonstersToUse", minMonstersToUse);
		return obj;
	}
	~AttackMethode() {};
	QString getName() const { return name; }
	Key getKey() const { return key; }
	int getCd() const { return cd; }
	int getCdGroup() const { return cdGroup; }
	int getMinMonsters() const { return minMonstersToUse; }
private:
	int cd = 0, cdGroup = 0, requiredMana = 0;
	QString name;
	Key key;
	int minMonstersToUse = 0;
};