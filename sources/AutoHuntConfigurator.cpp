#include "AutoHuntConfigurator.h"

AutoHuntConfigurator::AutoHuntConfigurator(QWidget* parent, Profile* profile)
    : QDialog(parent), prof(profile), ui(new Ui::AutoHuntConfiguratorClass()) {
  ui->setupUi(this);
  fillGuiFromProfileData(profile);
}

AutoHuntConfigurator::~AutoHuntConfigurator() {
  delete ui;
}

void AutoHuntConfigurator::selectRoute() {
  const QString dirPath    = PathResource::getProfileFolder().absolutePath();
  QString       pathToFile = Utilities::getFileByDialog("*.json", dirPath);
  QJsonObject   obj;
  if (!JsonParser::openJsonFile(obj, pathToFile)) {
    return;
  }  // todo

  QString name = Route(obj).getName();
  ui->routeLabel->setText(name);
  lastSelectedRoute_ = name;
};

void AutoHuntConfigurator::editRoute() {
  if (lastSelectedRoute_.isEmpty()) {
    return;
  }

  Route route;
  JsonParser::readRoute(route, lastSelectedRoute_);
  RouteCreator routeCreator(this, &route);
  routeCreator.exec();
};

void AutoHuntConfigurator::saveButtonClicked() {
  getDataFromGUI();
  prof->setAutoHuntData(autoHuntData);
  this->accept();
};

void AutoHuntConfigurator::cancelButtonClicked() {
  this->reject();
}

void AutoHuntConfigurator::checkBoxChanged() {
  int lastActiveCheckboxIndex = -1;
  for each (auto checkBox in checkBoxes_) {
    if (checkBox->isChecked()) {
      lastActiveCheckboxIndex++;
    }
  }

  for (int i = 0; i < checkBoxes_.size(); i++) {
    bool setEnable = checkBoxes_[i]->isChecked();
    attackKeysBoxes_[i]->setEnabled(setEnable);
    attackMonstersBoxes_[i]->setEnabled(setEnable);
    attackNamesBoxes_[i]->setEnabled(setEnable);
    bool isLast = i == checkBoxes_.size() - 1;
    if (!isLast && setEnable && i <= lastActiveCheckboxIndex) {
      checkBoxes_[i + 1]->setEnabled(true);
    }
  }
}

void AutoHuntConfigurator::fillKeyComboBoxes() {
  auto                allPosibleKeyNames = Key::getListOfAllPossibleKeys();
  QVector<QComboBox*> attackKeysBoxes    = {
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

void AutoHuntConfigurator::fillGuiFromProfileData(Profile* prof) {
  auto data    = prof->getAutoHuntData();
  autoHuntData = data;
  ui->minMonContinueSpiner->setValue(data.getMinMonToContinue());
  ui->minMonStopSpiner->setValue(data.getMinMonToStop());
  auto attacks = data.getAttacks();
  for (int i = 0; i < attacks.size(); i++) {
    checkBoxes_[i]->setEnabled(true);
    checkBoxes_[i]->setChecked(true);
    attackKeysBoxes_[i]->setCurrentIndex(attackKeysBoxes_[i]->findText(attacks[i].getKey().getKeyName()));
  }

  QStringList namesToFill = getNamesOfAttacksMethodes(prof->getProfession());
  for each (auto var in attackNamesBoxes_) {
    var->insertItems(0, namesToFill);
    var->setCurrentIndex(-1);
  }
  for (int i = 0; i < attacks.size(); i++) {
    attackNamesBoxes_[i]->setCurrentIndex(attackNamesBoxes_[i]->findText(attacks[i].getName()));
  }

  auto allPossibleKeyNames = Key::getListOfAllPossibleKeys();
  for each (auto box in attackKeysBoxes_) {
    box->addItems(allPossibleKeyNames);
    box->setCurrentIndex(-1);
  }

  int i = 0;
  for each (AttackMethode obj in prof->getAutoHuntData().getAttacks()) {
    attackNamesBoxes_[i]->setEnabled(true);
    attackKeysBoxes_[i]->setEnabled(true);
    attackMonstersBoxes_[i]->setEnabled(true);
    checkBoxes_[i]->setEnabled(true);
    attackNamesBoxes_[i]->setCurrentIndex(attackNamesBoxes_[i]->findText(obj.getName()));
    attackKeysBoxes_[i]->setCurrentIndex(attackKeysBoxes_[i]->findText(obj.getKey().getKeyName()));
    attackMonstersBoxes_[i]->setValue(obj.getMinMonsters());
    checkBoxes_[i]->setChecked(true);
    i++;
  }
}

QStringList AutoHuntConfigurator::getNamesOfAttacksMethodes(Profession profession) {
  QList<Spell> spells;
  auto         filterType = Spell::SpellType::Attack;
  JsonParser::readSpellsJson(spells, &filterType, &profession);
  QStringList namesToFill;
  namesToFill.append(JsonParser::readRunesNames());
  for each (Spell spell in spells) namesToFill.append(spell.getIncantation());
  namesToFill.sort();
  return namesToFill;
};

void AutoHuntConfigurator::getDataFromGUI() {
  autoHuntData.setMinMonToContinue(ui->minMonContinueSpiner->value());
  autoHuntData.setMinMonToStop(ui->minMonStopSpiner->value());

  QList<Spell> spells;
  auto         filterType = Spell::SpellType::Attack;
  JsonParser::readSpellsJson(spells, &filterType, &prof->getProfession());
  QStringList runesNames = JsonParser::readRunesNames();

  QVector<AttackMethode> attacks = {};
  for (int i = 0; i < checkBoxes_.size(); i++) {
    if (!checkBoxes_[i]->isChecked()) continue;
    QString name = attackNamesBoxes_[i]->currentText();
    Key     key(attackKeysBoxes_[i]->currentText());
    int     monstersAmount = attackMonstersBoxes_[i]->value();
    for each (Spell spell in spells) {
      if (spell.getIncantation() != name) continue;
      AttackMethode toAdd(spell, key, monstersAmount);
      attacks.push_back(toAdd);
    }
    for each (QString runeName in runesNames) {
      if (runeName != name) continue;
      AttackMethode toAdd(runeName, key, monstersAmount);
      attacks.push_back(toAdd);
    }
  }
  autoHuntData.setAttacks(attacks);
}
