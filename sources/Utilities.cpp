#include "Utilities.h"

 void Utilities::showMessageBox(QString title, QString text) {
	QMessageBox box;
	box.setText(text);
	box.setWindowTitle(title);
	box.setStandardButtons(QMessageBox::Ok);
	box.setDefaultButton(QMessageBox::Ok);
	box.exec();
}