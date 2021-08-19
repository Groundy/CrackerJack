#include "Market.h"
#include "ui_Market.h"

void Market::showOnly_GreenDijnn(){
}

void Market::showOnly_BlueDijnn()
{
}

void Market::showOnly_Yasir()
{
}

void Market::showOnly_Esrik()
{
}

void Market::showOnly_Rashid()
{
}

void Market::showOnly_All()
{
}

Market::Market(QWidget *parent)
	: QWidget(parent){
	ui = new Ui::Market();
	ui->setupUi(this);
}

Market::~Market()
{
	delete ui;
}

void Market::fillLists(Seller* seller, ItemType* itemType) {	
	JsonParser parser;
	QList<Item> items;
	parser.readItemJson(items);

	if (seller != NULL)
		parser.filtrItems(items, seller, NULL);
	if (itemType != NULL)
		parser.filtrItems(items, NULL, itemType);
}
