#pragma once
#include <qobject.h>
#include <qjsonvalue.h>

#include "RestorationMethode.h";
#include "Profession.h"
#include "Utilities.h"
class Potion {
public:
	enum class TypeOfPotion { HEALTH, MANA };
	Potion() {};
	Potion(QJsonObject obj) {
		try {
			if (!obj.contains("name") || !obj["name"].isString() || obj["name"].toString().isEmpty())
				throw std::exception("Error in parsing potion obj, invalid name field!");
			QString nameToSet = obj["name"].toString();

			if (!obj.contains("health") || !obj["health"].isDouble())
				throw std::exception("Error in parsing potion obj, invalid health field!");
			int healthToSet = obj["health"].toInt();

			if (!obj.contains("mana") || !obj["mana"].isDouble())
				throw std::exception("Error in parsing potion obj, invalid mana field!");
			int manaToSet = obj["mana"].toDouble();

			QVector<Profession> profsToSet;

			if (!obj.contains("for_mage") || !obj["for_mage"].isBool())
				throw std::exception("Error in parsing potion obj, invalid for_mage field!");
			if (obj["for_mage"].toBool()) {
				profsToSet.push_back(Profession::Type::MS);
				profsToSet.push_back(Profession::Type::ED);
			}

			if (!obj.contains("for_RP") || !obj["for_RP"].isBool())
				throw std::exception("Error in parsing potion obj, invalid for_RP field!");
			if (obj["for_RP"].toBool())
				profsToSet.push_back(Profession::Type::RP);

			if (!obj.contains("for_EK") || !obj["for_EK"].isBool())
				throw std::exception("Error in parsing potion obj, invalid for_EK field!");
			if (obj["for_EK"].toBool())
				profsToSet.push_back(Profession::Type::EK);

			this->name = nameToSet;
			this->healthReg = healthToSet;
			this->manaReg = manaToSet;
			this->userProfessions = profsToSet;
		}
		catch (const std::exception& e) {
			qDebug() << e.what();
		}
	};
	~Potion() {};
	RestoreActionEntity toRestorationEnity() {
		return RestoreActionEntity(0, 1, 1, RestoreActionEntity::EnitityType::POTION);
	}

	bool isForProf(const Profession& prof) const { return userProfessions.contains(prof); }
	bool isType(const TypeOfPotion& type) const {
		if (type == TypeOfPotion::HEALTH)
			return healthReg > 0;
		else if (type == TypeOfPotion::MANA)
			return manaReg > 0;
		else {
			QString msg = "Error Potion::isType";
			qDebug() << msg;
			Utilities::showMessageBox_INFO(msg);
			return false;
		}
	}
	bool isHealing() const { return healthReg > 0; }
	bool isMana() const { return manaReg > 0; }
	QString getName() const { return name; }

private:
	int manaReg, healthReg;
	QVector<Profession> userProfessions;
	QString name;
};
