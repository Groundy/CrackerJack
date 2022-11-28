#pragma once
#include <QObject>
#include <qjsonobject.h>

class Offer : QObject{
	Q_OBJECT
public:
	Offer() {};
	~Offer() {};
};
	/*
public:


	enum class Type { SELL, BUY };
	Offer(QString itemName, int minPrice, int maxPrice, int amount, bool placeOffer, Type type);
	Offer(QJsonObject obj);
	QJsonObject toJsonObj();
	QString toString();
	QString itemName;
	Type type;
	bool placeOffer;
	int minPrice, maxPrice, amount;
	bool operator ==(const Offer toComp) {
		bool sameName = this->itemName == toComp.itemName;
		bool sameAmount = this->amount == toComp.amount;
		bool sameMinPrice = this->minPrice == toComp.minPrice;
		bool sameMaxPrice = this->maxPrice == toComp.maxPrice;
		bool samePlaceOffer = this->placeOffer == toComp.placeOffer;
		bool sameType = this->type == toComp.type;
		bool toRet = sameName && sameAmount && sameMinPrice && sameMaxPrice && samePlaceOffer && sameType;
		return toRet;
	};
	~Offer();
	*/