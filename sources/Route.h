#pragma once
#include <qdir.h>
#include <qmap.h>
#include <qjsonarray.h>
#include "Point3D.h"
#include "Utilities.h"
#include "PathResource.h"
#include "RoutePoint.h"
class Route : JsonClass {
public:
	Route();
	Route(QJsonObject obj);
	~Route();

	QStringList toStringList();
	void addPoint(RoutePoint point);
	bool removePoint(int index);
	bool movePointUp(int index);
	bool movePointDown(int index);
	int size();
	void clear();
	bool checkRouteCorectness(QString& errorTextToDisplay);
	void setName(QString name) { routeName = name; };
	QString getName() const { return routeName; };
	RoutePoint getPoint(int index);
	QJsonObject toJson() const;
	bool isValid() const;
	bool checkIfPositionIsOnListOnIndex(Point3D toCheck, int indexToCheck);
private:
	QList<RoutePoint> route;
	QString routeName;
	QMap<RoutePoint::FieldType, QString> pointTypeNameMap = RoutePoint::getPointTypeNameMap();
};