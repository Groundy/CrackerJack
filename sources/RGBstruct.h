#pragma once
#include <QObject>

class RGBstruct : public QObject
{
	Q_OBJECT
public:
	uint r, g, b;
	static bool isPixelInRangeOfGrey(uint rgb, uint minValue, uint maxValue);
	RGBstruct(uint rgb);
	RGBstruct(uint r, uint g, uint b);
	bool operator==(RGBstruct& rgb) {
		bool sameR = this->r == rgb.r;
		bool sameB = this->b == rgb.b;
		bool sameG = this->g == rgb.g;
		return sameR && sameG && sameB;
	};
	~RGBstruct();
};
