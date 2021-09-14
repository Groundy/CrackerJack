#pragma once
#include <QObject>
#include <qrect.h>
#include "Utilities.h"
#include "ScreenSaver.h"
#include "ScreenAnalyzer.h"
#include "Offer.h"

struct AlreadyPostedOffer {
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
class MarketProcess : public QObject {
public:
	MarketProcess(VariablesClass* var, QList<Offer> offersThatShouldBe);
	~MarketProcess();
	enum class Type { BUY, SELL };
	enum class TradeAction {
		BUY_LAST_OFFER_ITEM,
		PLACE_OFFER_TO_SELL,
		PLACE_OFFER_TO_BUY,
		CANCEL_OFFER_TO_SELL,
		CANCEL_OFFER_TO_BUY,
		GO_NEXT_ITEM
	};

private:
	Pos pos;
	bool isPl;
	void recalculatePositions(QPoint leftTopCorner);
	void initPos();
	VariablesClass* var;
	QImage currentImg;
	HWND handlerToGame;
	const int HEIGH_OF_OFFER_ROW = 16 ;
	const int HEIGH_OF_OFFER_FIELD = 177;
	QList<AlreadyPostedOffer> myCurrentOffers_SELL, myCurrentOffers_BUY;
	QList<Offer> offerThatShouldBe_BUY;


	QPoint findTopLeftCornerOfMarketWin();
	bool askForScreenAndReadIt();
	void mainLoop();
	void readAllMyOfferts();
	bool readFirstOffertsOfCurrentItem(int& price, int& amount, Type type);
	int appendDisplayedOfferts(QList<AlreadyPostedOffer>& set, Type type);
	void splitPicToBlackWhiteRows(QImage& imgToSplit, QList<QImage>& rows);
	bool currentOfferIsMyOffer(QString nameWithoustSpaces, int price, Type type);
	int howMuchMoneyDoIHave();
	TradeAction decideWhatToDo(
		int currentCash,
		int amountOfLastSellOffer,
		int priceOfLastSellOffer,
		int amountOfLastBuyOffer,
		int priceOfLastBuyOffer
	);
};

