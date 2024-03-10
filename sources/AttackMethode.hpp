#pragma once
#include "Item.h"
#include "JsonClass.hpp"
#include "Key.h"
#include "Spell.h"
class AttackMethode : JsonClass {
 public:
  AttackMethode(int cd, int cdGroup, int requiredMana, QString name, Key key, int monsersToUse)
      : cd_(cd), cdGroup_(cdGroup), requiredMana_(requiredMana), name_(name), key_(key), minMonstersToUse_(monsersToUse){};
  AttackMethode(const Spell& spell, Key key, int monsersToUse) {
    cd_               = spell.getCd();
    cdGroup_          = spell.getCdGroup();
    requiredMana_     = spell.getManaNeeded();
    name_             = spell.getIncantation();
    key_              = key;
    minMonstersToUse_ = monsersToUse;
  };
  AttackMethode(const QString runeName, Key key, int monsersToUse) {
    cd_               = 2;
    cdGroup_          = 2;
    requiredMana_     = 0;
    name_             = runeName;
    minMonstersToUse_ = monsersToUse;
    key_              = key;
  };
  AttackMethode(const QJsonObject obj) {
    cd_               = obj["cd"].toInt();
    cdGroup_          = obj["cdGroup"].toInt();
    requiredMana_     = obj["requiredMana"].toInt();
    name_             = obj["name"].toString();
    minMonstersToUse_ = obj["minMonstersToUse"].toInt();
    key_              = Key(obj["key"].toObject());
  };
  bool isValid() const {
    return cd_ >= 1 && cdGroup_ >= 1 && requiredMana_ >= 0 && name_.size() > 5 && key_.isValid() && minMonstersToUse_ >= 1;
  }
  QJsonObject toJson() const {
    QJsonObject obj;
    obj.insert("cd", cd_);
    obj.insert("cdGroup", cdGroup_);
    obj.insert("requiredMana", requiredMana_);
    obj.insert("name", name_);
    obj.insert("key", key_.toJson());
    obj.insert("minMonstersToUse", minMonstersToUse_);
    return obj;
  }
  ~AttackMethode() = default;
  QString getName() const {
    return name_;
  }
  Key getKey() const {
    return key_;
  }
  int getCd() const {
    return cd_;
  }
  int getCdGroup() const {
    return cdGroup_;
  }
  int getMinMonsters() const {
    return minMonstersToUse_;
  }

 private:
  int     cd_ = 0, cdGroup_ = 0, requiredMana_ = 0;
  QString name_;
  Key     key_;
  int     minMonstersToUse_ = 0;
};
