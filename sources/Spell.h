#pragma once
#include <qvector.h>

#include "JsonClass.hpp"
#include "Profession.hpp"

class Spell {
 public:
  Spell(){};
  Spell(QJsonObject obj);
  ~Spell(){};
  enum class SpellType { Healing, Support, Attack };
  bool isForProf(const Profession& prof) {
    return profesUsingSpell.contains(prof);
  }
  bool isSpellType(SpellType type) {
    return spellType == type;
  }
  QString getIncantation() const {
    return incantations;
  }

  bool forRP() {
    return profesUsingSpell.contains(Profession::Type::RP);
  }
  bool forED() {
    return profesUsingSpell.contains(Profession::Type::ED);
  }
  bool forEK() {
    return profesUsingSpell.contains(Profession::Type::EK);
  }
  bool forMS() {
    return profesUsingSpell.contains(Profession::Type::MS);
  }
  int getManaNeeded() const {
    return mana;
  };
  int getCd() const {
    return cd;
  };
  int getCdGroup() const {
    return cdGroup;
  };

 private:
  QVector<Profession> profesUsingSpell;
  SpellType           spellType;
  QString             name, incantations;
  int                 mana = 0, cd = 0, cdGroup = 0, soulPoints = 0;
};

inline Spell::Spell(QJsonObject obj) {
  try {
    QString             nameToSet         = obj["name"].toString();
    QString             incantationsToSet = obj["incantantion"].toString();
    QVector<Profession> profsToSet        = {};
    if (obj["EK"].toBool()) profsToSet.push_back(Profession::Type::EK);
    if (obj["RP"].toBool()) profsToSet.push_back(Profession::Type::RP);
    if (obj["MS"].toBool()) profsToSet.push_back(Profession::Type::MS);
    if (obj["ED"].toBool()) profsToSet.push_back(Profession::Type::ED);
    int manaToSet       = obj["mana"].toInt();
    int cdToSet         = obj["CD"].toInt();
    int cdGroupToSet    = obj["CDGROUP"].toInt();
    int soulPointsToSet = obj["SoulPoints"].toInt();

    QString          typeAsStr = obj["TYPE"].toString();
    Spell::SpellType typeToSet = SpellType::Healing;
    if (typeAsStr == "HEALING")
      typeToSet = SpellType::Healing;
    else if (typeAsStr == "SUPPORT")
      typeToSet = SpellType::Support;
    else if (typeAsStr == "ATTACK")
      typeToSet = SpellType::Attack;

    this->name             = nameToSet;
    this->incantations     = incantationsToSet;
    this->profesUsingSpell = profsToSet;
    this->mana             = manaToSet;
    this->cd               = cdToSet;
    this->cdGroup          = cdGroupToSet;
    this->soulPoints       = soulPointsToSet;
    this->spellType        = typeToSet;
  } catch (const std::exception& e) {
    qDebug() << e.what();
    // Utilities::showMessageBox_INFO(e.what());
  }
};
