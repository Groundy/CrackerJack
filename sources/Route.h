#pragma once
#include <qdir.h>
#include <qmap.h>

#include "Point3D.h"
#include "JsonParser.h"
#include "Spell.h"
#include "RoutePoint.h"

class Route {
public:
	Route();
	~Route();

	QStringList toStringList();
	void addPoint(RoutePoint point);
	bool removePoint(int index);
	bool movePointUp(int index);
	bool movePointDown(int index);
	bool loadFromJsonFile(QString pathToFile);
	int size();
	void clear();
	QJsonObject toJson() const;
	bool checkRouteCorectness(QString& errorTextToDisplay);
	void setName(QString name) { routeName = name; };
	QString getName() const { return routeName; };
private:
	QList<RoutePoint> route;
	QString routeName;
	QMap<RoutePoint::FieldType, QString> pointTypeNameMap = RoutePoint::getPointTypeNameMap();
};