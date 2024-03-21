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
  void        getDataFromGUI();

  Profile*                       prof_;
  Ui::AutoHuntConfiguratorClass* ui;

  QVector<QComboBox*> attack_keys_boxes_  = {ui->atakKeyBox_1, ui->atakKeyBox_2, ui->atakKeyBox_3, ui->atakKeyBox_4};
  QVector<QComboBox*> attack_names_boxes_ = {ui->atakNameBox_1, ui->atakNameBox_2, ui->atakNameBox_3, ui->atakNameBox_4};
  AutoHuntData        auto_hunt_data_{};
  QString             last_selected_route_;
  QVector<QSpinBox*>  attack_monsters_boxes_{ui->attackMinMonsers_1,  //
                                            ui->attackMinMonsers_2,  //
                                            ui->attackMinMonsers_3,  //
                                            ui->attackMinMonsers_4};
  QVector<QCheckBox*> check_boxes_{ui->actvieAtackCheckBox_1,   //
                                   ui->actvieAtackCheckBox_2,   //
                                   ui->actvieAtackCheckBox_3,   //
                                   ui->actvieAtackCheckBox_4};  //
};
