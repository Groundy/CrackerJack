#pragma once
#include "Point3D.h"
#include "StringResource.h"

class Route{

public:
	Route();
	~Route();
	enum ROUTE_TYPE { CIRCLE, BACK_AND_FORTH };
	enum FIELDS_TYPE { REGULAR, STAIRS_UP, STAIRS_DOWN, ROPE_FIELD, SHOVEL_HOLE_ALWAYS_OPEN, SHOVEL_HOLE_NEED_SHOVEL, LADDER_UP, LADDER_DOWN, TELEPORT, EXIT_POINT };
	QStringList toStringList();
	void addPoint(Point3D point, FIELDS_TYPE type);
	bool removePoint(int index);
	ROUTE_TYPE routeType;
	bool movePointUp(int index);
	bool movePointDown(int index);
private:
	QList<QPair<Point3D, FIELDS_TYPE>> route;
	QString TRANSLATE_getPointTypeDescription(FIELDS_TYPE type);

};
