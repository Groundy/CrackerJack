#pragma once

#include <QWidget>
#include "ui_MainMenu.h"

class MainMenu : public QWidget
{
	Q_OBJECT

public:
	MainMenu(QWidget *parent = Q_NULLPTR);
	~MainMenu();

private:
	Ui::MainMenu ui;
};
