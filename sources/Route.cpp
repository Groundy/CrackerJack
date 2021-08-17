#include "Route.h"

Route::Route(){
	fillMaps();
}

Route::~Route()
{
}

void Route::fillMaps() {
	fieldTypesStrsUsedInJson.insert(Route::FIELDS_TYPE::REGULAR,"REGULAR");
	fieldTypesStrsUsedInJson.insert(Route::FIELDS_TYPE::STAIRS_UP,"STAIRS_UP");
	fieldTypesStrsUsedInJson.insert(Route::FIELDS_TYPE::STAIRS_DOWN,"STAIRS_DOWN");
	fieldTypesStrsUsedInJson.insert(Route::FIELDS_TYPE::ROPE_FIELD,"ROPE_FIELD");
	fieldTypesStrsUsedInJson.insert(Route::FIELDS_TYPE::SHOVEL_HOLE_ALWAYS_OPEN,"SHOVEL_HOLE_ALWAYS_OPEN");
	fieldTypesStrsUsedInJson.insert(Route::FIELDS_TYPE::SHOVEL_HOLE_NEED_SHOVEL,"SHOVEL_HOLE_NEED_SHOVEL");
	fieldTypesStrsUsedInJson.insert(Route::FIELDS_TYPE::LADDER_UP,"LADDER_UP");
	fieldTypesStrsUsedInJson.insert(Route::FIELDS_TYPE::LADDER_DOWN,"LADDER_DOWN");
	fieldTypesStrsUsedInJson.insert(Route::FIELDS_TYPE::TELEPORT,"TELEPORT");
	fieldTypesStrsUsedInJson.insert(Route::FIELDS_TYPE::EXIT_POINT,"EXIT_POINT");
	routeTypesStrsUsedInJson.insert(Route::ROUTE_TYPE::BACK_AND_FORTH, "BACK_AND_FORTH");
	routeTypesStrsUsedInJson.insert(Route::ROUTE_TYPE::CIRCLE, "CIRCLE");
};

QStringList Route::toStringList(){
	if(route.isEmpty())
		return QStringList();

	QStringList ToRet;
	for (int i = 0; i < route.size(); i++) {
		QString index = QString::number(i);
		QString pt = route[i].first.toString();
		QString type = TRANSLATE_getPointTypeDescription(route[i].second);
		ToRet << QString("[%1]  %2,  %3").arg(index, pt, type);
	}
	return ToRet;
}

void Route::addPoint(Point3D point, FIELDS_TYPE type){
	QPair<Point3D, FIELDS_TYPE> toAdd(point,type);
	route.push_back(toAdd);
}

bool Route::removePoint(int index){
	bool indexInRange  = index < route.size();
	if (!indexInRange)
		return false;
	route.removeAt(index);
	return true;
}

bool Route::movePointUp(int index){
	bool itFirst = index == 0;
	if (itFirst)
		return false;

	route[index].swap(route[index - 1]);
	return true;
}

bool Route::movePointDown(int index){
	bool isLast = index == route.size() - 1;
	if (isLast)
		return false;

	route[index].swap(route[index + 1]);
	return true;
}

bool Route::loadFromJsonFile(QString pathToFile){
	QJsonObject obj;
	JsonParser jsonParser;
	bool fileFound = jsonParser.openJsonFile(obj, pathToFile);
	if (!fileFound) {
		//todo logg
		//todo info window
		return false;
	}

	QString routeTypeValue = obj["Route Type"].toString();
	ROUTE_TYPE tmpRouteType = routeTypesStrsUsedInJson.key(routeTypeValue, ROUTE_TYPE::CIRCLE);

	QJsonArray pointsArray = obj["points"].toArray();
	if (pointsArray.isEmpty()) {
			//todo logg
			//todo info window
		return false;
	}

	QList<QPair<Point3D, FIELDS_TYPE>> tmpRoute;
	for each (QJsonValue val in pointsArray){
		QString positionStr = val["position"].toString();
		Point3D position(positionStr);
		QString typeStr = val["type"].toString();
		FIELDS_TYPE fieldTypeToSet = fieldTypesStrsUsedInJson.key(typeStr, FIELDS_TYPE::REGULAR);
		QPair<Point3D, FIELDS_TYPE> toAdd(position, fieldTypeToSet);
		tmpRoute.append(toAdd);
	}


	route = tmpRoute;
	routeType = tmpRouteType;
	return true;
}

int Route::size(){
	return route.size();
}

bool Route::writeToJsonFile(QString pathToDir, QString fileNameWithExtension){
	QJsonObject mainObj;

	QJsonObject routeTypeObj;
	mainObj.insert("Route Type", routeTypesStrsUsedInJson.value(routeType));

	QJsonArray arr;
	for each (QPair<Point3D, FIELDS_TYPE> pt in route){
		QJsonObject toAdd;
		QString postionStr = pt.first.toString();
		QString typeStr = fieldTypesStrsUsedInJson.value(pt.second);
		toAdd.insert("position", postionStr);
		toAdd.insert("type", typeStr);
		arr.append(toAdd);
	}
	mainObj.insert("points", QJsonValue(arr));

	QJsonDocument doc(mainObj);
	JsonParser jsonParser;
	bool ok = jsonParser.saveJsonFile(pathToDir,fileNameWithExtension,doc);
	return ok;
}

bool Route::routeIsOk(QString& errorTextToDisplay){
	bool isPl = StringResource::languageIsPl();
	if (route.size() < 2) {
		errorTextToDisplay = isPl ? QString::fromLocal8Bit("Trasa jest za krótka.") : "Route is too short.";
		return false;
	}

	QList<QPair<Point3D, FIELDS_TYPE>> tmpRoute;
	if (routeType == Route::ROUTE_TYPE::CIRCLE) {
		tmpRoute = route;
		QPair<Point3D, FIELDS_TYPE> toAdd;
		toAdd.first = route.first().first;
		toAdd.second = route.first().second;
		tmpRoute.push_back(toAdd);
	}
	else if (routeType == Route::ROUTE_TYPE::BACK_AND_FORTH) {
		tmpRoute = route;
		int startIndex = route.size() - 2;//skipped last point from doubling;
		for (size_t i = route.size() - 2; i > 0; i--){
			tmpRoute.push_back(route[i]);
		}
	}
	bool sizeOkCircle = tmpRoute.size() == route.size() - 1;//tmp
	bool sizeOkBackAndForth = tmpRoute.size() == 2*route.size() - 1;

	QList<FIELDS_TYPE> typesGoingDown = { 
		FIELDS_TYPE::EXIT_POINT,
		FIELDS_TYPE::LADDER_DOWN,
		FIELDS_TYPE::SHOVEL_HOLE_ALWAYS_OPEN,
		FIELDS_TYPE::SHOVEL_HOLE_NEED_SHOVEL,
		FIELDS_TYPE::STAIRS_DOWN,
		FIELDS_TYPE::TELEPORT
	};
	QList<FIELDS_TYPE> typesGoingSame = {
		FIELDS_TYPE::EXIT_POINT,
		FIELDS_TYPE::REGULAR,
		FIELDS_TYPE::TELEPORT
	};
	QList<FIELDS_TYPE> typesGoingUp = {
	FIELDS_TYPE::EXIT_POINT,
	FIELDS_TYPE::LADDER_UP,
	FIELDS_TYPE::ROPE_FIELD,
	FIELDS_TYPE::STAIRS_UP,
	FIELDS_TYPE::TELEPORT
	};

	for (size_t i = 0; i < tmpRoute.size() - 1; i++){
		Point3D current = tmpRoute[i].first;
		Point3D next = tmpRoute[i + 1].first;
		FIELDS_TYPE currentType = tmpRoute[i].second;
		if (current.floor < next.floor) {
			bool ok = typesGoingUp.contains(currentType);
		}
		if (current.floor == next.floor) {
			bool ok = typesGoingSame.contains(currentType);
		}
		if (current.floor > next.floor) {
			bool ok = typesGoingDown.contains(currentType);
		}
	}
	return true;
}

QString Route::TRANSLATE_getPointTypeDescription(FIELDS_TYPE type) {
	bool isPl = StringResource::languageIsPl();
	switch (type)
	{
	case Route::FIELDS_TYPE::REGULAR:
		return isPl ? QString::fromLocal8Bit("Zwyk³e pole") : "Regular field";
	case Route::FIELDS_TYPE::STAIRS_UP:
		return isPl ? QString::fromLocal8Bit("Schody do góry") : "Staris-up";
	case Route::FIELDS_TYPE::STAIRS_DOWN: 
		return isPl ? QString::fromLocal8Bit("Schody do do³u") : "Staris-down";
	case Route::FIELDS_TYPE::ROPE_FIELD:
		return isPl ? QString::fromLocal8Bit("Pole liny") : "Rope field";
	case Route::FIELDS_TYPE::SHOVEL_HOLE_ALWAYS_OPEN:
		return isPl ? QString::fromLocal8Bit("Dziura, otwarta") : "Hole, open";
	case Route::FIELDS_TYPE::SHOVEL_HOLE_NEED_SHOVEL:
		return isPl ? QString::fromLocal8Bit("Dziura, zamknieta") : "Hole, close";
	case Route::FIELDS_TYPE::LADDER_UP:
		return isPl ? QString::fromLocal8Bit("Drabina w górê") : "Ladder up";
	case Route::FIELDS_TYPE::LADDER_DOWN:
		return isPl ? QString::fromLocal8Bit("Drabian w dó³") : "Ladder down";
	case Route::FIELDS_TYPE::TELEPORT:
		return isPl ? QString::fromLocal8Bit("Teleport") : "Teleport";
	case Route::FIELDS_TYPE::EXIT_POINT:
		return isPl ? QString::fromLocal8Bit("Punkt wyjscia") : "Log out point";
	default:
		return isPl ? QString::fromLocal8Bit("") : "";
	}
}
