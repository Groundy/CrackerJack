#pragma once
#include <QDialog>
#include <memory>
#include <qdatetime.h>
#include <qsound.h>

#include "Profile.h"
#include "ScreenSaver.h"
#include "activeGameThread.h"
#include "VariablesClass.hpp"
#include "Utilities.h"
#include "Key.h"
#include "ManaHealthStateAnalyzer.h"
#include "ScreenAnalyzer.h"
#include "Calibrator.h"
#include "SelectProfileWindow.h"
#include "RouteCreator.h"

#include "AutoHunting.h"
#include "AttackMethode.hpp"
#include "ClickDetector.hpp"
namespace Ui { class MainMenu; };

class MainMenu : public QDialog
{
	Q_OBJECT
public:
	MainMenu(Profile* prof, QWidget *parent = Q_NULLPTR);
	~MainMenu();
public slots:
	void changeProfileButtonAction();
	void updateResourcesAmounts();
	void takeScreenShotCheckBoxChanged();
	void testButtonClicked();
	void autoHuntButtonClicked();

	//from threads
	void autoHealAndManaRegCheckBoxChanged();
	void onGameStateChanged(int state);
	void changedValueOfCharHealthOrMana(double healthPercentage, double manaPercentage, double manaShieldPercentage);
	void printToUserConsol(QStringList msgs);
	void updatePlayerPosition(QString x, QString y, QString f);
	void updateEnemiesAmount(int enemies);
	void updateHeadingPoint(QString toDisplay);
private:
	Ui::MainMenu *ui;
	Profile* prof;
	ActiveGameThread* activityThread;
	ScreenSaver* screenSaverThread;
	ScreenAnalyzer* screenAnalyzer;
	ManaHealthStateAnalyzer* healthManaStateAnalyzer;
	ClickDetector* clickDetector;
	AutoHunting* huntAutoThread = nullptr;
	std::shared_ptr<VariablesClass> var;
	std::shared_ptr<GameConnecter> gameConnector;
	ActiveGameThread::GameActivityStates gameActivitystate;
	QSound* sound = new QSound(":/sounds/testSound", this);
	void threadStarter();
	void startAutoHunting();
	void signalSlotConnector();
};