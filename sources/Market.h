#pragma once
#include "Utilities.h"
#include <QDialog>
#include "JsonParser.h"
#include "Item.h"
#include "SetNameWidnow.h"
#include "qfiledialog.h"
#include "MarketProcess.h"
#include "Calibrator.h"
#include "Offer.h"
#include <QTranslator>
#include "activeGameThread.h"
#include "GameConnecter.h"
#include <memory>
/*
namespace Ui { class Market; };
typedef Item::SELLER Seller;
typedef Item::TYPE_OF_ITEM ItemType;
*/

class Market : public QDialog{
	Q_OBJECT
/*
public slots:
	void sellerFiltrChange();
	void categoryChanged();
	void itemChanged();
	void addItemButtonPressed();
	void saveListToJsonFile();
	void readListFromJsonFile();
	void removeItem();
	void test();
	void offerTypeChanged();
	void startTradingProcess();
	void editItemButtonClicked();
public:
	Market(std::shared_ptr<VariablesClass> var, std::shared_ptr<GameConnecter> gameConnector);
	~Market();

private:
	Ui::Market *ui;
	std::shared_ptr<VariablesClass> var;
	QDir ditWithSavedItemLists;
	QList<Item> allItems;
	QList<Offer> offersList;
	Item currentlyDisplayedItem;
	QString listFileName;
	Seller filtr_seller;
	std::shared_ptr<GameConnecter> gameConnector;

	void fillCategoryLists();
	void fillLabels(Item* item);
	void fillItemList(Seller sellerFiltr, ItemType categoryFiltr);
	bool checkIfItemIsAlreadyOnList(Offer& offerToCheck);
	void repaitOfertsList();
	bool twoListAreTheSame(QList<Offer> &list1, QList<Offer> &list2);
	void setAndRepaintInfoLabel();
	*/
};
