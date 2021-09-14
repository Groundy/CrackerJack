#include "MarketProcess.h"

MarketProcess::MarketProcess(VariablesClass* varToSet, QList<Offer> offersThatShouldBeSet){
	this->var = varToSet;
	this->offerThatShouldBe_BUY = offersThatShouldBeSet;
	initPos();
	handlerToGame = Utilities::getHandlerToGameWindow(var->var_pidOfGame, var->var_winTitleOfGame);
	isPl = StringResource::languageIsPl();

	mainLoop();
}

MarketProcess::~MarketProcess()
{
}

void MarketProcess::recalculatePositions(QPoint pt){
	QList<QRect*> rects; 
	{
		rects.push_back(&pos.itemListOnTheLeftOnWholeScreen);
		rects.push_back(&pos.insertItemNameBar);
		rects.push_back(&pos.clearItemNameButton);
		rects.push_back(&pos.firstItemOnListOnLeftSide);

		rects.push_back(&pos.acceptOffer_SELL);
		rects.push_back(&pos.moreItems_SELL);
		rects.push_back(&pos.lessItems_SELL);
		rects.push_back(&pos.itemList_SELL);
		rects.push_back(&pos.nameCol_SELL);
		rects.push_back(&pos.amountCol_SELL);
		rects.push_back(&pos.piecePriceCol_SELL);
		rects.push_back(&pos.scrollUp_SELL);
		rects.push_back(&pos.scrollDown_SELL);

		rects.push_back(&pos.acceptOffer_BUY);
		rects.push_back(&pos.moreItems_BUY);
		rects.push_back(&pos.lessItems_BUY);
		rects.push_back(&pos.itemList_BUY);
		rects.push_back(&pos.nameCol_BUY);
		rects.push_back(&pos.amountCol_BUY);
		rects.push_back(&pos.piecePriceCol_BUY);
		rects.push_back(&pos.scrollUp_BUY);
		rects.push_back(&pos.scrollDown_BUY);

		rects.push_back(&pos.goldPossessionBar);

		rects.push_back(&pos.Close_ReturnToMarket_Button);
		rects.push_back(&pos.myOffer_offerHistory_Button);
		rects.push_back(&pos.currentOffer_details_Button);

		rects.push_back(&pos.makeOffer_button_SELL);
		rects.push_back(&pos.makeOffer_button_BUY);
		rects.push_back(&pos.makeOffer_currentAmountBar);
		rects.push_back(&pos.makeOffer_lessItems);
		rects.push_back(&pos.makeOffer_moreItems);
		rects.push_back(&pos.makeOffer_piecePrice);
		rects.push_back(&pos.makeOffer_createButton);
		rects.push_back(&pos.makeOffer_anonymousBox);

		rects.push_back(&pos.myOffersWin_goUp_SELL);
		rects.push_back(&pos.myOffersWin_goDOWN_SELL);
		rects.push_back(&pos.myOffersWin_nameColumn_SELL);
		rects.push_back(&pos.myOffersWin_amountColumn_SELL);
		rects.push_back(&pos.myOffersWin_piecePriceColumn_SELL);
		rects.push_back(&pos.myOffersWin_cancelOfferButton_SELL);

		rects.push_back(&pos.myOffersWin_goUp_BUY);
		rects.push_back(&pos.myOffersWin_goDOWN_BUY);
		rects.push_back(&pos.myOffersWin_nameColumn_BUY);
		rects.push_back(&pos.myOffersWin_amountColumn_BUY);
		rects.push_back(&pos.myOffersWin_piecePriceColumn_BUY);
		rects.push_back(&pos.myOffersWin_cancelOfferButton_BUY);

		//rects.push_back(&pos.firstOffer_Name_Sell);
		rects.push_back(&pos.firstOffer_Price_Sell);
		rects.push_back(&pos.firstOffer_Amount_Sell);
	}
	for each (QRect* rect in rects){
		int widthCpy = rect->width();
		int heightCpy = rect->height();
		int x = rect->x() + pt.x();
		int y = rect->y() + pt.y();
		*rect = QRect(x, y, widthCpy, heightCpy);
	}
}

void MarketProcess::initPos(){
	const int diff = 181;	
	pos.itemListOnTheLeftOnWholeScreen = QRect(17, 161, 147, 286); 
	pos.insertItemNameBar = QRect(QPoint(56, 472), QPoint(160, 485));
	pos.clearItemNameButton = QRect(QPoint(161,472 ), QPoint(174, 485));
	pos.firstItemOnListOnLeftSide = QRect(17, 161, 148, 36);

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

	pos.goldPossessionBar = QRect(QPoint(17, 510), QPoint(107, 527));

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
	pos.myOffersWin_piecePriceColumn_SELL = QRect(277, 59, 83, 14);
	pos.myOffersWin_cancelOfferButton_SELL = QRect(670, 28, 64, 20);


	pos.myOffersWin_goUp_BUY = QRect(721, 293, 12, 12);
	pos.myOffersWin_goDOWN_BUY = QRect(721, 473, 12, 12);
	pos.myOffersWin_nameColumn_BUY = QRect(17, 293, 199, 14);
	pos.myOffersWin_amountColumn_BUY = QRect(217, 293, 58, 14);
	pos.myOffersWin_piecePriceColumn_BUY = QRect(277, 293, 83, 14);
	pos.myOffersWin_cancelOfferButton_BUY = QRect(670, 262, 12, 12);

	pos.firstOffer_Amount_Sell = QRect(QPoint(317, 84), QPoint(374, 99));
	pos.firstOffer_Price_Sell = QRect(QPoint(377, 84), QPoint(459, 99));

	pos.firstOffer_Amount_Buy = QRect(QPoint(317, 265), QPoint(374, 280));
	pos.firstOffer_Price_Buy = QRect(QPoint(377, 265), QPoint(459, 280));
}

QPoint MarketProcess::findTopLeftCornerOfMarketWin(){
	QString code = "44_9_#58#59#59_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#51#51#51_#144#144#144_#144#144#144_#144#144#144_#64#64#64_#56#56#56_#53#53#53_#71#71#71_#61#61#61_#60#60#61_#53#53#53_#144#144#144_#144#144#144_#144#144#144_#59#59#59_#57#57#57_#49#49#49_#78#79#79_#55#55#55_#58#58#58_#52#52#52_#144#144#144_#144#144#144_#144#144#144_#61#61#61_#54#54#54_#58#58#58_#52#52#52_#50#51#51_#54#54#54_#58#58#58_#144#144#144_#52#52#52_#56#56#56_#59#59#59_#57#57#57_#57#58#58_#56#56#56_#55#55#55_#144#144#144_#58#58#58_#64#64#64_#53#53#53_#56#56#56_#62#62#63_#51#51#51_#55#55#55_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#52#52#52_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#55#55#56_#61#61#61_#51#51#51_#54#55#55_#57#57#57_#52#52#52_#49#50#50_#47#48#48_#55#56#56_#61#61#61_#56#56#56_#56#57#56_#53#53#53_#60#60#59_#67#67#67_#53#54#53_#49#49#49_#66#67#66_#62#62#62_#55#55#55_#56#56#56_#58#58#58_#49#49#49_#52#52#52_#144#144#144_#144#144#144_#49#49#49_#53#53#53_#62#62#62_#55#55#55_#144#144#144_#56#56#56_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#52#53#53_#56#56#56_#59#60#59_#144#144#144_#56#56#56_#144#144#144_#51#51#51_#58#58#59_#144#144#144_#62#62#62_#54#55#55_#61#61#61_#144#144#144_#53#53#53_#144#144#144_#58#59#59_#55#55#55_#144#144#144_#53#53#53_#59#60#59_#58#58#58_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#55#55#55_#56#56#56_#57#57#57_#58#58#58_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#61#61#61_#51#52#51_#58#58#58_#55#56#56_#71#71#71_#56#56#56_#68#68#68_#61#61#61_#67#67#67_#55#55#55_#61#61#61_#58#58#58_#55#55#54_#55#55#54_#58#59#58_#66#66#66_#59#59#59_#56#56#56_#55#55#55_#57#57#56_#57#58#56_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#55#55#55_#57#58#57_#58#58#57_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#66#66#66_#58#58#58_#61#61#61_#54#55#54_#144#144#144_#51#51#51_#58#58#58_#58#58#58_#56#55#55_#55#55#55_#61#61#61_#59#59#59_#144#144#144_#144#144#144_#56#57#56_#59#59#59_#59#59#59_#52#53#53_#49#49#49_#58#58#58_#61#61#61_#144#144#144_#144#144#144_#55#55#54_#58#58#58_#57#57#56_#56#56#56_#54#54#55_#44#44#44_#49#49#49_#53#53#53_#67#68#67_#67#67#67_#60#61#60_#60#60#60_#55#55#55_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#57#58#58_#55#55#55_#53#54#53_#49#49#49_#54#54#54_#144#144#144_#144#144#144_#58#58#57_#60#61#60_#55#55#55_#61#61#61_#58#58#58_#53#53#53_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#57#57#57_#56#56#56_#57#58#57_#60#60#60_#144#144#144_#144#144#144_#62#62#62_#64#65#64_#144#144#144_#144#144#144_#55#55#55_#51#51#51_#49#49#49_#144#144#144_#56#57#56_#54#54#54_#48#48#48_#66#67#66_#144#144#144_#59#59#59_#56#56#56_#53#53#53_#49#49#48_#61#61#61_#50#50#49_#54#55#53_#49#49#49_#64#64#64_#59#60#60_#53#53#53_#56#57#56_#61#61#61_#55#55#55_#66#66#65_#59#59#59_#44#44#44_#61#61#61_#55#55#55_#61#61#61_#55#55#55_#58#58#58_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#61#61#61_#54#54#54_#65#65#64_#61#61#61_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#57#57#57_#53#53#53_#56#56#55_#144#144#144_#56#56#55_#144#144#144_#52#52#51_#58#58#58_#144#144#144_#61#61#61_#52#52#52_#55#55#55_#144#144#144_#64#64#64_#144#144#144_#47#47#46_#54#54#53_#144#144#144_#62#62#62_#55#55#55_#53#53#53_#144#144#144_#144#144#144_#144#144#144_#56#56#55_#45#45#44_#144#144#144_#62#62#62_#66#67#66_#63#64#63_#52#53#52_#144#144#144_#144#144#144_#65#65#65_#144#144#144_#55#55#54_#59#60#59_#49#49#49_#55#55#55_#61#61#61_#53#53#53_#49#49#49_#58#58#58_#64#64#65_#54#55#55_#55#56#55_#66#67#67_#53#53#53_#144#144#144_#49#49#49_#53#53#53_#55#55#54_#52#52#52_#68#68#68_#56#56#56_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#49#49#49_#58#59#58_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#144#144#144_#56#56#56_#58#58#58_#55#55#55_#144#144#144_#51#51#51_#59#59#59_#55#55#55_#56#56#56_#144#144#144_#62#62#62_#60#60#60_#57#58#58_#144#144#144_#56#56#56_#49#49#49_#57#57#56_#57#57#57_#144#144#144";
	QImage marketTitleImg = Utilities::getImageFromAdvancedCode(code);
	bool ok = askForScreenAndReadIt();
	QList<QPoint> marketWindowTitlePosList = Calibrator::findStartPositionInImg(marketTitleImg, currentImg);
	if (marketWindowTitlePosList.size() == 1) {
		QPoint marketWindowTitlePos = marketWindowTitlePosList.first();
		const QPoint DISTANCE_BETWEEN_TITLE_AND_LEFT_CORNER(353,4);
		QPoint toRet = marketWindowTitlePos - DISTANCE_BETWEEN_TITLE_AND_LEFT_CORNER;
		return toRet;
	}
	else {
		QString text, textPl, textEng;
		textPl = QString::fromLocal8Bit("Nie znaleziono otwartego okna marketu.");
		textEng = "Market window not detected.";
		text = isPl ? textPl : textEng;
		Logger::logPotenialBug(text,"MarketProcess","findTopLeftCornerOfMarketWin");
		Utilities::showMessageBox("CrackerJack", text, QMessageBox::Ok);
		return QPoint();
	}
}

bool MarketProcess::askForScreenAndReadIt(){
	/*tmp
	QImage img;
	img.load("C:\\Users\\ADMIN\\Desktop\\a.png");
	currentImg = img;
	return true;
	tmp*/
	
	//todo poprawic bez pojawiwa sie tych procesow
	ScreenSaver t(NULL, this->var);
	t.sendScreenRequestToGame(Key::HOME);//todo change
	ScreenAnalyzer tt(NULL, this->var, NULL);
	bool ok = tt.loadScreen(currentImg);
	return ok;
}

void MarketProcess::mainLoop() {
	QPoint leftTopCorner = findTopLeftCornerOfMarketWin();
	bool notFound = leftTopCorner == QPoint(0, 0);
	if (notFound)
		return;

	recalculatePositions(leftTopCorner);
	Utilities::clickLeft(pos.myOffer_offerHistory_Button.center(),handlerToGame);
	Sleep(1000);

	readAllMyOfferts();
	Utilities::clickLeft(pos.Close_ReturnToMarket_Button.center(), handlerToGame);
	for each (Offer var in offerThatShouldBe_BUY) {
		Utilities::clickLeft(pos.clearItemNameButton.center(), handlerToGame);
		Sleep(100);
		Utilities::clickLeft(pos.insertItemNameBar.center(), handlerToGame);
		Sleep(10);
		Utilities::sendStringToGame(var.itemName, handlerToGame);
		Sleep(1000);
		Utilities::clickLeft(pos.firstItemOnListOnLeftSide.center(), handlerToGame);
		Sleep(1100);
		askForScreenAndReadIt();
		bool breakLoop = false;
		do {
			int priceOfLastOffer, amountOfLastOffer;
			readFirstOffertsOfCurrentItem(priceOfLastOffer, amountOfLastOffer, Type::SELL);
			bool skip = priceOfLastOffer == 0 || amountOfLastOffer == 0;
			if (skip)
				break;
			bool ItemHaveProperPriceToBeBought = priceOfLastOffer <= var.maxPrice;
			QString itemNameWithoutSpaces = var.itemName.remove(" ");
			bool lastOfferIsMyOffer = currentOfferIsMyOffer(itemNameWithoutSpaces, priceOfLastOffer, Type::SELL);
			int cash = howMuchMoneyDoIHave();
			if (cash == 0)
				break;
			bool haveMoneyForAtLeastOneItem = cash >= priceOfLastOffer;
			int howManyItemsICanBuy = min((cash / priceOfLastOffer), amountOfLastOffer);
			bool amountToBuyIsOk = howManyItemsICanBuy >= 0 || howManyItemsICanBuy <= 64000;

			bool buyItems = !lastOfferIsMyOffer && haveMoneyForAtLeastOneItem && ItemHaveProperPriceToBeBought;
			bool placeOfferAndGoToNextItem = !lastOfferIsMyOffer && haveMoneyForAtLeastOneItem && !ItemHaveProperPriceToBeBought;
			TradeAction actionToDo = decideWhatToDo(cash, 0, 0, 0, 0);
			if (buyItems) {
				QPoint pt = pos.moreItems_SELL.center();
				for (size_t i = 0; i < howManyItemsICanBuy - 1; i++) {
					Utilities::clickLeft(pt, handlerToGame);
					Sleep(20);
				}
				pt = pos.acceptOffer_SELL.center();
				QString str = QString("buying %1 for %2").arg(var.itemName, QString::number(priceOfLastOffer));
				qDebug() << str;
				//Utilities::clickLeft(pt, handlerToGame);
			}
			else if (!lastOfferIsMyOffer && haveMoneyForAtLeastOneItem && !ItemHaveProperPriceToBeBought) {
				//place offer and go to next item
			}
			else//continue
				breakLoop = true;
		} while (!breakLoop);
	}
}

void MarketProcess::readAllMyOfferts() {
	myCurrentOffers_BUY.clear();
	myCurrentOffers_SELL.clear();
	int addedPositions;
	do{
		Sleep(1111);
		askForScreenAndReadIt();
		addedPositions = appendDisplayedOfferts(myCurrentOffers_SELL, Type::SELL);
		for (size_t i = 0; i < 8; i++) {
			Utilities::clickLeft(pos.myOffersWin_goDOWN_SELL.center(), handlerToGame);
			Sleep(20);
		}
	}while (addedPositions > 0);

	do {
		Sleep(1111);
		askForScreenAndReadIt();
		addedPositions = appendDisplayedOfferts(myCurrentOffers_BUY, Type::BUY);
		for (size_t i = 0; i < 8; i++) {
			Utilities::clickLeft(pos.myOffersWin_goDOWN_BUY.center(), handlerToGame);
			Sleep(20);
		}
	} while (addedPositions > 0);

}

 
bool MarketProcess::readFirstOffertsOfCurrentItem(int& price, int& amount, Type type){
	QRect priceRect, amountRect;
	if (type == Type::SELL) {
		priceRect = pos.firstOffer_Price_Sell;
		amountRect = pos.firstOffer_Amount_Sell;
	}
	else {
		priceRect = pos.firstOffer_Price_Buy;
		amountRect = pos.firstOffer_Amount_Buy;
	}
	QImage priceImg = currentImg.copy(priceRect);
	QImage amountImg = currentImg.copy(amountRect);
	QString priceStr = Utilities::imgWithStrToStr(priceImg);
	QString amountStr = Utilities::imgWithStrToStr(amountImg);
	amount = amountStr.toInt();
	price = priceStr.toInt();
	bool ok = amount > 0 && price > 0;
	return ok;
}

int MarketProcess::appendDisplayedOfferts(QList<AlreadyPostedOffer>& set, Type type){
	int x, y, w = 358, h = HEIGH_OF_OFFER_FIELD;
	if (type == Type::SELL) {
		x = pos.myOffersWin_nameColumn_SELL.x();
		y = pos.myOffersWin_nameColumn_SELL.y() + HEIGH_OF_OFFER_ROW;
	}
	else {
		x = pos.myOffersWin_nameColumn_BUY.x();
		y = pos.myOffersWin_nameColumn_BUY.y() + HEIGH_OF_OFFER_ROW;
	}
	QRect fieldArea(x, y, w, h);
	QImage allDisplayedOffers = currentImg.copy(fieldArea);
	QList<QImage> rows;
	splitPicToBlackWhiteRows(allDisplayedOffers, rows);
	QList<AlreadyPostedOffer> offers;
	for each (QImage var in rows){
		h = var.height();
		y = 0;
		QImage nameImg, amountImg, priceImg;
		if (type == Type::SELL) {
			x = 0;
			w = pos.myOffersWin_nameColumn_SELL.width();
			nameImg = var.copy(QRect(x,y,w,h));

			x = pos.myOffersWin_nameColumn_SELL.width();
			w = pos.myOffersWin_amountColumn_SELL.width();
			amountImg = var.copy(QRect(x, y, w, h));

			x += pos.myOffersWin_amountColumn_SELL.width();
			w = pos.myOffersWin_piecePriceColumn_SELL.width();
			priceImg = var.copy(QRect(x, y, w, h));
		}
		else {
			x = 0;
			w = pos.myOffersWin_nameColumn_BUY.width();
			nameImg = var.copy(QRect(x, y, w, h));

			x = pos.myOffersWin_nameColumn_BUY.width();
			w = pos.myOffersWin_amountColumn_BUY.width();
			amountImg = var.copy(QRect(x, y, w, h));

			x += pos.myOffersWin_amountColumn_BUY.width();
			w = pos.myOffersWin_piecePriceColumn_BUY.width();
			priceImg = var.copy(QRect(x, y, w, h));
		}

		bool nameSizeOk = nameImg.width() >= 4 && nameImg.height() >= 4;
		bool amountSizeOk = amountImg.width() >= 4 && amountImg.height() >= 4;
		bool priceSizeOk = priceImg.width() >= 4 && priceImg.height() >= 4;
		bool atLeatOneImgIsEmpty = !nameSizeOk || !amountSizeOk || !priceSizeOk;
		if (atLeatOneImgIsEmpty)
			continue;

		QString name = Utilities::imgWithStrToStr(nameImg);
		int amount = Utilities::imgWithStrToStr(amountImg).toInt();
		int price = Utilities::imgWithStrToStr(priceImg).toInt();

		bool readCorrectly = !name.isEmpty() && amount > 0 && price > 0;
		if (!readCorrectly)
			continue;

		AlreadyPostedOffer offerToAdd;
		offerToAdd.amount = amount;
		offerToAdd.price = price;
		offerToAdd.name = name;
		offers.append(offerToAdd);
	}

	QList<AlreadyPostedOffer> offersToAdd;
	for each (AlreadyPostedOffer tmpListOffer in offers) {
		bool isAlreadyOnList = false;
		for each (AlreadyPostedOffer permListOffer in set) {
			bool sameName = tmpListOffer.name == permListOffer.name;
			bool samePrice = tmpListOffer.price == permListOffer.price;
			if (sameName && samePrice) {
				isAlreadyOnList = true;
				break;
			}
		}
		if (!isAlreadyOnList)
			offersToAdd.push_back(tmpListOffer);
	}
	for each (AlreadyPostedOffer var in offersToAdd)
		set.append(var);
	return offersToAdd.size();

}

void MarketProcess::splitPicToBlackWhiteRows(QImage& imgToSplit, QList<QImage>& rows){
	Utilities::imgToBlackAndWhiteAllColors(imgToSplit, 130);
	Utilities::cutBlackBordersOfImg(imgToSplit);
	const uint BLACK = qRgb(0, 0, 0);
	QList<int> fullBlackRows;
	for (size_t y = 0; y < imgToSplit.height(); y++){
		bool isFullBlack = true;
		for (size_t x = 0; x < imgToSplit.width(); x++)	{
			uint pixCol = imgToSplit.pixel(x, y);
			if (pixCol != BLACK) {
				isFullBlack = false;
				break;
			}
		}
		if(isFullBlack)
			fullBlackRows.append(y);
	}

	if (fullBlackRows.isEmpty()) {
		rows.push_back(imgToSplit);
		return;
	}


	QList<QPair<int, int>> toCut;
	toCut.push_back(QPair<int,int>(0, fullBlackRows.first()));


	fullBlackRows.push_back(imgToSplit.height() - 1);
	for (size_t i = 0; i < fullBlackRows.size() - 1; i++){
		int startIndex = fullBlackRows[i] + 1;
		int height = fullBlackRows[i + 1] - fullBlackRows[i];
		if (height == 1)
			continue;

		QPair<int, int> toAdd(startIndex, height);
		toCut.push_back(toAdd);
	}

	for each (QPair<int, int> var in toCut)	{
		QRect rectToCut(0, var.first, imgToSplit.width(), var.second);
		rows.push_back(imgToSplit.copy(rectToCut));
	}
}

bool MarketProcess::currentOfferIsMyOffer(QString nameWithoustSpaces, int price, Type type){
	nameWithoustSpaces = nameWithoustSpaces.toLower();
	if (type == Type::SELL) {
		for each (AlreadyPostedOffer var in myCurrentOffers_SELL) {
			if (var.name != nameWithoustSpaces)
				continue;
			if (var.price != price)
				continue;
			return true;
		}
	}
	else if( type == Type::BUY ) {
		for each (AlreadyPostedOffer var in myCurrentOffers_BUY) {
			if (var.name != nameWithoustSpaces)
				continue;
			if (var.price != price)
				continue;
			return true;
		}
	}
	return false;
}

int MarketProcess::howMuchMoneyDoIHave(){
	QImage cashImg = currentImg.copy(pos.goldPossessionBar);
	Utilities::imgToBlackAndWhiteAllColors(cashImg, 150);
	QString cashAsStr = Utilities::imgWithStrToStr(cashImg);
	return cashAsStr.toInt();
}

MarketProcess::TradeAction MarketProcess::decideWhatToDo(int currentCash, int amountOfLastSellOffer, int priceOfLastSellOffer, int amountOfLastBuyOffer, int priceOfLastBuyOffer){
	return TradeAction();
}
