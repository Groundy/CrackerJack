#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LoginWindow.h"

class LoginWindow : public QMainWindow
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
};
