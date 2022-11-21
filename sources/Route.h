#pragma once
#include "Point3D.h"
#include <qdir.h>
#include "JsonParser.h"
#include <qmap.h>
#include "Spell.h"
class Route{

public:
	Route();
	~Route();
	enum FIELDS_TYPE { REGULAR, STAIRS_UP, STAIRS_DOWN, ROPE_FIELD, SHOVEL_HOLE_ALWAYS_OPEN, SHOVEL_HOLE_NEED_SHOVEL, LADDER_UP, LADDER_DOWN, TELEPORT, EXIT_POINT };
	QStringList toStringList();
	void addPoint(Point3D point, FIELDS_TYPE type);
	bool removePoint(int index);
	bool movePointUp(int index);
	bool movePointDown(int index);
	bool loadFromJsonFile(QString pathToFile);
	int size();
	void clear();
	bool writeToJsonFile(QString pathToDir, QString fileNameWithExtension);
	QMap <FIELDS_TYPE, QString> fieldTypesStrsUsedInJson;
	bool checkRouteCorectness(QString& errorTextToDisplay);
	static QString TRANSLATE_getPointTypeDescription(FIELDS_TYPE type);
	
private:
	QList<QPair<Point3D, FIELDS_TYPE>> route;
	void fillMaps();
};
