#include "ScreenAnalyzer.h"
ScreenAnalyzer::ScreenAnalyzer(QObject* parent, QSharedPointer<VariablesClass> var)
    : QThread(parent), var_(var), calibrator_(Calibrator(var)) {
  auto screenshotDir = PathResource::getScreenShotFolder();
  if (screenshotDir.has_value()) {
    screenShotFolder_ = screenshotDir.value();
  } else {
    qCritical() << "No screenshot directory";
    exit(1);
  }
  var_->getSettings().setLoadingState(true);
  deleteScreenShotFolder();

  screenAnalyzerTimer_.setInterval(SLEEP_TIME_);
  if (!connect(&screenAnalyzerTimer_, &QTimer::timeout, this, &ScreenAnalyzer::execute)) {
    qCritical() << "Failed to connect ScreenAnalyzer timer";
    exit(1);
  }
  screenAnalyzerTimer_.start();
}

ScreenAnalyzer::~ScreenAnalyzer() {
  screenAnalyzerTimer_.stop();
  deleteScreenShotFolder();
  this->terminate();
}

void ScreenAnalyzer::execute() {
  if (!var_->getSettings().getLoadingState()) {
    return;
  }
  QImage img;
  bool   openCorrectly = loadScreen(img);
  if (!openCorrectly) {
    return;
  }
  if (var_->getVitalitty().needCalibration()) {
    bool basicCalibrationOk = calibrator_.calibrateBasicAreas(img);
    if (!basicCalibrationOk) {
      QString msg = "Problem z kalibracja!";
      logger_.log(msg, true, true, false);
      deleteScreenShotFolder();
      return;
    }
  }
  analyzeBattleList(img);
  deleteScreenShotFolder();

  cutHealthManaImgs(img);
  emit vitalityBarsReady();

  analyzeMiniMap(img);
  emit miniMapReady();

  analyzeEquipment(img);
}

bool ScreenAnalyzer::loadScreen(QImage& img) {
  QString nameOfImgToCapture = getNameOfLastTakenScreenShot();
  if (nameOfImgToCapture.isEmpty()) {
    return false;
  }
  QString pathToImg = screenShotFolder_.absoluteFilePath(nameOfImgToCapture);
  if (!img.load(pathToImg)) {
    return false;
  }
  return !img.isNull();
}

QString ScreenAnalyzer::getNameOfLastTakenScreenShot() {
  QStringList filtr = QStringList() << "*_*_*_Hotkey.png";
  screenShotFolder_.refresh();
  QStringList listOfFIlesNames = screenShotFolder_.entryList(filtr, QDir::Files);
  if (listOfFIlesNames.isEmpty()) {
    return QString();
  }

  return listOfFIlesNames[0];
}

void ScreenAnalyzer::deleteScreenShotFolder() {
  QStringList filterPatter("*_*_*_Hotkey.png");  //yyyy-MM-dd_hhmmsszzzz_CharNick_Methode.png
  screenShotFolder_.setNameFilters(filterPatter);
  screenShotFolder_.setFilter(QDir::Files);

  QStringList namesOfFoundFiles = screenShotFolder_.entryList();
  foreach (QString dirFile, namesOfFoundFiles) {
    screenShotFolder_.remove(dirFile);
  }
}
int ScreenAnalyzer::cutHealthManaImgs(const QImage& fullscreen) {
  Vitallity& vitality         = var_->getVitalitty();
  bool       healthFrameFound = !vitality.getHealthArea().isEmpty();
  bool       manaFrameFound   = !vitality.getManaArea().isEmpty();
  bool       manaShieldFound  = !vitality.getMSArea().isEmpty();
  bool       combinedBoxFound = !vitality.getCombinedArea().isEmpty();

  bool notEnoughFramesFound = !((manaFrameFound || combinedBoxFound) && healthFrameFound);
  if (notEnoughFramesFound) {
    return false;
  }

  const int ROTATION = vitality.getRotation();
  if (healthFrameFound) {
    CJ_Image healthValueImg = fullscreen.copy(vitality.getHealthArea());
    if (ROTATION != 0) {
      healthValueImg.rotateImgToRight(ROTATION);
    }
    vitality.setImageHealth(healthValueImg);
  }
  if (manaFrameFound) {
    CJ_Image manaValueImg = fullscreen.copy(vitality.getManaArea());
    if (ROTATION != 0) {
      manaValueImg.rotateImgToRight(ROTATION);
    }
    vitality.setImageMana(manaValueImg);
  }
  if (manaShieldFound) {
    CJ_Image manaShieldValueImg = fullscreen.copy(vitality.getMSArea());
    if (ROTATION != 0) {
      manaShieldValueImg.rotateImgToRight(ROTATION);
    }
    vitality.setImageMS(manaShieldValueImg);
  }
  if (combinedBoxFound) {
    CJ_Image combinedValueImg = fullscreen.copy(vitality.getCombinedArea());
    if (ROTATION != 0) {
      combinedValueImg.rotateImgToRight(ROTATION);
    }
    vitality.setImageCombined(combinedValueImg);
  }
  return true;
}
void ScreenAnalyzer::cutBattleList(const QImage& fullscreen) {
  if (var_->getSettings().getKeepAnalyzeMainGameWindow()) {
    return;
  }
  if (var_->getBattleList().getFrame().isEmpty()) {
    return;
  }

  QRect frame = var_->getBattleList().getFrame();
  var_->getBattleList().setImg(fullscreen.copy(frame));
}
void ScreenAnalyzer::analyzeBattleList(const QImage& fullscreen) {
  if (!var_->getSettings().getKeepHuntingAutomaticly()) return;
  if (var_->getBattleList().getFrame().isEmpty()) {
    bool foundBattleArea = calibrator_.calibrateBattleArea(fullscreen);
    if (!foundBattleArea) {
      QString msg = "Nie mozna otworzy battle listy";
      logger_.log(msg, true, true, false);
    }
  }
  cutBattleList(fullscreen);
}
void ScreenAnalyzer::analyzeMiniMap(const QImage& fullscreen) {
  if (!var_->getSettings().getKeepAnalyzeMiniMap()) {
    return;
  }

  QRect frame = var_->getMiniMap().getFrameMiniMap();
  var_->getMiniMap().setImgMiniMap(fullscreen.copy(frame));
  constexpr QPoint DIFF_DIST_SINCE_TOPLEFT_MINIMAP       = QPoint(137, 41);
  const QPoint     TOP_LEFT_START_OF_MINIMAP_LAYER_FRAME = frame.topLeft() + DIFF_DIST_SINCE_TOPLEFT_MINIMAP;
  constexpr QSize  SIZE_MINIMAP_LAYER_FRAME              = QSize(24, 71);
  QRect            miniMapLayerFrame(TOP_LEFT_START_OF_MINIMAP_LAYER_FRAME, SIZE_MINIMAP_LAYER_FRAME);
  var_->getMiniMap().setImgMiniMapLayer(fullscreen.copy(miniMapLayerFrame));
}
void ScreenAnalyzer::analyzeEquipment(const QImage& fullscreen) {
  bool needCalibration = var_->getEquipment().getStoreRect().isEmpty();
  if (needCalibration) {
    calibrator_.calibrateStoreButton(fullscreen);
  }
  if (var_->getSettings().getKeepAnalyzeStates()) {
    QRect stateBarRect = var_->getEquipment().getRect(Equipment::EqRect::StateBar);
    var_->getEquipment().setImg(Equipment::EqRect::StateBar, fullscreen.copy(stateBarRect));
  }
}
