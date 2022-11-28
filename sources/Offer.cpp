#include "Offer.h"
/*
Offer::Offer(QString itemNameToSet, int minPrice, int maxPrice, int amount, bool placeOffer, Type type) {
	this->itemName = itemNameToSet;
	this->amount = amount;
	this->minPrice = minPrice;
	this->maxPrice = maxPrice;
	this->placeOffer = placeOffer;
	this->type = type;
}

Offer::Offer(QJsonObject obj){
	itemName = obj.value("itemName").toString();
	minPrice = obj.value("minPrice").toInt();
	maxPrice = obj.value("maxPrice").toInt();
	amount = obj.value("amount").toInt();
	placeOffer = obj.value("placeOffer").toBool();
	QString typeStr = obj.value("type").toString();
	type = typeStr == "SELL" ? Type::SELL : Type::BUY;
}

QJsonObject Offer::toJsonObj(){
	QJsonObject obj;
	obj.insert("itemName", QJsonValue(itemName));
	obj.insert("minPrice", QJsonValue(minPrice));
	obj.insert("maxPrice", QJsonValue(maxPrice));
	obj.insert("amount", QJsonValue(amount));
	QString typeStr = type == Type::SELL ? "SELL" : "BUY";
	obj.insert("type", QJsonValue(typeStr));
	obj.insert("placeOffer", QJsonValue(placeOffer));
	return obj;
}

QString Offer::toString() {
	QString minPriceStr = QString::number(minPrice);
	QString maxPriceStr = QString::number(maxPrice);
	QString amountStr = QString::number(amount);
	QString placeStr = placeOffer ? "Yes" : "No";
	QString typeStr = type == Type::SELL ? "SELL" : "BUY";

	QString toRet;
	toRet.append(this->itemName);
	toRet.append(",  min: " + minPriceStr);
	toRet.append(",  max: " + maxPriceStr);
	toRet.append(",  amount: " + amountStr);
	toRet.append(",  make offer: " + placeStr);
	toRet.append(",  type: " + typeStr);
	return toRet;
}

*/