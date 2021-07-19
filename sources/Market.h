#pragma once
#include "Utilities.h"
#include <QWidget>
namespace Ui { class Market; };

class Market : public QWidget
{
	Q_OBJECT

public:
	Market(QWidget *parent = Q_NULLPTR);
	~Market();

private:
	Ui::Market *ui;
	void fillCategoryList();
};
