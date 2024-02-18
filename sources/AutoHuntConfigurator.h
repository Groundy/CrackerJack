#pragma once
#include "JsonParser.h"
#include "Profile.h"
#include "RouteCreator.h"
#include "ui_AutoHuntConfigurator.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class AutoHuntConfiguratorClass;
};
QT_END_NAMESPACE

class AutoHuntConfigurator : public QDialog {
  Q_OBJECT

public:
  AutoHuntConfigurator(QWidget *parent = nullptr, Profile *prof = nullptr);
  ~AutoHuntConfigurator();
public slots:
  void selectRoute() {
    QString pathToFile = Utilities::getFileByDialog(
        "*.json", PathResource::getPathToRouteFolder());
    QJsonObject obj;
    if (!JsonParser::openJsonFile(obj, pathToFile))
      return; // todo

    QString name = Route(obj).getName();
    ui->routeLabel->setText(name);
    lastSelectedRoute = name;
  };
  void editRoute() {
    if (lastSelectedRoute.isEmpty())
      return;

    Route route;
    JsonParser::readRoute(route, lastSelectedRoute);
    RouteCreator routeCreator(this, &route);
    routeCreator.exec();
  };
  void saveButtonClicked() {
    getDataFromGUI();
    prof->setAutoHuntData(autoHuntData);
    this->accept();
  };
  void cancelButtonClicked() { this->reject(); };
  void checkBoxChanged() {
    int lastActiveCheckboxIndex = -1;
    for each (auto checkBox in checkBoxes)
      if (checkBox->isChecked())
        lastActiveCheckboxIndex++;

    for (int i = 0; i < checkBoxes.size(); i++) {
      bool setEnable = checkBoxes[i]->isChecked();
      attackKeysBoxes[i]->setEnabled(setEnable);
      attackMonstersBoxes[i]->setEnabled(setEnable);
      attackNamesBoxes[i]->setEnabled(setEnable);
      bool isLast = i == checkBoxes.size() - 1;
      if (!isLast && setEnable && i <= lastActiveCheckboxIndex) {
        checkBoxes[i + 1]->setEnabled(true);
      }
    }
  }

private:
  void fillKeyComboBoxes() {
    auto allPosibleKeyNames = Key::getListOfAllPossibleKeys();
    QVector<QComboBox *> attackKeysBoxes = {
        ui->atakKeyBox_1,
        ui->atakKeyBox_2,
        ui->atakKeyBox_3,
        ui->atakKeyBox_4,
    };
    for each (auto keyBox in attackKeysBoxes) {
      keyBox->insertItems(0, allPosibleKeyNames);
      keyBox->setCurrentIndex(-1);
    }
  }
  void fillGuiFromProfileData(Profile *prof) {
    auto data = prof->getAutoHuntData();
    autoHuntData = data;
    ui->minMonContinueSpiner->setValue(data.getMinMonToContinue());
    ui->minMonStopSpiner->setValue(data.getMinMonToStop());
    auto attacks = data.getAttacks();
    for (int i = 0; i < attacks.size(); i++) {
      checkBoxes[i]->setEnabled(true);
      checkBoxes[i]->setChecked(true);
      attackKeysBoxes[i]->setCurrentIndex(
          attackKeysBoxes[i]->findText(attacks[i].getKey().getKeyName()));
    }

    QStringList namesToFill = getNamesOfAttacksMethodes(prof->getProfession());
    for each (auto var in attackNamesBoxes) {
      var->insertItems(0, namesToFill);
      var->setCurrentIndex(-1);
    }
    for (int i = 0; i < attacks.size(); i++)
      attackNamesBoxes[i]->setCurrentIndex(
          attackNamesBoxes[i]->findText(attacks[i].getName()));

    auto allPossibleKeyNames = Key::getListOfAllPossibleKeys();
    for each (auto box in attackKeysBoxes) {
      box->addItems(allPossibleKeyNames);
      box->setCurrentIndex(-1);
    }

    int i = 0;
    for each (AttackMethode obj in prof->getAutoHuntData().getAttacks()) {
      attackNamesBoxes[i]->setEnabled(true);
      attackKeysBoxes[i]->setEnabled(true);
      attackMonstersBoxes[i]->setEnabled(true);
      checkBoxes[i]->setEnabled(true);
      attackNamesBoxes[i]->setCurrentIndex(
          attackNamesBoxes[i]->findText(obj.getName()));
      attackKeysBoxes[i]->setCurrentIndex(
          attackKeysBoxes[i]->findText(obj.getKey().getKeyName()));
      attackMonstersBoxes[i]->setValue(obj.getMinMonsters());
      checkBoxes[i]->setChecked(true);
      i++;
    }
  }
  QStringList getNamesOfAttacksMethodes(Profession profession) {
    QList<Spell> spells;
    auto filterType = Spell::SpellType::Attack;
    JsonParser::readSpellsJson(spells, &filterType, &profession);
    QStringList namesToFill;
    namesToFill.append(JsonParser::readRunesNames());
    for each (Spell spell in spells)
      namesToFill.append(spell.getIncantation());
    namesToFill.sort();
    return namesToFill;
  }
  Profile *prof;
  Ui::AutoHuntConfiguratorClass *ui;
  QVector<QCheckBox *> checkBoxes = {};
  QVector<QComboBox *> attackKeysBoxes = {};
  QVector<QComboBox *> attackNamesBoxes = {};
  QVector<QSpinBox *> attackMonstersBoxes = {};
  void fillPointersGUI() {
    checkBoxes = {ui->actvieAtackCheckBox_1, ui->actvieAtackCheckBox_2,
                  ui->actvieAtackCheckBox_3, ui->actvieAtackCheckBox_4};
    attackKeysBoxes = {
        ui->atakKeyBox_1,
        ui->atakKeyBox_2,
        ui->atakKeyBox_3,
        ui->atakKeyBox_4,
    };
    attackNamesBoxes = {
        ui->atakNameBox_1,
        ui->atakNameBox_2,
        ui->atakNameBox_3,
        ui->atakNameBox_4,
    };
    attackMonstersBoxes = {ui->attackMinMonsers_1, ui->attackMinMonsers_2,
                           ui->attackMinMonsers_3, ui->attackMinMonsers_4};
  }
  void getDataFromGUI() {
    autoHuntData.setMinMonToContinue(ui->minMonContinueSpiner->value());
    autoHuntData.setMinMonToStop(ui->minMonStopSpiner->value());

    QList<Spell> spells;
    auto filterType = Spell::SpellType::Attack;
    JsonParser::readSpellsJson(spells, &filterType, &prof->getProfession());
    QStringList runesNames = JsonParser::readRunesNames();

    QVector<AttackMethode> attacks = {};
    for (int i = 0; i < checkBoxes.size(); i++) {
      if (!checkBoxes[i]->isChecked())
        continue;
      QString name = attackNamesBoxes[i]->currentText();
      Key key(attackKeysBoxes[i]->currentText());
      int monstersAmount = attackMonstersBoxes[i]->value();
      for each (Spell spell in spells) {
        if (spell.getIncantation() != name)
          continue;
        AttackMethode toAdd(spell, key, monstersAmount);
        attacks.push_back(toAdd);
      }
      for each (QString runeName in runesNames) {
        if (runeName != name)
          continue;
        AttackMethode toAdd(runeName, key, monstersAmount);
        attacks.push_back(toAdd);
      }
    }
    autoHuntData.setAttacks(attacks);
  }
  AutoHuntData autoHuntData = AutoHuntData();
  QString lastSelectedRoute = QString();
};
