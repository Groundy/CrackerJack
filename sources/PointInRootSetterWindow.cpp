#include "PointInRootSetterWindow.h"
#include "ui_PointInRootSetterWindow.h"

PointInRootSetterWindow::PointInRootSetterWindow(QDialog* parent, QString pointStr, RouteCreator::FIELDS_TYPE& type)
	: QDialog(parent){
	ui = new Ui::PointInRootSetterWindow();
	ui->setupUi(this);
	ui->positionLabel->setText(pointStr);
	ui->positionLabel->repaint();
	fieldInfoPtr = &type;
}

PointInRootSetterWindow::~PointInRootSetterWindow(){
	delete ui;
}

void PointInRootSetterWindow::cancelButtonClicked(){
	reject();
}

void PointInRootSetterWindow::okButtonClicked(){
	*fieldInfoPtr = getFieldTypeFromButtons();
	accept();
}

RouteCreator::FIELDS_TYPE PointInRootSetterWindow::getFieldTypeFromButtons(){
	if (ui->fieldButton_holeShovelNeeded->isChecked())
		return RouteCreator::FIELDS_TYPE::SHOVEL_HOLE_NEED_SHOVEL ;
	if (ui->fieldButton_ladderDown->isChecked())
		return RouteCreator::FIELDS_TYPE::LADDER_DOWN;
	if (ui->fieldButton_ladderUp->isChecked())
		return FIELDS_TYPE::LADDER_UP;
	if (ui->fieldButton_openHole->isChecked())
		return FIELDS_TYPE::SHOVEL_HOLE_ALWAYS_OPEN;
	if (ui->fieldButton_regular->isChecked())
		return FIELDS_TYPE::REGULAR;
	if (ui->fieldButton_ropeField->isChecked())
		return FIELDS_TYPE::ROPE_FIELD;
	if (ui->fieldButton_stairsDown->isChecked())
		return FIELDS_TYPE::STAIRS_DOWN;
	if (ui->fieldButton_stairsUp->isChecked())
		return FIELDS_TYPE::STAIRS_UP;
	if (ui->fieldButton_teleport->isChecked())
		return FIELDS_TYPE::TELEPORT;
}
y