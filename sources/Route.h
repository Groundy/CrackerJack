#pragma once
#include "Point3D.h"
#include <qdir.h>
#include "JsonParser.h"
#include <qmap.h>
#include "Spell.h"
class Route {
public:
	Route();
	~Route();
	enum FieldType { REGULAR, STAIRS_UP, STAIRS_DOWN, ROPE_FIELD, SHOVEL_HOLE_ALWAYS_OPEN, SHOVEL_HOLE_NEED_SHOVEL, LADDER_UP, LADDER_DOWN, TELEPORT, EXIT_POINT };
	struct PointOnRoute : JsonClass {
		PointOnRoute(Point3D position, Route::FieldType fieldType) : position(position), fieldType(fieldType){}
		PointOnRoute(QJsonObject jsonObj) {
			QString positionStr = jsonObj["position"].toString();
			Point3D position(positionStr);
			QString typeStr = jsonObj["type"].toString();
			//FieldType fieldTypeToSet = pointTypeNameMap.(typeStr, FieldType::REGULAR);
			this->position = position;
			//this->fieldType = fieldTypeToSet;
		}
		bool operator == (const PointOnRoute& point) {
			return position == point.position && fieldType == point.fieldType;
		}
		PointOnRoute& operator = (const PointOnRoute& point) {
			position = point.position;
			fieldType = point.fieldType;
			return *this;
		}
		QJsonObject toJson() const {
			QJsonObject obj;
			//QString typeStr = fieldTypesStrsUsedInJson.value(pt.second);
			obj.insert("position", position.toString());
			obj.insert("type", "");
			return obj;
		}
		bool isValid() const {
			return position.isValid();
		}
		Route::FieldType fieldType;
		Point3D position;
	};
	QStringList toStringList();

	void addPoint(PointOnRoute point);
	bool removePoint(int index);
	bool movePointUp(int index);
	bool movePointDown(int index);
	bool loadFromJsonFile(QString pathToFile);
	int size();
	void clear();
	bool writeToJsonFile(QString pathToDir, QString fileNameWithExtension);
	bool checkRouteCorectness(QString& errorTextToDisplay);
private:
	QList<PointOnRoute> route;
	QMap<Route::FieldType, QString> pointTypeNameMap = getPointTypeNameMap();

	QMap<Route::FieldType, QString> getPointTypeNameMap();
};
