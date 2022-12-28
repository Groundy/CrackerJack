#include "MainMenu.h"
#include "ui_MainMenu.h"

//const
MainMenu::MainMenu(Profile* prof, QWidget* parent)
	: QDialog(parent), prof(prof){
	ui = new Ui::MainMenu();
	ui->setupUi(this);

	var = std::shared_ptr<VariablesClass>(new VariablesClass(sound));
	gameConnector = std::shared_ptr<GameConnecter>(new GameConnecter(var, prof));
	ui->profileNameLabel->setText(prof->getName());
	
	ui->playerPosGroup->setVisible(false);
	ui->resourceGroup->setVisible(false);
	
	Settings& settings = var->getSettings();
	ui->keepHastedCheckBox->setChecked(settings.getKeepHasted());
	ui->keepUpgradedCheckBox->setChecked(settings.getKeepUpraded());

	threadStarter();
	signalSlotConnector();
}
MainMenu::~MainMenu(){
	QList<QThread*> threads;
	threads.push_back(activityThread);
	threads.push_back(screenSaverThread);
	threads.push_back(screenAnalyzer);
	threads.push_back(healthManaStateAnalyzer);
	threads.push_back(huntAutoThread);
	threads.push_back(clickDetector);
	for each (QThread* thread in threads){
		thread->terminate();
		delete thread;
	}
	delete sound;
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

	clickDetector = new ClickDetector(this, gameConnector);
	clickDetector->start();
}
void MainMenu::signalSlotConnector(){
	try{
		QObject* sigSender = healthManaStateAnalyzer;
		QObject* slotRec = this;
		const char* sig = SIGNAL(sendValueToMainThread(double, double, double));
		const char* slot = SLOT(changedValueOfCharHealthOrMana(double, double, double));
		bool connected = connect(sigSender, sig, slotRec, slot, Qt::UniqueConnection);
		if (!connected)
			throw std::exception("to do!");

		sigSender = &this->var->logger;
		slotRec = this;
		sig = SIGNAL(sendMsgToUserConsol(QStringList));
		slot = SLOT(printToUserConsol(QStringList));
		connected = connect(sigSender, sig, slotRec, slot, Qt::UniqueConnection);
		if (!connected)
			throw std::exception("to do!");
	}
	catch (const std::exception& e){
		//todo
		exit(0);
	}
}
void MainMenu::startAutoHunting() {
	Route route;
	JsonParser::readRoute(route, "trolls");
	huntAutoThread = new AutoHunting(this, var, gameConnector, route, prof);
	huntAutoThread->start();
	ui->playerPosGroup->setVisible(true);


	const char* sig = SIGNAL(updateHeadingPointInGUI(QString));
	const char* slot = SLOT(updateHeadingPoint(QString));
	bool connectionAccepted = connect(huntAutoThread, sig, this, slot, Qt::UniqueConnection);
	sig = SIGNAL(updateEnemiesAmountInGUI(int));
	slot = SLOT(updateEnemiesAmount(int));
	connectionAccepted = connect(huntAutoThread, sig, this, slot, Qt::UniqueConnection);

	QObject* sigSender = huntAutoThread->getMiniMapAnalyzer();
	QObject* slotRec = this;
	sig = SIGNAL(sendPostitionsToGUI(QString, QString, QString));
	slot = SLOT(updatePlayerPosition(QString, QString, QString));
	connectionAccepted = connect(sigSender, sig, slotRec, slot, Qt::UniqueConnection);
}

//slots
void MainMenu::changeProfileButtonAction(){
	this->accept();
}
void MainMenu::onGameStateChanged(int state){	
	QString toWrite = tr("Game status: ");
	typedef ActiveGameThread::GameActivityStates Type;
	switch (state)
	{
	case Type::ACTIVE:
		toWrite += tr("Game active, Logged");	break; 
	case Type::NO_ACTIVE:
		toWrite += tr("Game not found");	break;
	case Type::NO_HANDLER:
		toWrite += tr("Game found but no access");	break;
	case Type::NO_LOGGED:
		toWrite += tr("Game active but no char logged");	break;
	case Type::NO_WINDOW:
		toWrite += tr("Game is loading");	break;
	default:
		toWrite += tr("Error");	break;
	}
	ui->gameActiveLabel->setText(toWrite);
	ui->gameActiveLabel->repaint();
}
void MainMenu::checkBoxChanged(){
	QObject* senderObj = sender();
	if (senderObj == ui->takeScreenshotCheckBox) {
		bool toSet = ui->takeScreenshotCheckBox->isChecked();
		var->getSettings().setTakingScreensState(toSet);
	}
	if (senderObj == ui->restoreHealthMana) {
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
	if (senderObj == ui->keepHastedCheckBox) {
		bool toSet = ui->keepHastedCheckBox->isChecked();
		var->getSettings().setKeepHasted(toSet);
	}
	if (senderObj == ui->keepUpgradedCheckBox) {
		bool toSet = ui->keepUpgradedCheckBox->isChecked();
		var->getSettings().setKeepUpraded(toSet);
	}
}
void MainMenu::changedValueOfCharHealthOrMana(double healthPercentage, double manaPercentage, double manaShieldPercentage){
	if (healthPercentage != NULL) {
		QString healthStr = QString::number(healthPercentage, 'g', 3);
		QString msgToSet = QString::fromWCharArray(L"\u017Bycie: %1%").arg(healthStr);
		ui->healthInfoLabel->setText(msgToSet);
	}
	else
		ui->healthInfoLabel->setText(QString::fromWCharArray(L"\u017Bycie: ?"));
	ui->healthInfoLabel->repaint();

	if (manaPercentage != NULL) {
		QString manaStr = QString::number(manaPercentage, 'g', 3);
		QString msgToSet = QString("Mana: %1%").arg(manaStr);
		ui->manaInfoLabel->setText(msgToSet);
	}
	else
		ui->manaInfoLabel->setText("Mana: ?");
	ui->manaInfoLabel->repaint();

	if (manaShieldPercentage != NULL) {
		QString manaShieldStr = QString::number(manaShieldPercentage, 'g', 3);
		QString msgToSet = QString("Tarcza: %1%").arg(manaShieldStr);
		ui->manaShieldLabel->setText(msgToSet);
	}
	else
		ui->manaShieldLabel->setText("Tarcza: ?");
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
