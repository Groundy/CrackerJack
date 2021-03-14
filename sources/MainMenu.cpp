#include "MainMenu.h"
#include "ui_MainMenu.h"

MainMenu::MainMenu(Profile* selectedProf, QWidget* parent)
	: QDialog(parent)
{
	ui = new Ui::MainMenu();
	ui->setupUi(this);
	prof = selectedProf;
	ui->profileNameLabel->setText(prof->profileName);
	activityThread = new activeGameThread(this,&var);
	activityThread->start();
	screenSaverThread = new ScreenSaver(this, &var);
	screenSaverThread->start();
	screenAnalyzer = new ScreenAnalyzer(this, &var);
	screenAnalyzer->start();

}

MainMenu::~MainMenu()
{
	delete activityThread;
	delete screenSaverThread;
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
}

void MainMenu::skillingAction()
{
}

void MainMenu::onGameStateChanged(int state){
	qDebug() << "MainMenu::onGameStateChanged, signal recived: " + QString::number(state);
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
	label->setText(toWrite);
	label->repaint();
}

void MainMenu::setProblemsWindow(QStringList problemsToShow){
	ui->textBrowser->clear();
	for each (QString str in problemsToShow){
		ui->textBrowser->insertPlainText(str + "\n");
	}
}

