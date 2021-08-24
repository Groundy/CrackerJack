#include "Market.h"
#include "ui_Market.h"

Market::Market(){
	ui = new Ui::Market();
	ui->setupUi(this);
	JsonParser::readItemJson(this->allItems);
	ui->showItemsButton_All->setChecked(true);
	filtr_seller = Seller::ANY;
	fillCategoryLists();
	isPl = StringResource::languageIsPl();
	ditWithSavedItemLists = Utilities::getDirWithCrackerJackTmpFolder(Utilities::FOLDERS_OF_TMP_FOLDER::MarketLists);
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
		QString textToDisplayPl = QString::fromLocal8Bit("Obie wartoœci powinny byæ wiêksze od zera, wartoœæ maksymalna nie mo¿e byæ mniejsza ni¿ minimalna.");
		QString textToDisplayEng = "Both values have to be greater than zero, max value has to be same or greater than min value.";
		Logger::logPotenialBug(textToDisplayEng,"Market","addItemButtonPressed");
		QString msgToDisplay = isPl ? textToDisplayPl : textToDisplayEng;
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), msgToDisplay, QMessageBox::Ok);
		return;
	}
	bool amountOk = amount > 0 && amount <= 64000;
	if (!amountOk) {
		QString textToDisplayPl = QString::fromLocal8Bit("Iloœæ przedmiotów powinna byæ w przedziale od 1 do 64.000");
		QString textToDisplayEng = "Amount of items should be number between 1 and 640000";
		Logger::logPotenialBug(textToDisplayEng, "Market", "addItemButtonPressed");
		QString msgToDisplay = isPl ? textToDisplayPl : textToDisplayEng;
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), msgToDisplay, QMessageBox::Ok);
		return;
	}
	bool itemEmpty = currentlyDisplayedItem.name.isEmpty();
	if (itemEmpty) {
		QString textToDisplayPl = QString::fromLocal8Bit("Nie wybrano przedmiotu");
		QString textToDisplayEng = "Item not selected";
		Logger::logPotenialBug(textToDisplayEng, "Market", "addItemButtonPressed");
		QString msgToDisplay = isPl ? textToDisplayPl : textToDisplayEng;
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), msgToDisplay, QMessageBox::Ok);
		return;
	}

	Offert offert(currentlyDisplayedItem.name,minVal,maxVal,amount);
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
		ui->itemList->setCurrentRow(-1);
		ui->itemList->repaint();
	}
	else {
		QString textToDisplayPl = QString::fromLocal8Bit("Przedmiot ju¿ jest na liœcie.");
		QString textToDisplayEng = "Item is already on the list";
		Logger::logPotenialBug(textToDisplayEng, "Market", "addItemButtonPressed");
		QString msgToDisplay = isPl ? textToDisplayPl : textToDisplayEng;
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), msgToDisplay, QMessageBox::Ok);
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

	QString itemName = isPl ? QString::fromLocal8Bit("Nazwa: ") : "Name: ";
	QString priceStr = isPl ? QString::fromLocal8Bit("Cena: ") : "Price: ";
	QString weightStr = isPl ? QString::fromLocal8Bit("Waga: ") : "Weight: ";
	QString ratioStr = isPl ? QString::fromLocal8Bit("Cena/waga: ") : "Price/weight: ";
	QString buyerStr = isPl ? QString::fromLocal8Bit("Kupiec: ") : "Buyer: ";

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
		bool alreadyOnList = offertToCheck.itemName == var.itemName;
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

void Market::saveListToJsonFile(){
	QString msgToDiplasy = isPl ? QString::fromLocal8Bit("WprowadŸ nazwê pliku.") : "Insert file name.";
	QString nameForFile;
	SetNameWidnow win(this, "CrackerJack", msgToDiplasy, &nameForFile);
	int retCode = win.exec();
	bool accepted = retCode == QDialog::Accepted;
	if (!accepted)
		return;

	QJsonArray arr;
	for each (auto var in offertsList){
		QJsonObject toAdd;
		toAdd.insert("itemName", QJsonValue(var.itemName));
		toAdd.insert("minPrice", QJsonValue(var.minPrice));
		toAdd.insert("maxPrice", QJsonValue(var.maxPrice));
		toAdd.insert("amount", QJsonValue(var.amount));
		arr.append(toAdd);
	}
	QJsonObject mainObj;
	mainObj.insert("offerts", QJsonValue(arr));
	QJsonDocument docToSave(mainObj);
	
	QString pathToFolder = ditWithSavedItemLists.absolutePath();
	JsonParser::saveJsonFile(pathToFolder, nameForFile + ".json", docToSave);
}

void Market::readListFromJsonFile(){
	QFileDialog fDial(this, NULL, ditWithSavedItemLists.absolutePath(), "*.json");
	bool accepted = fDial.exec() == QDialog::Accepted;
	if (!accepted)
		return;
	QStringList fileList = fDial.selectedFiles();
	if (fileList.size() == 0)
		return;
	QJsonObject obj;
	JsonParser::openJsonFile(obj, fileList.first());
	QJsonArray arr = obj["offerts"].toArray();
	if (arr.count() == 0) {
		QString textToDisplayPl = QString::fromLocal8Bit("Plik nie jest poprawny lub jest pusty.");
		QString textToDisplayEng = "File is uncorrect or empty.";
		Logger::logPotenialBug(textToDisplayEng, "Market", "readListFromJsonFile");
		QString msgToDisplay = isPl ? textToDisplayPl : textToDisplayEng;
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), msgToDisplay, QMessageBox::Ok);
		return;
	}

	offertsList.clear();
	for each (QJsonValue var in arr){
		QJsonObject obj = var.toObject();
		QString itemNameStr = obj.value("itemName").toString();
		int minPrice = obj.value("minPrice").toInt();
		int maxPrice = obj.value("maxPrice").toInt();
		int amount = obj.value("amount").toInt();
		Offert offert(itemNameStr, minPrice, maxPrice, amount);
		offertsList.append(offert);
	}
	repaitOfertsList();
}

void Market::removeItem(){
	QListWidget* list = ui->listWidget;
	int currentRow = list->currentRow();
	int count = list->count();

	bool elementInRange = currentRow >= 0 && currentRow < count;
	if (!elementInRange)
		return;

	offertsList.removeAt(currentRow);
	repaitOfertsList();
}

Offert::Offert(QString itemNameToSet, int minPrice, int maxPrice, int amount){
	this->itemName = itemNameToSet;
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
	toRet.append(this->itemName);
	toRet.append(",  min: " + minPriceStr);
	toRet.append(",  max: " + maxPriceStr);
	toRet.append(",  amount: " + amountStr);
	//toRet.append(",   " + typeStr);
	return toRet;
}
