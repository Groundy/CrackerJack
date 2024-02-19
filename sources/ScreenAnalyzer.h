#pragma once
#include <QThread.h>
#include <qdatetime.h>
#include <qdebug.h>
#include <qdir.h>
#include <qimage.h>
#include <qmap.h>

#include "Calibrator.h"
#include "Equipment.hpp"
#include "Profile.h"
#include "RGBstruct.h"
#include "ScreenAnalyzer.h"
#include "VariablesClass.hpp"
class ScreenAnalyzer : public QThread {
  Q_OBJECT

 public:
  ScreenAnalyzer(QObject* parent, QSharedPointer<VariablesClass> var, Profile* prof);
  ~ScreenAnalyzer();
  void run();

 private:
  const int                      SLEEP_TIME = 50;
  QSharedPointer<VariablesClass> var;
  QDir                           screenShotFolder;
  Profile*                       profile;

  void    deleteScreenShotFolder();
  QString getNameOfLastTakenScreenShot();
  bool    loadScreen(QImage& img);
  int     cutHealthManaImgs(const QImage& fullscreen);
  void    cutBattleList(const QImage& fullscreen);
  void    analyzeBattleList(const QImage& fullscreen);
  void    analyzeMiniMap(const QImage& fullscreen);
  void    analyzeEquipment(const QImage& fullscreen);
};
