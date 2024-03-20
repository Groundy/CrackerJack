#pragma once
#include <QThread.h>
#include <qdatetime.h>
#include <qdebug.h>
#include <qdir.h>
#include <qimage.h>
#include <qmap.h>
#include <qtimer.h>

#include "Equipment.hpp"
#include "Profile.h"
#include "RGBstruct.h"
#include "ScreenAnalyzer.h"
#include "VariablesClass.hpp"
class ScreenAnalyzer : public QThread {
  Q_OBJECT

 public:
  ScreenAnalyzer(QObject* parent, QSharedPointer<VariablesClass> var);
  ~ScreenAnalyzer();

 signals:
  void vitalityBarsReady();
  void miniMapReady();

 private:
  const int                      SLEEP_TIME = 50;
  QSharedPointer<VariablesClass> var_;
  QDir                           screenShotFolder;
  Logger&                        logger = Logger::instance();
  QTimer                         screenAnalyzerTimer;
  Calibrator                     calibrator_{var_};

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
