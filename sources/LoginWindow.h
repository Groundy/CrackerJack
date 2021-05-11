#pragma once
#include "qdialog.h"
#include "ui_LoginWindow.h"
#include "SelectProfileWindow.h"
#include "StringResource.h"

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = Q_NULLPTR);  

public slots:
    void loginInserted();
    void loginButtonPressed();
    void passwordInserted();
    void signUpButtonClicked();
    
private:
    Ui::LoginWindowClass ui;

    void checkLogInPossibility();
    void getAndSetVersion();
    void showThatPasswordFailed();
    bool checkPasswordCorrectness();

};
