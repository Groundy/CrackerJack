#pragma once
#include <qstring.h>

#include "JsonClass.hpp"
#include "Key.h"
#include "Potion.h"
#include "Spell.h"

class RestorationMethode : JsonClass {
 public:
  enum class Type { POTION, SPELL };

  RestorationMethode::RestorationMethode(int threshold, Key key, QString restorationName)
      : threshold(threshold), key(key), name(restorationName) {}

  RestorationMethode::RestorationMethode() : threshold(0), key(Key()), name(""){};

  RestorationMethode::RestorationMethode(QJsonObject jsonObj) {
    QString field = "threshold";
    if (jsonObj.contains(field)) {
      QJsonValue value = jsonObj[field];
      if (value.isDouble()) {
        threshold = value.toDouble();
      } else {
        qWarning() << "Wrong type of rest method field " << field;
      }
    } else {
      qWarning() << "There is no" << field << "field in RestorationMethode in json prof file";
    }

    field = "methodeName";
    if (jsonObj.contains(field)) {
      QJsonValue value = jsonObj[field];
      if (value.isString()) {
        name = value.toString();
      } else {
        qWarning() << "Wrong type of rest method field " << field;
      }
    } else {
      qWarning() << "There is no" << field << "field in RestorationMethode in json prof file";
    }

    field = "key";
    if (jsonObj.contains(field)) {
      Key keyToSet(jsonObj[field].toObject());
      if (key.isValid()) {
        key = keyToSet;
      } else {
        qWarning() << "Wrong type of rest method field " << field;
      }
    } else {
      qWarning() << "There is no" << field << "field in RestorationMethode in json prof file";
    }

    field = "manaNeeded";
    if (jsonObj.contains(field)) {
      QJsonValue value = jsonObj[field];
      if (value.isDouble()) {
        manaNeeded = value.toDouble();
      } else {
        qWarning() << "Wrong type of rest method field " << field;
      }
    } else {
      qWarning() << "There is no" << field << "field in RestorationMethode in json prof file";
    }

    field = "cd";
    if (jsonObj.contains(field)) {
      QJsonValue value = jsonObj[field];
      if (value.isDouble()) {
        cd = value.toDouble();
      } else {
        qWarning() << "Wrong type of rest method field " << field;
      }
    } else {
      qWarning() << "There is no" << field << "field in RestorationMethode in json prof file";
    }

    field = "cdGroup";
    if (jsonObj.contains(field)) {
      QJsonValue value = jsonObj[field];
      if (value.isDouble()) {
        cdGroup = value.toDouble();
      } else {
        qWarning() << "Wrong type of rest method field " << field;
      }
    } else {
      qWarning() << "There is no" << field << "field in RestorationMethode in json prof file";
    }

    field = "type";
    if (jsonObj.contains(field)) {
      QJsonValue value = jsonObj[field];
      if (value.isString()) {
        type = (value.toString() == "potion") ? Type::POTION : Type::SPELL;
      } else {
        qWarning() << "Wrong type of rest method field " << field;
      }
    } else {
      qWarning() << "There is no" << field << "field in RestorationMethode in json prof file";
    }
  }

  //getters
  int getCd() const {
    return cd;
  };
  int getCdGroup() const {
    return cdGroup;
  };
  QString getName() const {
    return name;
  };
  int getThreshold() const {
    return threshold;
  };
  QString getKeyName() const {
    return key.getKeyName();
  }
  Key getKey() const {
    return key;
  }
  int getMana() const {
    return manaNeeded;
  }
  Type getType() const {
    return type;
  }

  //setters
  void fillDataDetails(const Spell& spell) {
    cd         = spell.getCd();
    cdGroup    = spell.getCdGroup();
    manaNeeded = spell.getManaNeeded();
    type       = Type::SPELL;
  };
  void fillDataDetails(const Potion& potion) {
    cd         = 1;
    cdGroup    = 1;
    manaNeeded = 1;
    type       = Type::POTION;
  };

  //funcs
  bool isPotion() const {
    return type == Type::POTION;
  };
  bool isSpell() const {
    return type == Type::SPELL;
  };
  bool isType(RestorationMethode::Type typeToCheck) const {
    return type == typeToCheck;
  }
  QJsonObject toJson() const {
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
  bool isValid() const {
    return threshold > 0 && threshold <= 100 && !name.isEmpty() && key.isValid();
  }

 private:
  int     threshold  = 0;
  Key     key        = Key();
  QString name       = QString();
  int     manaNeeded = 0, cd = 0, cdGroup = 0;
  Type    type = Type::POTION;
};
