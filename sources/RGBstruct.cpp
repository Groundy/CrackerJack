#include "RGBstruct.h"
RGBstruct::RGBstruct(uint rgb) {
  r = (rgb >> 16) & 0xff;
  g = (rgb >> 8) & 0xff;
  b = rgb & 0xff;
}
RGBstruct::RGBstruct(uint r, uint g, uint b) : r(r), g(g), b(b){};

bool RGBstruct::isPixelInRangeOfGrey(uint minValue, uint maxValue) {
  return (g >= minValue) && (g <= maxValue) && (b >= minValue) && (b <= maxValue) && (r >= minValue) && (r <= maxValue);
}
uint RGBstruct::toUint() const {
  return r << 16 | g << 8 | b;
}
QString RGBstruct::toString() const {
  uint    decimal  = this->toUint();
  QString hexvalue = QString("%1").arg(decimal, 8, 16, QLatin1Char('0'));
  return hexvalue;
}
QJsonObject RGBstruct::toJson() const {
  QJsonObject obj;
  obj.insert("r", (int)r);
  obj.insert("g", (int)g);
  obj.insert("b", (int)b);
  return obj;
};
bool RGBstruct::operator==(const RGBstruct& rgb) const {
  return (r == rgb.r) && (b == rgb.b) && (g == rgb.g);
}
bool RGBstruct::operator<(const RGBstruct& rgb) const {
  return (r + g + b) < (rgb.r + rgb.b + rgb.g);
}
bool RGBstruct::isGrey() const {
  return r == g && g == b;
}
bool RGBstruct::isYellow() const {
  return r == 255 && g == 255 && b == 0;
}
bool RGBstruct::isBlack() const {
  return r == 0 && g == 0 && b == 0;
}
bool RGBstruct::isMapRed() const {
  return r == 255 && g == 51 && b == 0;
}
bool RGBstruct::allColsEqualOrAboveThreshold(uint threshold) const {
  return r >= threshold && g >= threshold && b >= threshold;
}
uint RGBstruct::getMinColVal() const {
  return min(min(r, g), b);
}
uint RGBstruct::getMaxColVal() const {
  return max(max(r, g), b);
}
bool RGBstruct::oneColEqualOrAboveThreshold(uint threshold) const {
  return r >= threshold || g >= threshold || b >= threshold;
}
bool RGBstruct::isValid() const {
  return r >= 0 && g >= 0 && b >= 0 && r <= 255 && g <= 55 && b <= 255;
};
