#include "MainMenu.h"
#include "ui_MainMenu.h"

//const
MainMenu::MainMenu(Profile* selectedProf, QWidget* parent)
	: QDialog(parent), prof(selectedProf){
	ui = new Ui::MainMenu();
	ui->setupUi(this);

	var = std::shared_ptr<VariablesClass>(new VariablesClass());
	gameConnector = std::shared_ptr<GameConnecter>(new GameConnecter(this, var));
	ui->profileNameLabel->setText(prof->getName());
	threadStarter();
	signalSlotConnector();
}

MainMenu::~MainMenu(){
	delete activityThread;
	delete screenSaverThread;
	delete screenAnalyzer;
	delete healthManaStateAnalyzer;
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

}
void MainMenu::signalSlotConnector(){
	QObject *sigSender, *slotRec;
	const char *sig, *slot;
	
	sigSender = healthManaStateAnalyzer;
	slotRec = this;
	sig = SIGNAL(sendValueToMainThread(double, double, double));
	slot = SLOT(changedValueOfCharHealthOrMana(double, double, double));
	bool connectionAccepted_1 = connect(sigSender, sig, slotRec, slot, Qt::UniqueConnection);
	
	sigSender = &this->var->logger;
	slotRec = this;
	sig = SIGNAL(sendMsgToUserConsol(QStringList));
	slot = SLOT(printToUserConsol(QStringList));
	bool connectionAccepted_2 = connect(sigSender, sig, slotRec, slot, Qt::UniqueConnection);
	
}


//slots
void MainMenu::changeProfileButtonAction(){
	this->accept();
}
void MainMenu::takeScreenShotCheckBoxChanged() {
	bool toSet = ui->takeScreenshotCheckBox->isChecked();
	var->setSettingTakingScreensState(toSet);
}
void MainMenu::updateResourcesAmounts(){
}
void MainMenu::autoHealAndManaRegCheckBoxChanged() {
	bool stateOfSwitch = ui->restoreHealthMana->isChecked();
	var->setSettingRestoringState(stateOfSwitch);
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
	const QString NO_DATA_INFO = tr("No data to display");
	bool thereIsHealthData = healthPercentage >= 0 && healthPercentage <= 100 && healthPercentage != NULL;
	bool thereIsManaData = manaPercentage >= 0 && manaPercentage <= 100 && manaPercentage != NULL;
	bool thereIsManaShieldData = manaShieldPercentage >= 0 && manaShieldPercentage <= 100 && manaShieldPercentage != NULL;
	QString tmpHealth = QString::number(healthPercentage, 'g', 3) + "%";
	QString tmpMana = QString::number(manaPercentage, 'g', 3) + "%";
	QString tmpManaShield = QString::number(manaShieldPercentage, 'g', 3) + "%";
	QString lifeStr = thereIsHealthData ? tmpHealth : NO_DATA_INFO;
	QString manaStr = thereIsManaData ? tmpMana : NO_DATA_INFO;
	QString mShield = thereIsManaShieldData ? tmpManaShield : NO_DATA_INFO;
	lifeStr.push_front("Health: ");
	manaStr.push_front("Mana: ");
	mShield.push_front("Mana shield: ");
	ui->healthInfoLabel->setText(lifeStr);
	ui->manaInfoLabel->setText(manaStr);
	ui->manaShieldLabel->setText(mShield);
	ui->healthInfoLabel->repaint();
	ui->manaInfoLabel->repaint();
	ui->manaShieldLabel->repaint();
}
void MainMenu::printToUserConsol(QStringList msgs){
	ui->textBrowser->append(msgs[0]);
};