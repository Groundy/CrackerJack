#include "LoginWindow.h"


LoginWindow::LoginWindow(QWidget *parent)   :   QMainWindow(parent){
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
    bool loginOK = checkPasswordCorrectness();
    if (loginOK)
        goToSelectProfileWindow();
    else
        showThatPasswordFailed();
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
    QString msg = "That combination of login and password doesn't exist!\ntry again.";
    ui.wrongPasswordLabel->setText(msg);
    ui.wrongPasswordLabel->setVisible(true);
    ui.passwordLine->clear();
}

bool LoginWindow::checkPasswordCorrectness() {
    //[TODO]
    return true;
}

void LoginWindow::goToSelectProfileWindow() {
   auto t = new SelectProfileWindow();
   t->show();
   this->close();
}