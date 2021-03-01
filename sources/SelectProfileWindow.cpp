#include "SelectProfileWindow.h"
#include "ui_SelectProfileWindow.h"

SelectProfileWindow::SelectProfileWindow(QWidget *parent) : QDialog(parent){
	ui = new Ui::SelectProfileWindow();
	ui->setupUi(this);
	setFixedSize(this->size());
	prepareProfiles();
	ui->listOfProfs->setItemSelected(NULL, true);
	ui->deleteProfileButton->setEnabled(false);
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
	newProfWind.exec();
	dbManager.saveProfileToDatabase(prof);
	delete prof;
	prepareProfiles();
}

void SelectProfileWindow::editProfileButtonAction(){
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

}

