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
	bool fileFound = JsonParser::openJsonFile(obj, pathToFile);
	if (!fileFound) {
		Logger::logPotenialBug("Can't find file " + pathToFile, "loadFromJsonFile", "Route");
		QString text = QObject::tr("File: ") + pathToFile + QObject::tr(" doesn't exist.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}

	QJsonArray pointsArray = obj["points"].toArray();
	if (pointsArray.isEmpty()) {
		Logger::logPotenialBug(QString("file has wrong structure.").arg(pathToFile), "loadFromJsonFile", "Route");
		QString text = QObject::tr("File: ") + pathToFile + QObject::tr(" has wrong structure.");
		Utilities::showMessageBox_INFO(text);
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
	return true;
}

int Route::size(){
	return route.size();
}

void Route::clear(){
	route.clear();
}

bool Route::writeToJsonFile(QString pathToDir, QString fileNameWithExtension){
	QJsonObject mainObj;

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

bool Route::checkRouteCorectness(QString& errorTextToDisplay){
	if (route.size() < 2) {
		errorTextToDisplay = QObject::tr("Route is too short.");
		return false;
	}

	bool samePoint = route.first().first == route.last().first;
	bool sameType = route.first().second == route.last().second;
	bool twoPointsAreSame = sameType && samePoint;
	if (!twoPointsAreSame) {
		errorTextToDisplay = QObject::tr("Route should start and end in the same point.");
		return false;
	}

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

	enum class FLOOR_CHANGE { UP, SAME, DOWN };
	FLOOR_CHANGE flChange;

	for (size_t i = 0; i < route.size() - 1; i++){
		Point3D current = route[i].first;
		Point3D next = route[i + 1].first;
		FIELDS_TYPE currentType = route[i].second;

		bool canGoToNextPoint;

		if (current.floor < next.floor) {
			canGoToNextPoint = typesGoingUp.contains(currentType);
			flChange = FLOOR_CHANGE::UP;
		}
		else if (current.floor == next.floor) {
			canGoToNextPoint = typesGoingSame.contains(currentType);
			flChange = FLOOR_CHANGE::SAME;
		}
		else if (current.floor > next.floor) {
			canGoToNextPoint = typesGoingDown.contains(currentType);
			flChange = FLOOR_CHANGE::DOWN;
		}
		if (canGoToNextPoint)
			continue;

		QString mainText =
			QObject::tr("Can't find a way from point") +
			QString(" [%1]").arg(QString::number(i)) +
			QObject::tr("to point")+ 
			QString(" [%1]").arg(QString::number(i + 1));


		QString typeStr = TRANSLATE_getPointTypeDescription(currentType);
		QString argumentText = flChange == FLOOR_CHANGE::DOWN ? QObject::tr("down") : QObject::tr("up");
		QString reason =
			QObject::tr(", because type ") +
			typeStr +
			QObject::tr(", can't move to ") +
			argumentText +
			QObject::tr(" floor.");
		errorTextToDisplay = mainText + reason;
		
		return false;
	}
	return true;
}

QString Route::TRANSLATE_getPointTypeDescription(FIELDS_TYPE type) {
	switch (type)
	{
	case Route::FIELDS_TYPE::REGULAR:
		return QObject::tr("Regular field");
	case Route::FIELDS_TYPE::STAIRS_UP:
		return QObject::tr("Stairs-up");
	case Route::FIELDS_TYPE::STAIRS_DOWN: 
		return QObject::tr("Stairs-down");
	case Route::FIELDS_TYPE::ROPE_FIELD:
		return QObject::tr("Rope field");
	case Route::FIELDS_TYPE::SHOVEL_HOLE_ALWAYS_OPEN:
		return QObject::tr("Hole, open");
	case Route::FIELDS_TYPE::SHOVEL_HOLE_NEED_SHOVEL:
		return QObject::tr("Hole, close");
	case Route::FIELDS_TYPE::LADDER_UP:
		return QObject::tr("Ladder up");
	case Route::FIELDS_TYPE::LADDER_DOWN:
		return QObject::tr("Ladder down");
	case Route::FIELDS_TYPE::TELEPORT:
		return QObject::tr("Teleport");
	case Route::FIELDS_TYPE::EXIT_POINT:
		return QObject::tr("Log out point");
	default:
		return "";
	}
}
