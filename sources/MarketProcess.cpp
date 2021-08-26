#include "MarketProcess.h"

MarketProcess::MarketProcess(){
}

MarketProcess::~MarketProcess()
{
}

void MarketProcess::recalculatePositions(QPoint pt){
	QList<QRect&> rects; 
	{
		rects.push_back(pos.itemListOnTheLeftOnWholeScreen);
		rects.push_back(pos.insertItemNameBar);
		rects.push_back(pos.clearItemNameButton);

		rects.push_back(pos.acceptOffer_SELL);
		rects.push_back(pos.moreItems_SELL);
		rects.push_back(pos.lessItems_SELL);
		rects.push_back(pos.itemList_SELL);
		rects.push_back(pos.nameCol_SELL);
		rects.push_back(pos.amountCol_SELL);
		rects.push_back(pos.piecePriceCol_SELL);
		rects.push_back(pos.scrollUp_SELL);
		rects.push_back(pos.scrollDown_SELL);

		rects.push_back(pos.acceptOffer_BUY);
		rects.push_back(pos.moreItems_BUY);
		rects.push_back(pos.lessItems_BUY);
		rects.push_back(pos.itemList_BUY);
		rects.push_back(pos.nameCol_BUY);
		rects.push_back(pos.amountCol_BUY);
		rects.push_back(pos.piecePriceCol_BUY);
		rects.push_back(pos.scrollUp_BUY);
		rects.push_back(pos.scrollDown_BUY);

		rects.push_back(pos.goldPossessionBar);

		rects.push_back(pos.Close_ReturnToMarket_Button);
		rects.push_back(pos.myOffer_offerHistory_Button);
		rects.push_back(pos.currentOffer_details_Button);

		rects.push_back(pos.makeOffer_button_SELL);
		rects.push_back(pos.makeOffer_button_BUY);
		rects.push_back(pos.makeOffer_currentAmountBar);
		rects.push_back(pos.makeOffer_lessItems);
		rects.push_back(pos.makeOffer_moreItems);
		rects.push_back(pos.makeOffer_piecePrice);
		rects.push_back(pos.makeOffer_createButton);
		rects.push_back(pos.makeOffer_anonymousBox);

		rects.push_back(pos.myOffersWin_goUp_SELL);
		rects.push_back(pos.myOffersWin_goDOWN_SELL);
		rects.push_back(pos.myOffersWin_nameColumn_SELL);
		rects.push_back(pos.myOffersWin_amountColumn_SELL);
		rects.push_back(pos.myOffersWin_piecePriceColumn_SELL);
		rects.push_back(pos.myOffersWin_cancelOfferButton_SELL);

		rects.push_back(pos.myOffersWin_goUp_BUY);
		rects.push_back(pos.myOffersWin_goDOWN_BUY);
		rects.push_back(pos.myOffersWin_nameColumn_BUY);
		rects.push_back(pos.myOffersWin_amountColumn_BUY);
		rects.push_back(pos.myOffersWin_piecePriceColumn_BUY);
		rects.push_back(pos.myOffersWin_cancelOfferButton_BUY);
	}
	for each (QRect & var in rects){
		var.setX(var.x() + pt.x());
		var.setY(var.y() + pt.y());
	}
}

void MarketProcess::initPos()
{
	const int diff = 181;	
	pos.itemListOnTheLeftOnWholeScreen = QRect(17, 161, 147, 286); 
	pos.insertItemNameBar = QRect(161, 472, 105, 14);
	pos.clearItemNameButton = QRect(56, 472, 14, 14);

	pos.acceptOffer_SELL = QRect(691, 42, 43, 20);
	pos.moreItems_SELL = QRect(534, 46, 12, 12);
	pos.lessItems_SELL = QRect(396, 46, 21, 12);
	pos.itemList_SELL = QRect(187, 68, 533, 131);
	pos.nameCol_SELL = QRect(187, 68, 128, 14);
	pos.amountCol_SELL = QRect(317, 69, 59, 14);
	pos.piecePriceCol_SELL = QRect(377, 68, 83, 14);
	pos.scrollUp_SELL = QRect(721, 68, 12, 12);
	pos.scrollDown_SELL = QRect(721, 187, 12, 12);

	pos.acceptOffer_BUY = QRect(691, 42 + diff, 43, 20);
	pos.moreItems_BUY = QRect(534, 46 + diff, 12, 12);
	pos.lessItems_BUY = QRect(396, 46 + diff, 21, 12);
	pos.itemList_BUY = QRect(187, 68 + diff, 533, 131);
	pos.nameCol_BUY = QRect(187, 68 + diff, 128, 14);
	pos.amountCol_BUY = QRect(317, 69 + diff, 59, 14);
	pos.piecePriceCol_BUY = QRect(377, 68 + diff, 83, 14);
	pos.scrollUp_BUY = QRect(721, 68 + diff, 12, 12);
	pos.scrollDown_BUY = QRect(721, 187 + diff, 12, 12);

	pos.goldPossessionBar = QRect(17, 51, 102, 18);

	pos.Close_ReturnToMarket_Button = QRect(659, 509, 75, 20);
	pos.myOffer_offerHistory_Button = QRect(579, 509, 75, 20);
	pos.currentOffer_details_Button = QRect(499, 509, 75, 20);

	pos.makeOffer_button_SELL = QRect(188, 410, 8, 8);
	pos.makeOffer_button_BUY = QRect(188, 428, 8, 8);
	pos.makeOffer_currentAmountBar = QRect(367, 407, 96, 16);
	pos.makeOffer_lessItems = QRect(369, 426, 12, 12);
	pos.makeOffer_moreItems = QRect(507, 426, 12, 12);
	pos.makeOffer_piecePrice = QRect(370, 444, 137, 14);
	pos.makeOffer_createButton = QRect(691, 467, 43, 20);
	pos.makeOffer_anonymousBox = QRect(594, 472, 12, 12);

	pos.myOffersWin_goUp_SELL = QRect(721, 59, 12, 12);
	pos.myOffersWin_goDOWN_SELL = QRect(721, 239, 12, 12);
	pos.myOffersWin_nameColumn_SELL = QRect(17, 59, 199, 14);
	pos.myOffersWin_amountColumn_SELL = QRect(217, 59, 58, 14);
	pos.myOffersWin_piecePriceColumn_SELL = QRect(577, 59, 83, 14);
	pos.myOffersWin_cancelOfferButton_SELL = QRect(670, 28, 64, 20);


	pos.myOffersWin_goUp_BUY = QRect(721, 293, 12, 12);
	pos.myOffersWin_goDOWN_BUY = QRect(721, 473, 12, 12);
	pos.myOffersWin_nameColumn_BUY = QRect(17, 293, 199, 14);
	pos.myOffersWin_amountColumn_BUY = QRect(217, 293, 58, 14);
	pos.myOffersWin_piecePriceColumn_BUY = QRect(277, 293, 83, 14);
	pos.myOffersWin_cancelOfferButton_BUY = QRect(670, 262, 12, 12);
}
