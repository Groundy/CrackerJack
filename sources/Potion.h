#pragma once
#include <qjsonvalue.h>
#include <qobject.h>

#include "Profession.hpp"
class Potion {
 public:
  enum class TypeOfPotion { HEALTH, MANA };
  Potion(const QJsonObject& obj) {
    QString field = "name";
    if (!obj.contains(field) || !obj[field].isString() || obj[field].toString().isEmpty()) {
      qWarning() << "Error in parsing potion obj, invalid field:" << field;
      return;
    }
    this->name = obj["name"].toString();

    field = "health";
    if (!obj.contains(field) || !obj[field].isDouble()) {
      qWarning() << "Error in parsing potion obj, invalid field:" << field;
      return;
    }
    this->healthReg = obj[field].toInt();

    field = "mana";
    if (!obj.contains(field) || !obj[field].isDouble()) {
      qWarning() << "Error in parsing potion obj, invalid field:" << field;
      return;
    }
    this->manaReg = obj[field].toInt();

    QVector<Profession> profsToSet;
    field = "for_mage";
    if (!obj.contains(field) || !obj[field].isBool()) {
      qWarning() << "Error in parsing potion obj, invalid field:" << field;
      return;
    } else if (obj[field].toBool()) {
      profsToSet.push_back(Profession::Type::MS);
      profsToSet.push_back(Profession::Type::ED);
    }

    field = "for_RP";
    if (!obj.contains(field) || !obj[field].isBool()) {
      qWarning() << "Error in parsing potion obj, invalid field:" << field;
      return;
    } else if (obj[field].toBool()) {
      profsToSet.push_back(Profession::Type::RP);
    }

    field = "for_EK";
    if (!obj.contains(field) || !obj[field].isBool()) {
      qWarning() << "Error in parsing potion obj, invalid field:" << field;
      return;
    } else if (obj[field].toBool()) {
      profsToSet.push_back(Profession::Type::EK);
    }

    this->userProfessions = profsToSet;
  };

  bool isForProf(const Profession& prof) const {
    return userProfessions.contains(prof);
  }
  bool isType(const TypeOfPotion& type) const {
    if (type == TypeOfPotion::HEALTH) {
      return healthReg > 0;
    } else if (type == TypeOfPotion::MANA) {
      return manaReg > 0;
    }
    qDebug() << "Error Potion::isType";
    return false;
  }
  bool isHealing() const {
    return healthReg > 0;
  };
  bool isMana() const {
    return manaReg > 0;
  };
  QString getName() const {
    return name;
  };

 private:
  int                 manaReg, healthReg;
  QVector<Profession> userProfessions;
  QString             name;
};
