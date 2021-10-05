#include "SetNameWidnow.h"
#include "ui_SetNameWidnow.h"

SetNameWidnow::SetNameWidnow(QDialog *parent, QString title, QString textToDisplay, QString* insertedLine)
	: QDialog(parent){
	ui = new Ui::SetNameWidnow();
	ui->setupUi(this);

	ptToRetrunStr = insertedLine;
	ui->textLabel->setText(textToDisplay);
	this->setWindowTitle(title);
	ui->okButton->setEnabled(false);

	QString cancelButtonText = tr("Cancel");
	ui->cancelButton->setText(cancelButtonText);
	this->repaint();
}

SetNameWidnow::~SetNameWidnow()
{
	delete ui;
}

void SetNameWidnow::okButtonPressed(){
	QString str = ui->lineEdit->text();
	*ptToRetrunStr = str;
	accept();
}

void SetNameWidnow::textChanged(){
	QString str = ui->lineEdit->text();
	bool lengthOk = str.length() >= minLength && str.length() <= maxLength;
	bool shouldBeEnabled = lengthOk && allCharsAreOk(str);
	ui->okButton->setEnabled(shouldBeEnabled);
	ui->okButton->repaint();
}

void SetNameWidnow::cancelButtonPressed(){
	reject();
}

bool SetNameWidnow::allCharsAreOk(QString strToCheck){
	for each (QChar var in strToCheck){
		bool isAllowed = var.isLetterOrNumber() || var.isSpace();
		if (!isAllowed)
			return false;
	}
	return true;
}
