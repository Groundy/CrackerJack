#pragma once
#include <qdebug.h>
#include <qdir.h>
#include <qstring.h>

#include <optional>
namespace CJ {
class PathResource {
 public:
  static QDir getProfileFolder() {
    const QString dirName = "Profiles";

    QDir mainDir = get_local_dir();
    if (!mainDir.exists(dirName)) {
      if (!mainDir.mkdir(dirName)) {
        qWarning() << "Creating" << dirName << "directory failed.";
        return mainDir.path();
      }
    }
    mainDir.cd(dirName);
    return mainDir;
  }
  static QString getPathToProfileFile(const QString profileName) {
    return getProfileFolder().absoluteFilePath(profileName + ".json");
  }

  static QDir getRouteFolder() {
    const QString dirName = "Routes";

    QDir mainDir = get_local_dir();
    if (!mainDir.exists(dirName)) {
      if (!mainDir.mkdir(dirName)) {
        qWarning() << "Creating" << dirName << "directory failed.";
        return mainDir;
      }
    }
    mainDir.cd(dirName);
    return mainDir;
  }
  static QDir getScreenShotFolder() {
    QDir dir = QDir::tempPath();
    dir.cdUp();
    const bool moved = dir.cd("Tibia") && dir.cd("packages") && dir.cd("Tibia") && dir.cd("screenshots");

    if (!moved) {
      qCritical() << "Error in finding screenshot folder, current dir" << dir.absolutePath();
      exit(1);
    }
    return dir;
  }

  static QString getPathToRouteFile(const QString routeNameWithoutExtension) {
    return getRouteFolder().absoluteFilePath(routeNameWithoutExtension + ".json");
  }

  static QString getPathToTmpSavingImgsDir() {
    QDir dir = QDir::current();
    dir.cd("tmp_screens");
    return dir.absolutePath() + "\\";
  }
  static QString getPathToTmpJsonDir() {
    QDir dir = QDir::current();
    dir.cd("tmp_jsons");
    return dir.absolutePath() + "\\";
  }

  //jsons
  static QString getPathToSpellJsonFile() {
    return ":/jsons/spells";
  }
  static QString getPathToItemJsonFile() {
    return ":/jsons/items";
  }
  static QString getPathToPotionsJsonFile() {
    return ":/jsons/potions";
  }
  static QString getPathToKeysJsonFile() {
    return ":/jsons/keys";
  }
  //static QString getPathToFileWithBottomsBarDigitsCodes() {return ":/jsons/bottomBarsDigits.json";}
  static QString getPathToRunesFile() {
    return ":/jsons/runes.json";
  }

  //imgs
  static QString getPathToMiniMapSliderImg() {
    return ":/imgs/mapLayer";
  }
  static QString getPathToSideBarEntityStart() {
    return ":/imgs/sideBarEntityStart";
  }
  static QString getPathToSideBarEntityEnd() {
    return ":/imgs/sideBarEntityEnd";
  }
  static QString getPathToBattleList() {
    return ":/imgs/BattleList";
  }
  static QString getPathToStoreButton() {
    return ":/imgs/storeButton";
  }
  static QString getPathToMap(int floor, bool walkable = true) {
    QString floorStr = QString::number(floor);
    QString type     = walkable ? "Color" : "WaypointCost";
    return QString(":/maps/%1_%2").arg(floorStr, type);
  }

 private:
  static QDir get_local_dir() {
    QDir tmpDir = QDir::temp();
    tmpDir.cdUp();
    const QString mainFolderName = "CrackerJack";  //TODO
    if (!tmpDir.exists(mainFolderName)) {
      if (!tmpDir.mkdir(mainFolderName)) {
        qWarning() << "Creating main directory failed.";
        return QDir::current();
      }
    }
    tmpDir.cd(mainFolderName);
    return tmpDir;
  }
};
}  // namespace CJ
