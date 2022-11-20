#include "SelectProfileWindow.h"
#include "ui_SelectProfileWindow.h"
#include <NewProfileConfiguartor.h>

SelectProfileWindow::SelectProfileWindow(QWidget *parent, Profile* prof) : 
	QDialog(parent), profToSelect(prof)
{
	ui = new Ui::SelectProfileWindow();
	ui->setupUi(this);
	setFixedSize(this->size());
	setUpGui();
	refreshProfilesOnList();
	ui->listOfProfs->setItemSelected(NULL, true);
	selectListAction();
	readAndSetLastUsedProFromINI();
}

SelectProfileWindow::~SelectProfileWindow(){
	delete ui;
	this->reject();
}

void SelectProfileWindow::refreshProfilesOnList(){
	ui->listOfProfs->clear();
	QStringList list;
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

QString SelectProfileWindow::getSelectedProfName(){
	int row = ui->listOfProfs->currentRow();
	QString nameOfProfToSplit = ui->listOfProfs->item(row)->text();
	QStringList nameParts = nameOfProfToSplit.split("] ");
	QString profileName = nameParts[1];
	return profileName;
}

void SelectProfileWindow::addNewProfileButtonAction(){
	Profile profile;
	NewProfileConfiguartor newProfWind(&profile, this);
	int result = newProfWind.exec();
	if (result == QDialog::Accepted) {
		JsonParser().saveProfiles(&profile);
		Sleep(20);
		refreshProfilesOnList();
	}
}

void SelectProfileWindow::editProfileButtonAction(){
	/*
	QString profName = getSelectedProfName();
	Profile profToEdit;
	auto newProfDialog = std::make_unique<NewProfileConfiguartor>(&profToEdit, this);
	newProfDialog->fillWidgetsWithDataFromProf(&profToEdit);
	bool accepted = newProfDialog->exec() == QDialog::Accepted;
	if (accepted) {
		JsonParser().saveProfiles(&profToEdit);	
		refreshProfilesOnList();
	}
	*/
}

void SelectProfileWindow::deleteProfileButtonAction(){
	QString profileNameToDelete = getSelectedProfName();
	
	QString msgText = QObject::tr("Do you really want to delete profile ") + profileNameToDelete + " ?";
	bool accepted = Utilities::showMessageBox_NO_YES(msgText);
	if (accepted) {
		JsonParser().deleteProfileFile(profileNameToDelete);
		refreshProfilesOnList();
		ui->listOfProfs->setItemSelected(NULL, true);
	}
}

void SelectProfileWindow::selectListAction(){
	bool oneItemIsSelected = ui->listOfProfs->selectedItems().size() == 1;
	ui->deleteProfileButton->setEnabled(oneItemIsSelected);
	ui->editProfileButton->setEnabled(oneItemIsSelected);
}

void SelectProfileWindow::profSelected(){
	QString profName = getSelectedProfName();
	this->accept();
}

