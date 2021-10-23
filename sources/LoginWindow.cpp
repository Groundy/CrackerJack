#include "LoginWindow.h"


LoginWindow::LoginWindow(QWidget* parent) :
	QDialog(parent) {
	ui.setupUi(this);
	QString login = Utilities::readFromIniFile(Utilities::FieldsOfIniFile::LAST_USED_LOGIN);
	bool setChecked = login.size() > 0;
	ui.loginLine->setText(login);
	ui.remeberLoginChecbox->setChecked(setChecked);
	ui.loginLine->repaint();
	ui.remeberLoginChecbox->repaint();

	/*to do wyswietlic aktualnie uzywany jezyk TODO
	int index = isPl ? 1 : 0;
	ui.laungageComboBox->setCurrentIndex(index);
	ui.laungageComboBox->repaint();
	QString strToSave = ui.laungageComboBox->itemText(index);
	Utilities::writeIniFile(Utilities::FieldsOfIniFile::LANGUAGE, strToSave);
	*/
	setUpGUI();
}

LoginWindow::~LoginWindow(){
	int index = ui.laungageComboBox->currentIndex();
	QString strFromBox = ui.laungageComboBox->itemText(index);
	bool checked = ui.remeberLoginChecbox->isChecked();
	QString strWithLaungage = checked ? strFromBox : "";
	QString strWithLogin = ui.loginLine->text();
	Utilities::writeIniFile(Utilities::FieldsOfIniFile::LAST_USED_LOGIN, strWithLogin);
	Utilities::writeIniFile(Utilities::FieldsOfIniFile::LANGUAGE, strWithLaungage);
}
//slots
void LoginWindow::loginInserted() {
	checkLogInPossibility();
}

void LoginWindow::loginButtonPressed() {
	
	this->accept();// TODO 
}

void LoginWindow::passwordInserted() {
	checkLogInPossibility();
}

void LoginWindow::signUpButtonClicked() {

}

void LoginWindow::languageChanged() {
	QString language = ui.laungageComboBox->currentText();
	Utilities::writeIniFile(Utilities::FieldsOfIniFile::LANGUAGE, language);
	setUpGUI();
}

void LoginWindow::checkLogInPossibility() {
	bool logIsNotEmpty = !ui.loginLine->text().isEmpty();
	bool passIsNotEmpty = !ui.passwordLine->text().isEmpty();
	bool canLogIn = logIsNotEmpty && passIsNotEmpty;
	ui.logInButton->setEnabled(canLogIn);
}

void LoginWindow::showThatPasswordFailed() {
	QString msg = tr("That combination of login and password doesn't exist.");
	ui.wrongPasswordLabel->setText(msg);
	ui.wrongPasswordLabel->setVisible(true);
	ui.passwordLine->clear();
}

bool LoginWindow::checkPasswordCorrectness() {
	//[TODO]
	return true;
}

void LoginWindow::setUpGUI() {
	QString passwordLabelText = tr("Password");
	ui.passwordLabel->setText(passwordLabelText);
	ui.passwordLabel->repaint();

	QString checkBoxText = tr("Remember password");
	ui.remeberLoginChecbox->setText(checkBoxText);
	ui.remeberLoginChecbox->repaint();

	QString LogInText = tr("Log in");
	ui.logInButton->setText(LogInText);
	ui.logInButton->repaint();

	QString SignUpText = tr("Sign up");
	ui.signUPButton->setText(SignUpText);
	ui.signUPButton->repaint();

	QString forgotPassText = tr("Forgot your password?");
	ui.forgetPassButton->setText(forgotPassText);
	ui.forgetPassButton->repaint();

	QString textToSet = tr("Version ");
	QString verVal = VER_FILEVERSION_STR;
	textToSet += verVal;
	ui.versionLabel->setText(textToSet);
	ui.versionLabel->repaint();
}
