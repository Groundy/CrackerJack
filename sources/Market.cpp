#include "Market.h"
#include "ui_Market.h"

Market::Market(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::Market();
	ui->setupUi(this);
}

Market::~Market()
{
	delete ui;
}
