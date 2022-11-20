#include "MainMenu.h"
#include "ui_MainMenu.h"
#include "MinimapAnalyzer.h"
MainMenu::MainMenu(Profile* selectedProf, QWidget* parent)
	: QDialog(parent), prof(selectedProf){
	ui = new Ui::MainMenu();
	ui->setupUi(this);
	var = std::shared_ptr<VariablesClass>(new VariablesClass());
	gameConnector = std::shared_ptr<GameConnecter>(new GameConnecter(this));
	ui->profileNameLabel->setText(prof->profileName);
	threadStarter();
	signalSlotConnector();
	this->activityThread->exit();
}

MainMenu::~MainMenu(){
	/*
	delete activityThread;
	delete screenSaverThread;
	delete screenAnalyzer;
	delete healthManaStateAnalyzer;
	delete ui;
	ProfileDataBaseManager db;
	db.saveProfileToDataBase(*this->prof);
	*/
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
	this->screenAnalyzer->enableScreenAnalyzer = shouldBeActive;
	this->screenSaverThread->enableScreenCapture = shouldBeActive;
	
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

void MainMenu::setProblemsWindow(QStringList problemsToShow){
	ui->textBrowser->clear();
	for each (QString str in problemsToShow){
		ui->textBrowser->insertPlainText(str + "\n");
	}
}

void MainMenu::threadStarter(){
	activityThread = new ActiveGameThread(this, var);
	activityThread->start();

	screenSaverThread = new ScreenSaver(this, var, gameConnector);
	screenSaverThread->start();

	screenAnalyzer = new ScreenAnalyzer(this, var, prof);
	screenAnalyzer->start();

	healthManaStateAnalyzer = new ManaHealthStateAnalyzer(this, prof , var);
	healthManaStateAnalyzer->start();
}

void MainMenu::signalSlotConnector(){
	QObject *sigSender, *slotRec;
	const char *sig, *slot;
	

	sigSender = healthManaStateAnalyzer;
	slotRec = this;
	sig = SIGNAL(sendValueToMainThread(double, double, double));
	slot = SLOT(changedValueOfCharHealthOrMana(double, double, double));
	bool connectionAccepted_2 = connect(sigSender, sig, slotRec, slot, Qt::UniqueConnection);

	sigSender = this->screenAnalyzer;
	slotRec = this->healthManaStateAnalyzer;
	sig = SIGNAL(sendAllowenceToAnalyze(bool));
	slot = SLOT(setThreadEnabilityToRun(bool));
	bool connectionAccepted_3 = connect(sigSender, sig, slotRec, slot, Qt::UniqueConnection);

	sigSender = this->healthManaStateAnalyzer;
	slotRec = this;
	sig = SIGNAL(sendInfoAboutPotAmountsToGUI(QStringList));
	slot = SLOT(getAndDisplayPotionAmountInfo(QStringList));
	bool connectionAccepted_4 = connect(sigSender, sig, slotRec, slot, Qt::UniqueConnection);
}


void MainMenu::autoHealAndManaRegCheckBoxChanged() {
	bool stateOfSwitch = ui->autoManaHealChechBox->isChecked();
	var->HealthAndManaRestorationShouldBeActive = stateOfSwitch;
}

void MainMenu::changeProfileButtonAction(){
	/*
	ProfileDataBaseManager db;
	db.saveProfileToDataBase(*this->prof);
	Profile profTmp;
	SelectProfileWindow selectProfWin(this, &profTmp);
	int res = selectProfWin.exec();
	if (res == QDialog::Accepted) {
		this->prof->copyFrom(profTmp);
		this->ui->profileNameLabel->setText(this->prof->profileName);
		this->ui->profileNameLabel->repaint();
	}
	*/
}

void MainMenu::editProfileButtonAction(){
	/*
	Profile profTmp;
	NewProfileConfiguartor profConfig(&profTmp,this);
	profConfig.fillWidgetsWithDataFromProf(this->prof);
	int res = profConfig.exec();
	if (res == QDialog::Accepted) {
		this->prof->copyFrom(profTmp);
		this->ui->profileNameLabel->setText(this->prof->profileName);
		this->ui->profileNameLabel->repaint();
	}
	*/
}

void MainMenu::manualHuntAction(){
}

void MainMenu::autoHuntAction(){
}

void MainMenu::tradingAction(){
	Market market(var, gameConnector);
	market.exec();
}

void MainMenu::skillingAction(){
}

void MainMenu::getAndDisplayPotionAmountInfo(QStringList list){
	const int MAX_POSIBLE_LIST_LENGTH = 3;
	QList<QLabel*> labels;
	labels.push_back(ui->potion_label_1);
	labels.push_back(ui->potion_label_2);
	labels.push_back(ui->potion_label_3);

	for (size_t i = 0; i < MAX_POSIBLE_LIST_LENGTH; i++){
		bool shouldBeDisplayed = i < list.size();
		if (shouldBeDisplayed) {
			QString textToSet = shouldBeDisplayed ? list[i] : QString();
			labels[i]->setText(textToSet);	
		}
		labels[i]->setVisible(shouldBeDisplayed);
		labels[i]->repaint();
	}
}
