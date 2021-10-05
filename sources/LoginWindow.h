#pragma once
#include "qdialog.h"
#include "ui_LoginWindow.h"
#include "SelectProfileWindow.h"
#include "version.h"
class LoginWindow : public QDialog
{
	Q_OBJECT

public:
	LoginWindow(QWidget *parent = Q_NULLPTR);  
	~LoginWindow();

public slots:
	void loginInserted();
	void loginButtonPressed();
	void passwordInserted();
	void signUpButtonClicked();
	void languageChanged();
private:
	Ui::LoginWindowClass ui;
	void checkLogInPossibility();
	void showThatPasswordFailed();
	bool checkPasswordCorrectness();
	void setUpGUI();

};
