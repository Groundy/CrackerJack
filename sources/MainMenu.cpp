#include "MainMenu.h"
#include "ui_MainMenu.h"

MainMenu::MainMenu(Profile* selectedProf, QWidget* parent)
	: QDialog(parent)
{
	ui = new Ui::MainMenu();
	ui->setupUi(this);
	prof = selectedProf;
	ui->profileNameLabel->setText(prof->profileName);
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

void MainMenu::onGameStateChanged(int state){
	QString previousText = "game status: ";
	QString toWrite;
	QLabel* label = ui->gameActiveLabel;
	switch (state)
	{
	case activeGameThread::gameActivityStates::ACTIVE: {
		toWrite = "game active, Logged";
		break; 
	}
	case activeGameThread::gameActivityStates::NO_ACTIVE: {
		toWrite = "game not found";
		break;
	}
	case activeGameThread::gameActivityStates::NO_HANDLER: {
		toWrite = "game found but no access";
		break;
	}
	case activeGameThread::gameActivityStates::NO_LOGGED: {
		toWrite = "game active but no char logged";
		break;
	}
	case activeGameThread::gameActivityStates::NO_WINDOW: {
		toWrite = "game is loading";
		break;
	}
	default: {
		toWrite = "Error";
		break;
	}
	}
	label->setText(previousText + toWrite);
	label->repaint();

	if (state == activeGameThread::gameActivityStates::ACTIVE) {
		this->screenAnalyzer->enableScreenAnalyzer = true;
		this->screenSaverThread->enableScreenCapture = true;
	}
	else {
		this->screenAnalyzer->enableScreenAnalyzer = false;
		this->screenSaverThread->enableScreenCapture = false;
	}
}

void MainMenu::changedValueOfCharHealthOrMana(QString healthPercentage, QString manaPercentage, QString manaShieldPercentage){
	QString tmpHealth = healthPercentage.rightJustified(3, ' ');
	QString tmpMana = manaPercentage.rightJustified(3, ' ');
	QString tmpManaShield = manaShieldPercentage.rightJustified(3, ' ');
	QString healthFinal = "health: " + tmpHealth;
	QString manaFinal= "mana: " + tmpMana;
	QString manaShieldFinal = "mana shield: " + tmpManaShield;
	ui->healthInfoLabel->setText(healthFinal);
	ui->manaInfoLabel->setText(manaFinal);
	ui->manaShieldLabel->setText(manaShieldFinal);
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
	keySender = new KeySender(this, prof, &var);
	keySender->start();
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
		bool shouldBeDisplayed = list.size() - 1 >= i;
		textToSet = shouldBeDisplayed ? list[i] : "";
		labels[i]->setVisible(shouldBeDisplayed);
		labels[i]->setText(textToSet);
		labels[i]->repaint();
	}
}
