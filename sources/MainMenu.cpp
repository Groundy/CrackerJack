#include "MainMenu.h"

#include "ui_MainMenu.h"
namespace CJ {
MainMenu::MainMenu(QSharedPointer<Profile> prof, QWidget* parent) : QDialog(parent) {
  ui = new Ui::MainMenu();
  ui->setupUi(this);

  prof_           = prof;
  var_            = QSharedPointer<VariablesClass>(new VariablesClass(prof));
  game_connector_ = QSharedPointer<GameConnecter>(new GameConnecter(var_));

  ui->profileNameLabel->setText(prof->getName());
  ui->playerPosGroup->setVisible(false);
  ui->resourceGroup->setVisible(false);
  auto settings = var_->getSettings();
  ui->keepHastedCheckBox->setChecked(settings->getKeepHasted());
  ui->keepUpgradedCheckBox->setChecked(settings->getKeepUpraded());

  screen_analyzer_.start();

  connectSignals();
}
MainMenu::~MainMenu() {
  QList<QThread*> threads{&screen_saver_, &screen_analyzer_, &vitality_analyzer_, &click_detector_};  //huntAutoThread
  foreach (QThread* thread, threads) {
    if (thread == nullptr) {
      continue;
    }
    thread->terminate();
  }
  delete ui;
}

//funcs
void MainMenu::connectSignals() {
  const auto exec_in_reciver_option = static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection);

  if (!connect(&Logger::instance(), &Logger::sendMsgToUserConsol, this, &MainMenu::printToUserConsol, exec_in_reciver_option)) {
    qCritical() << "Connection failed. Logger";
    exit(1);
  }

  if (!connect(&Logger::instance(), &Logger::sendMsgToUserConsolRed, this, &MainMenu::printToUserConsolRed, exec_in_reciver_option)) {
    qCritical() << "Connection failed. Logger";
    exit(1);
  }

  if (!connect(&vitality_analyzer_, &VitalityAnalyzer::sendValueToMainThread, this, &MainMenu::changedValueOfCharHealthOrMana,
               exec_in_reciver_option)) {
    qCritical() << "Connection failed. Health & Mana analyzer";
    exit(1);
  }

  if (!connect(&activity_checker_, &GameActivityChecker::gameStateChanged, this, &MainMenu::onGameStateChanged, exec_in_reciver_option)) {
    qCritical() << "Failed to connect thread signal of game activity";
    exit(1);
  }

  if (!connect(&screen_analyzer_, &ScreenAnalyzer::vitalityBarsReady, &vitality_analyzer_, &VitalityAnalyzer::execute,
               exec_in_reciver_option)) {
    qCritical() << "Failed to connect execute vitalityBarsAnalyzer";
    exit(1);
  }

  if (!connect(&screen_analyzer_, &ScreenAnalyzer::miniMapReady, &mini_map_analyzer_, &MinimapAnalyzer::execute, exec_in_reciver_option)) {
    qCritical() << "Failed to connect execute mini map ready";
    exit(1);
  }
  /*
  if (!connect(huntAutoThread, &AutoHunting::updateHeadingPointInGUI, this, &MainMenu::updateHeadingPoint, Qt::UniqueConnection)) {
    qCritical() << "Failed to connect thread signal. Auto hunt heading point";
    exit(1);
  }

  if (!connect(huntAutoThread, &AutoHunting::updateEnemiesAmountInGUI, this, &MainMenu::updateEnemiesAmount, Qt::UniqueConnection)) {
    qCritical() << "Failed to connect thread signal. Auto hunt enemies amount";
    exit(1);
  }
  */
  if (!connect(&mini_map_analyzer_, &MinimapAnalyzer::sendPostitionsToGUI, this, &MainMenu::updatePlayerPosition, Qt::UniqueConnection)) {
    qCritical() << "Failed to connect thread signal. player position";
    exit(0);
  }

  if (!connect(&mini_map_analyzer_, &MinimapAnalyzer::sendPostitionsToGUI, this, &MainMenu::updatePlayerPosition, Qt::UniqueConnection)) {
    qCritical() << "Failed to connect thread signal. player position";
    exit(0);
  }
}
void MainMenu::startAutoHunting() {
  /*
  var->getSettings().setKeepHuntingAutomaticly(true);
  var->getSettings().setKeepAnalyzeMiniMap(true);
  Route route;
  JsonParser::readRoute(route, "trolls");
  huntAutoThread = new AutoHunting(this, var, gameConnector, route);
  huntAutoThread->start();
  ui->playerPosGroup->setVisible(true);
  */
}

//slots
void MainMenu::changeProfileButtonAction() {
  this->accept();
}
void MainMenu::onGameStateChanged(GameActivityStates state) {
  QString toWrite = tr("Game status: ");
  switch (state) {
    case GameActivityStates::ACTIVE:
      toWrite += tr("Game active, Logged");
      break;
    case GameActivityStates::NO_ACTIVE:
      toWrite += tr("Game not found");
      break;
    case GameActivityStates::NO_HANDLER:
      toWrite += tr("Game found but no access");
      break;
    case GameActivityStates::NO_LOGGED:
      toWrite += tr("Game active but no char logged");
      break;
    case GameActivityStates::NO_WINDOW:
      toWrite += tr("Game is loading");
      break;
    default:
      toWrite += tr("Error");
      break;
  }
  ui->gameActiveLabel->setText(toWrite);
  ui->gameActiveLabel->repaint();
}

void MainMenu::checkBoxChanged() {
  auto     settings  = var_->getSettings();
  QObject* senderObj = sender();
  if (senderObj == ui->takeScreenshotCheckBox) {
    bool toSet = ui->takeScreenshotCheckBox->isChecked();
    settings->setTakingScreensState(toSet);
  }
  if (senderObj == ui->restoreHealthMana) {
    bool enable = ui->restoreHealthMana->isChecked();
    settings->setRestoringState(enable);
    if (enable) {
      ui->healthInfoLabel->clear();
      ui->manaInfoLabel->clear();
      ui->manaShieldLabel->clear();
    } else {
      ui->healthInfoLabel->setText("?");
      ui->manaInfoLabel->setText("?");
      ui->manaShieldLabel->setText("?");
    }
  }
  if (senderObj == ui->keepHastedCheckBox) {
    bool toSet = ui->keepHastedCheckBox->isChecked();
    settings->setKeepHasted(toSet);
  }
  if (senderObj == ui->keepUpgradedCheckBox) {
    bool toSet = ui->keepUpgradedCheckBox->isChecked();
    settings->setKeepUpraded(toSet);
  }
}
void MainMenu::changedValueOfCharHealthOrMana(double healthPercentage, double manaPercentage, double manaShieldPercentage) {
  if (healthPercentage != NULL) {
    QString healthStr = QString::number(healthPercentage, 'g', 3);
    QString msgToSet  = QString::fromWCharArray(L"\u017Bycie: %1%").arg(healthStr);
    ui->healthInfoLabel->setText(msgToSet);
  } else
    ui->healthInfoLabel->setText(QString::fromWCharArray(L"\u017Bycie: ?"));
  ui->healthInfoLabel->repaint();

  if (manaPercentage != NULL) {
    QString manaStr  = QString::number(manaPercentage, 'g', 3);
    QString msgToSet = QString("Mana: %1%").arg(manaStr);
    ui->manaInfoLabel->setText(msgToSet);
  } else
    ui->manaInfoLabel->setText("Mana: ?");
  ui->manaInfoLabel->repaint();

  if (manaShieldPercentage != NULL) {
    QString manaShieldStr = QString::number(manaShieldPercentage, 'g', 3);
    QString msgToSet      = QString("Tarcza: %1%").arg(manaShieldStr);
    ui->manaShieldLabel->setText(msgToSet);
  } else
    ui->manaShieldLabel->setText("Tarcza: ?");
  ui->manaShieldLabel->repaint();
}
void MainMenu::printToUserConsol(QStringList msgs) {
  ui->textBrowser->append(msgs[0]);
}
void MainMenu::printToUserConsolRed(QString msg) {
  ui->textBrowser->setTextColor(QColor::fromRgb(255, 0, 0));
  ui->textBrowser->append(msg);
  ui->textBrowser->setTextColor(QColor::fromRgb(0, 0, 0));
}
void MainMenu::updatePlayerPosition(QString x, QString y, QString f) {
  QString str = QString("%1, %2, %3").arg(x, y, f);
  ui->positonLabel->setText(str);
}
void MainMenu::updateEnemiesAmount(int enemies) {
  QString toSet = enemies >= 0 ? QString::number(enemies) : "?";
  ui->enemiesBattleLabel->setText(toSet);
}
void MainMenu::updateHeadingPoint(QString toDisplay) {
  ui->headinglabel->setText(toDisplay);
}
void MainMenu::testButtonClicked() {
  RouteCreator(this).exec();
}
void MainMenu::autoHuntButtonClicked() {
  /*
  bool threadAlreadyCreated = huntAutoThread != nullptr;
  if (!threadAlreadyCreated)
    startAutoHunting();
  else {
    bool enable = !var->getSettings().getKeepHuntingAutomaticly();
    var->getSettings().setKeepHuntingAutomaticly(enable);
    ui->playerPosGroup->setVisible(enable);
  }
  */
}
}  // namespace CJ
