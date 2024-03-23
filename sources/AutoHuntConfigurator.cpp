#include "AutoHuntConfigurator.h"
namespace CJ {
AutoHuntConfigurator::AutoHuntConfigurator(QWidget* parent, Profile* profile)
    : QDialog(parent), prof_(profile), ui(new Ui::AutoHuntConfiguratorClass()) {
  ui->setupUi(this);
  fillGuiFromProfileData(profile);
}

AutoHuntConfigurator::~AutoHuntConfigurator() {
  delete ui;
}

void AutoHuntConfigurator::selectRoute() {
  const QString dirPath    = PathResource::getProfileFolder().absolutePath();
  const QString pathToFile = Utilities::getFileByDialog("*.json", dirPath);
  const QString routeName  = JsonParser::readRoute(pathToFile).getName();
  ui->routeLabel->setText(routeName);
  last_selected_route_ = routeName;
};

void AutoHuntConfigurator::editRoute() {
  if (last_selected_route_.isEmpty()) {
    return;
  }

  Route        route = JsonParser::readRoute(last_selected_route_);
  RouteCreator routeCreator(this, &route);
  routeCreator.exec();
};

void AutoHuntConfigurator::saveButtonClicked() {
  getDataFromGUI();
  prof_->setAutoHuntData(auto_hunt_data_);
  this->accept();
};

void AutoHuntConfigurator::cancelButtonClicked() {
  this->reject();
}

void AutoHuntConfigurator::checkBoxChanged() {
  int lastActiveCheckboxIndex = -1;
  foreach (auto checkBox, check_boxes_) {
    if (checkBox->isChecked()) {
      lastActiveCheckboxIndex++;
    }
  }

  for (int i = 0; i < check_boxes_.size(); i++) {
    bool setEnable = check_boxes_[i]->isChecked();
    attack_keys_boxes_[i]->setEnabled(setEnable);
    attack_monsters_boxes_[i]->setEnabled(setEnable);
    attack_names_boxes_[i]->setEnabled(setEnable);
    bool isLast = i == check_boxes_.size() - 1;
    if (!isLast && setEnable && i <= lastActiveCheckboxIndex) {
      check_boxes_[i + 1]->setEnabled(true);
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
  foreach (auto keyBox, attackKeysBoxes) {
    keyBox->insertItems(0, allPosibleKeyNames);
    keyBox->setCurrentIndex(-1);
  }
}

void AutoHuntConfigurator::fillGuiFromProfileData(Profile* prof) {
  auto data       = prof->getAutoHuntData();
  auto_hunt_data_ = data;
  ui->minMonContinueSpiner->setValue(data.getMinMonToContinue());
  ui->minMonStopSpiner->setValue(data.getMinMonToStop());
  auto attacks = data.getAttacks();
  for (int i = 0; i < attacks.size(); i++) {
    check_boxes_[i]->setEnabled(true);
    check_boxes_[i]->setChecked(true);
    attack_keys_boxes_[i]->setCurrentIndex(attack_keys_boxes_[i]->findText(attacks[i].getKey().getKeyName()));
  }

  QStringList namesToFill = getNamesOfAttacksMethodes(prof->getProfession());
  foreach (auto var, attack_names_boxes_) {
    var->insertItems(0, namesToFill);
    var->setCurrentIndex(-1);
  }
  for (int i = 0; i < attacks.size(); i++) {
    attack_names_boxes_[i]->setCurrentIndex(attack_names_boxes_[i]->findText(attacks[i].getName()));
  }

  auto allPossibleKeyNames = Key::getListOfAllPossibleKeys();
  foreach (auto box, attack_keys_boxes_) {
    box->addItems(allPossibleKeyNames);
    box->setCurrentIndex(-1);
  }

  int i = 0;
  foreach (AttackMethode obj, prof->getAutoHuntData().getAttacks()) {
    attack_names_boxes_[i]->setEnabled(true);
    attack_keys_boxes_[i]->setEnabled(true);
    attack_monsters_boxes_[i]->setEnabled(true);
    check_boxes_[i]->setEnabled(true);
    attack_names_boxes_[i]->setCurrentIndex(attack_names_boxes_[i]->findText(obj.getName()));
    attack_keys_boxes_[i]->setCurrentIndex(attack_keys_boxes_[i]->findText(obj.getKey().getKeyName()));
    attack_monsters_boxes_[i]->setValue(obj.getMinMonsters());
    check_boxes_[i]->setChecked(true);
    i++;
  }
}

QStringList AutoHuntConfigurator::getNamesOfAttacksMethodes(const Profession&& profession) {
  QStringList    namesToFill;
  QVector<Spell> spells = JsonParser::readSpellsJson(Spell::SpellType::Attack, std::move(profession));
  namesToFill.append(JsonParser::readRunesNames());
  foreach (Spell spell, spells) {
    namesToFill.append(spell.getIncantation());
  };
  namesToFill.sort();
  return namesToFill;
};

void AutoHuntConfigurator::getDataFromGUI() {
  auto_hunt_data_.setMinMonToContinue(ui->minMonContinueSpiner->value());
  auto_hunt_data_.setMinMonToStop(ui->minMonStopSpiner->value());

  QVector<Spell> spells     = JsonParser::readSpellsJson(Spell::SpellType::Attack, prof_->getProfession());
  QStringList    runesNames = JsonParser::readRunesNames();

  QVector<AttackMethode> attacks = {};
  for (int i = 0; i < check_boxes_.size(); i++) {
    if (!check_boxes_[i]->isChecked()) continue;
    QString name = attack_names_boxes_[i]->currentText();
    Key     key(attack_keys_boxes_[i]->currentText());
    int     monstersAmount = attack_monsters_boxes_[i]->value();
    foreach (Spell spell, spells) {
      if (spell.getIncantation() != name) continue;
      AttackMethode toAdd(spell, key, monstersAmount);
      attacks.push_back(toAdd);
    }
    foreach (QString runeName, runesNames) {
      if (runeName != name) continue;
      AttackMethode toAdd(runeName, key, monstersAmount);
      attacks.push_back(toAdd);
    }
  }
  auto_hunt_data_.setAttacks(attacks);
}
}  // namespace CJ
