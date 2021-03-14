#pragma once
#include <QObject>

class RGBstruct : public QObject
{
	Q_OBJECT
public:
	uint r, g, b;
	static bool isPixelInRangeOfGrey(uint rgb, uint minValue, uint maxValue);
	RGBstruct(uint rgb);
	~RGBstruct();
};
