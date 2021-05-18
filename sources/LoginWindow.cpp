#include "LoginWindow.h"


LoginWindow::LoginWindow(QWidget* parent) :
	QDialog(parent) {
	ui.setupUi(this);
	getAndSetVersion();
	ui.laungageComboBox->addItem("eng");
	ui.laungageComboBox->addItem("pl");

	QString login = Utilities::readFromIniFile(Utilities::FieldsOfIniFile::LAST_USED_LOGIN);
	ui.loginLine->setText(login);
	ui.remeberLoginChecbox->repaint();
	bool setChecked = login.size() > 0;
	ui.remeberLoginChecbox->setChecked(setChecked);
	int index = StringResource::languageIsPl() ? 1 : 0;
	ui.laungageComboBox->setCurrentIndex(index);
	ui.laungageComboBox->repaint();

	setUpGUI();
}
//slots
void LoginWindow::loginInserted() {
	checkLogInPossibility();
}

void LoginWindow::loginButtonPressed() {
	// TODO 
}

void LoginWindow::passwordInserted() {
	checkLogInPossibility();
}

void LoginWindow::signUpButtonClicked() {

}

void LoginWindow::languageChanged() {
	//dodac to podczas wychodzenia z tej klasy
	QString language = ui.laungageComboBox->currentText();
	Utilities::writeIniFile(Utilities::FieldsOfIniFile::LANGUAGE, language);
	setUpGUI();
}

//fun
void LoginWindow::checkLogInPossibility() {
	bool canLogIn = ui.loginLine->text().size() != 0 && ui.passwordLine->text().size() != 0 ? true : false;
	ui.logInButton->setEnabled(canLogIn);
}

QString LoginWindow::getAndSetVersion() {
	QString verVal = "0.0.0";    //TODO
	return verVal;
}

void LoginWindow::showThatPasswordFailed() {
	QString msg = StringResource::LoginWindow_WrongLoginPasword();
	ui.wrongPasswordLabel->setText(msg);
	ui.wrongPasswordLabel->setVisible(true);
	ui.passwordLine->clear();
}

bool LoginWindow::checkPasswordCorrectness() {
	//[TODO]
	return true;
}

void LoginWindow::setUpGUI() {
	bool isPl = StringResource::languageIsPl();

	QString passwordLabelText;
	passwordLabelText = isPl ? QString::fromLocal8Bit("Has�o") : "Password";
	ui.passwordLabel->setText(passwordLabelText);
	ui.passwordLabel->repaint();

	QString checkBoxText;
	checkBoxText = isPl ? QString::fromLocal8Bit("Zapami�taj login") : "Remember password";
	ui.remeberLoginChecbox->setText(checkBoxText);
	ui.remeberLoginChecbox->repaint();

	QString LogInText;
	LogInText = isPl ? QString::fromLocal8Bit("Zaloguj") : "Log in";
	ui.logInButton->setText(LogInText);
	ui.logInButton->repaint();

	QString SignUpText;
	SignUpText = isPl ? QString::fromLocal8Bit("Zarejestruj si�") : "Sign up";
	ui.signUPButton->setText(SignUpText);
	ui.signUPButton->repaint();

	QString forgotPassText;
	forgotPassText = isPl ? QString::fromLocal8Bit("Zapomnia�e� has�a?") : "Forgot your password?";
	ui.forgetPassButton->setText(forgotPassText);
	ui.forgetPassButton->repaint();

	QString textToSet;
	textToSet = isPl ? QString::fromLocal8Bit("Wersja ") : "Version ";
	QString verVal = getAndSetVersion();
	textToSet += verVal;
	ui.versionLabel->setText(textToSet);
	ui.versionLabel->repaint();
}
