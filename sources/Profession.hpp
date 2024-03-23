#pragma once
#include <qstring.h>
namespace CJ {
class Profession {
 public:
  enum class Type { RP, EK, ED, MS, UNDEFINED };
  Profession() : type(Type::UNDEFINED){};
  Profession(Profession::Type type) : type(type){};
  Profession(QString profName) {
    if (profName == "ED")
      type = Type::ED;
    else if (profName == "EK")
      type = Type::EK;
    else if (profName == "MS")
      type = Type::MS;
    else if (profName == "RP")
      type = Type::RP;
    else
      type = Type::UNDEFINED;
  };

  QString getTypeName() const {
    switch (type) {
      case Type::RP:
        return "RP";
      case Type::EK:
        return "EK";
      case Type::ED:
        return "ED";
      case Type::MS:
        return "MS";
      default:
        return "";
    }
  }
  Type getType() const {
    return type;
  };
  bool isMage() {
    return isED() || isMS();
  }
  bool isRP() {
    return type == Type::RP;
  }
  bool isEK() {
    return type == Type::EK;
  }
  bool isED() {
    return type == Type::ED;
  }
  bool isMS() {
    return type == Type::MS;
  }
  bool operator==(const Profession& prof) const {
    return type == prof.getType();
  }

 private:
  Type type;
};
}  // namespace CJ
