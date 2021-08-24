#include "Market.h"
#include "ui_Market.h"

Market::Market(){
	ui = new Ui::Market();
	ui->setupUi(this);
	filtr_seller = Seller::ANY;
	JsonParser parser;
	parser.readItemJson(this->allItems);
	int t = 9;
}

Market::~Market(){
	delete ui;
}

void Market::sellerGroupBoxChanged() {
	if (ui->showItemsButton_Rashid->isChecked())
		filtr_seller = Seller::RASHID;
	else if (ui->showItemsButton_Blue->isChecked())
		filtr_seller = Seller::BLUE_DJIN;
	else if (ui->showItemsButton_Green->isChecked())
		filtr_seller = Seller::GREEN_DJIN;
	else if (ui->showItemsButton_Esrik->isChecked())
		filtr_seller = Seller::ZAO;
	else if (ui->showItemsButton_Yasir->isChecked())
		filtr_seller = Seller::YASIR;
	else if (ui->showItemsButton_All->isChecked())
		filtr_seller = Seller::ANY;

	fillCategoryLists();
}

void Market::categoryChanged(){
	int indexOfCategory = ui->categoriesWindow->currentRow();
	bool indexInRange = indexOfCategory >= 0 && indexOfCategory < ui->categoriesWindow->count();
	Item::TYPE_OF_ITEM filtr_category;
	if (indexInRange) {
		QString currentCategory = ui->categoriesWindow->currentItem()->text();
		filtr_category = Item::descriptionMap.key(currentCategory);
	}
	fillItemList(filtr_seller, filtr_category);
}

void Market::itemChanged(){
	int currentItemIndex = ui->itemList->currentRow();
	if (currentItemIndex < 0 || currentItemIndex >= ui->itemList->count())
		return;

	QString nameToLookFor = ui->itemList->currentItem()->text();
	Item item;
	for each (Item var in allItems)	{
		if (var.name == nameToLookFor) {
			item = var;
			break;
		}
	}
	currentlyDisplayedItem = item;
	fillLabels(&item);
}

void Market::addItemButtonPressed(){
	int minVal = ui->minPriceValue->value();
	int maxVal = ui->maxPriceValue->value();
	int amount = ui->amountValue->value();
	
	bool valuesOk = minVal > 0 && maxVal > 0 && maxVal >= minVal;
	if (!valuesOk) {
		//log
		//user info
		//todo
		return;
	}
	bool amountOk = amount > 0 && amount < 64000;
	if (!amountOk) {
		//log
		//user info
		//todo
		return;
	}
	bool itemEmpty = currentlyDisplayedItem.name.isEmpty();
	if (itemEmpty) {
		//log
		//user info
		//todo
		return;
	}

	Offert offert(currentlyDisplayedItem,minVal,maxVal,amount);
	bool alreadyOnList = checkIfItemIsAlreadyOnList(offert);
	if (!alreadyOnList) {
		offertsList.push_back(offert);
		repaitOfertsList();
		currentlyDisplayedItem = Item();
		fillLabels(NULL);
		ui->minPriceValue->setValue(0);
		ui->maxPriceValue->setValue(0);
		ui->amountValue->setValue(0);
		ui->minPriceValue->repaint();
		ui->maxPriceValue->repaint();
		ui->amountValue->repaint();
	}
	else {
		//log
		//todo
		//user info;
		return;
	}
}

void Market::fillCategoryLists() {
	ui->itemList->clear();
	ui->itemList->setCurrentRow(-1);
	ui->itemList->repaint();
	ui->categoriesWindow->clear();	
	ui->categoriesWindow->setCurrentRow(-1);

	QList<Item> items = this->allItems;

	bool filtrItemBySeller = filtr_seller != Seller::ANY;
	if (filtrItemBySeller)
		Item::filrItemList(items, &filtr_seller, NULL);

	int sizeOfEnum = (int)Item::TYPE_OF_ITEM::ANY;
	for (size_t i = 0; i < sizeOfEnum; i++){
		QList<Item> tmpList = items;
		Item::TYPE_OF_ITEM itemType = (Item::TYPE_OF_ITEM)i;
		Item::filrItemList(tmpList, NULL, &itemType);
		if (tmpList.isEmpty())
			continue;

		QString nameOfCategory = Item::descriptionMap.value(itemType);
		ui->categoriesWindow->addItem(nameOfCategory);
	}

	ui->categoriesWindow->repaint();
}

void Market::fillLabels(Item* item){

	QString itemName = TEXT_ON_LABEL_ITEM_NAME;
	QString priceStr = TEXT_ON_LABEL_ITEM_PRICE;
	QString weightStr = TEXT_ON_LABEL_ITEM_WEIGHT;
	QString ratioStr = TEXT_ON_LABEL_RATION;
	QString buyerStr = TEXT_ON_LABEL_BUYER;

	if (item != NULL) {
		itemName += item->name;
		priceStr += QString::number(item->price);
		weightStr += QString::number(item->weight);
		if (item->weight > 0 && item->price > 0)
			ratioStr.append(QString::number((item->price * 1.0) / item->weight));
		buyerStr += Item::buyerMap.value(item->seller);
	}

	ui->buyerInfoLabel->setText(buyerStr);
	ui->itemNameInfoLabel->setText(itemName);
	ui->weightInfoLabel->setText(weightStr);
	ui->priceWeightRatioLabel->setText(ratioStr);
	ui->priceInfoLabel->setText(priceStr);

	ui->buyerInfoLabel->repaint();
	ui->itemNameInfoLabel->repaint();
	ui->weightInfoLabel->repaint();
	ui->priceWeightRatioLabel->repaint();
	ui->priceInfoLabel->repaint();
}

void Market::fillItemList(Seller sellerFiltr, ItemType categoryFiltr){
	QList<Item> listCpy = allItems;

	bool filtrBySeller = sellerFiltr != Seller::ANY;
	bool filrByCategory = categoryFiltr != ItemType::ANY;

	if(filtrBySeller && filrByCategory)
		Item::filrItemList(listCpy, &sellerFiltr, &categoryFiltr);
	else if(filtrBySeller && !filrByCategory)
		Item::filrItemList(listCpy, &sellerFiltr, NULL);
	else if (!filtrBySeller && filrByCategory)
		Item::filrItemList(listCpy, NULL, &categoryFiltr);

	
	QStringList namesList;
	for each (Item var in listCpy)
		namesList << var.name;
	namesList.sort();

	ui->itemList->clear();
	for each (QString var in namesList)
		ui->itemList->addItem(var);

	ui->itemList->repaint();
}

bool Market::checkIfItemIsAlreadyOnList(Offert& offertToCheck){
	for each (auto var in offertsList){
		bool alreadyOnList = offertToCheck.item.name == var.item.name;
		if (alreadyOnList)
			return true;
	}
	return false;
}

void Market::repaitOfertsList(){
	QListWidget* list = ui->listWidget;
	list->clear();
	for each (auto var in offertsList)
		list->addItem(var.toString());
	list->repaint();
}

Offert::Offert(Item item, int minPrice, int maxPrice, int amount){
	this->item = item;
	this->amount = amount;
	this->minPrice = minPrice;
	this->maxPrice = maxPrice;
}

QString Offert::toString(){
	QString minPriceStr = QString::number(minPrice);
	QString maxPriceStr = QString::number(maxPrice);
	QString amountStr = QString::number(amount);
	//QString typeStr = OFERT_TYPE::BUY == type ? "BUY" : "SELL";

	QString toRet;
	toRet.append(this->item.name);
	toRet.append(",  min: " + minPriceStr);
	toRet.append(",  max: " + maxPriceStr);
	toRet.append(",  amount: " + amountStr);
	//toRet.append(",   " + typeStr);
	return toRet;
}
