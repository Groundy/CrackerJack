#include "MainMenu.h"
#include "ui_MainMenu.h"

//const
MainMenu::MainMenu(Profile* prof, QWidget* parent)
	: QDialog(parent), prof(prof){
	ui = new Ui::MainMenu();
	ui->setupUi(this);

	var = std::shared_ptr<VariablesClass>(new VariablesClass());
	gameConnector = std::shared_ptr<GameConnecter>(new GameConnecter(this, var, prof));
	ui->profileNameLabel->setText(prof->getName());
	
	ui->playerPosGroup->setVisible(false);
	ui->resourceGroup->setVisible(false);

	threadStarter();
	signalSlotConnector();
}

MainMenu::~MainMenu(){
	QList<QThread*> threads;
	threads.push_back(activityThread);
	threads.push_back(screenSaverThread);
	threads.push_back(screenAnalyzer);
	threads.push_back(healthManaStateAnalyzer);
	threads.push_back(miniMapAnalyzer);
	threads.push_back(huntAutoThread);
	for each (QThread* thread in threads){
		thread->terminate();
		delete thread;
	}

	delete ui;
}


//funcs
void MainMenu::threadStarter(){
	activityThread = new ActiveGameThread(this, var);
	activityThread->start();

	screenSaverThread = new ScreenSaver(this, var, gameConnector, prof);
	screenSaverThread->start();
	
	screenAnalyzer = new ScreenAnalyzer(this, var, prof);
	screenAnalyzer->start();

	healthManaStateAnalyzer = new ManaHealthStateAnalyzer(this, prof , var, gameConnector);
	healthManaStateAnalyzer->start();

	miniMapAnalyzer = new MinimapAnalyzer(this, var);
	miniMapAnalyzer->start();
}
void MainMenu::signalSlotConnector(){
	QObject *sigSender, *slotRec;
	const char *sig, *slot;
	bool allSlotsConnected = true;

	sigSender = healthManaStateAnalyzer;
	slotRec = this;
	sig = SIGNAL(sendValueToMainThread(double, double, double));
	slot = SLOT(changedValueOfCharHealthOrMana(double, double, double));
	bool connectionAccepted = connect(sigSender, sig, slotRec, slot, Qt::UniqueConnection);
	allSlotsConnected = allSlotsConnected && connectionAccepted;
	
	sigSender = &this->var->logger;
	slotRec = this;
	sig = SIGNAL(sendMsgToUserConsol(QStringList));
	slot = SLOT(printToUserConsol(QStringList));
	connectionAccepted = connect(sigSender, sig, slotRec, slot, Qt::UniqueConnection);
	allSlotsConnected = allSlotsConnected && connectionAccepted;

	sigSender = this->miniMapAnalyzer;
	slotRec = this;
	sig = SIGNAL(sendPostitionsToGUI(QString, QString, QString));
	slot = SLOT(updatePlayerPosition(QString, QString, QString));
	connectionAccepted = connect(sigSender, sig, slotRec, slot, Qt::UniqueConnection);
	allSlotsConnected = allSlotsConnected && connectionAccepted;
}
void MainMenu::startAutoHunting() {
	Route route;
	JsonParser::readRoute(route, "Lethe2");
	QVector<AttackMethode> attackMethodes = {
		AttackMethode(2, 2, 6, "chuj", Key("F3"))
	};
	huntAutoThread = new AutoHunting(this, var, gameConnector, route, attackMethodes);
	huntAutoThread->start();
	ui->playerPosGroup->setVisible(true);


	const char* sig = SIGNAL(updateHeadingPointInGUI(QString));
	const char* slot = SLOT(updateHeadingPoint(QString));
	bool connectionAccepted = connect(huntAutoThread, sig, this, slot, Qt::UniqueConnection);
	sig = SIGNAL(updateEnemiesAmountInGUI(int));
	slot = SLOT(updateEnemiesAmount(int));
	connectionAccepted = connect(huntAutoThread, sig, this, slot, Qt::UniqueConnection);
}

//slots
void MainMenu::changeProfileButtonAction(){
	this->accept();
}
void MainMenu::takeScreenShotCheckBoxChanged() {
	bool toSet = ui->takeScreenshotCheckBox->isChecked();
	var->getSettings().setTakingScreensState(toSet);
}
void MainMenu::updateResourcesAmounts(){
}
void MainMenu::autoHealAndManaRegCheckBoxChanged() {
	bool enable = ui->restoreHealthMana->isChecked();
	var->getSettings().setRestoringState(enable);
	if (enable) {
		ui->healthInfoLabel->clear();
		ui->manaInfoLabel->clear();
		ui->manaShieldLabel->clear();
	}
	else {
		ui->healthInfoLabel->setText("?");
		ui->manaInfoLabel->setText("?");
		ui->manaShieldLabel->setText("?");
	}
}
void MainMenu::onGameStateChanged(int state){	
	QString toWrite = tr("Game status: ");
	typedef ActiveGameThread::GameActivityStates Type;
	switch (state)
	{
	case Type::ACTIVE:
		toWrite += tr("Game active, Logged");
		break; 
	case Type::NO_ACTIVE:
		toWrite += tr("Game not found");
		break;
	case Type::NO_HANDLER:
		toWrite += tr("Game found but no access");
		break;
	case Type::NO_LOGGED:
		toWrite += tr("Game active but no char logged");
		break;
	case Type::NO_WINDOW:
		toWrite += tr("Game is loading");
		break;
	default:
		toWrite += tr("Error");
		break;
	}
	ui->gameActiveLabel->setText(toWrite);
	ui->gameActiveLabel->repaint();

	bool shouldBeActive = state == Type::ACTIVE;	
}
void MainMenu::changedValueOfCharHealthOrMana(double healthPercentage, double manaPercentage, double manaShieldPercentage){
	if (healthPercentage != NULL) {
		QString healthStr = QString::number(healthPercentage, 'g', 3);
		QString msgToSet = QString::fromWCharArray(L"\u017Bycie: %1%").arg(healthStr);
		ui->healthInfoLabel->setText(msgToSet);
	}
	else
		ui->healthInfoLabel->setText(QString::fromWCharArray(L"\u017Bycie: ?"));

	if (manaPercentage != NULL) {
		QString manaStr = QString::number(manaPercentage, 'g', 3);
		QString msgToSet = QString("Mana: %1%").arg(manaStr);
		ui->manaInfoLabel->setText(msgToSet);
	}
	else
		ui->manaInfoLabel->setText("Mana: ?");

	if (manaShieldPercentage != NULL) {
		QString manaShieldStr = QString::number(manaShieldPercentage, 'g', 3);
		QString msgToSet = QString("Tarcza: %1%").arg(manaShieldStr);
		ui->manaShieldLabel->setText(msgToSet);
	}
	else
		ui->manaShieldLabel->setText("Tarcza: ?");

	ui->healthInfoLabel->repaint();
	ui->manaInfoLabel->repaint();
	ui->manaShieldLabel->repaint();
}
void MainMenu::printToUserConsol(QStringList msgs){
	ui->textBrowser->append(msgs[0]);
}
void MainMenu::updatePlayerPosition(QString x, QString y, QString f){
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
void MainMenu::autoHuntButtonClicked(){
	bool threadAlreadyCreated = huntAutoThread != nullptr;
	if(!threadAlreadyCreated)
		startAutoHunting();
	else {
		bool enable = !var->getSettings().getKeepHuntingAutomaticly();
		var->getSettings().setKeepHuntingAutomaticly(enable);
		ui->playerPosGroup->setVisible(enable);
	}
}
