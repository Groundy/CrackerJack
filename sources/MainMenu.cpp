#include "MainMenu.h"
#include "ui_MainMenu.h"

MainMenu::MainMenu(Profile* selectedProf,QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::MainMenu();
	ui->setupUi(this);
	prof = selectedProf;
	ui->profileNameLabel->setText(prof->profileName);
	activityThread = new activeGameThread(this);
	//bool good = connect(activityThread, SIGNAL(GameStateChanged()), this, SLOT(onGameStateChanged()));
}

MainMenu::~MainMenu()
{
	delete ui;
}

void MainMenu::helpButtonAction()
{
}

void MainMenu::changeProfileButtonAction()
{
}

void MainMenu::editProfileButtonAction(){
}

void MainMenu::manualHuntAction(){
}

void MainMenu::autoHuntAction()
{
}

void MainMenu::tradingAction(){
	qDebug() << "cht";
	startThreads();
}

void MainMenu::skillingAction()
{
}

void MainMenu::onGameStateChanged(activeGameThread::gameActivityStates state){
	QDateTime tt;
	QString ff = tt.currentDateTime().toString("mm:ss");
	ui->profileNameLabel->setText(ff);
	ui->profileNameLabel->repaint();
}

void MainMenu::setProblemsWindow(QStringList problemsToShow){
	ui->textBrowser->clear();
	for each (QString str in problemsToShow){
		ui->textBrowser->insertPlainText(str + "\n");
	}
}

void MainMenu::startThreads(){
	activityThread = new activeGameThread(this);
	activityThread->start();
	int g = 4;
}
