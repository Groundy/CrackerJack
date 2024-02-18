#pragma once
#include <qdatetime.h>
#include <qsound.h>

#include <QDialog>

#include "AttackMethode.hpp"
#include "AutoHunting.h"
#include "Calibrator.h"
#include "ClickDetector.hpp"
#include "Key.h"
#include "ManaHealthStateAnalyzer.h"
#include "Profile.h"
#include "RouteCreator.h"
#include "ScreenAnalyzer.h"
#include "ScreenSaver.h"
#include "SelectProfileWindow.h"
#include "VariablesClass.hpp"
#include "activeGameThread.h"
namespace Ui {
class MainMenu;
};

class MainMenu : public QDialog {
  Q_OBJECT
 public:
  MainMenu(Profile* prof, QWidget* parent = Q_NULLPTR);
  ~MainMenu();
 public slots:
  void checkBoxChanged();
  void changeProfileButtonAction();
  void testButtonClicked();
  void autoHuntButtonClicked();

  //from threads
  void onGameStateChanged(int state);
  void changedValueOfCharHealthOrMana(double healthPercentage, double manaPercentage, double manaShieldPercentage);
  void printToUserConsol(QStringList msgs);
  void printToUserConsolRed(QString msg);
  void updatePlayerPosition(QString x, QString y, QString f);
  void updateEnemiesAmount(int enemies);
  void updateHeadingPoint(QString toDisplay);

 private:
  Ui::MainMenu*                        ui;
  Profile*                             prof;
  ActiveGameThread*                    activityThread;
  ScreenSaver*                         screenSaverThread;
  ScreenAnalyzer*                      screenAnalyzer;
  ManaHealthStateAnalyzer*             healthManaStateAnalyzer;
  ClickDetector*                       clickDetector;
  AutoHunting*                         huntAutoThread = nullptr;
  QSharedPointer<VariablesClass>       var;
  QSharedPointer<GameConnecter>        gameConnector;
  ActiveGameThread::GameActivityStates gameActivitystate;
  QSound*                              sound = new QSound(":/sounds/testSound", this);
  void                                 threadStarter();
  void                                 startAutoHunting();
  void                                 signalSlotConnector();
};
