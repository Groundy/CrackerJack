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
	QString toString() const;
	bool operator==(Point3D pt);
	int getFloor() { return floor; }
	int getX() { return x; }
	int getY() { return y; }
	int setFloor(int floor) { return this->floor = floor; }
	int setX(int x) { return this->x = x; }
	int setY(int y) { return this->y = y; }
	QPoint getXY() { return QPoint(x, y);}
	bool isValid() const {
		return x >= 0 && x <= 2560 && y >= 0 && y < 2560 && floor >= -8 && floor <= 7;
	}
private:
	int x, y, floor;
};
