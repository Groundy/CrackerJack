#include "NewProfileConfiguartor.h"

//const dest
NewProfileConfiguartor::NewProfileConfiguartor(Profile* prof, QWidget* parent) : QDialog(parent), profToEdit(prof) {
  ui = new Ui::NewProfileConfiguartor();
  ui->setupUi(this);
  fillGuiPtrs();

  auto                allPossibleKeyNames = Key::getListOfAllPossibleKeys();
  QVector<QComboBox*> comboBoxesToFill    = {ui->autoAttackBox, ui->shovelBox, ui->ropeBox, ui->screenShotBox};
  for each (auto box in comboBoxesToFill) {
    box->insertItems(0, allPossibleKeyNames);
    box->setCurrentIndex(-1);
  }
}
NewProfileConfiguartor::~NewProfileConfiguartor() {
  delete ui;
}

//correctnessCheck
bool NewProfileConfiguartor::checkNameGroup() {
  const QString nameOfProf  = ui->nameField->text();
  const bool    wrongLength = nameOfProf.size() > 50 || nameOfProf.size() < 3;
  if (wrongLength) {
    const QString msg = "Profile name must be between 3 and 50 characters long";
    qDebug() << msg;
    Utilities::showMessageBox_INFO(msg);
    return false;
  }

  const bool nameConsistForbiddenChars = nameOfProf.contains(QChar::LineFeed) || nameOfProf.contains(QChar::CarriageReturn);
  if (nameConsistForbiddenChars) {
    const QString msg = "Profile name can't have any special characters, please use only letters, numbers or spaces.";
    qDebug() << msg;
    Utilities::showMessageBox_INFO(msg);
    return false;
  }

  for each (auto character in nameOfProf) {
    bool allowed = character.isLetterOrNumber() || character.isSpace();
    if (!allowed) {
      const QString msg = "Profile name can't have any special characters, please use only letters, numbers or spaces.";
      qDebug() << msg;
      Utilities::showMessageBox_INFO(msg);
      return false;
    }
  }
  return true;
}
bool NewProfileConfiguartor::checkControlsGroup() {
  //change //TODO

  int  indexOfControlBox = ui->controlsBox->currentIndex();
  bool controlsWrong     = indexOfControlBox < 0 || indexOfControlBox > ui->controlsBox->count() - 1 || indexOfControlBox < 0;
  if (controlsWrong) {
    QString msg = "Something is wrong with selected controls";
    qWarning() << msg;
    Utilities::showMessageBox_INFO(msg);
    return false;
  }
  int  indexOfAutoLootlBox = ui->autoLootBox->currentIndex();
  bool autoLootWrongs      = indexOfControlBox < 0 || indexOfAutoLootlBox > ui->autoLootBox->count() - 1 || indexOfAutoLootlBox < 0;
  if (autoLootWrongs) {
    QString msg = "Something is wrong with selected auto loot key";
    qWarning() << msg;
    Utilities::showMessageBox_INFO(msg);
    return false;
  }
  return true;
}
bool NewProfileConfiguartor::checkSlidersGroup(GuiPointers guiPtrs) {
  int activeElements = guiPtrs.activeElementsCounter->value();
  if (activeElements == 0) {
    return true;
  }

  QVector<int> slidersValue;
  QStringList  keyNames, methodeNames;
  for (size_t i = 0; i < activeElements; i++) {
    slidersValue.push_back(guiPtrs.sliders[i]->value());
    keyNames.push_back(guiPtrs.keyShortCuts[i]->currentText());
    methodeNames.push_back(guiPtrs.methodeNames[i]->currentText());
  }

  if (activeElements > 1) {
    for (int i = 1; i < activeElements; i++) {
      if (slidersValue[i - 1] < slidersValue[i]) {
        QString msg = "Sliders are in wrong order, please set it from biggest value to lowest.";
        qWarning() << msg;
        Utilities::showMessageBox_INFO(msg);
        return false;
      }
      if (slidersValue[i - 1] == slidersValue[i]) {
        QString msg = "Two sliders can't be in the same position.";
        qWarning() << msg;
        Utilities::showMessageBox_INFO(msg);
        return false;
      }
    }
  }

  if (slidersValue.last() == 0) {
    QString msg = "Last slider has to have value above zero.";
    qWarning() << msg;
    Utilities::showMessageBox_INFO(msg);
    return false;
  }

  for (size_t i = 0; i < activeElements; i++) {
    if (keyNames[i].isEmpty()) {
      QString msg = "One of key field doesn't have hotkey assigned to itself.";
      qWarning() << msg;
      Utilities::showMessageBox_INFO(msg);
      return false;
    }
    if (methodeNames[i].isEmpty()) {
      QString msg = "Methode field can't be empty.";
      qWarning() << msg;
      Utilities::showMessageBox_INFO(msg);
      return false;
    }
  }

  if (keyNames.removeDuplicates() > 0) {
    QString msg = "Two key fields can't share the same key.";
    qWarning() << msg;
    Utilities::showMessageBox_INFO(msg);
    return false;
  }
  if (methodeNames.removeDuplicates() > 0) {
    QString msg = "Two fields can't share the same value.";
    qWarning() << msg;
    Utilities::showMessageBox_INFO(msg);
    return false;
  }

  return true;
}
bool NewProfileConfiguartor::checkHealthGroup() {
  return checkSlidersGroup(healthPtrs);
}
bool NewProfileConfiguartor::checkManaGroup() {
  return checkSlidersGroup(manaPtrs);
}
bool NewProfileConfiguartor::checkAllKeyBoxes() {
  QVector<QComboBox*> keyBoxes;
  keyBoxes.append(healthPtrs.keyShortCuts);
  keyBoxes.append(manaPtrs.keyShortCuts);
  keyBoxes.append(ui->screenShotBox);
  QStringList keysName;
  for each (auto keyBox in keyBoxes) {
    if (!keyBox->isEnabled()) continue;
    if (keyBox->currentIndex() == -1) {
      Utilities::showMessageBox_INFO("Found one key box without value!");
      return false;
    }
    keysName.append(keyBox->currentText());
  }
  int duplicatedKeys = keysName.removeDuplicates();
  if (duplicatedKeys != 0) {
    Utilities::showMessageBox_INFO("some key boxes have the same value!");
    return false;
  }
  return true;
}
bool NewProfileConfiguartor::checkAllForms() {
  if (!checkNameGroup()) return false;
  if (!checkHealthGroup()) return false;
  if (!checkManaGroup()) return false;
  if (!checkControlsGroup()) return false;
  if (!checkAllKeyBoxes()) return false;
  return true;
}

//slots
void NewProfileConfiguartor::finishButtonAction() {
  if (!checkAllForms()) return;

  QString text     = tr("Are you sure that you want finish creating new profile?");
  bool    accepted = Utilities::showMessageBox_NO_YES(text);
  if (accepted) {
    saveDataToProfile();
    JsonParser::saveProfile(profToEdit);
    this->accept();
  }
}
void NewProfileConfiguartor::cancelButtonAction() {
  QString text     = tr("Are you sure that you want cancel edit/adding new profile?");
  bool    accepted = Utilities::showMessageBox_NO_YES(text);
  if (accepted) this->reject();
}
void NewProfileConfiguartor::healingGroupCounterChanged() {
  counterChanged(healthPtrs);
  fillRestoriationMethodes(getSelectedProf(), healthPtrs);
  refreshSlidersLabels(healthPtrs);
}
void NewProfileConfiguartor::manaGroupCounterChanged() {
  counterChanged(manaPtrs);
  fillRestoriationMethodes(getSelectedProf(), manaPtrs);
  refreshSlidersLabels(manaPtrs);
}
void NewProfileConfiguartor::healingSlidersChanged() {
  refreshSlidersLabels(healthPtrs);
}
void NewProfileConfiguartor::manaSlidersChanged() {
  refreshSlidersLabels(manaPtrs);
}
void NewProfileConfiguartor::controlsOrAutoLootListAction() {
  bool enableAutoLoot = ui->autoLootBox->currentIndex() == Profile::Controls::CLSSIC_CONTROLS;
  ui->autoLootBox->setEnabled(enableAutoLoot);
  ui->autoLootBox->setCurrentIndex(0);
  ui->autoLootBox->repaint();
}
void NewProfileConfiguartor::professionChanged() {
  Profession profession = getSelectedProf();
  fillRestoriationMethodes(profession, manaPtrs);
  fillRestoriationMethodes(profession, healthPtrs);
  for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) {
    healthPtrs.methodeNames[i]->setCurrentIndex(-1);
    manaPtrs.methodeNames[i]->setCurrentIndex(-1);
  }
}
void NewProfileConfiguartor::openAuoHuntSettings() {
  AutoHuntConfigurator(this, profToEdit).exec();
}
//funcs
Profession NewProfileConfiguartor::getSelectedProf() {
  QString profName = ui->professionCombBox->currentText();
  return Profession(profName);
}
void NewProfileConfiguartor::refreshSlidersLabels(GuiPointers guiPointers) {
  int activeElements = guiPointers.activeElementsCounter->value();
  if (activeElements == 0) {
    for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) guiPointers.percentageLabels[i]->setText(QString());
    return;
  }

  QStringList textToSetOnSliders;
  for (int i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) {
    bool shouldBeEnabled = i < activeElements;
    if (!shouldBeEnabled) {
      textToSetOnSliders.push_back(QString());
      continue;
    }

    bool    isLastActiveSlider = i == activeElements - 1;
    QString valueOfSliderStr   = QString::number(guiPointers.sliders[i]->value());
    if (isLastActiveSlider) {
      QString txtToSet = QString::number(0) + "% - " + valueOfSliderStr + "%";
      textToSetOnSliders.push_back(txtToSet);
    } else {
      int     valueOfPreviousSlider = guiPointers.sliders[i + 1]->value();
      QString txtToSet              = QString::number(valueOfPreviousSlider) + "% - " + valueOfSliderStr + "%";
      textToSetOnSliders.push_back(txtToSet);
    }
  }
  if (textToSetOnSliders.size() != MAX_NUMBER_OF_ACTIVE_WIDGETS) {
    //todo
  }

  for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) guiPointers.percentageLabels[i]->setText(textToSetOnSliders[i]);
}
void NewProfileConfiguartor::fillSlidersGroup(GuiPointers guiPointers, QVector<RestorationMethode> restorationsMethodes) {
  int size = restorationsMethodes.size();
  guiPointers.activeElementsCounter->setValue(size);
  for (size_t i = 0; i < size; i++) {
    RestorationMethode* current = &restorationsMethodes[i];
    guiPointers.sliders[i]->setValue(current->getThreshold());

    auto keyBox = guiPointers.keyShortCuts[i];
    keyBox->setEnabled(true);
    int keyIndex = keyBox->findText(current->getKeyName());
    keyBox->setCurrentIndex(keyIndex);

    auto methodeBox = guiPointers.methodeNames[i];
    methodeBox->setEnabled(true);
    int methodeIndex = methodeBox->findText(current->getName());
    methodeBox->setCurrentIndex(methodeIndex);
  }
}
void NewProfileConfiguartor::fillRestoriationMethodes(Profession profession, GuiPointers& guiPointers) {
  QStringList methodesNames = {};
  if (&guiPointers == &healthPtrs)
    methodesNames = JsonParser::getNamesOfHealingPotsAndSpellsForProf(profession);
  else if (&guiPointers == &manaPtrs)
    methodesNames = JsonParser::getNamesOManaPotsForProf(profession);
  else
    return;

  int         slotsToFill     = guiPointers.activeElementsCounter->value();
  QStringList allPossibleKeys = Key::getListOfAllPossibleKeys();
  for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) {
    QComboBox* methodeBox = guiPointers.methodeNames[i];
    QComboBox* keyBox     = guiPointers.keyShortCuts[i];
    methodeBox->clear();
    keyBox->clear();
    bool enable = i < slotsToFill;
    if (enable) {
      methodeBox->insertItems(0, methodesNames);
      methodeBox->setCurrentIndex(-1);
      keyBox->insertItems(0, allPossibleKeys);
      keyBox->setCurrentIndex(-1);
    }
  }
}
void NewProfileConfiguartor::fillGuiPtrs() {
  healthPtrs.sliders.push_back(ui->healthSlider_1);
  healthPtrs.sliders.push_back(ui->healthSlider_2);
  healthPtrs.sliders.push_back(ui->healthSlider_3);
  healthPtrs.sliders.push_back(ui->healthSlider_4);
  healthPtrs.sliders.push_back(ui->healthSlider_5);

  manaPtrs.sliders.push_back(ui->manaSlider_1);
  manaPtrs.sliders.push_back(ui->manaSlider_2);
  manaPtrs.sliders.push_back(ui->manaSlider_3);
  manaPtrs.sliders.push_back(ui->manaSlider_4);
  manaPtrs.sliders.push_back(ui->manaSlider_5);

  healthPtrs.percentageLabels.push_back(ui->healthLabel_1);
  healthPtrs.percentageLabels.push_back(ui->healthLabel_2);
  healthPtrs.percentageLabels.push_back(ui->healthLabel_3);
  healthPtrs.percentageLabels.push_back(ui->healthLabel_4);
  healthPtrs.percentageLabels.push_back(ui->healthLabel_5);

  manaPtrs.percentageLabels.push_back(ui->manaLabel_1);
  manaPtrs.percentageLabels.push_back(ui->manaLabel_2);
  manaPtrs.percentageLabels.push_back(ui->manaLabel_3);
  manaPtrs.percentageLabels.push_back(ui->manaLabel_4);
  manaPtrs.percentageLabels.push_back(ui->manaLabel_5);

  healthPtrs.methodeNames.push_back(ui->healthMethodeName_1);
  healthPtrs.methodeNames.push_back(ui->healthMethodeName_2);
  healthPtrs.methodeNames.push_back(ui->healthMethodeName_3);
  healthPtrs.methodeNames.push_back(ui->healthMethodeName_4);
  healthPtrs.methodeNames.push_back(ui->healthMethodeName_5);

  manaPtrs.methodeNames.push_back(ui->manaMethodeName_1);
  manaPtrs.methodeNames.push_back(ui->manaMethodeName_2);
  manaPtrs.methodeNames.push_back(ui->manaMethodeName_3);
  manaPtrs.methodeNames.push_back(ui->manaMethodeName_4);
  manaPtrs.methodeNames.push_back(ui->manaMethodeName_5);

  healthPtrs.keyShortCuts.push_back(ui->healthKey_1);
  healthPtrs.keyShortCuts.push_back(ui->healthKey_2);
  healthPtrs.keyShortCuts.push_back(ui->healthKey_3);
  healthPtrs.keyShortCuts.push_back(ui->healthKey_4);
  healthPtrs.keyShortCuts.push_back(ui->healthKey_5);

  manaPtrs.keyShortCuts.push_back(ui->manaKey_1);
  manaPtrs.keyShortCuts.push_back(ui->manaKey_2);
  manaPtrs.keyShortCuts.push_back(ui->manaKey_3);
  manaPtrs.keyShortCuts.push_back(ui->manaKey_4);
  manaPtrs.keyShortCuts.push_back(ui->manaKey_5);

  healthPtrs.activeElementsCounter = ui->amountHealthRegMethodes;
  manaPtrs.activeElementsCounter   = ui->amountManaRegMethodes;
}
void NewProfileConfiguartor::counterChanged(GuiPointers guiPointers) {
  for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) {
    bool enable = i < guiPointers.activeElementsCounter->value();
    guiPointers.keyShortCuts[i]->setEnabled(enable);
    guiPointers.methodeNames[i]->setEnabled(enable);
    guiPointers.sliders[i]->setEnabled(enable);
    guiPointers.percentageLabels[i]->setEnabled(enable);
    if (!enable) {
      //guiPointers.keyShortCuts[i]->setEnabled(enable);
      //guiPointers.methodeNames[i]->s(enable);
      guiPointers.sliders[i]->setValue(0);
      guiPointers.percentageLabels[i]->setText(QString());
    }
  }
}
void NewProfileConfiguartor::fillFormsFromDataFromProf(Profile& prof) {
  ui->nameField->setText(prof.getName());

  auto    profComboBox    = ui->professionCombBox;
  QString professionName  = prof.getProfession().getTypeName();
  int     professionIndex = profComboBox->findText(professionName);
  profComboBox->setCurrentIndex(professionIndex);

  int screenShotKeyIndex = ui->screenShotBox->findText(prof.getScreenShotKey().getKeyName());
  ui->screenShotBox->setCurrentIndex(screenShotKeyIndex);

  fillSlidersGroup(healthPtrs, prof.getRestMethodesHealth());
  fillSlidersGroup(manaPtrs, prof.getRestMethodesMana());

  ui->controlsBox->setCurrentIndex(prof.getControls());
  ui->autoLootBox->setCurrentIndex(prof.getAutoLoot());
  ui->leftBarsCounter->setValue(prof.getBarsLeft());
  ui->rightBarsCounter->setValue(prof.getBarsRight());

  int autoAttackKeyIndex = ui->autoAttackBox->findText(prof.getAutoAttackKey().getKeyName());
  ui->autoAttackBox->setCurrentIndex(autoAttackKeyIndex);

  int ropeKeyIndex = ui->ropeBox->findText(prof.getRopeKey().getKeyName());
  ui->ropeBox->setCurrentIndex(ropeKeyIndex);

  int shovelKeyIndex = ui->shovelBox->findText(prof.getShovelKey().getKeyName());
  ui->shovelBox->setCurrentIndex(shovelKeyIndex);
}
QVector<RestorationMethode> NewProfileConfiguartor::getRestorationMethodesFromGUI(GuiPointers guiPotiners) {
  QVector<RestorationMethode> toRet          = {};
  int                         activeElements = guiPotiners.activeElementsCounter->value();
  for (size_t i = 0; i < activeElements; i++) {
    int     threshold   = guiPotiners.sliders[i]->value();
    QString keyName     = guiPotiners.keyShortCuts[i]->currentText();
    QString methodeName = guiPotiners.methodeNames[i]->currentText();
    toRet.push_back(RestorationMethode(threshold, Key(keyName), methodeName));
  }
  fillRestorationMethodesDetails(toRet);
  return toRet;
}
void NewProfileConfiguartor::saveDataToProfile() {
  profToEdit->setName(ui->nameField->text());
  profToEdit->setProfession(getSelectedProf());
  profToEdit->setRestMethodesHealth(getRestorationMethodesFromGUI(healthPtrs));
  profToEdit->setRestMethodesMana(getRestorationMethodesFromGUI(manaPtrs));
  profToEdit->setScreenShotKey(Key(ui->screenShotBox->currentText()));
  profToEdit->setBars(ui->leftBarsCounter->value(), ui->rightBarsCounter->value());
  profToEdit->setControls(ui->controlsBox->currentIndex());
  profToEdit->setAutoLoot(ui->autoLootBox->currentIndex());
  profToEdit->setShovelKey(Key(ui->shovelBox->currentText()));
  profToEdit->setAutoAttackKey(Key(ui->autoAttackBox->currentText()));
  profToEdit->setRopeKey(Key(ui->ropeBox->currentText()));
}
void NewProfileConfiguartor::fillRestorationMethodesDetails(QVector<RestorationMethode>& restorationMethodes) {
  if (restorationMethodes.size() == 0) return;

  QList<Spell>  spells;
  QList<Potion> potions;
  Profession    profession = getSelectedProf();
  auto          spellType  = Spell::SpellType::Healing;
  JsonParser::readSpellsJson(spells, &spellType, &profession);
  JsonParser::readPotions(potions, &profession, NULL);

  for (size_t i = 0; i < restorationMethodes.size(); i++) {
    QString name       = restorationMethodes[i].getName();
    bool    alreadySet = false;
    for each (auto spell in spells) {
      if (spell.getIncantation() == name) {
        restorationMethodes[i].fillDataDetails(spell);
        alreadySet = true;
        break;
      }
    }
    if (alreadySet) continue;

    for each (auto potion in potions) {
      if (potion.getName() == name) {
        restorationMethodes[i].fillDataDetails(potion);
        alreadySet = true;
        break;
      }
    }
    if (!alreadySet) throw std::exception("Cant get details for rest methode");
  }
}
