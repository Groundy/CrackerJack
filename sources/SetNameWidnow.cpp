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
	bool isPl = StringResource::languageIsPl();
	QString cancelButtonText = isPl ? QString::fromLocal8Bit("Anuluj") : "Cancel";
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
	ui->okButton->setEnabled(lengthOk);
	ui->okButton->repaint();
}

void SetNameWidnow::cancelButtonPressed(){
	reject();
}
