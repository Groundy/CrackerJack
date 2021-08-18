#pragma once
#include <qstring.h>
#include <qstringlist.h>
#include "Logger.h"
class Point3D
{
public:
	int x, y, floor;

	Point3D();	
	Point3D(int X, int Y, int F); 
	Point3D(QString str);
	~Point3D();

	QString toString();
	bool operator==(Point3D pt);
};
