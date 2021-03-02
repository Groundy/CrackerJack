#pragma once
#include <qdialog.h>
#include "qlistwidget.h"
#include "ui_NewProfileConfiguartor.h"
#include "Utilities.h"
#include "qstring.h"
#include "StringResource.h"
#include "Profile.h"

namespace Ui { class NewProfileConfiguartor; };

class NewProfileConfiguartor : public QDialog
{
	Q_OBJECT

public:
	NewProfileConfiguartor( Profile* prof, QWidget *parent = Q_NULLPTR);
	~NewProfileConfiguartor();
	void fillWidgetsWithDataFromProf(Profile* prof);
public slots:
	void cancelButtonAction();
	void nextPageButtonAction();
	void previousPageButtonAction();
	void helpButtonAction();

	void _3_healingEnabledChanged();
	void _3_spinChanged();
	void _3_slidersChanged();

	void _4_ManaEnabledChanged();
	void _4_spinChanged();
	void _4_slidersChanged();

private:
	Ui::NewProfileConfiguartor *ui;
	void refreshGUI();	
	Profile* profToEdit;
	int pageNumber = 1;
	int MAX_PAGE;
	void finishAddingNewProfile();
	void additionalGuiSettings();
	
	bool pageIsCorrectlyFilled();
	void saveDataToProfile(Profile* prof);
	
};
