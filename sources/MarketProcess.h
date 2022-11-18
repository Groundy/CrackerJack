#pragma once
#include <QThread>
#include <qrect.h>
#include "Utilities.h"
#include "ScreenSaver.h"
#include "ScreenAnalyzer.h"
#include "Offer.h"
#include "MarketProcessGui.h"
#include "GameConnecter.h"
#include <memory>
struct AlreadyPostedOffer {
	AlreadyPostedOffer() {};
	AlreadyPostedOffer(QString nameToSet, int priceToSet, int amountToSet) {
		name = nameToSet;
		price = priceToSet;
		amount = amountToSet;
	};
	bool operator ==(const AlreadyPostedOffer &offToComp) {
		bool sameName = this->name == offToComp.name;
		bool samePrice = this->price == offToComp.price;
		bool sameAmount = this->amount == offToComp.amount;
		return sameName && samePrice && sameAmount;
	}
	QString name;
	int price;
	int amount;
};
struct Pos {
	QRect itemListOnTheLeftOnWholeScreen;
	QRect insertItemNameBar;
	QRect clearItemNameButton;
	QRect firstItemOnListOnLeftSide;

	QRect acceptOffer_SELL;
	QRect moreItems_SELL;
	QRect lessItems_SELL;
	QRect itemList_SELL;
	QRect nameCol_SELL;
	QRect amountCol_SELL;
	QRect piecePriceCol_SELL;
	QRect scrollUp_SELL;
	QRect scrollDown_SELL;

	QRect acceptOffer_BUY;
	QRect moreItems_BUY;
	QRect lessItems_BUY;
	QRect itemList_BUY;
	QRect nameCol_BUY;
	QRect amountCol_BUY;
	QRect piecePriceCol_BUY;
	QRect scrollUp_BUY;
	QRect scrollDown_BUY;

	QRect goldPossessionBar;

	QRect Close_ReturnToMarket_Button;
	QRect myOffer_offerHistory_Button;
	QRect currentOffer_details_Button;

	QRect makeOffer_button_SELL;
	QRect makeOffer_button_BUY;
	QRect makeOffer_currentAmountBar;
	QRect makeOffer_lessItems;
	QRect makeOffer_moreItems;
	QRect makeOffer_piecePrice;
	QRect makeOffer_createButton;
	QRect makeOffer_anonymousBox;

	QRect myOffersWin_goUp_SELL;
	QRect myOffersWin_goDOWN_SELL;
	QRect myOffersWin_nameColumn_SELL;
	QRect myOffersWin_amountColumn_SELL;
	QRect myOffersWin_piecePriceColumn_SELL;
	QRect myOffersWin_cancelOfferButton_SELL;

	QRect myOffersWin_goUp_BUY;
	QRect myOffersWin_goDOWN_BUY;
	QRect myOffersWin_nameColumn_BUY;
	QRect myOffersWin_amountColumn_BUY;
	QRect myOffersWin_piecePriceColumn_BUY;
	QRect myOffersWin_cancelOfferButton_BUY;


	QRect firstOffer_Price_Sell;
	QRect firstOffer_Amount_Sell;

	QRect firstOffer_Price_Buy;
	QRect firstOffer_Amount_Buy;
};
namespace Ui { class MarketProcess; };

class MarketProcess : public QThread {
	Q_OBJECT
public:
	MarketProcess(VariablesClass* var, QList<Offer> offersThatShouldBe, std::shared_ptr<GameConnecter> gameConnector, QWidget* parent);
	~MarketProcess();
	enum class Type { BUY, SELL };
	enum class Actions { LOOKING_FOR_MARKET_WINDOW,SCANNING_OFFER_LIST, LOOKING_FOR_ITEM, PLACING_OFFER, CANCELING_OFFER, FINISHED, CANCELING };
	enum class TradeAction {
		GO_NEXT_ITEM,
		BUY_LAST_OFFER_ITEM,
		PLACE_OFFER_TO_SELL,
		PLACE_OFFER_TO_BUY,
		CANCEL_OFFER_TO_SELL,
		CANCEL_OFFER_TO_BUY
	};
public slots:
	void startThread();
	void endProcess();
signals:
	void repaintLabelInGui(QString str);
	void paintProgressOnBar(int currValue, int maxValue);
	void addTextToDisplayOnList(QString str);
	void noMarketSignFound(QString str);
private:
	std::shared_ptr<GameConnecter> gameConnector;
	Pos pos;
	VariablesClass* var;
	QImage currentImg;
	HWND handlerToGame;
	const int HEIGH_OF_OFFER_ROW = 16 ;
	const int HEIGH_OF_OFFER_FIELD = 177;
	QList<AlreadyPostedOffer> myCurrentOffers_SELL, myCurrentOffers_BUY;
	QList<Offer> userOfferList;
	bool loopEnabled = true;

	void initPos();
	void recalculatePositions(QPoint leftTopCorner);
	QPoint findTopLeftCornerOfMarketWin();
	bool askForScreenAndReadIt();
	void mainLoop();
	void readAllMyOfferts();
	bool readFirstOffertsOfCurrentItem(int& price, int& amount, Type type);
	int appendDisplayedOfferts(QList<AlreadyPostedOffer>& set, Type type);
	void splitPicToBlackWhiteRows(QImage& imgToSplit, QList<QImage>& rows);
	bool currentOfferIsMyOffer(QString nameWithoustSpaces, int price, Type type);
	void cancelOffer(QString name, Type type);
	int howMuchMoneyDoIHave();
	TradeAction decideWhatToDo(
		int currentCash,
		int amountOfLastSellOffer,
		int priceOfLastSellOffer,
		int amountOfLastBuyOffer,
		int priceOfLastBuyOffer,
		Offer offer
	);
	void scrollDownListOfmyOffers(Type type);
	bool checkIfThisItemIscurrentlyOnMarket(QString itemName, Type type);
	void setOffer(Type type, int lastOfferPrice, int cash, Offer offer);
	void appendStrToTradeLog(QString strToWrite);
	void buyLastOffer(int currentlyPossesedCash, int priceOfLastOffer_SELL,int amountOfLastOffer_SELL, QString itemName);
	void sendTextToDisplay(Actions action, QString itemName);
	void run();
};

