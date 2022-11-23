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
#include "RestorationMethode.h"
#include "Spell.h"

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
	void healingGroupCounterChanged();
	void manaGroupCounterChanged();
	void healingSlidersChanged();
	void manaSlidersChanged();
	void controlsOrAutoLootListAction();
	void professionChanged();

	void fillFormsFromDataFromProf(Profile& profToEdit);
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
	bool checkAllKeyBoxes();
	bool checkAllForms();


	//funcs
	void saveDataToProfile();
	void refreshSlidersLabels(GuiPointers guiPointers);
	void fillSlidersGroup(GuiPointers guiPointers, QVector<RestorationMethode> stucts);
	void fillRestoriationMethodes(Profession prof, GuiPointers& guiPointers);
	void counterChanged(GuiPointers guiPointers);
	QVector<RestorationMethode> getRestorationMethodesFromGUI(GuiPointers guiPotiners);
	Profession getSelectedProf();
	void fillGuiPtrs();
	void fillRestorationMethodesDetails(QVector<RestorationMethode>& restorationMethodes) {
		if (restorationMethodes.size() == 0)
			return;

		QList<Spell> spells;
		QList<Potion> potions;
		Profession profession = getSelectedProf();
		auto spellType = Spell::SpellType::Healing;
		JsonParser().readSpellsJson(spells, &spellType, &profession);
		JsonParser().readPotions(potions, &profession, NULL);
		
		for (size_t i = 0; i < restorationMethodes.size(); i++){
			QString name = restorationMethodes[i].getName();
			bool alreadySet = false;
			for each (auto spell in spells){
				if (spell.getIncantation() == name){
					restorationMethodes[i].fillDataDetails(spell);
					alreadySet = true;
					break;
				}
			}
			if (alreadySet)
				continue;

			for each (auto potion in potions){
				if (potion.getName() == name) {
					restorationMethodes[i].fillDataDetails(potion);
					alreadySet = true;
					break;
				}
			}
			if (!alreadySet)
				throw std::exception("Cant get details for rest methode");
		}
	}
};
