#pragma once
#include <QThread.h>
#include <qdatetime.h>
#include <qdebug.h>
#include <qdir.h>
#include <qimage.h>
#include <qmap.h>
#include <qtimer.h>

#include "Calibrator.h"
#include "Equipment.h"
#include "Profile.h"
#include "RGBstruct.h"
#include "ScreenAnalyzer.h"
#include "Settings.hpp"
#include "VariablesClass.hpp"
namespace CJ {
class ScreenAnalyzer : public QThread {
  Q_OBJECT

 public:
  ScreenAnalyzer(QObject* parent, QSharedPointer<VariablesClass> var);
  ~ScreenAnalyzer();

 signals:
  void vitalityBarsReady();
  void miniMapReady();

 private:
  const int  sleep_time_ = 50;
  QDir       screenshot_dir_;
  Logger&    logger_ = Logger::instance();
  QTimer     screenAnalyzerTimer_;
  Calibrator calibrator_;

  QSharedPointer<Settings>   settings_;
  QSharedPointer<BattleList> battle_list_;
  QSharedPointer<Equipment>  equipment_;
  QSharedPointer<Vitallity>  vitalitty_;
  QSharedPointer<MiniMap>    minimap_;

  void    deleteScreenShotFolder();
  QString getNameOfLastTakenScreenShot();
  bool    loadScreen(QImage& img);
  int     cutHealthManaImgs(const QImage& fullscreen);
  void    cutBattleList(const QImage& fullscreen);
  void    analyzeBattleList(const QImage& fullscreen);
  void    analyzeMiniMap(const QImage& fullscreen);
  void    analyzeEquipment(const QImage& fullscreen);
  void    execute();
};
}  // namespace CJ
