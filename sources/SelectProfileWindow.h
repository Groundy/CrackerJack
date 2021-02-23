#pragma once
#include <QDialog>
#include "ProfileDataBaseManager.h"
namespace Ui { class SelectProfileWindow; };

class SelectProfileWindow : public QDialog
{
	Q_OBJECT

public:
	SelectProfileWindow(QWidget *parent = Q_NULLPTR);
	~SelectProfileWindow();
	void test();

private:
	Ui::SelectProfileWindow *ui;
	void prepareProfiles();
};
