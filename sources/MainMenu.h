#pragma once
#include <QDialog>
#include "Profile.h"
#include "ScreenSaver.h"
#include "activeGameThread.h"
#include "qdatetime.h"
#include "VariablesClass.h"
#include "Utilities.h"
#include "Key.h"
#include "ManaHealthStateAnalyzer.h"
#include "ScreenAnalyzer.h"
#include "memory"
#include "Calibrator.h"
#include "SelectProfileWindow.h"
#include "RouteCreator.h"
#include "Market.h"
namespace Ui { class MainMenu; };

class MainMenu : public QDialog
{
	Q_OBJECT

public:
	MainMenu(Profile* prof, QWidget *parent = Q_NULLPTR);
	~MainMenu();
	activeGameThread::gameActivityStates gameActivitystate;
public slots:
	void helpButtonAction();
	void changeProfileButtonAction();
	void editProfileButtonAction();
	void manualHuntAction();
	void autoHuntAction();
	void tradingAction();
	void skillingAction();
	void getAndDisplayPotionAmountInfo(QStringList);
	//from threads
	void autoHealAndManaRegCheckBoxChanged();
	void onGameStateChanged(int state);
	void changedValueOfCharHealthOrMana(double healthPercentage, double manaPercentage, double manaShieldPercentage0);
private:
	unsigned int pidOfGame;
	QString gameWindowTitle;
	Ui::MainMenu *ui;
	Profile* prof;


	ScreenSaver* screenSaverThread;
	activeGameThread* activityThread;
	ScreenAnalyzer* screenAnalyzer;
	ManaHealthStateAnalyzer* healthManaStateAnalyzer;
	VariablesClass var;


	void setProblemsWindow(QStringList problemsToShow);
	void threadStarter();
	void signalSlotConnector();
};