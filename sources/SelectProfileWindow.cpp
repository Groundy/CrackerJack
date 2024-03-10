#include "SelectProfileWindow.h"

#include "ui_SelectProfileWindow.h"

//const
SelectProfileWindow::SelectProfileWindow(QSharedPointer<Profile> profileToBeChoosen, QWidget* parent) : QDialog(parent) {
  ui = new Ui::SelectProfileWindow();
  ui->setupUi(this);
  this->profileToBeChoosen = profileToBeChoosen;
  setFixedSize(this->size());
  setUpGui();
  refreshProfilesOnList();
  ui->listOfProfs->setItemSelected(NULL, true);
  selectListAction();
}
SelectProfileWindow::~SelectProfileWindow() {
  delete ui;
  this->reject();
}

//slots
void SelectProfileWindow::addNewProfileButtonAction() {
  Profile                profile;
  NewProfileConfiguartor newProfWind(&profile, this);
  int                    result = newProfWind.exec();
  if (result == QDialog::Accepted) {
    JsonParser::saveProfile(&profile);
    Sleep(20);
    refreshProfilesOnList();
  }
}
void SelectProfileWindow::editProfileButtonAction() {
  QString profName      = getSelectedProfName();
  Profile profToEdit    = JsonParser::loadProfile(profName);
  auto    newProfDialog = std::make_unique<NewProfileConfiguartor>(&profToEdit, this);
  newProfDialog->fillFormsFromDataFromProf(profToEdit);
  bool accepted = newProfDialog->exec() == QDialog::Accepted;
  if (accepted) {
    JsonParser::saveProfile(&profToEdit);
    refreshProfilesOnList();
  }
}
void SelectProfileWindow::deleteProfileButtonAction() {
  QString profileNameToDelete = getSelectedProfName();
  QString msgText             = QObject::tr("Do you really want to delete profile ") + profileNameToDelete + " ?";
  bool    accepted            = Utilities::showMessageBox_NO_YES(msgText);
  if (accepted) {
    JsonParser::deleteProfileFile(profileNameToDelete);
    refreshProfilesOnList();
    ui->listOfProfs->setItemSelected(NULL, true);
  }
}
void SelectProfileWindow::selectListAction() {
  bool oneItemIsSelected = ui->listOfProfs->selectedItems().size() == 1;
  ui->deleteProfileButton->setEnabled(oneItemIsSelected);
  ui->editProfileButton->setEnabled(oneItemIsSelected);
}
void SelectProfileWindow::profSelected() {
  QString profName    = getSelectedProfName();
  *profileToBeChoosen = JsonParser::loadProfile(profName);
  this->accept();
}

//funcs
void SelectProfileWindow::refreshProfilesOnList() {
  const int EXTENSION_SIZE = QString(".json").size();
  ui->listOfProfs->clear();
  QStringList list = JsonParser::readNamesOfAllSavedProfiles();
  for each (QString fileName in list) {
    bool tooShortName = fileName.size() < (EXTENSION_SIZE + 1);
    if (tooShortName) {
      QString msg = "Too short profile json file name";
      qWarning() << msg;
      ui->listOfProfs->clear();
      Utilities::showMessageBox_INFO(msg);
      return;
    }
    QString profName = fileName.left(fileName.size() - EXTENSION_SIZE);
    ui->listOfProfs->addItem(profName);
  }
  ui->listOfProfs->repaint();
}
void SelectProfileWindow::setUpGui() {
  ui->addProfileButton->setText(tr("Add profile"));
  ui->addProfileButton->repaint();

  ui->editProfileButton->setText(tr("Edit profile"));
  ui->editProfileButton->repaint();

  ui->deleteProfileButton->setText(tr("Delete profile"));
  ui->deleteProfileButton->repaint();

  ui->label->setText(tr("Select Profile"));
  ui->label->repaint();
}

QString SelectProfileWindow::getSelectedProfName() {
  int     row         = ui->listOfProfs->currentRow();
  QString profileName = ui->listOfProfs->item(row)->text();
  return profileName;
}
