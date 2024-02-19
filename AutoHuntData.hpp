#include "AttackMethode.hpp"
#include "JsonClass.hpp"
#include "qjsonarray.h"
class AutoHuntData : JsonClass {
 public:
  AutoHuntData()  = default;
  ~AutoHuntData() = default;
  bool isValid() const {
    return true;
  }
  QJsonObject toJson() const {
    QJsonObject obj;
    obj.insert("minEnemiesToStop", minEnemiesToStop);
    obj.insert("minEnemiesToContinue", minEnemiesToContinue);
    QJsonArray attackMethodesArr;
    for each (auto methode in attackMethodes) {
      attackMethodesArr.append(methode.toJson());
    }
    obj.insert("attackMethodes", attackMethodesArr);
    return obj;
  }
  AutoHuntData(QJsonObject obj) {
    minEnemiesToContinue         = obj["minEnemiesToContinue"].toInt();
    minEnemiesToStop             = obj["minEnemiesToStop"].toInt();
    QJsonArray attackMethodesArr = obj["attackMethodes"].toArray();
    for each (auto methode in attackMethodesArr) this->attackMethodes.append(AttackMethode(methode.toObject()));
  }
  int getMinMonToContinue() const {
    return minEnemiesToContinue;
  }
  int getMinMonToStop() const {
    return minEnemiesToStop;
  }
  QVector<AttackMethode> getAttacks() const {
    return attackMethodes;
  }
  void setMinMonToContinue(int toSet) {
    this->minEnemiesToContinue = toSet;
  }
  void setMinMonToStop(int toSet) {
    this->minEnemiesToStop = toSet;
  }
  void setAttacks(QVector<AttackMethode> toSet) {
    this->attackMethodes = toSet;
  }
  AutoHuntData& operator=(const AutoHuntData& data) {
    this->setMinMonToContinue(data.getMinMonToContinue());
    this->setMinMonToStop(data.getMinMonToStop());
    this->setAttacks(data.getAttacks());
    return *this;
  }

 private:
  int                    minEnemiesToStop = 0, minEnemiesToContinue = 0;
  QVector<AttackMethode> attackMethodes = {};
};
