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
	TRANSLATE_addNamesOfFieldTypesToList();
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
	QListWidget *list = ui->pointsList;
	Route::FIELDS_TYPE typeOfPoint = getFieldTypeComboBox();
	QString indexStr = QString::number(list->count());
	QString text = QString("[%1] %2").arg(indexStr, currentChoosenPoint.toString());
	route.addPoint(currentChoosenPoint, typeOfPoint);
	list->addItem(text);
	selectedItemOnListChanged();
	list->repaint();
	ui->fieldTypesBox->setCurrentIndex(0);
	ui->fieldTypesBox->repaint();
}

void RouteCreator::moveListItemUp(){
	QListWidget* list = ui->pointsList;
	int currentRow = list->currentRow();
	bool allRight = route.movePointUp(currentRow);
	if (allRight) {
		list->setCurrentRow(currentRow - 1);
		selectedItemOnListChanged();
		list->repaint();
	}
}

void RouteCreator::moveListItemDown(){
	QListWidget* list = ui->pointsList;
	int currentRow = list->currentRow();
	bool allRight = route.movePointDown(currentRow);
	if (allRight) {
		list->setCurrentRow(currentRow + 1);
		selectedItemOnListChanged();
		list->repaint();
	}
}

void RouteCreator::deletePoint(){
	auto list = ui->pointsList;
	int rowToDelete = list->currentRow();
	route.removePoint(rowToDelete);

	QStringList textOfAllPoints = route.toStringList();
	list->clear();
	for (size_t i = 0; i < textOfAllPoints.size(); i++) {
		list->addItem(textOfAllPoints[i]);
	}

	selectedItemOnListChanged();
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

void RouteCreator::TRANSLATE_addNamesOfFieldTypesToList(){
	QComboBox* box = ui->fieldTypesBox;
	bool isPl = StringResource::languageIsPl();
	QString textToSet;
	if (isPl) {
		textToSet = QString::fromLocal8Bit("[01] Zwyk³e pole");
		box->addItem(textToSet);
		textToSet = QString::fromLocal8Bit("[02] Schody do góry");
		box->addItem(textToSet);
		textToSet = QString::fromLocal8Bit("[03] Schody do do³u");
		box->addItem(textToSet);
		textToSet = QString::fromLocal8Bit("[04] Pole liny");
		box->addItem(textToSet);
		textToSet = QString::fromLocal8Bit("[05] Dziura, otwarta");
		box->addItem(textToSet);
		textToSet = QString::fromLocal8Bit("[06] Dziura, zamknieta");
		box->addItem(textToSet);
		textToSet = QString::fromLocal8Bit("[07] Drabina w górê");
		box->addItem(textToSet);
		textToSet = QString::fromLocal8Bit("[08] Drabian w dó³");
		box->addItem(textToSet);
		textToSet = QString::fromLocal8Bit("[09] Teleport");
		box->addItem(textToSet);
		textToSet = QString::fromLocal8Bit("[10] Punkt wyjscia");
		box->addItem(textToSet);
	}
	else {
		textToSet = "[01] Regular Field";
		box->addItem(textToSet);
		textToSet = "[02] Stairs-up";
		box->addItem(textToSet);
		textToSet = "[03] Stairs-down";
		box->addItem(textToSet);
		textToSet = "[04] Rope field";
		box->addItem(textToSet);
		textToSet = "[05] Hole, open";
		box->addItem(textToSet);
		textToSet = "[06] Hole, close";
		box->addItem(textToSet);
		textToSet = "[07] Ladder up";
		box->addItem(textToSet);
		textToSet = "[08] Ladder down";
		box->addItem(textToSet);
		textToSet = "[09] Teleport";
		box->addItem(textToSet);
		textToSet = "[10] Log out point";
		box->addItem(textToSet);
	}
	box->repaint();
}

void RouteCreator::routeTypeChanged(){
	typedef Route::ROUTE_TYPE Type;
	bool shouldBeCircle = route.routeType == Type::BACK_AND_FORTH;
	route.routeType = shouldBeCircle ? Type::CIRCLE : Type::BACK_AND_FORTH;

	bool isPl = StringResource::languageIsPl();
	QString buttonText = isPl ? QString::fromLocal8Bit("Typ: ") : "Type: ";
	QString routeTypeAsStr; 
	if(isPl)
		routeTypeAsStr = shouldBeCircle ? QString::fromLocal8Bit("Pêtla") : QString::fromLocal8Bit("w tê i z powrotem");
	else
		routeTypeAsStr = shouldBeCircle ? "Circle" : "Back-and-forth";
	buttonText.append(routeTypeAsStr);
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
	QListWidget* list = ui->pointsList;
	int size = list->count();
	int curr = list->currentRow();

	bool isFirstElement = curr == 0;
	bool elementCanBeMovedUp = !isFirstElement && (size >= 2);
	ui->movePointUpButton->setEnabled(elementCanBeMovedUp);

	bool isLastElement = curr == (size - 1);
	bool elementCanBeMovedDown = !isLastElement && (size >= 2);
	ui->movePointDownButton->setEnabled(elementCanBeMovedDown);

	bool thereAreElementsOnList = size > 0;
	bool thereIsSlectedItem = curr > -1;
	bool desiredState = thereAreElementsOnList && thereIsSlectedItem;
	ui->deletePointButton->setEnabled(desiredState);

	
	QStringList textOfAllPoints = route.toStringList();
	for (size_t i = 0; i < size; i++){
		bool textForItemExist = i < textOfAllPoints.size();
		if (!textForItemExist)
			break;
		list->item(i)->setText(textOfAllPoints[i]);
	}
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

Route::FIELDS_TYPE RouteCreator::getFieldTypeComboBox() {
	QString textFromBox = ui->fieldTypesBox->currentText();
	QString indexOfFieldType = textFromBox.mid(1, 2);
	int index = indexOfFieldType.toInt();
	index--;

	return (Route::FIELDS_TYPE)index;
}
