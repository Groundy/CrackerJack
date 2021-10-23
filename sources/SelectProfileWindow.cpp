#include "SelectProfileWindow.h"
#include "ui_SelectProfileWindow.h"

SelectProfileWindow::SelectProfileWindow(QWidget *parent, Profile* prof) : QDialog(parent){
	ui = new Ui::SelectProfileWindow();
	ui->setupUi(this);
	setFixedSize(this->size());
	setUpGui();
	prepareProfiles();
	ui->listOfProfs->setItemSelected(NULL, true);
	selectListAction();
	profToSelect = prof;
	readAndSetLastUsedProFromINI();
}

SelectProfileWindow::~SelectProfileWindow(){
	delete ui;
	this->reject();
}

void SelectProfileWindow::prepareProfiles(){
	ui->listOfProfs->clear();
	ProfileDataBaseManager dbManager;
	QStringList list;
	bool ok = dbManager.getNamesOfAllFilesInFormToDisplay(list);
	for each (QString str in list)
		ui->listOfProfs->addItem(str);
	ui->listOfProfs->repaint();
}

void SelectProfileWindow::setUpGui(){
	ui->addProfileButton->setText(tr("Add profile"));
	ui->addProfileButton->repaint();


	ui->editProfileButton->setText(tr("Edit profile"));
	ui->editProfileButton->repaint();


	ui->deleteProfileButton->setText(tr("Delete profile"));
	ui->deleteProfileButton->repaint();

	ui->label->setText(tr("Select Profile"));
	ui->label->repaint();
}

void SelectProfileWindow::readAndSetLastUsedProFromINI(){
	QString lastUsedProfName = Utilities::readFromIniFile(Utilities::FieldsOfIniFile::LAST_USED_PROFILE_NAME);
	if (lastUsedProfName.size() == 0)
		return;
	QStringList listOfDisplayNames;
	ProfileDataBaseManager dbManager;
	bool ok = dbManager.getNamesOfAllFilesInFormToDisplay(listOfDisplayNames);
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
}

void SelectProfileWindow::addNewProfileButtonAction(){
	Profile prof;
	NewProfileConfiguartor newProfWind(&prof,this);
	int result = newProfWind.exec();
	if (result == QDialog::Accepted) {
		ProfileDataBaseManager dbManager;
		dbManager.saveProfileToDataBase(prof);
	}
	Sleep(20);
	prepareProfiles();
}

void SelectProfileWindow::editProfileButtonAction(){
	int row = ui->listOfProfs->currentRow();
	QString nameOfProfToSplit = ui->listOfProfs->item(row)->text();
	QStringList nameParts = nameOfProfToSplit.split("] ");
	QString profileName = nameParts[1];

	Profile profToBeRead;
	ProfileDataBaseManager dbManager;
	bool readCompleted = dbManager.readProfileFromDataBase(profileName, profToBeRead);
	//TODO obsluga w przypadku gdy profil zostal zle wczytany
	NewProfileConfiguartor* newProfDialog = new NewProfileConfiguartor(&profToBeRead, this);
	newProfDialog->fillWidgetsWithDataFromProf(&profToBeRead);
	int result = newProfDialog->exec();
	bool accepted = result == QDialog::Accepted;
	if (accepted) {
		dbManager.deleteProfile(profileName);
		dbManager.saveProfileToDataBase(profToBeRead);
	}
	delete newProfDialog;
	prepareProfiles();
}

void SelectProfileWindow::deleteProfileButtonAction(){
	QString profileNameToDelete;
	profileNameToDelete = ui->listOfProfs->selectedItems().first()->text();
	QStringList choppedString = profileNameToDelete.split("] ");
	QString onlyProfileName;
	if (choppedString.size() >= 2)
		onlyProfileName = choppedString[1];
	else {
		Logger::logPotenialBug("Name of profile to delete is diffrent from profile file name.","SelectProfileWindow","deleteProfileButtonAction");
		return;
	}
	
	QString msgText = QObject::tr("Do you really want to delete profile ") + onlyProfileName  + " ?";
	bool accepted = Utilities::showMessageBox_NO_YES(msgText);
	if (accepted) {
		ProfileDataBaseManager dbManager;
		dbManager.deleteProfile(onlyProfileName);
		prepareProfiles();
		ui->listOfProfs->setItemSelected(NULL, true);
	}
}

void SelectProfileWindow::selectListAction(){
	bool oneItemIsSelected = ui->listOfProfs->selectedItems().size() == 1;
	ui->deleteProfileButton->setEnabled(oneItemIsSelected);
	ui->editProfileButton->setEnabled(oneItemIsSelected);
}

void SelectProfileWindow::profSelected(){
	int row = ui->listOfProfs->currentRow();
	QString nameOfProfToSplit = ui->listOfProfs->item(row)->text();
	QStringList nameParts = nameOfProfToSplit.split("] ");
	QString profileName;
	ProfileDataBaseManager dbManager;
	if (nameParts.size() >= 2) 
		profileName = nameParts[1];
	else {
		Logger::logPotenialBug("Name of profile to choose is diffrent from profile file name.", "SelectProfileWindow", "profSelected");
		prepareProfiles();
		return;
	}

	bool error = !dbManager.readProfileFromDataBase(profileName, *profToSelect);
	if (error) {
		Logger::logPotenialBug("Profile can't be read", "SelectProfileWindow", "profSelected");
		prepareProfiles();
		return;
	}

	Utilities::writeIniFile(Utilities::FieldsOfIniFile::LAST_USED_PROFILE_NAME, profileName);
	LONG64 currentTimeInSec = Utilities::getCurrentTimeInMiliSeconds() / 1000;
	QString lastLoginInSecToSet = QString::number(currentTimeInSec);
	dbManager.modifyFieldValue(profileName, ProfileDataBaseManager::LAST_LOGIN, lastLoginInSecToSet);
	this->accept();

}

