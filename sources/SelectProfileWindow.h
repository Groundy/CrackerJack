#pragma once
#include <QDialog>
#include <memory>

#include "Profile.h"
#include "Profession.h"
#include "NewProfileConfiguartor.h"
namespace Ui { class SelectProfileWindow; };

class SelectProfileWindow : public QDialog
{
	Q_OBJECT

public:
	SelectProfileWindow(QWidget *parent = Q_NULLPTR, Profile* profileToBeChoosen = NULL);
	~SelectProfileWindow();
public slots:
	void addNewProfileButtonAction();
	void editProfileButtonAction();
	void deleteProfileButtonAction();
	void selectListAction();
	void profSelected();

private:
	Ui::SelectProfileWindow *ui;
	Profile* profileToBeChoosen;

	void refreshProfilesOnList();
	void setUpGui();
	void readAndSetLastUsedProFromINI();
	QString getSelectedProfName();
};
