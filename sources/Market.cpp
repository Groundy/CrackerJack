#include "Market.h"
#include "ui_Market.h"

Market::Market(std::shared_ptr<VariablesClass> var, std::shared_ptr<GameConnecter> gameConnector) :
	var(var), gameConnector(gameConnector)
{
	ui = new Ui::Market();
	ui->setupUi(this);
	JsonParser().readItemJson(this->allItems);
	ui->buyTypeRadioButton->setChecked(true);
	filtr_seller = Seller::ANY;
	fillCategoryLists();
	ditWithSavedItemLists = Utilities::getDirWithCrackerJackTmpFolder(Utilities::FOLDERS_OF_TMP_FOLDER::MarketLists);
}

Market::~Market(){
	delete ui;
}

void Market::sellerFiltrChange() {
	filtr_seller = Seller(ui->merchantFiltr->currentIndex());
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
	QString itemNameFieldValue = ui->itemNameInfoLabel->text();
	const QString pattern = ": ";
	int index = itemNameFieldValue.indexOf(pattern);
	QString itemName = itemNameFieldValue.mid(index + pattern.size());
	bool shouldPlaceOffers = ui->placeOfferCheckBox->isChecked();
	Offer::Type typeToSet = ui->buyTypeRadioButton->isChecked() ? Offer::Type::BUY : Offer::Type::SELL;

	bool valuesOk = minVal > 0 && maxVal > 0 && maxVal >= minVal;
	if (!valuesOk) {
		QString text = tr("Both values have to be greater than zero, max value has to be same or greater than min value.");
		Logger::logPotenialBug(text,"Market","addItemButtonPressed");
		Utilities::showMessageBox_INFO(text);		return;
	}
	bool amountOk = amount > 0 && amount <= 64000;
	if (!amountOk) {
		QString text = tr("Amount of items should be number between 1 and 640000.");
		Logger::logPotenialBug(text, "Market", "addItemButtonPressed");
		Utilities::showMessageBox_INFO(text);
		return;
	}
	bool itemEmpty = itemName.isEmpty();
	if (itemEmpty) {
		QString text = tr("Item not selected.");
		Logger::logPotenialBug(text, "Market", "addItemButtonPressed");
		Utilities::showMessageBox_INFO(text);
		return;
	}

	Offer offer(itemName, minVal, maxVal, amount, shouldPlaceOffers, typeToSet);
	bool alreadyOnList = checkIfItemIsAlreadyOnList(offer);
	if (!alreadyOnList) {
		offersList.push_back(offer);
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
		QString text = tr("Item is already on the list");
		Logger::logPotenialBug(text, "Market", "addItemButtonPressed");
		Utilities::showMessageBox_INFO(text);
	}
	setAndRepaintInfoLabel();
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

	QString itemName = tr("Name: ");
	QString priceStr = tr("Price: ");
	QString weightStr = tr("Weight: ");
	QString ratioStr = tr("Price/weight: ");
	QString buyerStr = tr("Buyer: ");

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

bool Market::checkIfItemIsAlreadyOnList(Offer& offerToCheck){
	for each (auto var in offersList){
		if (var == offerToCheck)
			return true;
	}
	return false;
}

void Market::repaitOfertsList(){
	QListWidget* list = ui->offerListWidget;
	list->clear();
	for each (auto var in offersList)
		list->addItem(var.toString());
	list->repaint();
}

bool Market::twoListAreTheSame(QList<Offer>& list1, QList<Offer>& list2){
	bool sameSize = list1.size() == list2.size();
	if (!sameSize)
		return false;

	for (size_t i = 0; i < list1.size(); i++){
		bool thisIsTheSameOffer = list1[i] == list2[i];
		if (!thisIsTheSameOffer)
			return false;
	}
	return true;
}

void Market::setAndRepaintInfoLabel(){
	QString sizeOfList = QString::number(offersList.size());
	QString toDisplay = QString("%1(%2)").arg(listFileName, sizeOfList);
	ui->itemListFileNameLabel->setText(toDisplay);
	ui->itemListFileNameLabel->repaint();
}

void Market::saveListToJsonFile(){
	QString msgToDiplasy = tr("Insert file name.");
	QString nameForFile;
	auto absolutePath = QFileDialog::getSaveFileName(this, "CrackerJack - save market list", ditWithSavedItemLists.absolutePath(), "*.json");
	if (absolutePath.isEmpty())
		return;
	listFileName = absolutePath.section("/", -1, -1);
	if (listFileName.isEmpty())
		return;

	QJsonArray arr;
	for each (auto var in offersList)
		arr.append(var.toJsonObj());

	QJsonObject mainObj;
	mainObj.insert("offers", QJsonValue(arr));
	QJsonDocument docToSave(mainObj);
	
	QString pathToFolder = ditWithSavedItemLists.absolutePath();
	JsonParser::saveJsonFile(pathToFolder, listFileName, docToSave);
	setAndRepaintInfoLabel();
}

void Market::readListFromJsonFile(){
	QStringList itemLists = ditWithSavedItemLists.entryList(QStringList("*.json"), QDir::Files | QDir::NoSymLinks);
	bool moreThanOneFile = itemLists.size() != 1;
	QString pathToFile;
	QFileInfo info;

	if (moreThanOneFile) {
		QFileDialog fDial(this, NULL, ditWithSavedItemLists.absolutePath(), "*.json");
		bool accepted = fDial.exec() == QDialog::Accepted;
		if (!accepted)
			return;
		QStringList fileList = fDial.selectedFiles();
		if (fileList.size() == 0)
			return;
		pathToFile = fileList.first();
	}
	else
		pathToFile = ditWithSavedItemLists.absoluteFilePath(itemLists.first());

	listFileName = QFileInfo(pathToFile).fileName();

	QJsonObject obj;	
	JsonParser().openJsonFile(obj, pathToFile);
	QJsonArray arr = obj["offers"].toArray();
	if (arr.count() == 0) {
		QString text = tr("File is uncorrect or empty.");
		Logger::logPotenialBug(text, "Market", "readListFromJsonFile");
		Utilities::showMessageBox_INFO(text);
		return;
	}
	offersList.clear();
	for each (QJsonValue var in arr)
		offersList.append(Offer(var.toObject()));

	setAndRepaintInfoLabel();
	repaitOfertsList();
}

void Market::removeItem(){
	QListWidget* list = ui->offerListWidget;
	int currentRow = list->currentRow();
	int count = list->count();

	bool elementInRange = currentRow >= 0 && currentRow < count;
	if (!elementInRange)
		return;

	offersList.removeAt(currentRow);
	repaitOfertsList();
	setAndRepaintInfoLabel();
}

void Market::test() {
	MarketProcess marketProcess(var, offersList, this->gameConnector, NULL);
	MarketProcessGui marketProcessGui;
	

	bool ok1 = connect(&marketProcessGui, SIGNAL(startMarketProcess()), &marketProcess, SLOT(startThread()), Qt::UniqueConnection);
	bool ok2 = connect(&marketProcessGui, SIGNAL(stopMarketProcess()), &marketProcess, SLOT(endProcess()), Qt::UniqueConnection);
	bool ok3 = connect(&marketProcess, SIGNAL(repaintLabelInGui(QString)), &marketProcessGui, SLOT(repaintLabel(QString)), Qt::UniqueConnection);
	bool ok4 = connect(&marketProcess, SIGNAL(paintProgressOnBar(int, int)), &marketProcessGui, SLOT(printValueToProgressBar(int,int)), Qt::UniqueConnection);
	bool ok5 = connect(&marketProcess, SIGNAL(addTextToDisplayOnList(QString)), &marketProcessGui, SLOT(addTextToDisplayList(QString)), Qt::UniqueConnection);
	bool ok6 = connect(&marketProcess, SIGNAL(noMarketSignFound(QString)), &marketProcessGui, SLOT(displayNoMarketSignFoundEffect(QString)), Qt::UniqueConnection);
	bool allThingsAreConnected = ok1 && ok2 && ok3 && ok4 && ok5 && ok6;

	if (!allThingsAreConnected) {
		QString textUser = tr("Unexpected error occured, trading process will not begin.");
		QString textLog = "Error in connecting signals/slots";
		Logger::logPotenialBug(textLog, "Market", "test");
		return;
	}

	marketProcessGui.exec();
}

void Market::offerTypeChanged(){
	bool sellType = ui->sellTypeRadioButton->isChecked();
	QString min = sellType ? tr("Min. sell price") : tr("Min. buy price");
	QString max = sellType ? tr("Max. sell price") : tr("Max. buy price");
	ui->maxPriceLabel->setText(max);
	ui->minPriceLabel->setText(min);
	ui->maxPriceLabel->repaint();
	ui->minPriceLabel->repaint();
}

void Market::startTradingProcess(){
}

void Market::editItemButtonClicked(){
	auto listPtr = ui->offerListWidget;
	int index = listPtr->currentRow();
	bool isInRange = index >= 0 && index < offersList.size();
	if (!isInRange)
		return;
	Offer offer = offersList[index];
	ui->minPriceValue->setValue(offer.minPrice);
	ui->minPriceValue->repaint();
	ui->maxPriceValue->setValue(offer.maxPrice);
	ui->maxPriceValue->repaint();
	ui->amountValue->setValue(offer.amount);
	ui->amountValue->repaint();
	offerTypeChanged();
	ui->placeOfferCheckBox->setChecked(offer.placeOffer);
	ui->placeOfferCheckBox->repaint();
	for each (Item var in allItems){
		if (var.name == offer.itemName) {
			fillLabels(&var);
			break;
		}
	}
	removeItem();
}
