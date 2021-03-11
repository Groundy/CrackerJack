#pragma once
#include <QDialog>
#include "Profile.h"
#include "activeGameThread.h"
#include "qdatetime.h"
namespace Ui { class MainMenu; };
class MainMenu : public QDialog
{
	Q_OBJECT

public:
	MainMenu(Profile* prof, QWidget *parent = Q_NULLPTR);
	~MainMenu();
public slots:
	void helpButtonAction();
	void changeProfileButtonAction();
	void editProfileButtonAction();
	void manualHuntAction();
	void autoHuntAction();
	void tradingAction();
	void skillingAction();
	void onGameStateChanged(int state);

private:
	Ui::MainMenu *ui;
	Profile* prof;
	activeGameThread::gameActivityStates gameActivitystate;
	activeGameThread* activityThread;
	void setProblemsWindow(QStringList problemsToShow);
	void startThreads();
};
