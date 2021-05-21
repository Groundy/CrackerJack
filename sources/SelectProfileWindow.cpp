#include "SelectProfileWindow.h"
#include "ui_SelectProfileWindow.h"

SelectProfileWindow::SelectProfileWindow(QWidget *parent, Profile* prof) : QDialog(parent){
	ui = new Ui::SelectProfileWindow();
	ui->setupUi(this);
	isPl = StringResource::languageIsPl();
	setFixedSize(this->size());
	setUpGui();
	prepareProfiles();
	ui->listOfProfs->setItemSelected(NULL, true);
	selectListAction();
	profToSelect = prof;
}

SelectProfileWindow::~SelectProfileWindow()
{
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
	QString addProfileButtonText = isPl ? QString::fromLocal8Bit("Dodaj profil") : "Add profile";
	ui->addProfileButton->setText(addProfileButtonText);
	ui->addProfileButton->repaint();


	QString editProfileButtonText = isPl ? QString::fromLocal8Bit("Edytuj profil") : "Edit profile";
	ui->editProfileButton->setText(editProfileButtonText);
	ui->editProfileButton->repaint();


	QString deleteProfileButtonText = isPl ? QString::fromLocal8Bit("Usuñ profil") : "Delete profile";
	ui->deleteProfileButton->setText(deleteProfileButtonText);
	ui->deleteProfileButton->repaint();


	QString labelText = isPl ? QString::fromLocal8Bit("Wybierz profil.") : "Select Profile";
	ui->label->setText(labelText);
	ui->label->repaint();


	QString windowTitleText = isPl ? QString::fromLocal8Bit("CrackerJack - Okno Wyboru Profilu") : "CrackerJack - Select Profile Window";
	this->setWindowTitle(windowTitleText);
	this->repaint();

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
	auto result = newProfDialog->exec();
	if (result == QDialog::Accepted) {
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
		onlyProfileName = choppedString[1];//TODO obsluga przypadku gdy po splitowaniu nazwa profilu nie jest zapisana w choppedString[1]
	

	QString msgText = StringResource::SelectProfileWindow_sureToDeleteProfile() + onlyProfileName  + " ?";
	auto buttons = QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No;
	int retCode = Utilities::showMessageBox("CrackerJack", msgText, buttons);
	bool shouldBeDeleted = retCode == QMessageBox::StandardButton::Yes;
	if (shouldBeDeleted) {
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
	if(nameParts.size() >= 2)
		profileName = nameParts[1];//TODO obsluga przypadku gdy po splitowaniu nazwa profilu nie jest zapisana w nameParts[1]

	ProfileDataBaseManager dbManager;
	dbManager.readProfileFromDataBase(profileName, *profToSelect);
	this->accept();
}

