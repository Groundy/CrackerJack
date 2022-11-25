#include "RGBstruct.h"

bool RGBstruct::isPixelInRangeOfGrey(uint minValue, uint maxValue){
	return
		(g >= minValue) && (g <= maxValue) &&
		(b >= minValue) && (b <= maxValue) &&
		(r >= minValue) && (r <= maxValue);
}

RGBstruct::RGBstruct(uint rgb){
	r = (rgb >> 16) & 0xff;
	g = (rgb >> 8) & 0xff;
	b = rgb & 0xff;
}

RGBstruct::RGBstruct(uint r, uint g, uint b) :
	r(r), g(g), b(b) {
}

uint RGBstruct::toUint() const{
	return r << 16 | g << 8 | b;
}

QString RGBstruct::toString() const {
	uint decimal = this->toUint();
	QString hexvalue = QString("%1").arg(decimal, 8, 16, QLatin1Char('0'));
	return hexvalue;
}

RGBstruct::~RGBstruct()
{
}
