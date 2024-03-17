#pragma once
#include <QDialog.h>
#include <qdatetime.h>

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
  MainMenu(QSharedPointer<Profile> prof, QWidget* parent = Q_NULLPTR);
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
  Ui::MainMenu*                  ui;
  QSharedPointer<Profile>        prof           = QSharedPointer<Profile>(new Profile());
  QSharedPointer<VariablesClass> var            = QSharedPointer<VariablesClass>(new VariablesClass(prof));
  QSharedPointer<GameConnecter>  gameConnector  = QSharedPointer<GameConnecter>(new GameConnecter(var));
  ActiveGameThread               activityThread = ActiveGameThread(var);  //Change name of this class and variable

  ScreenSaver             screenSaverThread{this, var, gameConnector};
  ScreenAnalyzer          screenAnalyzer{this, var};
  ManaHealthStateAnalyzer healthManaStateAnalyzer{this, var, gameConnector};
  ClickDetector           clickDetector{this, gameConnector};
  AutoHunting*            huntAutoThread = nullptr;

  ActiveGameThread::GameActivityStates gameActivitystate;

  void threadStarter();
  void startAutoHunting();
};
