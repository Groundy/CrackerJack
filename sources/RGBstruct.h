#pragma once
#include <qobject.h>
#include <minmax.h>
class RGBstruct{
public:
	RGBstruct(uint rgb);
	RGBstruct(uint r, uint g, uint b);
	~RGBstruct();
	bool operator==(const RGBstruct& rgb) {
		return (r == rgb.r) && (b == rgb.b) && (g == rgb.g);
	}
	uint toUint() const;
	bool isGrey() const {
		return r == g && g == b;
	}
	bool isYellow() const  {
		return r == 255 && g == 255 && b == 0;
	}
	bool isBlack() const {
		return r == 0 && g == 0 && b == 0;
	}
	bool allColsEqualOrAboveThreshold(uint threshold)  const {
		return r >= threshold && g >= threshold && b >= threshold;
	}
	uint getMinColVal() const {
		return min(min(r, g), b);
	}
	uint getMaxColVal() {
		return max(max(r, g), b);
	}
	bool oneColEqualOrAboveThreshold(uint threshold) const {
		return r >= threshold || g >= threshold || b >= threshold;
	}
	QString toString() const;
	bool isPixelInRangeOfGrey(uint minValue, uint maxValue);
private:
	uint r, g, b;
};
