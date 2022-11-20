#pragma once
#include <QDialog>
#include "memory"

#include "Profile.h"
#include "ScreenSaver.h"
#include "activeGameThread.h"
#include "qdatetime.h"
#include "VariablesClass.h"
#include "Utilities.h"
#include "Key.h"
#include "ManaHealthStateAnalyzer.h"
#include "ScreenAnalyzer.h"
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
public slots:
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
	Ui::MainMenu *ui;
	Profile* prof;
	ActiveGameThread* activityThread;
	ScreenSaver* screenSaverThread;
	ScreenAnalyzer* screenAnalyzer;
	ManaHealthStateAnalyzer* healthManaStateAnalyzer;
	std::shared_ptr<VariablesClass> var;
	std::shared_ptr<GameConnecter> gameConnector;
	ActiveGameThread::GameActivityStates gameActivitystate;

	void setProblemsWindow(QStringList problemsToShow);
	void threadStarter();
	void signalSlotConnector();
};