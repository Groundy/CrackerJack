#pragma once
#include "Point3D.h"
#include <qdir.h>
#include "JsonParser.h"
#include <qmap.h>
#include "Spell.h"
struct PointOnRoute : JsonClass {
	enum FieldType { REGULAR, STAIRS_UP, STAIRS_DOWN, ROPE_FIELD, SHOVEL_HOLE_ALWAYS_OPEN, SHOVEL_HOLE_NEED_SHOVEL, LADDER_UP, LADDER_DOWN, TELEPORT, EXIT_POINT };
	PointOnRoute(Point3D position, FieldType fieldType) : position(position), fieldType(fieldType){}
	PointOnRoute(QJsonObject jsonObj) {
		Point3D position(jsonObj["position"].toObject());
		QString typeStr = jsonObj["type"].toString();
		FieldType fieldTypeToSet = getPointTypeNameMap().key(typeStr, FieldType::REGULAR);
		this->position = position;
		this->fieldType = fieldTypeToSet;
	}
	
	static QMap<FieldType, QString> getPointTypeNameMap() {
		QMap<FieldType, QString> toRet;
		toRet.insert(FieldType::REGULAR, "Regular field");
		toRet.insert(FieldType::STAIRS_UP, "Stairs-up");
		toRet.insert(FieldType::STAIRS_DOWN, "Stairs-down");
		toRet.insert(FieldType::ROPE_FIELD, "Rope field");
		toRet.insert(FieldType::SHOVEL_HOLE_ALWAYS_OPEN, "Hole, open");
		toRet.insert(FieldType::SHOVEL_HOLE_NEED_SHOVEL, "Hole, close");
		toRet.insert(FieldType::LADDER_UP, "Ladder up");
		toRet.insert(FieldType::LADDER_DOWN, "Ladder down");
		toRet.insert(FieldType::TELEPORT, "Teleport");
		toRet.insert(FieldType::EXIT_POINT, "Log out point");
		return toRet;
	}
	static QVector<FieldType> getGoingUpTypes() {
		QVector<FieldType> typesGoingUp = {
			FieldType::EXIT_POINT,
			FieldType::LADDER_UP,
			FieldType::ROPE_FIELD,
			FieldType::STAIRS_UP,
			FieldType::TELEPORT
		};
		return typesGoingUp;
	};
	static QVector<FieldType> getGoingDownTypes() {
		QVector<FieldType> typesGoingDown = {
			FieldType::EXIT_POINT,
			FieldType::LADDER_DOWN,
			FieldType::SHOVEL_HOLE_ALWAYS_OPEN,
			FieldType::SHOVEL_HOLE_NEED_SHOVEL,
			FieldType::STAIRS_DOWN,
			FieldType::TELEPORT
		};
		return typesGoingDown;
	};
	static QVector<FieldType> getStayingSameLevelTypes() {
		QVector<FieldType> typesGoingSameLevel = {
			FieldType::EXIT_POINT,
			FieldType::REGULAR,
			FieldType::TELEPORT
		};
		return typesGoingSameLevel;
	};

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
		QMap<FieldType, QString> pointTypeNameMap = getPointTypeNameMap();
		QString typeStr = pointTypeNameMap.value(fieldType, pointTypeNameMap.value((FieldType::REGULAR)));

		obj.insert("position", position.toJson());
		obj.insert("type", typeStr);
		return obj;
	};
	bool isValid() const { return position.isValid(); };

	FieldType fieldType;
	Point3D position;
};

class Route {
public:
	Route();
	~Route();

	QStringList toStringList();
	void addPoint(PointOnRoute point);
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
	QList<PointOnRoute> route;
	QString routeName;
	QMap<PointOnRoute::FieldType, QString> pointTypeNameMap = PointOnRoute::getPointTypeNameMap();
};
