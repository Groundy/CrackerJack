#pragma once
#include <QObject>
#include <qjsonobject.h>

class Offer{
public:
	Offer(QString itemName, int minPrice, int maxPrice, int amount);
	Offer(QJsonObject obj);
	QJsonObject toJsonObj();
	QString toString();
	QString itemName;
	int minPrice, maxPrice, amount;
	~Offer();
};
