#pragma once
#include <qobject.h>
#include <minmax.h>
class RGBstruct{
public:
	RGBstruct(uint rgb);
	RGBstruct(uint r, uint g, uint b);
	bool operator==(const RGBstruct& rgb) {
		return (this->r == rgb.r) && (this->b == rgb.b) && (this->g == rgb.g);
	}
	uint toUint();
	~RGBstruct();
	bool isGrey() {
		return r == g && g == b;
	}
	bool isYellow() {
		return r == 255 && g == 255 && b == 0;
	}
	bool isBlack() {
		return r == 0 && g == 0 && b == 0;
	}
	bool allColsEqualOrAboveThreshold(uint threshold) {
		return r >= threshold && g >= threshold && b >= threshold;
	}
	uint getMinColVal() {
		return min(min(r, g), b);
	}
	uint getMaxColVal() {
		return min(min(r, g), b);
	}
	bool oneColEqualOrAboveThreshold(uint threshold) {
		return r >= threshold || g >= threshold || b >= threshold;
	}
	QString toString() {
		const QString HASH = QString("#");
		const QString FLOOR = QString("_");
		return HASH + r + HASH + g + HASH + b + FLOOR;
	}
	bool isPixelInRangeOfGrey(uint minValue, uint maxValue);
private:
	uint r, g, b;
};
