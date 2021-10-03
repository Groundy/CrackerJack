#include "MarketProcess.h"
#include "ui_MarketProcess.h"
MarketProcess::MarketProcess(VariablesClass* varToSet, QList<Offer> offersThatShouldBeSet, QWidget* parent = NULL) {
	ui = new Ui::MarketProcess();
	ui->setupUi(this);
	this->setParent(parent);
	this->var = varToSet;
	this->userOfferList = offersThatShouldBeSet;
	this->setWindowTitle("CrackerJack");
	QString cancelButtonText = isPl ? "Anuluj" : "Cancel";
	ui->cancelButton->setText(cancelButtonText);
	ui->cancelButton->repaint();
	QString runButtonText = "Start";
	ui->runButton->setText(runButtonText);
	ui->runButton->repaint();
	handlerToGame = Utilities::getHandlerToGameWindow(var->var_pidOfGame, var->var_winTitleOfGame);
	isPl = StringResource::languageIsPl();
}

MarketProcess::~MarketProcess()
{
}

void MarketProcess::recalculatePositions(QPoint pt) {
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


		rects.push_back(&pos.firstOffer_Price_Sell);
		rects.push_back(&pos.firstOffer_Amount_Sell);
		rects.push_back(&pos.firstOffer_Price_Buy);
		rects.push_back(&pos.firstOffer_Amount_Buy);
	}
	for each (QRect * rect in rects) {
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
	displayInfoOnInfoLabel(Actions::LOOKING_FOR_MARKET_WINDOW, NULL);
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
	//todo poprawic bez pojawiwa sie tych procesow
	ScreenSaver t(NULL, this->var);
	t.sendScreenRequestToGame(Key::HOME);//todo change
	Sleep(1500);
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
	Utilities::clickLeft(pos.myOffer_offerHistory_Button.center(), handlerToGame);
	Sleep(1000);

	readAllMyOfferts();
	Utilities::clickLeft(pos.Close_ReturnToMarket_Button.center(), handlerToGame);
	Sleep(20);
	Utilities::clickLeft(pos.makeOffer_anonymousBox.center(),handlerToGame);
	for (size_t i = 0; i < userOfferList.size(); i++) {
		Offer offer = userOfferList[i];
		displayInfoOnInfoLabel(Actions::LOOKING_FOR_ITEM, offer.itemName);
		int progress = (i*100) / userOfferList.size();
		ui->progressBar->setValue(progress);
		ui->progressBar->repaint();

		Utilities::clickLeft(pos.clearItemNameButton.center(), handlerToGame);
		Sleep(100);
		Utilities::clickLeft(pos.insertItemNameBar.center(), handlerToGame);
		Sleep(10);
		Utilities::sendStringToGame(offer.itemName, handlerToGame);
		Sleep(1000);
		Utilities::clickLeft(pos.firstItemOnListOnLeftSide.center(), handlerToGame);
		Sleep(1100);
		bool breakLoop = false;
		do {
			askForScreenAndReadIt();
			int priceOfLastOffer_SELL = 0, amountOfLastOffer_SELL = 0, priceOfLastOffer_BUY = 0, amountOfLastOffer_BUY = 0;
			readFirstOffertsOfCurrentItem(priceOfLastOffer_SELL, amountOfLastOffer_SELL, Type::SELL);
			readFirstOffertsOfCurrentItem(priceOfLastOffer_BUY, amountOfLastOffer_BUY, Type::BUY);

			int cash = howMuchMoneyDoIHave();
			if (cash == 0)
				break;

			TradeAction actionToDo = decideWhatToDo(cash, amountOfLastOffer_SELL, priceOfLastOffer_SELL, amountOfLastOffer_BUY, priceOfLastOffer_BUY, offer);
			switch (actionToDo)
			{
			case MarketProcess::TradeAction::BUY_LAST_OFFER_ITEM:
				buyLastOffer(cash, priceOfLastOffer_SELL, amountOfLastOffer_SELL, offer.itemName);
				break;
			case MarketProcess::TradeAction::PLACE_OFFER_TO_SELL:
				displayInfoOnInfoLabel(Actions::PLACING_OFFER, offer.itemName);
				setOffer(Type::SELL, priceOfLastOffer_SELL, cash, offer);
				break;
			case MarketProcess::TradeAction::PLACE_OFFER_TO_BUY:
				displayInfoOnInfoLabel(Actions::PLACING_OFFER, offer.itemName);
				setOffer(Type::BUY, priceOfLastOffer_BUY, cash, offer);
				break;
			case MarketProcess::TradeAction::CANCEL_OFFER_TO_SELL:
				displayInfoOnInfoLabel(Actions::CANCELING_OFFER, offer.itemName);
				cancelOffer(offer.itemName, Type::SELL);
				break;
			case MarketProcess::TradeAction::CANCEL_OFFER_TO_BUY:
				displayInfoOnInfoLabel(Actions::CANCELING_OFFER, offer.itemName);
				cancelOffer(offer.itemName, Type::BUY);
				break; 
			default:
				breakLoop = true;
				break; 
			}
		} while (!breakLoop);
	}
	ui->progressBar->setValue(100);
	ui->progressBar->repaint();
	displayInfoOnInfoLabel(Actions::FINISHED,NULL);
}

void MarketProcess::readAllMyOfferts() {
	displayInfoOnInfoLabel(Actions::SCANNING_OFFER_LIST, NULL);
	myCurrentOffers_BUY.clear();
	myCurrentOffers_SELL.clear();
	int addedPositions_Sell = 1, addedPositions_Buy = 1;
	int i = 0;
	do{
		askForScreenAndReadIt();
		if (addedPositions_Sell != 0) {
			addedPositions_Sell = appendDisplayedOfferts(myCurrentOffers_SELL, Type::SELL);
			scrollDownListOfmyOffers(Type::SELL);
		}
		if (addedPositions_Buy != 0) {
			addedPositions_Buy = appendDisplayedOfferts(myCurrentOffers_BUY, Type::BUY);
			scrollDownListOfmyOffers(Type::BUY);
		}
		i++;
	} while ((addedPositions_Buy > 0 || addedPositions_Sell > 0) && i < 13);
}
 
bool MarketProcess::readFirstOffertsOfCurrentItem(int& price, int& amount, Type type){
	bool sellType = type == Type::SELL;
	QRect priceRect = sellType ? pos.firstOffer_Price_Sell : pos.firstOffer_Price_Buy;
	QRect amountRect = sellType ? pos.firstOffer_Amount_Sell : pos.firstOffer_Amount_Buy;
	QImage priceImg = currentImg.copy(priceRect);
	QImage amountImg = currentImg.copy(amountRect);
	Utilities::imgToBlackAndWhiteOneColor(priceImg, 170);
	Utilities::imgToBlackAndWhiteOneColor(amountImg, 170);
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
			bool isNotBlack = imgToSplit.pixel(x, y) != BLACK;
			if (isNotBlack) {
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

bool MarketProcess::currentOfferIsMyOffer(QString name, int price, Type type){
	name = name.remove(" ").toLower();
	QList<AlreadyPostedOffer>* list = (type == Type::SELL) ? &myCurrentOffers_SELL : &myCurrentOffers_BUY;
	for each (AlreadyPostedOffer var in *list) {
		if(var.name == name && var.price == price)
			return true;
	}
	return false;
}

void MarketProcess::cancelOffer(QString itemName, Type type){
	QString itemNameModified = itemName.remove(" ").toLower();
	bool sellType = type == Type::SELL;
	QList<AlreadyPostedOffer>* offerList = sellType ? &myCurrentOffers_SELL : &myCurrentOffers_BUY;
	QString priceOfOfferStr;
	int indexInListOffer = -1;
	for (size_t i = 0; i < offerList->size(); i++){
		if (offerList->at(i).name == itemNameModified) {
			indexInListOffer = i;
			break;
		}
	}
	Utilities::clickLeft(pos.myOffer_offerHistory_Button.center(), handlerToGame);

	int i = 13;
	while (i-->0) {
		askForScreenAndReadIt();
		QList<AlreadyPostedOffer> list;
		appendDisplayedOfferts(list, type);
		bool offerFound = false;
		QPoint pointWithOffer;
		for (int i = 0; i < list.size(); i++) {
			offerFound = offerFound = itemNameModified == list[i].name;
			if (offerFound) {
				int xPosOnScreen = sellType ? pos.myOffersWin_amountColumn_SELL.x() : pos.myOffersWin_amountColumn_BUY.x();
				int yPosOnScreen = sellType ? pos.myOffersWin_amountColumn_SELL.y() : pos.myOffersWin_amountColumn_BUY.y();
				int heightOfAnyRow = pos.firstOffer_Amount_Sell.height();
				yPosOnScreen += (i + 1.5) * heightOfAnyRow;
				QString priceOfCanceledOfferStr = QString::number(list[i].price);
				pointWithOffer = QPoint(xPosOnScreen, yPosOnScreen);
				break;
			}
		}
		if (offerFound) {
			Utilities::clickLeft(pointWithOffer, handlerToGame);
			QPoint cancelButtonPos = sellType ? pos.myOffersWin_cancelOfferButton_SELL.center() : pos.myOffersWin_cancelOfferButton_BUY.center();
			Sleep(50);
			Utilities::clickLeft(cancelButtonPos, handlerToGame);
			{
				QString textOfMsg;
				QString priceStr = QString::number(offerList->at(indexInListOffer).price);
				if (isPl) {
					QString typeOfPlaceOffer = sellType? QString::fromLocal8Bit("sprzeda�y") : QString::fromLocal8Bit("kupna");
					textOfMsg = QString::fromLocal8Bit("Anulowanie oferty %1 %2 w cenie %3")
						.arg(typeOfPlaceOffer, itemName, priceStr);
				}
				else {
					QString typeOfPlaceOffer = sellType ? "sell" : "buy";
					textOfMsg = QString::fromLocal8Bit("Canceled %1 %2 offer in price %3")
						.arg(typeOfPlaceOffer, itemName, priceStr);
				}
				appendStrToTradeLog(textOfMsg);
				ui->textBrowser->append(textOfMsg);
			}
			i = 0;
			offerList->removeAt(indexInListOffer);
			break;
		}
		else {
			scrollDownListOfmyOffers(type);
		}
	}
	Utilities::clickLeft(pos.Close_ReturnToMarket_Button.center(), handlerToGame);
}

int MarketProcess::howMuchMoneyDoIHave(){
	QImage cashImg = currentImg.copy(pos.goldPossessionBar);
	Utilities::imgToBlackAndWhiteAllColors(cashImg, 150);
	QString cashAsStr = Utilities::imgWithStrToStr(cashImg);
	return cashAsStr.toInt();
}

MarketProcess::TradeAction MarketProcess::decideWhatToDo(int currentCash, int amountOfLastSellOffer, int priceOfLastSellOffer, int amountOfLastBuyOffer, int priceOfLastBuyOffer, Offer offer){
	if (offer.type == Offer::Type::BUY) {
		bool lastSellOfferHasGoodPrice = priceOfLastSellOffer <= offer.maxPrice;
		bool lastSellOfferIsMyOffer = currentOfferIsMyOffer(offer.itemName, priceOfLastSellOffer, Type::SELL);
		bool haveMoneyForAtLeastOneItem = currentCash > priceOfLastSellOffer && priceOfLastSellOffer != 0;
		bool buyLastItem = !lastSellOfferIsMyOffer && lastSellOfferHasGoodPrice && haveMoneyForAtLeastOneItem;

		if (buyLastItem)
			return TradeAction::BUY_LAST_OFFER_ITEM;
	
		if (offer.placeOffer) {
			bool itemOfferAlreadyOnMarket = checkIfThisItemIscurrentlyOnMarket(offer.itemName, Type::BUY);
			bool lastBuyOfferIsMine = currentOfferIsMyOffer(offer.itemName, priceOfLastBuyOffer, Type::BUY);
			
			bool shouldCancelOffer = itemOfferAlreadyOnMarket && !lastBuyOfferIsMine;
			if (shouldCancelOffer)
				return TradeAction::CANCEL_OFFER_TO_BUY;

			bool lastBuyOfferHasGoodPrice = priceOfLastBuyOffer <= offer.maxPrice - 1;
			bool iHaveMoneyToPostAtLeastOneOffer = currentCash >= (priceOfLastBuyOffer * 1.01) && priceOfLastBuyOffer != 0;
			bool icanPlaceOffer = myCurrentOffers_SELL.size() + myCurrentOffers_BUY.size() < 100;
			bool shouldPlaceOffer = lastBuyOfferHasGoodPrice && iHaveMoneyToPostAtLeastOneOffer && icanPlaceOffer && !lastBuyOfferIsMine;

			if (shouldPlaceOffer)
				return TradeAction::PLACE_OFFER_TO_BUY;
		}
	}
	else {
		bool lastSellOfferIsMineOffer = currentOfferIsMyOffer(offer.itemName, priceOfLastSellOffer, Type::SELL);
		bool offerOnThisItemIsAlreadyOnMarket = checkIfThisItemIscurrentlyOnMarket(offer.itemName, Type::SELL);
		
		bool shouldCancelMySellOffer = !lastSellOfferIsMineOffer && offerOnThisItemIsAlreadyOnMarket;
		if (shouldCancelMySellOffer)
			return TradeAction::CANCEL_OFFER_TO_SELL;

		bool lastOfferHaveAcceptablePrice = priceOfLastSellOffer >= offer.minPrice;
		bool icanPlaceOffer = myCurrentOffers_SELL.size() + myCurrentOffers_BUY.size() < 100;
		bool iHaveMoneyToPostAtLeastOneOffer = currentCash >= ((priceOfLastBuyOffer - 1.0) * 1.01) && priceOfLastBuyOffer != 0;

		bool shouldPlaceOffer = lastOfferHaveAcceptablePrice && iHaveMoneyToPostAtLeastOneOffer && icanPlaceOffer && !lastSellOfferIsMineOffer;
		if (shouldPlaceOffer)
			return TradeAction::PLACE_OFFER_TO_SELL;
	}
	return TradeAction::GO_NEXT_ITEM;
}

void MarketProcess::scrollDownListOfmyOffers(Type type){
	QPoint scrollDownPoint = type == Type::SELL ? pos.myOffersWin_goDOWN_SELL.center() : pos.myOffersWin_goDOWN_BUY.center();
	for (size_t i = 0; i < 8; i++) {
		Utilities::clickLeft(scrollDownPoint, handlerToGame);
		Sleep(20);
	}
}

bool MarketProcess::checkIfThisItemIscurrentlyOnMarket(QString itemName, Type type){
	itemName = itemName.remove(" ").toLower();
	QList<AlreadyPostedOffer>* list = (type == Type::SELL) ? &myCurrentOffers_SELL : &myCurrentOffers_BUY;
	for each (AlreadyPostedOffer var in *list){
		bool sameName = var.name == itemName;
		if (sameName)
			return true;
	}
	return false;
}

void MarketProcess::setOffer(Type typeOfOfferToSet, int lastOfferPrice, int cash, Offer offer){
	QString modifiedItemName = offer.itemName.remove(" ").toLower();
	bool isSellType = typeOfOfferToSet == Type::SELL;
	QPoint moreItemsButtonPoint = pos.makeOffer_moreItems.center();
	QPoint typeOfPlacedOffer = isSellType ? pos.makeOffer_button_SELL.center() : pos.makeOffer_button_BUY.center();
	QPoint piecePriceBar = pos.makeOffer_piecePrice.center();
	QPoint createOffer = pos.makeOffer_createButton.center();

	int newPrice = isSellType ? lastOfferPrice - 1 : lastOfferPrice + 1;
	if (isSellType)
		newPrice = min(newPrice, offer.maxPrice);
	else
		newPrice = std::max(newPrice, offer.minPrice);
	double factor = isSellType ? 0.01 : 1.01;
	int howManyItemsICanTrade = (cash / (newPrice * factor));
	int howManyItemsIShouldTrade = min(offer.amount, howManyItemsICanTrade);

	Utilities::clickLeft(typeOfPlacedOffer, handlerToGame);
	Sleep(20);
	Utilities::clickLeft(piecePriceBar, handlerToGame);
	Sleep(20);
	QString priceStr = QString::number(newPrice);
	Utilities::sendStringToGame(priceStr,handlerToGame);
	Sleep(30);
	for (size_t i = 0; i < howManyItemsIShouldTrade - 1; i++){
		Utilities::clickLeft(moreItemsButtonPoint, handlerToGame);
		Sleep(20);
	}
	Utilities::clickLeft(createOffer, handlerToGame);
	QList<AlreadyPostedOffer>* listPtr = isSellType ? &myCurrentOffers_SELL : &myCurrentOffers_BUY;
	AlreadyPostedOffer toAdd(modifiedItemName, newPrice, howManyItemsIShouldTrade);
	listPtr->push_back(toAdd);

	{
		QString amountStr = QString::number(howManyItemsIShouldTrade);
		QString priceStr = QString::number(newPrice);
		QString textOfMsg;
		if (isPl) {
			QString typeOfPlaceOffer = isSellType ? QString::fromLocal8Bit("sprzeda�y") : QString::fromLocal8Bit("kupna");
			textOfMsg = QString::fromLocal8Bit("Wstawiono oferte %1 %2 %3 za cen� %4")
			.arg(typeOfPlaceOffer, amountStr, modifiedItemName, priceStr);
		}
		else {
			QString typeOfPlaceOffer = isSellType ? "sell" : "buy";
			textOfMsg = QString::fromLocal8Bit("Placed offer to %1 %2 %3 for %4")
				.arg(typeOfPlaceOffer, amountStr, modifiedItemName, priceStr);
		}
		appendStrToTradeLog(textOfMsg);
		ui->textBrowser->append(textOfMsg);
	}
}

void MarketProcess::appendStrToTradeLog(QString strToWrite){
	QDir tmpFolder = Utilities::getDirWithCrackerJackTmpFolder(Utilities::FOLDERS_OF_TMP_FOLDER::TradeReports);
	QString formatStrDate = "dd_MM_yyyy";
	QString formatStrHour = "[hh:mm:ss]";
	QString fileName = "//" + QDateTime::currentDateTimeUtc().toString(formatStrDate) + ".txt";
	QString timeStamp = QDateTime::currentDateTimeUtc().toString(formatStrHour);
	QString pathToFile = tmpFolder.absolutePath() + fileName;

	QFile file(pathToFile);
	file.open(QIODevice::Append);
	file.write(timeStamp.toUtf8() + strToWrite.toUtf8() + "\n");
	file.close();
}

void MarketProcess::buyLastOffer(int currentlyPossesedCash, int priceOfLastOffer_SELL, int amountOfLastOffer_SELL, QString itemName) {
	int howManyItemsICanBuy = min((currentlyPossesedCash / priceOfLastOffer_SELL), amountOfLastOffer_SELL, QString itemName);
	bool amountToBuyIsOk = howManyItemsICanBuy >= 1 || howManyItemsICanBuy <= 64000;

	QPoint pt = pos.moreItems_SELL.center();
	for (size_t i = 0; i < howManyItemsICanBuy - 1; i++) {
		Utilities::clickLeft(pt, handlerToGame);
		Sleep(20);
	}
	pt = pos.acceptOffer_SELL.center();
	Utilities::clickLeft(pt, handlerToGame);
	{
		QString amountStr = QString::number(howManyItemsICanBuy);
		QString priceStr = QString::number(priceOfLastOffer_SELL);
		QString textOfMsg;
		if (isPl) {
			textOfMsg = QString::fromLocal8Bit("Kupiono %1 %2 za cen� %3")
				.arg(amountStr, itemName, priceStr);
		}
		else {
			textOfMsg = QString::fromLocal8Bit("Bought %1 %2 for %4")
				.arg(amountStr, itemName, priceStr);
		}
		appendStrToTradeLog(textOfMsg);
		ui->textBrowser->append(textOfMsg);
	}
}

void MarketProcess::displayInfoOnInfoLabel(Actions action, QString itemName){
	QString pl, eng;
	switch (action)
	{
	case MarketProcess::Actions::LOOKING_FOR_MARKET_WINDOW:
		pl = "Wyszukiwanie okna marketu.";
		eng = "Searching for market Window.";
		break;
	case MarketProcess::Actions::SCANNING_OFFER_LIST:
		pl = QString::fromLocal8Bit("Skanowanie listy wystawionych ju� ofert.");
		eng = "Scanning list of already posted offers.";
		break;
	case MarketProcess::Actions::LOOKING_FOR_ITEM: 
		pl = QString::fromLocal8Bit("Wyszukiwanie: %1").arg(itemName);
		eng = QString("Searching: %1").arg(itemName);
		break;
	case MarketProcess::Actions::PLACING_OFFER:
		pl = QString::fromLocal8Bit("Wystawianie oferty: %1").arg(itemName);
		eng = QString("Placing offer: %1").arg(itemName);
		break;
	case MarketProcess::Actions::CANCELING_OFFER:
		pl = QString::fromLocal8Bit("Anulowanie oferty %1").arg(itemName);
		eng = QString("Canceling offer %1").arg(itemName);
		break;
	case MarketProcess::Actions::FINISHED:
		pl = QString::fromLocal8Bit("Zako�czono.");
		break;
	default:
		break;
	}
	ui->itemNameLabel->setText(isPl ? pl : eng);
	ui->itemNameLabel->repaint();
}

void MarketProcess::runButtonClicked(){
	ui->progressBar->setValue(0);
	ui->progressBar->repaint();
	ui->runButton->setEnabled(false);
	initPos();
	mainLoop();
	ui->runButton->setEnabled(true);
}

void MarketProcess::cancelButtonClicked(){
}
