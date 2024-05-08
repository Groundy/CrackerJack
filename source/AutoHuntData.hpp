#include "AttackMethode.hpp"
#include "JsonClass.hpp"
#include "qjsonarray.h"
namespace CJ {
class AutoHuntData : JsonClass {
 public:
  AutoHuntData()  = default;
  ~AutoHuntData() = default;
  bool isValid() const {
    return true;
  }
  QJsonObject toJson() const {
    QJsonObject obj;
    obj.insert("minEnemiesToStop", static_cast<int>(min_enemies_to_stop_));
    obj.insert("min_enemies_to_continue_", static_cast<int>(min_enemies_to_continue_));
    QJsonArray attackMethodesArr;
    foreach (const auto& methode, attack_methodes_) {
      attackMethodesArr.append(methode.toJson());
    }
    obj.insert("attackMethodes", attackMethodesArr);
    return obj;
  }
  AutoHuntData(QJsonObject obj) {
    min_enemies_to_continue_     = obj["min_enemies_to_continue_"].toInt();
    min_enemies_to_stop_         = obj["minEnemiesToStop"].toInt();
    QJsonArray attackMethodesArr = obj["attackMethodes"].toArray();
    foreach (const QJsonValue& methode, attackMethodesArr) {
      this->attack_methodes_.append(AttackMethode(methode.toObject()));
    }
  }
  int getMinMonToContinue() const {
    return min_enemies_to_continue_;
  }
  int getMinMonToStop() const {
    return min_enemies_to_stop_;
  }
  QVector<AttackMethode> getAttacks() const {
    return attack_methodes_;
  }
  void setMinMonToContinue(int toSet) {
    this->min_enemies_to_continue_ = toSet;
  }
  void setMinMonToStop(int toSet) {
    this->min_enemies_to_stop_ = toSet;
  }
  void setAttacks(QVector<AttackMethode> toSet) {
    this->attack_methodes_ = toSet;
  }
  AutoHuntData& operator=(const AutoHuntData& data) {
    this->setMinMonToContinue(data.getMinMonToContinue());
    this->setMinMonToStop(data.getMinMonToStop());
    this->setAttacks(data.getAttacks());
    return *this;
  }

 private:
  uint                   min_enemies_to_stop_     = 0;
  uint                   min_enemies_to_continue_ = 0;
  QVector<AttackMethode> attack_methodes_         = {};
};
}  // namespace CJ
