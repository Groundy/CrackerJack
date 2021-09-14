#include "Offer.h"

Offer::Offer(QString itemNameToSet, int minPrice, int maxPrice, int amount) {
	this->itemName = itemNameToSet;
	this->amount = amount;
	this->minPrice = minPrice;
	this->maxPrice = maxPrice;
}

Offer::Offer(QJsonObject obj){
	itemName = obj.value("itemName").toString();
	minPrice = obj.value("minPrice").toInt();
	maxPrice = obj.value("maxPrice").toInt();
	amount = obj.value("amount").toInt();
}

QJsonObject Offer::toJsonObj(){
	QJsonObject obj;
	obj.insert("itemName", QJsonValue(itemName));
	obj.insert("minPrice", QJsonValue(minPrice));
	obj.insert("maxPrice", QJsonValue(maxPrice));
	obj.insert("amount", QJsonValue(amount));
	return obj;
}

Offer::~Offer()
{
}
QString Offer::toString() {
	QString minPriceStr = QString::number(minPrice);
	QString maxPriceStr = QString::number(maxPrice);
	QString amountStr = QString::number(amount);

	QString toRet;
	toRet.append(this->itemName);
	toRet.append(",  min: " + minPriceStr);
	toRet.append(",  max: " + maxPriceStr);
	toRet.append(",  amount: " + amountStr);
	return toRet;
}