#include "Route.h"

Route::Route(){
}
Route::~Route()
{
}

QStringList Route::toStringList(){
	if(route.isEmpty())
		return QStringList();

	QStringList ToRet;
	for (int i = 0; i < route.size(); i++) {
		QString index = QString::number(i);
		QString pt = route[i].getPosition().toString();
		QString type = pointTypeNameMap.value(route[i].getFieldType());
		ToRet << QString("[%1]  %2,  %3").arg(index, pt, type);
	}
	return ToRet;
}
void Route::addPoint(RoutePoint routePointToAdd){
	RoutePoint toAdd(routePointToAdd);
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
	route.swap(index, index - 1);
	return true;
}
bool Route::movePointDown(int index){
	bool isLast = index == route.size() - 1;
	if (isLast)
		return false;

	route.swap(index, index + 1);
	return true;
}
bool Route::loadFromJsonFile(QString pathToFile){
	try{
		QJsonObject obj;
		bool fileFound = JsonParser::openJsonFile(obj, pathToFile);
		if (!fileFound) {
			QString text = QString("Plik %1 nie istnieje").arg(pathToFile);
			throw std::exception(text.toStdString().c_str());
		}

		QJsonArray pointsArray = obj["points"].toArray();
		if (pointsArray.isEmpty()) {
			QString text = QString("Plik %1 ma niewlasciwa strukture").arg(pathToFile);//no polish chars
			throw std::exception(text.toStdString().c_str());
		}

		QList<RoutePoint> tmpRoute;
		for each (QJsonValue val in pointsArray){
			RoutePoint toAdd(val.toObject());
			tmpRoute.append(toAdd);
		}
		if(tmpRoute.size() < 2) {
			QString text = QString("Plik %1 ma niewystarczajaca liczbe punktow").arg(pathToFile);//no polish chars
			throw std::exception(text.toStdString().c_str());
		}
		route = tmpRoute;
		routeName = obj.value("routeName").toString();
		return true;
	}
	catch (const std::exception& e){
		Logger::staticLog(e.what());
		return false;
	}
}
int Route::size(){
	return route.size();
}
void Route::clear(){
	route.clear();
}
QJsonObject Route::toJson() const {
	QJsonArray arr;
	for each (RoutePoint pt in route) {
		arr.append(pt.toJson());
	}
	QJsonObject mainObj;
	mainObj.insert("routeName", routeName);
	mainObj.insert("points", QJsonValue(arr));
	return mainObj;
};
bool Route::checkRouteCorectness(QString& errorTextToDisplay){
	typedef RoutePoint::FieldType FieldType;
	try{
		if (route.size() < 2)
			throw std::exception("Route is too short.");

		bool samePoint = route.first() == route.last();
		if (!samePoint) 
			throw std::exception("Route should start and end in the same point.");

		QVector<FieldType> typesGoingDown = RoutePoint::getGoingDownTypes();
		QVector<FieldType> typesGoingSame = RoutePoint::getStayingSameLevelTypes();
		QVector<FieldType> typesGoingUp = RoutePoint::getGoingUpTypes();

		enum class FLOOR_CHANGE { UP, SAME, DOWN };
		FLOOR_CHANGE flChange;

		for (int i = 0; i < route.size() - 1; i++){
			Point3D current = route[i].getPosition();
			Point3D next = route[i + 1].getPosition();
			FieldType currentType = route[i].getFieldType();

			bool canGoToNextPoint;
			const int currentF = current.getFloor();
			const int nextF = next.getFloor();
			if (currentF < nextF) {
				canGoToNextPoint = typesGoingUp.contains(currentType);
				flChange = FLOOR_CHANGE::UP;
			}
			else if (currentF == nextF) {
				canGoToNextPoint = typesGoingSame.contains(currentType);
				flChange = FLOOR_CHANGE::SAME;
			}
			else if (currentF > nextF) {
				canGoToNextPoint = typesGoingDown.contains(currentType);
				flChange = FLOOR_CHANGE::DOWN;
			}
			if (canGoToNextPoint)
				continue;


			QString typeStr = pointTypeNameMap.value(currentType);
			QString argumentText = flChange == FLOOR_CHANGE::DOWN ? "down" : "up";
			QString errorText = QString("Can't find a way from point [%1] to point [%2], because type %3, can't move to %4 floor.")
				.arg(QString::number(i), QString::number(i + 1), typeStr, argumentText);

			throw std::exception(errorText.toStdString().c_str());
		}

		return true;
	}
	catch (const std::exception& e){
		errorTextToDisplay = e.what();
		return false;
	}
}
