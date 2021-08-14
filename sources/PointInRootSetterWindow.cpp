#include "PointInRootSetterWindow.h"
#include "ui_PointInRootSetterWindow.h"

PointInRootSetterWindow::PointInRootSetterWindow(QDialog* parent, QString pointStr, Route::FIELDS_TYPE* fieldTypePtr)
	: QDialog(parent){
	ui = new Ui::PointInRootSetterWindow();
	ui->setupUi(this);
	ui->positionLabel->setText(pointStr);
	ui->positionLabel->repaint();
	fieldTypeToRet = fieldTypePtr;
	TRANSLATE_setRadioButtonDescrpitions();
	ui->fieldButton_regular->setChecked(true);
}

PointInRootSetterWindow::~PointInRootSetterWindow(){
	delete ui;
}

void PointInRootSetterWindow::cancelButtonClicked(){
	reject();
}

void PointInRootSetterWindow::okButtonClicked(){
	*fieldTypeToRet = getFieldTypeFromButtons();
	accept();
}

Route::FIELDS_TYPE PointInRootSetterWindow::getFieldTypeFromButtons(){
	typedef Route::FIELDS_TYPE Type;
	if (ui->fieldButton_holeShovelNeeded->isChecked())
		return Type::SHOVEL_HOLE_NEED_SHOVEL;
	else if (ui->fieldButton_ladderDown->isChecked())
		return Type::LADDER_DOWN;
	else if (ui->fieldButton_ladderUp->isChecked())
		return Type::LADDER_UP;
	else if (ui->fieldButton_openHole->isChecked())
		return Type::SHOVEL_HOLE_ALWAYS_OPEN;
	else if (ui->fieldButton_regular->isChecked())
		return Type::REGULAR;
	else if (ui->fieldButton_ropeField->isChecked())
		return Type::ROPE_FIELD;
	else if (ui->fieldButton_stairsDown->isChecked())
		return Type::STAIRS_DOWN;
	else if (ui->fieldButton_stairsUp->isChecked())
		return Type::STAIRS_UP;
	else if (ui->fieldButton_teleport->isChecked())
		return Type::TELEPORT;
	else
		return Type::REGULAR;
}

void PointInRootSetterWindow::TRANSLATE_setRadioButtonDescrpitions() {
	bool isPl = StringResource::languageIsPl();
	QString textToSet;
	if (isPl) {
		textToSet = QString::fromLocal8Bit("Zwyk³e pole");
		ui->fieldButton_regular->setText((textToSet));
		textToSet = QString::fromLocal8Bit("Teleport");
		ui->fieldButton_teleport->setText((textToSet));
		textToSet = QString::fromLocal8Bit("Schody do góry");
		ui->fieldButton_stairsUp->setText((textToSet));
		textToSet = QString::fromLocal8Bit("Schody do do³u");
		ui->fieldButton_stairsDown->setText((textToSet));
		textToSet = QString::fromLocal8Bit("Pole liny");
		ui->fieldButton_ropeField->setText((textToSet));
		textToSet = QString::fromLocal8Bit("Dziura, otwarta");
		ui->fieldButton_openHole->setText((textToSet));
		textToSet = QString::fromLocal8Bit("Dziura, zamknieta");
		ui->fieldButton_holeShovelNeeded->setText((textToSet));
		textToSet = QString::fromLocal8Bit("Drabina w górê");
		ui->fieldButton_ladderUp->setText((textToSet));
		textToSet = QString::fromLocal8Bit("Drabian w dó³");
		ui->fieldButton_ladderDown->setText((textToSet));
		//addThatToGui todo
		//textToSet = QString::fromLocal8Bit("Punkt wyjscia");
		//ui->fieldx->setText((textToSet));
	}
	else {
		textToSet = "Regular Field";
		ui->fieldButton_regular->setText((textToSet));
		textToSet = "Teleport";
		ui->fieldButton_teleport->setText((textToSet));
		textToSet = "Staris-up";
		ui->fieldButton_stairsUp->setText((textToSet));
		textToSet = "Staris-down";
		ui->fieldButton_stairsDown->setText((textToSet));
		textToSet = "Rope field";
		ui->fieldButton_ropeField->setText((textToSet));
		textToSet = "Hole, open";
		ui->fieldButton_openHole->setText((textToSet));
		textToSet = "Hole, close";
		ui->fieldButton_holeShovelNeeded->setText((textToSet));
		textToSet = "Ladder up";
		ui->fieldButton_ladderUp->setText((textToSet));
		textToSet = "Ladder down";
		ui->fieldButton_ladderDown->setText((textToSet));
		//addThatToGui todo
		//textToSet = "Log out point";
		//ui->fieldx->setText((textToSet));
	}
	ui->fieldButton_regular->repaint();
	ui->fieldButton_teleport->repaint();
	ui->fieldButton_stairsUp->repaint();
	ui->fieldButton_stairsDown->repaint();
	ui->fieldButton_ropeField->repaint();
	ui->fieldButton_openHole->repaint();
	ui->fieldButton_holeShovelNeeded->repaint();
	ui->fieldButton_ladderUp->repaint();
	ui->fieldButton_ladderDown->repaint();
}
