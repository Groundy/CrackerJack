#include "SelectProfileWindow.h"
#include "ui_SelectProfileWindow.h"

SelectProfileWindow::SelectProfileWindow(QWidget *parent, Profile* prof) : QDialog(parent){
	ui = new Ui::SelectProfileWindow();
	ui->setupUi(this);
	setFixedSize(this->size());
	prepareProfiles();
	ui->listOfProfs->setItemSelected(NULL, true);
	selectListAction();
	profToSelect = prof;
}

SelectProfileWindow::~SelectProfileWindow()
{
	delete ui;
}

void SelectProfileWindow::prepareProfiles(){
	ui->listOfProfs->clear();
	QStringList list = dbManager.getListOfAllRecords();
	for each (QString str in list)
		ui->listOfProfs->addItem(str);
	ui->listOfProfs->repaint();
}

void SelectProfileWindow::addNewProfileButtonAction(){
	Profile* prof = new Profile();
	NewProfileConfiguartor newProfWind(prof,this);
	int result = newProfWind.exec();
	if (result == QDialog::Accepted)
		dbManager.saveProfileToDatabase(prof);
	delete prof;
	Sleep(20);
	prepareProfiles();
}

void SelectProfileWindow::editProfileButtonAction(){
	int row = ui->listOfProfs->currentRow();
	QString nameOfProfToSplit = ui->listOfProfs->item(row)->text();
	QStringList nameParts = nameOfProfToSplit.split("] ");
	QString profileName = nameParts[1];

	Profile* profToBeRead = new Profile();
	dbManager.readProfileFroDataBase(profToBeRead, profileName);
	NewProfileConfiguartor* newProfDialog = new NewProfileConfiguartor(profToBeRead, this);
	newProfDialog->fillWidgetsWithDataFromProf(profToBeRead);
	profToBeRead->clearProfile();
	auto result = newProfDialog->exec();
	if (result == QDialog::Accepted) {
		dbManager.deleteRecord(profileName);
		dbManager.saveProfileToDatabase(profToBeRead);
	}
	delete newProfDialog;
	delete profToBeRead;

	prepareProfiles();
}

void SelectProfileWindow::deleteProfileButtonAction(){
	QString profileNameToDelete;
	profileNameToDelete = ui->listOfProfs->selectedItems().first()->text();
	QStringList choppedString = profileNameToDelete.split("] ");
	if (choppedString.size() != 2)
		;//err diag
	QString onlyProfileName = choppedString[1];

	QString msgText = "Do you really want to delete profile \"" + onlyProfileName + "\" ";
	auto buttons = QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No;
	int shouldDelte = Utilities::showMessageBox("CrackerJack", msgText, buttons);
	if (shouldDelte == QMessageBox::StandardButton::Yes) {
		dbManager.deleteRecord(onlyProfileName);
		prepareProfiles();
		ui->listOfProfs->setItemSelected(NULL, true);
	}
}

void SelectProfileWindow::selectListAction(){
	bool oneItemIsSelected = ui->listOfProfs->selectedItems().size() == 1 ? true : false;
	ui->deleteProfileButton->setEnabled(oneItemIsSelected);
	ui->editProfileButton->setEnabled(oneItemIsSelected);
}

void SelectProfileWindow::profSelected(){
	int row = ui->listOfProfs->currentRow();
	QString nameOfProfToSplit = ui->listOfProfs->item(row)->text();
	QStringList nameParts = nameOfProfToSplit.split("] ");
	QString profileName = nameParts[1];
	dbManager.readProfileFroDataBase(profToSelect, profileName);
	this->accept();
}

