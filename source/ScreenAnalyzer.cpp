#include "ScreenAnalyzer.h"
namespace CJ {
ScreenAnalyzer::ScreenAnalyzer(QObject* parent, QSharedPointer<VariablesClass> var) : QThread(parent), calibrator_(Calibrator(var)) {
  settings_    = var->getSettings();
  battle_list_ = var->getBattleList();
  equipment_   = var->getEquipment();
  vitalitty_   = var->getVitalitty();
  minimap_     = var->getMiniMap();

  screenshot_dir_ = PathResource::getScreenShotFolder();
  settings_->setLoadingState(true);
  deleteScreenShotFolder();

  screenAnalyzerTimer_.setInterval(sleep_time_);
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
  if (!settings_->getLoadingState()) {
    return;
  }
  QImage img;
  bool   openCorrectly = loadScreen(img);
  if (!openCorrectly) {
    return;
  }
  if (vitalitty_->needCalibration()) {
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
  QString pathToImg = screenshot_dir_.absoluteFilePath(nameOfImgToCapture);
  if (!img.load(pathToImg)) {
    return false;
  }
  return !img.isNull();
}

QString ScreenAnalyzer::getNameOfLastTakenScreenShot() {
  QStringList filtr = QStringList() << "*_*_*_Hotkey.png";
  screenshot_dir_.refresh();
  QStringList listOfFIlesNames = screenshot_dir_.entryList(filtr, QDir::Files);
  if (listOfFIlesNames.isEmpty()) {
    return QString();
  }

  return listOfFIlesNames[0];
}

void ScreenAnalyzer::deleteScreenShotFolder() {
  QStringList filterPatter("*_*_*_Hotkey.png");  //yyyy-MM-dd_hhmmsszzzz_CharNick_Methode.png
  screenshot_dir_.setNameFilters(filterPatter);
  screenshot_dir_.setFilter(QDir::Files);

  QStringList namesOfFoundFiles = screenshot_dir_.entryList();
  foreach (QString dirFile, namesOfFoundFiles) {
    screenshot_dir_.remove(dirFile);
  }
}
int ScreenAnalyzer::cutHealthManaImgs(const QImage& fullscreen) {
  const bool healthFrameFound = !vitalitty_->getHealthArea().isEmpty();
  const bool manaFrameFound   = !vitalitty_->getManaArea().isEmpty();
  const bool manaShieldFound  = !vitalitty_->getMSArea().isEmpty();
  const bool combinedBoxFound = !vitalitty_->getCombinedArea().isEmpty();

  bool notEnoughFramesFound = !((manaFrameFound || combinedBoxFound) && healthFrameFound);
  if (notEnoughFramesFound) {
    return false;
  }

  const int ROTATION = vitalitty_->getRotation();
  if (healthFrameFound) {
    CJ_Image healthValueImg = fullscreen.copy(vitalitty_->getHealthArea());
    if (ROTATION != 0) {
      healthValueImg.rotateImgToRight(ROTATION);
    }
    vitalitty_->setImageHealth(healthValueImg);
  }
  if (manaFrameFound) {
    CJ_Image manaValueImg = fullscreen.copy(vitalitty_->getManaArea());
    if (ROTATION != 0) {
      manaValueImg.rotateImgToRight(ROTATION);
    }
    vitalitty_->setImageMana(manaValueImg);
  }
  if (manaShieldFound) {
    CJ_Image manaShieldValueImg = fullscreen.copy(vitalitty_->getMSArea());
    if (ROTATION != 0) {
      manaShieldValueImg.rotateImgToRight(ROTATION);
    }
    vitalitty_->setImageMS(manaShieldValueImg);
  }
  if (combinedBoxFound) {
    CJ_Image combinedValueImg = fullscreen.copy(vitalitty_->getCombinedArea());
    if (ROTATION != 0) {
      combinedValueImg.rotateImgToRight(ROTATION);
    }
    vitalitty_->setImageCombined(combinedValueImg);
  }
  return true;
}
void ScreenAnalyzer::cutBattleList(const QImage& fullscreen) {
  if (settings_->getKeepAnalyzeMainGameWindow()) {
    return;
  }
  if (battle_list_->getFrame().isEmpty()) {
    return;
  }

  QRect frame = battle_list_->getFrame();
  battle_list_->setImg(fullscreen.copy(frame));
}
void ScreenAnalyzer::analyzeBattleList(const QImage& fullscreen) {
  if (!settings_->getKeepHuntingAutomaticly()) {
    return;
  }
  if (battle_list_->getFrame().isEmpty()) {
    bool foundBattleArea = calibrator_.calibrateBattleArea(fullscreen);
    if (!foundBattleArea) {
      QString msg = "Nie mozna otworzy battle listy";
      logger_.log(msg, true, true, false);
    }
  }
  cutBattleList(fullscreen);
}
void ScreenAnalyzer::analyzeMiniMap(const QImage& fullscreen) {
  if (!settings_->getKeepAnalyzeMiniMap()) {
    return;
  }

  QRect frame = minimap_->getFrameMiniMap();
  minimap_->setImgMiniMap(fullscreen.copy(frame));
  constexpr QPoint DIFF_DIST_SINCE_TOPLEFT_MINIMAP       = QPoint(137, 41);
  const QPoint     TOP_LEFT_START_OF_MINIMAP_LAYER_FRAME = frame.topLeft() + DIFF_DIST_SINCE_TOPLEFT_MINIMAP;
  constexpr QSize  SIZE_MINIMAP_LAYER_FRAME              = QSize(24, 71);
  const QRect      miniMapLayerFrame(TOP_LEFT_START_OF_MINIMAP_LAYER_FRAME, SIZE_MINIMAP_LAYER_FRAME);
  minimap_->setImgMiniMapLayer(fullscreen.copy(miniMapLayerFrame));
}

void ScreenAnalyzer::analyzeEquipment(const QImage& fullscreen) {
  bool needCalibration = equipment_->getStoreRect().isEmpty();
  if (needCalibration) {
    calibrator_.calibrateStoreButton(fullscreen);
  }
  if (settings_->getKeepAnalyzeStates()) {
    QRect stateBarRect = equipment_->getRect(EqRect::StateBar);
    equipment_->setImg(EqRect::StateBar, fullscreen.copy(stateBarRect));
  }
}
}  // namespace CJ
