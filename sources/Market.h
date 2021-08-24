#pragma once
#include "Utilities.h"
#include <QDialog>
#include "JsonParser.h"
#include "Item.h"
#include "StringResource.h"
namespace Ui { class Market; };
typedef Item::SELLER Seller;
typedef Item::TYPE_OF_ITEM ItemType;
class Offert {
public:
	Offert(Item item, int minPrice, int maxPrice, int amount);
	QString toString();
	Item item;
	int minPrice, maxPrice, amount;
};

class Market : public QDialog{
	Q_OBJECT
public slots:
	void sellerGroupBoxChanged();
	void categoryChanged();
	void itemChanged();
	void addItemButtonPressed();

public:
	Market();
	~Market();

private:
	Ui::Market *ui;
	QDir ditWithSavedItemLists;
	QList<Item> allItems;
	QList<Offert> offertsList;
	Item currentlyDisplayedItem;

	Seller filtr_seller;

	bool isPl = StringResource::languageIsPl();
	//change to map
	const QString TEXT_ON_LABEL_ITEM_NAME = isPl ? QString::fromLocal8Bit("Nazwa: ") : "Name: ";
	const QString TEXT_ON_LABEL_ITEM_PRICE = isPl ? QString::fromLocal8Bit("Cena: ") : "Price: ";
	const QString TEXT_ON_LABEL_ITEM_WEIGHT = isPl ? QString::fromLocal8Bit("Waga: ") : "Weight: ";
	const QString TEXT_ON_LABEL_RATION = isPl ? QString::fromLocal8Bit("Cena/waga: ") : "Price/weight: ";
	const QString TEXT_ON_LABEL_BUYER = isPl ? QString::fromLocal8Bit("Kupiec: ") : "Buyer: ";

	void fillCategoryLists();
	void fillLabels(Item* item);
	void fillItemList(Seller sellerFiltr, ItemType categoryFiltr);
	bool checkIfItemIsAlreadyOnList(Offert& offertToCheck);
	void repaitOfertsList();
};
