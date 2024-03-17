#include "ScreenAnalyzer.h"

ScreenAnalyzer::ScreenAnalyzer(QObject* parent, QSharedPointer<VariablesClass> var) : QThread(parent), var(var) {
  auto screenshotDir = PathResource::getScreenShotFolder();
  if (!screenshotDir.has_value()) {
    qCritical() << "No screenshot directory";
    exit(0);
  }
  screenShotFolder = screenshotDir.value();
  var->getSettings().setLoadingState(true);
  deleteScreenShotFolder();
}

ScreenAnalyzer::~ScreenAnalyzer() {
  deleteScreenShotFolder();
  this->terminate();
}

void ScreenAnalyzer::run() {
  while (true) {
    msleep(SLEEP_TIME);
    if (!var->getSettings().getLoadingState()) {
      continue;
    }
    QImage img;
    bool   openCorrectly = loadScreen(img);
    if (!openCorrectly) {
      continue;
    }
    if (var->getVitalitty().needCalibration()) {
      bool basicCalibrationOk = Calibrator(var).calibrateBasicAreas(img);
      if (!basicCalibrationOk) {
        QString msg = "Problem z kalibracja!";
        logger.log(msg, true, true, false);
        deleteScreenShotFolder();
        continue;
      }
    }
    analyzeBattleList(img);
    deleteScreenShotFolder();
    cutHealthManaImgs(img);
    emit vitalityBarsCut();
    analyzeMiniMap(img);
    analyzeEquipment(img);
  }
}

bool ScreenAnalyzer::loadScreen(QImage& img) {
  QString nameOfImgToCapture = getNameOfLastTakenScreenShot();
  if (nameOfImgToCapture.isEmpty()) {
    return false;
  }
  QString pathToImg = screenShotFolder.absoluteFilePath(nameOfImgToCapture);
  if (!img.load(pathToImg)) {
    return false;
  }
  return !img.isNull();
}
QString ScreenAnalyzer::getNameOfLastTakenScreenShot() {
  QStringList filtr = QStringList() << "*_*_*_Hotkey.png";
  screenShotFolder.refresh();
  QStringList listOfFIlesNames = screenShotFolder.entryList(filtr, QDir::Files);
  if (listOfFIlesNames.isEmpty()) {
    return QString();
  }

  return listOfFIlesNames[0];
}

void ScreenAnalyzer::deleteScreenShotFolder() {
  QStringList filterPatter("*_*_*_Hotkey.png");  //yyyy-MM-dd_hhmmsszzzz_CharNick_Methode.png
  screenShotFolder.setNameFilters(filterPatter);
  screenShotFolder.setFilter(QDir::Files);

  QStringList namesOfFoundFiles = screenShotFolder.entryList();
  foreach (QString dirFile, namesOfFoundFiles) screenShotFolder.remove(dirFile);
}
int ScreenAnalyzer::cutHealthManaImgs(const QImage& fullscreen) {
  bool healthFrameFound = !var->getVitalitty().getHealthArea().isEmpty();
  bool manaFrameFound   = !var->getVitalitty().getManaArea().isEmpty();
  bool manaShieldFound  = !var->getVitalitty().getMSArea().isEmpty();
  bool combinedBoxFound = !var->getVitalitty().getCombinedArea().isEmpty();

  bool notEnoughFramesFound = !((manaFrameFound || combinedBoxFound) && healthFrameFound);
  if (notEnoughFramesFound) {
    return false;
  }

  const int ROTATION = var->getVitalitty().getRotation();
  if (healthFrameFound) {
    CJ_Image healthValueImg = fullscreen.copy(var->getVitalitty().getHealthArea());
    if (ROTATION != 0) {
      healthValueImg.rotateImgToRight(ROTATION);
    }
    var->getVitalitty().setImageHealth(healthValueImg);
  }
  if (manaFrameFound) {
    CJ_Image manaValueImg = fullscreen.copy(var->getVitalitty().getManaArea());
    if (ROTATION != 0) {
      manaValueImg.rotateImgToRight(ROTATION);
    }
    var->getVitalitty().setImageMana(manaValueImg);
  }
  if (manaShieldFound) {
    CJ_Image manaShieldValueImg = fullscreen.copy(var->getVitalitty().getMSArea());
    if (ROTATION != 0) {
      manaShieldValueImg.rotateImgToRight(ROTATION);
    }
    var->getVitalitty().setImageMS(manaShieldValueImg);
  }
  if (combinedBoxFound) {
    CJ_Image combinedValueImg = fullscreen.copy(var->getVitalitty().getCombinedArea());
    if (ROTATION != 0) {
      combinedValueImg.rotateImgToRight(ROTATION);
    }
    var->getVitalitty().setImageCombined(combinedValueImg);
  }
  return true;
}
void ScreenAnalyzer::cutBattleList(const QImage& fullscreen) {
  if (var->getSettings().getKeepAnalyzeMainGameWindow()) return;
  if (var->getBattleList().getFrame().isEmpty()) return;

  QRect frame = var->getBattleList().getFrame();
  var->getBattleList().setImg(fullscreen.copy(frame));
}
void ScreenAnalyzer::analyzeBattleList(const QImage& fullscreen) {
  if (!var->getSettings().getKeepHuntingAutomaticly()) return;
  if (var->getBattleList().getFrame().isEmpty()) {
    bool foundBattleArea = Calibrator(var).calibrateBattleArea(fullscreen);
    if (!foundBattleArea) {
      QString msg = "Nie mozna otworzy battle listy";
      logger.log(msg, true, true, false);
    }
  }
  cutBattleList(fullscreen);
}
void ScreenAnalyzer::analyzeMiniMap(const QImage& fullscreen) {
  if (!var->getSettings().getKeepAnalyzeMiniMap()) {
    return;
  }

  QRect frame = var->getMiniMap().getFrameMiniMap();
  var->getMiniMap().setImgMiniMap(fullscreen.copy(frame));
  const QPoint DIFF_DIST_SINCE_TOPLEFT_MINIMAP       = QPoint(137, 41);
  const QPoint TOP_LEFT_START_OF_MINIMAP_LAYER_FRAME = frame.topLeft() + DIFF_DIST_SINCE_TOPLEFT_MINIMAP;
  const QSize  SIZE_MINIMAP_LAYER_FRAME              = QSize(24, 71);
  QRect        miniMapLayerFrame(TOP_LEFT_START_OF_MINIMAP_LAYER_FRAME, SIZE_MINIMAP_LAYER_FRAME);
  var->getMiniMap().setImgMiniMapLayer(fullscreen.copy(miniMapLayerFrame));
}
void ScreenAnalyzer::analyzeEquipment(const QImage& fullscreen) {
  bool needCalibration = var->getEquipment().getStoreRect().isEmpty();
  if (needCalibration) {
    Calibrator(var).calibrateStoreButton(fullscreen);
  }
  if (var->getSettings().getKeepAnalyzeStates()) {
    QRect stateBarRect = var->getEquipment().getRect(Equipment::EqRect::StateBar);
    var->getEquipment().setImg(Equipment::EqRect::StateBar, fullscreen.copy(stateBarRect));
  }
}
