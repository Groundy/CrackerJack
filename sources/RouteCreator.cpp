#include "RouteCreator.h"
#include "ui_RouteCreator.h"

RouteCreator::RouteCreator(QDialog* parent)
	: QDialog(parent){
	ui = new Ui::RouteCreator();
	ui->setupUi(this);
	loadMap(currentChoosenPoint.floor);
	repaintMap();
	refreshPositionLabel();
	ui->movePointUpButton->setEnabled(false);
	ui->movePointDownButton->setEnabled(false);
	ui->deletePointButton->setEnabled(false);
}

RouteCreator::~RouteCreator(){
	delete ui;
}

void RouteCreator::imgMoved_left(){
	moveMap(DIRECTIONS::LEFT, 1);
}
void RouteCreator::imgMoved_left_fast(){
	moveMap(DIRECTIONS::LEFT, 5);
}
void RouteCreator::imgMoved_left_very_fast(){
	moveMap(DIRECTIONS::LEFT, 100);
}
void RouteCreator::imgMoved_right(){
	moveMap(DIRECTIONS::RIGHT, 1);
}
void RouteCreator::imgMoved_right_fast(){
	moveMap(DIRECTIONS::RIGHT, 5);
}
void RouteCreator::imgMoved_right_very_fast(){
	moveMap(DIRECTIONS::RIGHT, 100);
}
void RouteCreator::imgMoved_top(){
	moveMap(DIRECTIONS::UP, 1);
}
void RouteCreator::imgMoved_top_fast() {
	moveMap(DIRECTIONS::UP, 5);
}
void RouteCreator::imgMoved_top_very_fast() {
	moveMap(DIRECTIONS::UP, 100);
}
void RouteCreator::imgMoved_down() {
	moveMap(DIRECTIONS::DOWN, 1);
}
void RouteCreator::imgMoved_down_fast() {
	moveMap(DIRECTIONS::DOWN, 5);
}
void RouteCreator::imgMoved_down_very_fast() {
	moveMap(DIRECTIONS::DOWN, 100);
}


void RouteCreator::floorDown(){
	if (currentChoosenPoint.floor == -8)
		return;
	currentChoosenPoint.floor--;
	loadMap(currentChoosenPoint.floor);
	refreshPositionLabel();
	repaintMap();
}

void RouteCreator::floorUp(){
	if (currentChoosenPoint.floor == 7)
		return;
	currentChoosenPoint.floor++;
	loadMap(currentChoosenPoint.floor);
	refreshPositionLabel();
	repaintMap();
}

void RouteCreator::addPoint(){
	QListWidget *list = ui->listWidget;
	int positionIndex = list->count();
	QString index = QString::number(positionIndex + 1);
	QString text = QString("[%1] %2").arg(index, currentChoosenPoint.toString());
	bool pointAlreadyAddedOnLastPos = false;
	if (positionIndex != 0) {
		QString lastElementText = list->item(positionIndex - 1)->text();
		QString lastPositionIndex = QString::number(positionIndex);
		QString textTmp = QString("[%1] %2").arg(lastPositionIndex, currentChoosenPoint.toString());
		pointAlreadyAddedOnLastPos = lastElementText == textTmp;
	}
	if (pointAlreadyAddedOnLastPos)
		return;

	FIELDS_TYPE typeOfPoint;
	PointInRootSetterWindow ptSetter(this,currentChoosenPoint.toString(), typeOfPoint);
	ptSetter.exec();

	list->addItem(text);
	selectedItemOnListChanged();
	list->repaint();
}

void RouteCreator::moveListItemUp(){
	QListWidget* list = ui->listWidget;
	int currentRow = list->currentRow();

	bool itIsFirstElement = currentRow == 0;
	bool itemNotSelected = currentRow == -1;
	if (itIsFirstElement || itemNotSelected)
		return;

	QString textOfItemCurrent = list->item(currentRow)->text();
	QString textOfItemAbove = list->item(currentRow - 1)->text();

	QStringList partsCurrent = textOfItemCurrent.split("] ", Qt::SkipEmptyParts);
	QStringList partsAbove = textOfItemAbove.split("] ", Qt::SkipEmptyParts);

	bool okStructs = partsCurrent.size() == 2 && partsAbove.size() == 2;
	if (!okStructs) {
		//todo log
		return;
	}

	partsAbove[0].swap(partsCurrent[0]);
	textOfItemCurrent = partsCurrent[0] + "] " + partsCurrent[1];
	textOfItemAbove = partsAbove[0] + "] " + partsAbove[1];

	list->item(currentRow)->setText(textOfItemAbove);
	list->item(currentRow + 1)->setText(textOfItemCurrent);

	list->repaint();
}

void RouteCreator::moveListItemDown(){
	QListWidget* list = ui->listWidget;
	int currentRow = list->currentRow();

	bool itIsLastElement = currentRow + 1  == list->count();
	bool itemNotSelected = (currentRow == -1);
	if (itIsLastElement || itemNotSelected)
		return;

	QString textOfItemCurrent = list->item(currentRow)->text();
	QString textOfItemBelow = list->item(currentRow + 1)->text();
	
	QStringList partsCurrent = textOfItemCurrent.split("] ", Qt::SkipEmptyParts);
	QStringList partsBelow = textOfItemBelow.split("] ", Qt::SkipEmptyParts);

	bool okStructs = partsCurrent.size() == 2 && partsBelow.size() == 2;
	if (!okStructs) {
		//todo log
		return;
	}

	partsBelow[0].swap(partsCurrent[0]);
	textOfItemCurrent = partsCurrent[0] + "] " + partsCurrent[1];
	textOfItemBelow = partsBelow[0] + "] " + partsBelow[1];

	list->item(currentRow)->setText(textOfItemBelow);
	list->item(currentRow + 1)->setText(textOfItemCurrent);

	list->repaint();
}

void RouteCreator::deletePoint(){
	auto list = ui->listWidget;
	int rowToDelete = list->currentRow();
	list->takeItem(rowToDelete);

	int sizeOfList = list->count();
	for (size_t i = rowToDelete; i < sizeOfList; i++){
		QString textOfCurrentItem = list->item(i)->text();
		QStringList partsOfPoint = textOfCurrentItem.split("] ",Qt::SkipEmptyParts);
		QString indexAsStr = partsOfPoint[0].remove("[");
		int index = indexAsStr.toInt();
		index--;
		indexAsStr = QString::number(index);
		QString textToSet = QString("[%1] %2").arg(indexAsStr, partsOfPoint[1]);
		list->item(i)->setText(textToSet);
	}

	list->repaint();
}

bool RouteCreator::loadMap(int floor){
	//floor (-8 bottom, 0 ground, 7 top)
	floor += 8;// (0 bottom, 8 ground, 15 top)
	floor = 15 - floor;// (15 bottom, 7 ground, 0 top)
	bool wrongInput = floor < 0 || floor >15;
	if (wrongInput)
		;//todo logg

	QString floorAsStr = QString::number(floor);
	if (floorAsStr.size() == 1)
		floorAsStr.push_front(QString::number(0));
		
	//todo zmeinic
	QString pathToMapFile = "C:\\Users\\ADMIN\\Desktop\\maps\\floor-" + floorAsStr + "-map.png";
	QImage tmpImg;
	bool loadedCorrectly = tmpImg.load(pathToMapFile);
	if (loadedCorrectly) {
		currentMap = tmpImg;
		currentlyLoadedFloor = floor;
	}
	return loadedCorrectly;
}

QPixmap RouteCreator::getPixMapWithZoomAndCenterPix(QImage imgWithMap, QSize sizeToScale) {
	QPixmap pixMaptoDisplay(imgWithMap.size());
	double ratio = 1.0 / (2*zoom);
	int xStart = imgWithMap.width() * (0.5 - ratio);
	int xEnd = imgWithMap.width() * (0.5 + ratio);
	int yStart = imgWithMap.height() * (0.5 - ratio);
	int yEnd = imgWithMap.height() * (0.5 + ratio);
	QImage displayImg(sizeToDisplay, QImage::Format::Format_ARGB32);

	for (size_t x = xStart; x < xEnd; x++) {
		for (size_t y = yStart; y < yEnd; y++) {
			bool itsMiddlePix = (x == imgWithMap.width() / 2) && (y == imgWithMap.height() / 2);
			uint pixColToSet = itsMiddlePix ? CHOOSEN_POINT_COLOR : imgWithMap.pixel(x, y);
			int startPoint_X = zoom * (x - xStart);
			int startPoint_Y = zoom * (y - yStart);
			for (size_t xTMP = 0; xTMP < zoom; xTMP++) {
				for (size_t yTMP = 0; yTMP < zoom; yTMP++)
					displayImg.setPixel(startPoint_X + xTMP, startPoint_Y + yTMP, pixColToSet);
			}
		}
	}
	pixMaptoDisplay = QPixmap::fromImage(displayImg).scaled(sizeToScale);
	return pixMaptoDisplay;
}

void RouteCreator::routeTypeChanged(){
	bool shouldBeCircle = routeType == ROUTE_TYPE::BACK_AND_FORTH;
	routeType = shouldBeCircle ? ROUTE_TYPE::CIRCLE : ROUTE_TYPE::BACK_AND_FORTH;

	QString routeTypeAsStr = shouldBeCircle ? "Circle" : "Back-and-forth";
	QString buttonText = QString("Route: %1").arg(routeTypeAsStr);
	ui->routeTypeButton->setText(buttonText);
	ui->routeTypeButton->repaint();
}

bool RouteCreator::repaintMap(){
	const QSize sizeOfLabel = ui->imgLabel->size();
	int startX = currentChoosenPoint.x - sizeToDisplay.width() / 2;
	int width =  sizeToDisplay.width();
	int startY = currentChoosenPoint.y - sizeToDisplay.height() / 2;
	int height =  sizeToDisplay.height();
	QRect rect(startX, startY, width, height);
	QImage orgImg = currentMap.copy(rect);
	QPixmap pixMaptoDisplay;
	pixMaptoDisplay = getPixMapWithZoomAndCenterPix(orgImg, sizeOfLabel);
	ui->imgLabel->setPixmap(pixMaptoDisplay);
	ui->imgLabel->repaint();
	return true;
}

void RouteCreator::zoomChanged(){
	zoom*=2;
	if (zoom == 8)
		zoom = 1;
	QString textOnButton = QString("Zoom %1x").arg(QString::number(zoom));
	ui->zoomButton->setText(textOnButton);
	repaintMap();
}

void RouteCreator::refreshPositionLabel(){
	QString textToDisplay = currentChoosenPoint.toString();
	ui->positionCordinateLabel->setText(textToDisplay);
	ui->positionCordinateLabel->repaint();
}

void RouteCreator::selectedItemOnListChanged(){
	QListWidget* list = ui->listWidget;

	bool isFirstElement = 0 == list->currentRow();
	bool elementCanBeMovedUp = !isFirstElement && (list->count() >= 2);
	ui->movePointUpButton->setEnabled(elementCanBeMovedUp);

	bool isLastElement = (list->count() - 1) == list->currentRow();
	bool elementCanBeMovedDown = !isLastElement && (list->count() >= 2);
	ui->movePointDownButton->setEnabled(elementCanBeMovedDown);

	bool thereAreElementsOnList = list->count() > 0;
	bool thereIsSlectedItem = list->currentRow() > -1;
	bool desiredState = thereAreElementsOnList && thereIsSlectedItem;
	ui->deletePointButton->setEnabled(desiredState);
}

void RouteCreator::moveMap(DIRECTIONS direction, int step){
	const int maxX = currentMap.width() - 1;
	const int maxY = currentMap.height() - 1;//to do zmienic na prawdziwe wartosci
	const int DISPLAY_HEIGHT = sizeToDisplay.height() / zoom;
	const int DISPLAY_WIDTH = sizeToDisplay.width() / zoom;
	switch (direction)	{
	case DIRECTIONS::UP: {
		int tmpCordinateY = currentChoosenPoint.y - step;
		bool imgWillGoOutOfRange = tmpCordinateY - DISPLAY_HEIGHT < 0;
		currentChoosenPoint.y = imgWillGoOutOfRange ? DISPLAY_HEIGHT : tmpCordinateY;
		break;
	}
	case DIRECTIONS::DOWN: {
		int tmpCordinateY = currentChoosenPoint.y + step;
		bool imgWillGoOutOfRange = tmpCordinateY + DISPLAY_HEIGHT > maxY;
		currentChoosenPoint.y = imgWillGoOutOfRange ? (maxY - DISPLAY_HEIGHT) : tmpCordinateY;
		break;
	}
	case DIRECTIONS::RIGHT: {
		int tmpCordinateX = currentChoosenPoint.x + step;
		bool imgWillGoOutOfRange = tmpCordinateX + DISPLAY_WIDTH > maxX;
		currentChoosenPoint.x = imgWillGoOutOfRange ? maxX - DISPLAY_WIDTH : tmpCordinateX;
		break;
	}
	case DIRECTIONS::LEFT: {
		int tmpCordinateX = currentChoosenPoint.x - step;
		bool imgWillGoOutOfRange = tmpCordinateX - DISPLAY_WIDTH < 0;
		currentChoosenPoint.x = imgWillGoOutOfRange ? DISPLAY_WIDTH : tmpCordinateX;
		break;
	}
	default:
		break;
	}

	refreshPositionLabel();
	repaintMap();
}

