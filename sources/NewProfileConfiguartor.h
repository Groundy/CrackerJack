#pragma once
#include <qdialog.h>
#include "qlistwidget.h"
#include "ui_NewProfileConfiguartor.h"
#include "Utilities.h"
#include "qstring.h"
#include "StringResource.h"
#include "Profile.h"
#include "JsonParser.h"
#include "Key.h"

namespace Ui { class NewProfileConfiguartor; };

class NewProfileConfiguartor : public QDialog
{
	Q_OBJECT

public:
	NewProfileConfiguartor(Profile* prof, QWidget *parent = Q_NULLPTR);
	~NewProfileConfiguartor();
	void fillWidgetsWithDataFromProf(Profile* profile);
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
	
	void _5_listAction();

private:
	Ui::NewProfileConfiguartor *ui;
	void refreshGUI();	
	Profile* profToEdit;
	int pageNumber = 1;
	int MAX_PAGE;
	bool finishAddingNewProfile();
	void additionalGuiSettings();
	bool pageIsCorrectlyFilled();
	void saveDataToProfile(Profile* prof);

	bool checkCorrectnessOfPage_1();
	bool checkCorrectnessOfPage_2();
	bool checkCorrectnessOfPage_3();
	bool checkCorrectnessOfPage_4();
	bool checkCorrectnessOfPage_5();
	QStringList getNamesOfHealthRestoringMethodes(Profile::PROFESSION prof);
	QStringList getNamesOfManaRestoringMethodes(Profile::PROFESSION prof);
	void fillGuiElementsWithNamesRestoreMethodes_Health(Profile::PROFESSION prof);
	void fillGuiElementsWithNamesRestoreMethodes_Mana(Profile::PROFESSION prof);
	Profile::PROFESSION getProfessionFromRadioButtonOnPage2();
};
