#pragma once
#include <QDialog>
#include "ProfileDataBaseManager.h"
#include "StringResource.h"
namespace Ui { class SelectProfileWindow; };

class SelectProfileWindow : public QDialog
{
	Q_OBJECT

public:
	SelectProfileWindow(QWidget *parent = Q_NULLPTR, Profile* selectedProf = NULL);
	~SelectProfileWindow();
	ProfileDataBaseManager dbManager;
public slots:
	void addNewProfileButtonAction();
	void editProfileButtonAction();
	void deleteProfileButtonAction();
	void selectListAction();
	void profSelected();
private:
	bool isPl;
	Ui::SelectProfileWindow *ui;
	void prepareProfiles();
	void setUpGui();
	Profile* profToSelect;
	void readAndSetLastUsedProFromINI();
};
