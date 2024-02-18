#include "SelectProfileWindow.h"

#include "ui_SelectProfileWindow.h"

//const
SelectProfileWindow::SelectProfileWindow(Profile* profileToBeChoosen, QWidget* parent)
    : QDialog(parent), profileToBeChoosen(profileToBeChoosen) {
  ui = new Ui::SelectProfileWindow();
  ui->setupUi(this);
  setFixedSize(this->size());
  setUpGui();
  refreshProfilesOnList();
  ui->listOfProfs->setItemSelected(NULL, true);
  selectListAction();
  readAndSetLastUsedProFromINI();
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
  try {
    const int EXTENSION_SIZE = QString(".json").size();
    ui->listOfProfs->clear();
    QStringList list = JsonParser::readNamesOfAllSavedProfiles();
    for each (QString fileName in list) {
      bool tooShortName = fileName.size() < (EXTENSION_SIZE + 1);
      if (tooShortName) throw std::exception("Too short profile json file name");
      QString profName = fileName.left(fileName.size() - EXTENSION_SIZE);
      ui->listOfProfs->addItem(profName);
    }
    ui->listOfProfs->repaint();
  } catch (const std::exception& e) {
    ui->listOfProfs->clear();
    qDebug() << e.what();
    Utilities::showMessageBox_INFO(e.what());
  }
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
void SelectProfileWindow::readAndSetLastUsedProFromINI() {
  /*
	if (lastUsedProfName.size() == 0)
		return;
	QStringList listOfDisplayNames;
	if (!ok)
		return;
	int indexToSet = 0;
	for (size_t i = 0; i < listOfDisplayNames.size(); i++){
		QString nameToSplit = listOfDisplayNames[i];
		QStringList partOfName = nameToSplit.split("] ",Qt::SkipEmptyParts);
		if (partOfName.size() >= 2) {
			QString exactProfName = partOfName[1];
			bool matched = exactProfName == lastUsedProfName;
			if (matched) {
				indexToSet = i;
				break;
			}
		}
	}
	ui->listOfProfs->setCurrentRow(indexToSet);
	ui->listOfProfs->repaint();
	*/
}
QString SelectProfileWindow::getSelectedProfName() {
  int     row         = ui->listOfProfs->currentRow();
  QString profileName = ui->listOfProfs->item(row)->text();
  return profileName;
}
