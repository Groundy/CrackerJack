#pragma once
#include <qdir.h>
#include <qstring.h>
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
    try {
      QDir dir = QDir::tempPath();
      if (!dir.cdUp()) throw std::exception("Error in setting up screenshot folder");
      if (!dir.cd("Tibia"))
        throw std::exception(QString("Error in finding screenshot folder, No Tibia folder in : %1").arg(dir.path()).toStdString().c_str());
      if (!dir.cd("packages"))
        throw std::exception(
            QString("Error in finding screenshot folder, No packages folder in : %1").arg(dir.path()).toStdString().c_str());
      if (!dir.cd("Tibia"))
        throw std::exception(QString("Error in finding screenshot folder, No Tibia folder in : %1").arg(dir.path()).toStdString().c_str());
      if (!dir.cd("screenshots"))
        throw std::exception(
            QString("Error in finding screenshot folder, No screenshots folder in : %1").arg(dir.path()).toStdString().c_str());
      return dir;
    } catch (const std::exception& e) {
      qDebug() << e.what();
      return QDir();
    }
  }
  static QString getPathToRouteFile(const QString routeNameWithoutExtension) {
    return getRouteFolder().absoluteFilePath(routeNameWithoutExtension + ".json");
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
