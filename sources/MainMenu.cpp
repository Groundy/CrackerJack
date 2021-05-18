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
