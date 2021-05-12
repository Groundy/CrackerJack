#include "RGBstruct.h"

bool RGBstruct::isPixelInRangeOfGrey(uint rgb, uint minValue, uint maxValue){
	RGBstruct rgbStr(rgb);
	bool gIsInRange = (rgbStr.g >= minValue) && (rgbStr.g <= maxValue);
	bool bIsInRange = (rgbStr.b >= minValue) && (rgbStr.b <= maxValue);
	bool rIsInRange = (rgbStr.r >= minValue) && (rgbStr.r <= maxValue);
	bool toRet = gIsInRange && bIsInRange && rIsInRange;
	return toRet;
}

RGBstruct::RGBstruct(uint rgb){
	r = (rgb >> 16) & 0xff;
	g = (rgb >> 8) & 0xff;
	b = rgb & 0xff;
}

RGBstruct::RGBstruct(uint r, uint g, uint b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

RGBstruct::~RGBstruct()
{
}
