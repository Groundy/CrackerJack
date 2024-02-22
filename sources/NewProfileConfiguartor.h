#pragma once
#include <qdialog.h>
#include <qlistwidget.h>
#include <qstring.h>
#include <qvector.h>

#include <iostream>

#include "AutoHuntConfigurator.h"
#include "JsonParser.h"
#include "Key.h"
#include "Profession.hpp"
#include "Profile.h"
#include "RestorationMethode.hpp"
#include "Spell.h"
#include "Utilities.hpp"
#include "ui_NewProfileConfiguartor.h"

namespace Ui {
class NewProfileConfiguartor;
};

class NewProfileConfiguartor : public QDialog {
  Q_OBJECT

 public:
  NewProfileConfiguartor(Profile* prof, QWidget* parent = Q_NULLPTR);
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
  void openAuoHuntSettings();

  void fillFormsFromDataFromProf(Profile& profToEdit);

 private:
  struct GuiPointers {
    QVector<QAbstractSlider*> sliders;
    QVector<QLabel*>          percentageLabels;
    QVector<QComboBox*>       methodeNames;
    QVector<QComboBox*>       keyShortCuts;
    QSpinBox*                 activeElementsCounter;
  };

  GuiPointers                 healthPtrs, manaPtrs;
  const int                   MAX_NUMBER_OF_ACTIVE_WIDGETS = 5;
  Ui::NewProfileConfiguartor* ui;
  Profile*                    profToEdit;

  //correctnessCheck
  bool checkNameGroup();
  bool checkSlidersGroup(GuiPointers guiPtrs);
  bool checkHealthGroup();
  bool checkManaGroup();
  bool checkControlsGroup();
  bool checkAllKeyBoxes();
  bool checkAllForms();

  //funcs
  void                        saveDataToProfile();
  void                        refreshSlidersLabels(GuiPointers guiPointers);
  void                        fillSlidersGroup(GuiPointers guiPointers, QVector<RestorationMethode> stucts);
  void                        fillRestoriationMethodes(Profession prof, GuiPointers& guiPointers);
  void                        counterChanged(GuiPointers guiPointers);
  QVector<RestorationMethode> getRestorationMethodesFromGUI(GuiPointers guiPotiners);
  Profession                  getSelectedProf();
  void                        fillGuiPtrs();
  void                        fillRestorationMethodesDetails(QVector<RestorationMethode>& restorationMethodes);
};
