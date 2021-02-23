#include "SelectProfileWindow.h"
#include "ui_SelectProfileWindow.h"

SelectProfileWindow::SelectProfileWindow(QWidget *parent) : QDialog(parent){
	ui = new Ui::SelectProfileWindow();
	ui->setupUi(this);
	setFixedSize(this->size());
	prepareProfiles();
	test();
}

SelectProfileWindow::~SelectProfileWindow()
{
	delete ui;
}

void SelectProfileWindow::prepareProfiles(){
	//[TODO]
}

void SelectProfileWindow::test() {
	auto dbManager = new ProfileDataBaseManager();
	
}