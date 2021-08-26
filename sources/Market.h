#pragma once
#include "Utilities.h"
#include <QDialog>
#include "JsonParser.h"
#include "Item.h"
#include "StringResource.h"
#include "SetNameWidnow.h"
#include "qfiledialog.h"
namespace Ui { class Market; };
typedef Item::SELLER Seller;
typedef Item::TYPE_OF_ITEM ItemType;
class Offer {
public:
	Offer(QString itemName, int minPrice, int maxPrice, int amount);
	QString toString();
	QString itemName;
	int minPrice, maxPrice, amount;
};

class Market : public QDialog{
	Q_OBJECT
public slots:
	void sellerGroupBoxChanged();
	void categoryChanged();
	void itemChanged();
	void addItemButtonPressed();
	void saveListToJsonFile();
	void readListFromJsonFile();
	void removeItem();

public:
	Market();
	~Market();

private:
	Ui::Market *ui;
	QDir ditWithSavedItemLists;
	QList<Item> allItems;
	QList<Offer> offersList;
	Item currentlyDisplayedItem;

	Seller filtr_seller;

	bool isPl;

	void fillCategoryLists();
	void fillLabels(Item* item);
	void fillItemList(Seller sellerFiltr, ItemType categoryFiltr);
	bool checkIfItemIsAlreadyOnList(Offer& offerToCheck);
	void repaitOfertsList();
};
