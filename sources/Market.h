#pragma once
#include "Utilities.h"
#include <QWidget>
#include "JsonParser.h"
#include "Item.h"
namespace Ui { class Market; };
typedef Item::SELLER Seller;
typedef Item::TYPE_OF_ITEM ItemType;

class Market : public QWidget{
	Q_OBJECT
public slots:
	void showOnly_GreenDijnn();
	void showOnly_BlueDijnn();
	void showOnly_Yasir();
	void showOnly_Esrik();
	void showOnly_Rashid();
	void showOnly_All();


public:
	Market(QWidget *parent = Q_NULLPTR);
	~Market();

private:
	Ui::Market *ui;
	QDir ditWithSavedItemLists;
	void fillLists(Seller* seller, ItemType* type);
};
