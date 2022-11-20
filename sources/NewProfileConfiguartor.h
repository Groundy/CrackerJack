#pragma once
#include <qdialog.h>
#include <qlistwidget.h>
#include <qstring.h>
#include <iostream>

#include "ui_NewProfileConfiguartor.h"
#include "Utilities.h"
#include "Profile.h"
#include "JsonParser.h"
#include "Key.h"
#include "Profession.h"

namespace Ui { class NewProfileConfiguartor; };

class NewProfileConfiguartor : public QDialog
{
	Q_OBJECT

public:
	NewProfileConfiguartor(Profile* prof, QWidget *parent = Q_NULLPTR);
	~NewProfileConfiguartor();

public slots:
	void cancelButtonAction();
	void finishButtonAction();
	void healingOrManaGroupCounterChanged();
	void healingSlidersChanged();
	void manaSlidersChanged();
	void controlsOrAutoLootListAction();

private:
	struct GuiPointers {
		QVector<QAbstractSlider*> sliders;
		QVector<QLabel*> percentageLabels;
		QVector<QComboBox*> methodeNames;
		QVector<QComboBox*> keyShortCuts;
		QSpinBox* activeElementsCounter;
	};

	GuiPointers healthPtrs, manaPtrs;
	const int MAX_NUMBER_OF_ACTIVE_WIDGETS = 5;
	Ui::NewProfileConfiguartor *ui;
	Profile* profToEdit;

	//correctnessCheck
	bool checkNameGroup();	
	bool checkSlidersGroup(GuiPointers guiPtrs);
	bool checkHealthGroup();
	bool checkManaGroup();
	bool checkControlsGroup();
	bool checkAllForms();


	//funcs
	void saveDataToProfile();
	void refreshSliders(GuiPointers guiPointers);
	void fillSlidersGroup(GuiPointers guiPointers, QList<RestorationStruct> stucts);
	void fillRestoriationMethodesForProfession(Profession prof);
	void fillFormsFromDataFromProf(const Profile& profToEdit);
	QList<RestorationStruct> getRestorationMethodesFromGUI(GuiPointers guiPotiners);
	Profession getSelectedProf();
	void fillGuiPtrs();
};
