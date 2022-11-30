#include "RoutePoint.h"

RoutePoint::RoutePoint(Point3D position, FieldType fieldType) : position(position), fieldType(fieldType) {}
RoutePoint::RoutePoint(QJsonObject jsonObj) {
	Point3D position(jsonObj["position"].toObject());
	QString typeStr = jsonObj["type"].toString();
	FieldType fieldTypeToSet = getPointTypeNameMap().key(typeStr, FieldType::REGULAR);
	this->position = position;
	this->fieldType = fieldTypeToSet;
}
QMap<RoutePoint::FieldType, QString>  RoutePoint::getPointTypeNameMap() {
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
QVector<RoutePoint::FieldType> RoutePoint::getGoingUpTypes() {
	QVector<FieldType> typesGoingUp = {
		FieldType::EXIT_POINT,
		FieldType::LADDER_UP,
		FieldType::ROPE_FIELD,
		FieldType::STAIRS_UP,
		FieldType::TELEPORT
	};
	return typesGoingUp;
};
QVector<RoutePoint::FieldType> RoutePoint::getGoingDownTypes() {
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
QVector<RoutePoint::FieldType> RoutePoint::getStayingSameLevelTypes() {
	QVector<FieldType> typesGoingSameLevel = {
		FieldType::EXIT_POINT,
		FieldType::REGULAR,
		FieldType::TELEPORT
	};
	return typesGoingSameLevel;
};
bool RoutePoint::operator == (const RoutePoint& point) {
	return position == point.position && fieldType == point.fieldType;
}
RoutePoint& RoutePoint::operator = (const RoutePoint& point) {
	position = point.position;
	fieldType = point.fieldType;
	return *this;
}
QJsonObject RoutePoint::toJson() const {
	QMap<FieldType, QString> pointTypeNameMap = getPointTypeNameMap();
	QString typeStr = pointTypeNameMap.value(fieldType, pointTypeNameMap.value(FieldType::REGULAR));
	QJsonObject obj;
	obj.insert("position", position.toJson());
	obj.insert("type", typeStr);
	return obj;
}
bool RoutePoint::isValid() const {
	return position.isValid();
}