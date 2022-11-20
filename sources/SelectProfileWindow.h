#pragma once
#include <QDialog>
#include <memory>

#include "Profile.h"
namespace Ui { class SelectProfileWindow; };

class SelectProfileWindow : public QDialog
{
	Q_OBJECT

public:
	SelectProfileWindow(QWidget *parent = Q_NULLPTR, Profile* selectedProf = NULL);
	~SelectProfileWindow();
public slots:
	void addNewProfileButtonAction();
	void editProfileButtonAction();
	void deleteProfileButtonAction();
	void selectListAction();
	void profSelected();
private:
	Ui::SelectProfileWindow *ui;
	void refreshProfilesOnList();
	void setUpGui();
	Profile* profToSelect;
	void readAndSetLastUsedProFromINI();
	QString getSelectedProfName();
};
