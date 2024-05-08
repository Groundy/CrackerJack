#pragma once
#include <qjsonobject.h>
namespace CJ {
class JsonClass {
 public:
  virtual QJsonObject toJson() const  = 0;
  virtual bool        isValid() const = 0;

 private:
};
}  // namespace CJ
