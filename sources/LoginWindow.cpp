#include "LoginWindow.h"


LoginWindow::LoginWindow(QWidget* parent) :
	QDialog(parent) {
	ui.setupUi(this);
	getAndSetVersion();
	bool isPl = StringResource::languageIsPl();
	ui.laungageComboBox->addItem("eng");
	ui.laungageComboBox->addItem("pl");

	QString login = Utilities::readFromIniFile(Utilities::FieldsOfIniFile::LAST_USED_LOGIN);
	bool setChecked = login.size() > 0;
	ui.loginLine->setText(login);
	ui.remeberLoginChecbox->setChecked(setChecked);
	ui.loginLine->repaint();
	ui.remeberLoginChecbox->repaint();

	int index = isPl ? 1 : 0;
	ui.laungageComboBox->setCurrentIndex(index);
	ui.laungageComboBox->repaint();
	QString strToSave = ui.laungageComboBox->itemText(index);
	Utilities::writeIniFile(Utilities::FieldsOfIniFile::LANGUAGE, strToSave);
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
	// TODO 
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
	passwordLabelText = isPl ? QString::fromLocal8Bit("Has³o") : "Password";
	ui.passwordLabel->setText(passwordLabelText);
	ui.passwordLabel->repaint();

	QString checkBoxText;
	checkBoxText = isPl ? QString::fromLocal8Bit("Zapamiêtaj login") : "Remember password";
	ui.remeberLoginChecbox->setText(checkBoxText);
	ui.remeberLoginChecbox->repaint();

	QString LogInText;
	LogInText = isPl ? QString::fromLocal8Bit("Zaloguj") : "Log in";
	ui.logInButton->setText(LogInText);
	ui.logInButton->repaint();

	QString SignUpText;
	SignUpText = isPl ? QString::fromLocal8Bit("Zarejestruj siê") : "Sign up";
	ui.signUPButton->setText(SignUpText);
	ui.signUPButton->repaint();

	QString forgotPassText;
	forgotPassText = isPl ? QString::fromLocal8Bit("Zapomnia³eœ has³a?") : "Forgot your password?";
	ui.forgetPassButton->setText(forgotPassText);
	ui.forgetPassButton->repaint();

	QString textToSet;
	textToSet = isPl ? QString::fromLocal8Bit("Wersja ") : "Version ";
	QString verVal = getAndSetVersion();
	textToSet += verVal;
	ui.versionLabel->setText(textToSet);
	ui.versionLabel->repaint();
}
