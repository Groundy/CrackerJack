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
/*
Market::Market(QDialog*parent)
	: QDialog(parent){
	ui = new Ui::Market();
	ui->setupUi(this);
}*/
Market::Market(){
	ui = new Ui::Market();
	ui->setupUi(this);
	JsonParser parser;
	parser.readItemJson(this->allItems);
	auto type = Seller::RASHID;
	fillLists(&type);
}

Market::~Market(){
	delete ui;
}

void Market::fillLists(Seller* seller) {	
	JsonParser parser;
	QList<Item> items = this->allItems;
	parser.readItemJson(items);

	if (seller != NULL)
		;//parser.filtrItems(items, seller, NULL);

	int sizeOfEnum = (int)Item::TYPE_OF_ITEM::OTHER + 1;
	QStringList notEmptyCategories, itemsOnCategories;
	for (size_t i = 0; i < sizeOfEnum; i++){
		QList<Item> tmpList = items;
		Item::TYPE_OF_ITEM itemType = (Item::TYPE_OF_ITEM)i;
		parser.filtrItems(tmpList, NULL, &itemType);
		if (tmpList.isEmpty())
			continue;

		//int alreadyFilledCategories = itemsToDisplay.namesOfCategories.size();
		QString nameOfCategory = Item::descriptionMap.value(itemType);
		itemsToDisplay.namesOfCategories.push_back(nameOfCategory);
		itemsToDisplay.itemsOfCategory.push_back(tmpList);
	}
	int f = 3;
	
	ui->categoriesWindow->clear();
	ui->itemList->clear();

	for each (QString var in itemsToDisplay.namesOfCategories)
		ui->itemList->addItem(var);

	ui->categoriesWindow->repaint();
	ui->itemList->repaint();
}
