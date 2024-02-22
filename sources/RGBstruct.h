#pragma once
#include <minmax.h>
#include <qobject.h>

#include "JsonClass.hpp"
#include "qjsonobject.h"
class RGBstruct : JsonClass {
 public:
  RGBstruct(uint rgb);
  RGBstruct(uint r, uint g, uint b);
  bool        operator==(const RGBstruct& rgb) const;
  bool        operator<(const RGBstruct& rgb) const;
  uint        toUint() const;
  bool        isGrey() const;
  bool        isYellow() const;
  bool        isBlack() const;
  bool        isMapRed() const;
  bool        allColsEqualOrAboveThreshold(uint threshold) const;
  uint        getMinColVal() const;
  uint        getMaxColVal() const;
  bool        oneColEqualOrAboveThreshold(uint threshold) const;
  QString     toString() const;
  bool        isPixelInRangeOfGrey(uint minValue, uint maxValue);
  QJsonObject toJson() const;
  bool        isValid() const;

 private:
  uint r, g, b;
};
