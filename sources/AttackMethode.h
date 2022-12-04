#pragma once
#include "Spell.h"
#include "Item.h"
#include "Key.h"
#include "JsonClass.h"
class AttackMethode : JsonClass
{
public:
	AttackMethode(int cd, int cdGroup, int requiredMana, QString name, Key key)
	: cd(cd), cdGroup(cdGroup), requiredMana(requiredMana), name(name), key(key){
	}
	AttackMethode(const Spell& spell, Key key) {
		this->cd = spell.getCd();
		this->cdGroup = spell.getCdGroup();
		this->requiredMana = spell.getManaNeeded();
		this->name = spell.getIncantation();
		this->key = key;
	};
	AttackMethode(const Item& rune, Key key) {
		this->cd = 2;
		this->cdGroup = 2;
		this->requiredMana = 0;
		this->name = rune.name;
		this->key = key;
	};
	bool isValid() const {
		return cd >= 1 && cdGroup >= 1 && requiredMana >= 0 && name.size() > 5 && key.isValid();
	}
	QJsonObject toJson() const {
		QJsonObject obj;
		obj.insert("cd", cd);
		obj.insert("cdGroup", cdGroup);
		obj.insert("requiredMana", requiredMana);
		obj.insert("name", name);
		obj.insert("key", key.toJson());
		return obj;
	}
	~AttackMethode() {};
	QString getName() const { return name; }
	Key getKey() const { return key; }
	int getCd() const { return cd; }
	int getCdGroup() const { return cdGroup; }
private:
	int cd = 0, cdGroup = 0, requiredMana = 0;
	QString name;
	Key key;
};