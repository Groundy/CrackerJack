#include "LoginWindow.h"


LoginWindow::LoginWindow(QWidget *parent): 
        QDialog(parent){
    ui.setupUi(this);
    getAndSetVersion();
    checkLogInPossibility();
    setFixedSize(this->size());
    ui.wrongPasswordLabel->setVisible(false);
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

//fun
void LoginWindow::checkLogInPossibility() {
    bool canLogIn = ui.loginLine->text().size() != 0 && ui.passwordLine->text().size() != 0 ? true : false;
    ui.logInButton->setEnabled(canLogIn);
}

void LoginWindow::getAndSetVersion() {
    //TODO
    ui.versionLabel->setText("Version 0.0.0");
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
