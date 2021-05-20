#include "MainMenu.h"
#include "ui_MainMenu.h"

MainMenu::MainMenu(Profile* selectedProf, QWidget* parent)
	: QDialog(parent)
{
	ui = new Ui::MainMenu();
	ui->setupUi(this);
	isPl = StringResource::languageIsPl();
	prof = selectedProf;
	ui->profileNameLabel->setText(prof->profileName);
	threadStarter();
	signalSlotConnector();
	setUpGui();
}

MainMenu::~MainMenu(){
	delete activityThread;
	delete screenSaverThread;
	delete screenAnalyzer;
	delete healthManaStateAnalyzer;
	delete ui;
}

void MainMenu::onGameStateChanged(int state){
	bool isPl = StringResource::languageIsPl();
	
	QString previousText = isPl ? QString::fromLocal8Bit("Status gry: ") : "game status: ";
	QString toWrite;
	QLabel* label = ui->gameActiveLabel;
	typedef activeGameThread::gameActivityStates Type;
	switch (state)
	{
	case Type::ACTIVE: {
		QString pl = QString::fromLocal8Bit("Gra aktywna, zalogowano");
		QString eng = "Game active, Logged";
		toWrite = isPl ? pl : eng;
		break; 
	}
	case Type::NO_ACTIVE: {
		QString pl = QString::fromLocal8Bit("Nie znaleziono okna gry");
		QString eng = "Game not found";
		toWrite = isPl ? pl : eng;
		break;
	}
	case Type::NO_HANDLER: {
		QString pl = QString::fromLocal8Bit("Okno gry wykryte, brak dostêpu");
		QString eng = "Game found but no access";
		toWrite = isPl ? pl : eng;
		break;
	}
	case Type::NO_LOGGED: {
		QString pl = QString::fromLocal8Bit("Gra aktywna, nie zalogowano");
		QString eng = "Game active but no char logged";
		toWrite = isPl ? pl : eng;
		break;
	}
	case Type::NO_WINDOW: {
		QString pl = QString::fromLocal8Bit("Gra siê ³aduje");
		QString eng = "Game is loading";
		toWrite = isPl ? pl : eng;
		break;
	}
	default: {
		QString pl = QString::fromLocal8Bit("B³¹d");
		QString eng = "Error";
		toWrite = isPl ? pl : eng;
		break;
	}
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
		QString healthPrefix = isPl ? QString::fromLocal8Bit("¯ycie: ") : "Health: ";
		QString healthFinal = healthPrefix + tmpHealth;
		ui->healthInfoLabel->setText(healthFinal);
	}

	if (manaPercentage == CALIBRATION_STRING_ENG || manaPercentage == CALIBRATION_STRING_PL)
		ui->healthInfoLabel->setText(manaPercentage);
	else {
		QString tmpMana = manaPercentage.rightJustified(3, ' ');
		QString manaPrefix = isPl ? QString::fromLocal8Bit("Mana: ") : "Mana: ";
		QString manaFinal = manaPrefix + tmpMana;
		ui->manaInfoLabel->setText(manaFinal);
	}

	if (manaShieldPercentage == CALIBRATION_STRING_ENG || manaShieldPercentage == CALIBRATION_STRING_PL)
		ui->healthInfoLabel->setText(manaShieldPercentage);
	else {
		QString tmpManaShield = manaShieldPercentage.rightJustified(3, ' ');
		QString manaShieldPrefix = isPl ? QString::fromLocal8Bit("Tarcza many: ") : "Mana shield: ";
		QString manaShieldFinal = manaShieldPrefix + tmpManaShield;
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
	
	QObject* sigSender = this->healthManaStateAnalyzer;
	QObject* slotRec = this->screenAnalyzer;
	const char* sig = SIGNAL(demandReCalibration());
	const char* slot = SLOT(reCalibrate());
	bool connectionAccepted_1 = connect(sigSender, sig, slotRec, slot, Qt::UniqueConnection);
	
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

void MainMenu::setUpGui(){

	ui->potion_label_1->setVisible(false);
	ui->potion_label_2->setVisible(false);
	ui->potion_label_3->setVisible(false);

	QString changeProfileButtomText = isPl ? QString::fromLocal8Bit("Zmieñ profil") : "Change Profile";
	ui->changeProfileButton->setText(changeProfileButtomText);
	ui->changeProfileButton->repaint();
	
	QString editProfileButtomText = isPl ? QString::fromLocal8Bit("Edytuj profil") : "Edit Profile";
	ui->editProfileButton->setText(editProfileButtomText);
	ui->editProfileButton->repaint();
	
	QString helpButtomText = isPl ? QString::fromLocal8Bit("Pomoc") : "Help";
	ui->helpButton ->setText(helpButtomText);
	ui->helpButton->repaint();
	
	QString labelActionText = isPl ? QString::fromLocal8Bit("Co chcesz zrobiæ?") : "What do you want to do?";
	ui->infoLabel->setText(labelActionText);
	ui->infoLabel->repaint();
	
	QString hauntManuallyButtomText = isPl ? QString::fromLocal8Bit("Polowanie rêczne") : "Haunt manually";
	ui->huntMannuallyButton->setText(hauntManuallyButtomText);
	ui->huntMannuallyButton->repaint();
	
	QString hauntAutomaticalyButtomText = isPl ? QString::fromLocal8Bit("Polowanie automatyczne") : "Haunt automatically";
	ui->huntAutomaticallyButton->setText(hauntAutomaticalyButtomText);
	ui->huntAutomaticallyButton->repaint();
	
	QString tradingButtomText = isPl ? QString::fromLocal8Bit("Trading") : "Handel";
	ui->tradingButton->setText(tradingButtomText);
	ui->tradingButton->repaint();
	
	QString skillButtomText = isPl ? QString::fromLocal8Bit("Skillowanie") : "Skilling";
	ui->skillingButton->setText(skillButtomText);
	ui->skillingButton->repaint();
	
	QString autoHealAndManaChechBoxText = isPl ? QString::fromLocal8Bit("Odnawiaj ¿ycie i mane") : "Heal and mana";
	ui->autoManaHealChechBox->setText(autoHealAndManaChechBoxText);
	ui->autoManaHealChechBox->repaint();
	
	QString healthLabel = isPl ? QString::fromLocal8Bit("¯ycie: ") : "Health: ";
	ui->healthInfoLabel->setText(healthLabel);
	ui->healthInfoLabel->repaint();
	
	QString manaLabel = isPl ? QString::fromLocal8Bit("Mana: ") : "Mana: ";
	ui->manaInfoLabel->setText(manaLabel);
	ui->manaInfoLabel->repaint();
	
	QString manaShieldLabel = isPl ? QString::fromLocal8Bit("Tarcza many: ") : "Mana shield: ";
	ui->manaShieldLabel->setText(manaShieldLabel);
	ui->manaShieldLabel->repaint();

	QString windowTitle = isPl ? QString::fromLocal8Bit("CrackerJack - Okno g³ówne") : "CrackerJack - Main menu";
	ui->manaShieldLabel->setText(manaShieldLabel);
	ui->manaShieldLabel->repaint();
	// = isPl ? QString::fromLocal8Bit("") : "";
}

void MainMenu::autoHealAndManaRegCheckBoxChanged() {
	bool stateOfSwitch = ui->autoManaHealChechBox->isChecked();
	var.HealthAndManaRestorationShouldBeActive = stateOfSwitch;
}

void MainMenu::helpButtonAction(){
}

void MainMenu::changeProfileButtonAction(){
}

void MainMenu::editProfileButtonAction(){
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
