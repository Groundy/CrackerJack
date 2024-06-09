#pragma once
#include <qdatetime.h>

#include <QDialog>

#include "AttackMethode.hpp"
#include "AutoHunting.h"
#include "Calibrator.h"
#include "ClickDetector.hpp"
#include "GameActivityChecker.h"
#include "Key.h"
#include "MinimapAnalyzer.h"
#include "Profile.h"
#include "RouteCreator.h"
#include "ScreenAnalyzer.h"
#include "ScreenSaver.h"
#include "SelectProfileWindow.h"
#include "VariablesClass.hpp"
#include "VitalityAnalyzer.h"

namespace Ui {
class MainMenu;
};
namespace CJ {
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
  void onGameStateChanged(GameActivityStates state);
  void changedValueOfCharHealthOrMana(double healthPercentage, double manaPercentage, double manaShieldPercentage);
  void printToUserConsol(QStringList msgs);
  void printToUserConsolRed(QString msg);
  void updatePlayerPosition(QString x, QString y, QString f);
  void updateEnemiesAmount(int enemies);
  void updateHeadingPoint(QString toDisplay);

 private:
  Ui::MainMenu* ui;

  //variables
  QSharedPointer<Profile>        prof_           = QSharedPointer<Profile>(new Profile());
  QSharedPointer<VariablesClass> var_            = QSharedPointer<VariablesClass>(new VariablesClass(prof_));
  QSharedPointer<GameConnecter>  game_connector_ = QSharedPointer<GameConnecter>(new GameConnecter(var_));
  GameActivityStates             game_activity_state_;

  //workers
  GameActivityChecker activity_checker_ = GameActivityChecker(var_);
  ScreenSaver         screen_saver_{this, var_, game_connector_};
  ScreenAnalyzer      screen_analyzer_{this, var_};
  VitalityAnalyzer    vitality_analyzer_{this, var_, game_connector_};
  //ClickDetector       click_detector_{this, game_connector_};
  MinimapAnalyzer mini_map_analyzer_{this, var_};
  //AutoHunting                             huntAutoThread{};

  void connectSignals();
  void startAutoHunting();
};
}  // namespace CJ
