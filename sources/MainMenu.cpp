#include "MainMenu.h"
#include "ui_MainMenu.h"
#include "MinimapAnalyzer.h"
MainMenu::MainMenu(Profile* selectedProf, QWidget* parent)
	: QDialog(parent)
{
	ui = new Ui::MainMenu();
	ui->setupUi(this);
	prof = selectedProf;
	ui->profileNameLabel->setText(prof->profileName);
	threadStarter();
	signalSlotConnector();
	this->activityThread->exit();
	test();
}

MainMenu::~MainMenu(){
	delete activityThread;
	delete screenSaverThread;
	delete screenAnalyzer;
	delete healthManaStateAnalyzer;
	delete ui;
	ProfileDataBaseManager db;
	db.saveProfileToDataBase(*this->prof);
}

void MainMenu::onGameStateChanged(int state){	
	QString previousText = tr("Game status: ");
	QString toWrite;
	QLabel* label = ui->gameActiveLabel;
	typedef activeGameThread::gameActivityStates Type;
	switch (state)
	{
	case Type::ACTIVE:
		toWrite = tr("Game active, Logged");
		break; 
	case Type::NO_ACTIVE:
		toWrite = tr("Game not found");
		break;
	case Type::NO_HANDLER:
		toWrite = tr("Game found but no access");
		break;
	case Type::NO_LOGGED:
		toWrite = tr("Game active but no char logged");
		break;
	case Type::NO_WINDOW:
		toWrite = tr("Game is loading");
		break;
	default:
		toWrite = tr("Error");
		break;
	}
	label->setText(previousText + toWrite);
	label->repaint();

	bool shouldBeActive = state == Type::ACTIVE;
	this->screenAnalyzer->enableScreenAnalyzer = shouldBeActive;
	this->screenSaverThread->enableScreenCapture = shouldBeActive;
	
}

void MainMenu::changedValueOfCharHealthOrMana(QString healthPercentage, QString manaPercentage, QString manaShieldPercentage){
	if(healthPercentage == CALIBRATION_STRING_ENG || healthPercentage == CALIBRATION_STRING_PL)
		ui->healthInfoLabel->setText(healthPercentage);
	else {
		QString tmpHealth = healthPercentage.rightJustified(3, ' ');
		QString healthFinal = tr("Health: ") + tmpHealth;
		ui->healthInfoLabel->setText(healthFinal);
	}

	if (manaPercentage == CALIBRATION_STRING_ENG || manaPercentage == CALIBRATION_STRING_PL)
		ui->healthInfoLabel->setText(manaPercentage);
	else {
		QString tmpMana = manaPercentage.rightJustified(3, ' ');
		QString manaFinal = "Mana: " + tmpMana;
		ui->manaInfoLabel->setText(manaFinal);
	}

	if (manaShieldPercentage == CALIBRATION_STRING_ENG || manaShieldPercentage == CALIBRATION_STRING_PL)
		ui->healthInfoLabel->setText(manaShieldPercentage);
	else {
		QString tmpManaShield = manaShieldPercentage.rightJustified(3, ' ');
		QString manaShieldFinal = "Mana shield: " + tmpManaShield;
		ui->manaShieldLabel->setText(manaShieldFinal);
	}

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
	activityThread = new activeGameThread(this,&var);
	activityThread->start();
	screenSaverThread = new ScreenSaver(this, &var);
	screenSaverThread->start();
	screenAnalyzer = new ScreenAnalyzer(this, &var, prof);
	screenAnalyzer->start();
	healthManaStateAnalyzer = new ManaHealthStateAnalyzer(this, prof , &var);
	healthManaStateAnalyzer->start();
}

void MainMenu::signalSlotConnector(){
	QObject *sigSender, *slotRec;
	const char *sig, *slot;
	

	sigSender = healthManaStateAnalyzer;
	slotRec = this;
	sig = SIGNAL(sendValueToMainThread(QString, QString, QString));
	slot = SLOT(changedValueOfCharHealthOrMana(QString, QString, QString));
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
	var.HealthAndManaRestorationShouldBeActive = stateOfSwitch;
}

void MainMenu::helpButtonAction(){
	//TODO
	//QString msg = tr("");
	//Utilities::showMessageBox_INFO(msg);
}

void MainMenu::changeProfileButtonAction(){
	ProfileDataBaseManager db;
	db.saveProfileToDataBase(*this->prof);
	Profile profTmp;
	SelectProfileWindow selectProfWin(this, &profTmp);
	int res = selectProfWin.exec();
	if (res == QDialog::Accepted) {
		this->prof->getDataFromOtherProf(profTmp);
		this->ui->profileNameLabel->setText(this->prof->profileName);
		this->ui->profileNameLabel->repaint();
	}
}

void MainMenu::editProfileButtonAction(){
	Profile profTmp;
	NewProfileConfiguartor profConfig(&profTmp,this);
	profConfig.fillWidgetsWithDataFromProf(this->prof);
	int res = profConfig.exec();
	if (res == QDialog::Accepted) {
		this->prof->getDataFromOtherProf(profTmp);
		this->ui->profileNameLabel->setText(this->prof->profileName);
		this->ui->profileNameLabel->repaint();
	}
}

void MainMenu::manualHuntAction(){
}

void MainMenu::autoHuntAction(){
}

void MainMenu::tradingAction(){
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
		QString textToSet;
		bool shouldBeDisplayed = i < list.size();
		if (shouldBeDisplayed) {
			textToSet = shouldBeDisplayed ? list[i] : "";
			labels[i]->setText(textToSet);	
		}
		labels[i]->setVisible(shouldBeDisplayed);
		labels[i]->repaint();
	}
}
