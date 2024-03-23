#pragma once
#include <qjsonvalue.h>
#include <qobject.h>

#include "Profession.hpp"
namespace CJ {
class Potion {
 public:
  enum class TypeOfPotion { HEALTH, MANA };
  Potion(const QJsonObject& obj) {
    QString field = "name";
    if (!obj.contains(field) || !obj[field].isString() || obj[field].toString().isEmpty()) {
      qWarning() << "Error in parsing potion obj, invalid field:" << field;
      return;
    }
    name_ = obj["name"].toString();

    field = "health";
    if (!obj.contains(field) || !obj[field].isDouble()) {
      qWarning() << "Error in parsing potion obj, invalid field:" << field;
      return;
    }
    health_reg_ = obj[field].toInt();

    field = "mana";
    if (!obj.contains(field) || !obj[field].isDouble()) {
      qWarning() << "Error in parsing potion obj, invalid field:" << field;
      return;
    }
    mana_reg_ = obj[field].toInt();

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

    user_professions_ = profsToSet;
  };

  bool isForProf(const Profession& prof) const {
    return user_professions_.contains(prof);
  }
  bool isType(const TypeOfPotion& type) const {
    if (type == TypeOfPotion::HEALTH) {
      return health_reg_ > 0;
    } else if (type == TypeOfPotion::MANA) {
      return mana_reg_ > 0;
    }
    qDebug() << "Error Potion::isType";
    return false;
  }
  bool isHealing() const {
    return health_reg_ > 0;
  };
  bool isMana() const {
    return mana_reg_ > 0;
  };
  QString getName() const {
    return name_;
  };

 private:
  uint                mana_reg_   = 0;
  uint                health_reg_ = 0;
  QVector<Profession> user_professions_;
  QString             name_;
};
}  // namespace CJ
