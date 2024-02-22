#pragma once
#include <qjsonobject.h>
class JsonClass {
 public:
  virtual QJsonObject toJson() const  = 0;
  virtual bool        isValid() const = 0;

 private:
};
