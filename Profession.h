#pragma once
#include <qstring.h>
class Profession
{
public:
	Profession() : type(UNDEFINED) {};
	Profession(QString profName) {
		if (profName == "ED")
			type = ED;
		else if (profName == "EK")
			type = EK;
		else if (profName == "MS")
			type = MS;
		else if (profName == "RP")
			type = RP;
		else
			type = UNDEFINED;
	};	
	enum Type{ RP, EK, ED, MS, UNDEFINED };
	QString getTypeName() {
		switch (type)
		{
		case RP:
			return "RP";
		case EK:
			return "EK";
		case ED:
			return "ED";
		case MS:
			return "MS";
		default:
			return "";
		}
	}
	Type getType() { return type; };
	bool isMage() {
		return isED() || isMS();
	}
	bool isRP() {
		return type == RP;
	}
	bool isEK() {
		return type == EK;
	}
	bool isED() {
		return type == ED;
	}
	bool isMS() {
		return type == MS;
	}
private:
	Type type;
};
