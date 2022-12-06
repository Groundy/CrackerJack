#pragma once
#include <qstring.h>
class PathResource{
public:
	static QString getPathToMergedColorMap(const int floor) {
		const QString PATH_TO_FOLDER = "C:\\Moje\\pliki\\repos\\CrackerJackClient\\ResourcesUsing\\maps";
		return QString("%1\\%2_Color.png").arg(PATH_TO_FOLDER, QString::number(floor));
	}
	static QString getPathToMergedWalkableMap(const int floor) {
		const QString PATH_TO_FOLDER = "C:\\Moje\\pliki\\repos\\CrackerJackClient\\ResourcesUsing\\maps";
		return QString("%1\\%2_WaypointCost.png").arg(PATH_TO_FOLDER, QString::number(floor));
	}
	static QString getPathToItemJsonFile() {
		return "C:\\Moje\\pliki\\repos\\CrackerJackClient\\ResourcesUsing\\items.json";
	}
	static QString getPathToSpellJsonFile() {
		return "C:\\Moje\\pliki\\repos\\CrackerJackClient\\ResourcesUsing\\spells.json";
	}
	static QString getPathToPotionsJsonFile() {
		return "C:\\Moje\\pliki\\repos\\CrackerJackClient\\ResourcesUsing\\potions.json";
	}
	static QString getPathToKeysJsonFile() {
		return "C:\\Moje\\pliki\\repos\\CrackerJackClient\\ResourcesUsing\\keys.json";
	}
	static QString getPathToProfileFolder() {
		return "C:\\Moje\\pliki\\repos\\CrackerJackClient\\Profiles\\";
	}
	static QString getPathToProfileFile(const QString profileName) {
		return getPathToProfileFolder() + profileName + ".json";
	}
	static QString getPathToMiniMapSliderImg() {
		return getPathToImgsFolder() + "mapLayer.png";
	}
	static QString getPathToSideBarEntityStart() {
		return getPathToImgsFolder() + "sideBarEntityStart.png";
	}
	static QString getPathToSideBarEntityEnd() {
		return getPathToImgsFolder() + "sideBarEntityEnd.png";
	}
	static QString getPathToBattleList() {
		return getPathToImgsFolder() + "BattleList.png";
	}
	static QString getPathToFileWithBottomsBarDigitsCodes() {
		return "C:\\Moje\\pliki\\repos\\CrackerJackClient\\ResourcesUsing\\bottomBarsDigits.json";
	}
	static QString getPathToRouteFolder() {
		return "C:\\Moje\\pliki\\repos\\CrackerJackClient\\Routes\\";
	}
	static QString getPathToRouteFile(const QString routeNameWithoutExtension) {
		return QString("%1%2.json").arg(getPathToRouteFolder(), routeNameWithoutExtension);
	}
	static QString getPathToRunesFile() {
		return "C:\\Moje\\pliki\\repos\\CrackerJackClient\\ResourcesUsing\\runes.json";
	}
private:
	static QString getPathToImgsFolder() {
		return "C:\\Moje\\pliki\\repos\\CrackerJackClient\\ResourcesUsing\\imgs\\";
	}
};

