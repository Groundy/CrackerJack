#include "LoginWindow.h"


LoginWindow::LoginWindow(QWidget *parent)   :   QMainWindow(parent){
    ui.setupUi(this);
    getAndSetVersion();
    checkLogInPossibility();
    setFixedSize(this->size());
}
//slots
void LoginWindow::loginInserted() {
    checkLogInPossibility();
}

void LoginWindow::loginButtonPressed() {

}

void LoginWindow::passwordInserted() {
    checkLogInPossibility();
}

void LoginWindow::signUpButtonClicked() {
   
}

void LoginWindow::checkLogInPossibility() {
    bool canLogIn = ui.loginLine->text().size() != 0 && ui.passwordLine->text().size() != 0 ? true : false;
    ui.logInButton->setEnabled(canLogIn);
}

void LoginWindow::getAndSetVersion() {
    //TODO
    ui.versionLabel->setText("Version 0.0.0");
}