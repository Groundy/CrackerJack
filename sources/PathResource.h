#pragma once
#include <qstring.h>
class PathResource{
public:
	static QString getPathToProfileFolder() {
		return "C:\\Moje\\pliki\\repos\\CrackerJackClient\\Profiles\\";
	}
	static QString getPathToProfileFile(const QString profileName) {
		return getPathToProfileFolder() + profileName + ".json";
	}
	static QString getPathToRouteFolder() {
		return "C:\\Moje\\pliki\\repos\\CrackerJackClient\\Routes\\";
	}
	static QString getPathToRouteFile(const QString routeNameWithoutExtension) {
		return QString("%1%2.json").arg(getPathToRouteFolder(), routeNameWithoutExtension);
	}

	//jsons
	static QString getPathToSpellJsonFile() {return ":/jsons/spells";}
	static QString getPathToItemJsonFile() {return ":/jsons/items";}
	static QString getPathToPotionsJsonFile() {return ":/jsons/potions";}
	static QString getPathToKeysJsonFile() {return ":/jsons/keys";}
	//static QString getPathToFileWithBottomsBarDigitsCodes() {return ":/jsons/bottomBarsDigits.json";}
	static QString getPathToRunesFile() {return ":/jsons/runes.json";}
	//imgs
	static QString getPathToMiniMapSliderImg() {return ":/imgs/mapLayer";}
	static QString getPathToSideBarEntityStart() {return ":/imgs/sideBarEntityStart";}
	static QString getPathToSideBarEntityEnd() {return ":/imgs/sideBarEntityEnd";}
	static QString getPathToBattleList() {return ":/imgs/BattleList";}
	static QString getPathToStoreButton() { return ":/imgs/storeButton"; }
	static QString getPathToMap(int floor, bool walkable = true) {
		QString floorStr = QString::number(floor);
		QString type = walkable ? "Color" : "WaypointCost";
		return QString(":/maps/%1_%2").arg(floorStr, type);
	}
private:
};

