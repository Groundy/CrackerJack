#pragma once
#include <QObject>
#include <qrect.h>
struct Pos {
	QRect itemListOnTheLeftOnWholeScreen;
	QRect insertItemNameBar;
	QRect clearItemNameButton;

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
	QRect makeOffer_piecePrice;
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

};

class MarketProcess : public QObject{
public:
	MarketProcess();
	~MarketProcess();

private:
	Pos pos;
	void recalculatePositions(QPoint leftTopCorner);
	void initPos();
	/*
	void clickOnPoint(QPoint pt);
	void clickLeft(QPoint pt);
	void clickRight(QPoint pt);
	void clickOnPoint(QRect rect);
	void insertName(QString itemName);
	*/
	static const int heightOfColumnsNames = 0;//
};

