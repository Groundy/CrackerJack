#pragma once
#include "Utilities.h"
#include <QDialog>
#include "JsonParser.h"
#include "Item.h"
namespace Ui { class Market; };
typedef Item::SELLER Seller;
typedef Item::TYPE_OF_ITEM ItemType;

struct ItemsSorted {
	QStringList namesOfCategories;
	QList<QList<Item>> itemsOfCategory;
};

class Market : public QDialog{
	Q_OBJECT
public slots:
	void showOnly_GreenDijnn();
	void showOnly_BlueDijnn();
	void showOnly_Yasir();
	void showOnly_Esrik();
	void showOnly_Rashid();
	void showOnly_All();


public:
	//Market(QDialog *parent = Q_NULLPTR);
	Market();
	~Market();

private:
	Ui::Market *ui;
	QDir ditWithSavedItemLists;
	QList<Item> allItems;
	ItemsSorted itemsToDisplay;
	void fillLists(Seller* seller);
};
