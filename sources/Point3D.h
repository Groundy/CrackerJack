#pragma once
#include <qstring.h>
#include <qstringlist.h>
#include <qjsonobject.h>

#include "JsonClass.h"
#include "Logger.h"
class Point3D : JsonClass
{
public:
	Point3D();	
	Point3D(int x, int y, int f); 
	Point3D(QString str);
	Point3D(QJsonObject obj);
	~Point3D();
	QString toString() const;
	bool operator==(Point3D pt);
	int getFloor() const { return floor; }
	int getX() const { return x; }
	int getY() const { return y; }
	int setFloor(int floor) { return this->floor = floor; }
	int setX(int x) { return this->x = x; }
	int setY(int y) { return this->y = y; }
	QPoint getXY() { return QPoint(x, y);}
	bool isValid() const;
	QJsonObject toJson() const;
private:
	int x, y, floor;
};
