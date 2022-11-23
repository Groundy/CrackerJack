#pragma once
#include <qstring.h>
#include <qstringlist.h>
class Point3D
{
public:
	Point3D();	
	Point3D(int x, int y, int f); 
	Point3D(QString str);
	~Point3D();
	QString toString();
	bool operator==(Point3D pt);
	int getFloor() { return floor; }
	int getX() { return x; }
	int getY() { return y; }
	int setFloor(int floor) { return this->floor = floor; }
	int setX(int x) { return this->x = x; }
	int setY(int y) { return this->y = y; }
	QPoint getXY() { return QPoint(x, y);}
private:
	int x, y, floor;
};
