#pragma onc
#include "qdialog.h"
#include "ui_LoginWindow.h"
#include "SelectProfileWindow.h"


class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = Q_NULLPTR);  
    //[Przeniesc do private na koniec testow]
    void goToSelectProfileWindow();

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
