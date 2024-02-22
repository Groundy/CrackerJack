#pragma once
#include <QDialog>

#include "JsonParser.h"
#include "Profile.h"
#include "RouteCreator.h"
#include "ui_AutoHuntConfigurator.h"

namespace Ui {
class AutoHuntConfiguratorClass;
};

class AutoHuntConfigurator : public QDialog {
  Q_OBJECT

 public:
  AutoHuntConfigurator(QWidget* parent = nullptr, Profile* prof = nullptr);
  ~AutoHuntConfigurator();

 public slots:
  void selectRoute();
  void editRoute();
  void saveButtonClicked();
  void cancelButtonClicked();
  void checkBoxChanged();

 private:
  void        fillKeyComboBoxes();
  void        fillGuiFromProfileData(Profile* prof);
  QStringList getNamesOfAttacksMethodes(Profession profession);
  void        fillPointersGUI();
  void        getDataFromGUI();

  Profile*                       prof;
  Ui::AutoHuntConfiguratorClass* ui;
  QVector<QCheckBox*>            checkBoxes          = {};
  QVector<QComboBox*>            attackKeysBoxes     = {};
  QVector<QComboBox*>            attackNamesBoxes    = {};
  QVector<QSpinBox*>             attackMonstersBoxes = {};
  AutoHuntData                   autoHuntData        = AutoHuntData();
  QString                        lastSelectedRoute   = QString();
};
