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
	TRANSLATE_gui();

	QDir dir = QDir::temp();
	dir.cdUp();
	bool CrackerJackDirExist = dir.cd("CrackerJack");
	if (!CrackerJackDirExist) {
		bool dirMade = dir.mkdir("CrackerJack");
		if (!dirMade)
			Logger::logPotenialBug("Can't create path to folder with routes","RouteCreator","RouteCreator");
		dir.cd("CrackerJack");
	}
	bool RoutesDirExist = dir.cd("Routes");
	if (!RoutesDirExist) {
		bool dirMade = dir.mkdir("Routes");
		if (!dirMade)
			Logger::logPotenialBug("Can't create path to folder with routes", "RouteCreator", "RouteCreator");
		dir.cd("Routes");
	}
	dirWithRoutes = dir;
}

RouteCreator::~RouteCreator(){
	delete ui;
}

void RouteCreator::imgMoved_left(){
	moveMap(DIRECTIONS::LEFT, 2);
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
	route.addPoint(currentChoosenPoint, typeOfPoint);
	selectedItemOnListChanged();
	repaintList();
	ui->fieldTypesBox->setCurrentIndex(0);
	ui->fieldTypesBox->repaint();
}

void RouteCreator::moveListItemUp(){
	QListWidget* list = ui->pointsList;
	int currentRow = list->currentRow();
	if (currentRow == -1 || currentRow == 0)
		return;
	bool allRight = route.movePointUp(currentRow);
	if (allRight) {
		selectedItemOnListChanged();
		repaintList();
		list->setCurrentRow(currentRow - 1);
	}
}

void RouteCreator::moveListItemDown(){
	QListWidget* list = ui->pointsList;
	int currentRow = list->currentRow();
	if (currentRow == -1 || currentRow == route.size() - 1)
		return;
	bool allRight = route.movePointDown(currentRow);
	if (allRight) {
		selectedItemOnListChanged();
		repaintList();
		list->setCurrentRow(currentRow + 1);
	}
}

void RouteCreator::deletePoint(){
	auto list = ui->pointsList;
	int rowToDelete = list->currentRow();
	if (rowToDelete == -1)
		return;
	route.removePoint(rowToDelete);
	selectedItemOnListChanged();
	repaintList();

	bool delRowWasntLast = rowToDelete < route.size();
	int rowToSet = delRowWasntLast ? rowToDelete : route.size() - 1;
	list->setCurrentRow(rowToDelete);
}

bool RouteCreator::loadMap(int floor){
	//floor (-8 bottom, 0 ground, 7 top)
	floor += 8;// (0 bottom, 8 ground, 15 top)
	floor = 15 - floor;// (15 bottom, 7 ground, 0 top)
	bool wrongInput = floor < 0 || floor >15;
	if (wrongInput) {
		Logger::logPotenialBug("Wrong input, input=" + QString::number(floor), "RouteCreator", "loadMap");
	}

	QString floorAsStr = QString::number(floor);
	if (floorAsStr.size() == 1)
		floorAsStr.push_front(QString::number(0));
		

	QString mapFileName = "floor-" + floorAsStr + "-map.png";
	QString walkMapFileName = "floor-" + floorAsStr + "-path.png";
	QString pathToMap = dirWithMaps.absoluteFilePath(mapFileName);
	QString pathToWalkMap = dirWithMaps.absoluteFilePath(mapFileName);

	QImage mapTmpImg, walkMapTmpImg;
	bool mapLoadedCorrectly = mapTmpImg.load(pathToMap);
	bool walkMapLoadedCorrectly = walkMapTmpImg.load(pathToWalkMap);

	if (mapLoadedCorrectly && walkMapLoadedCorrectly) {
		currentMap = mapTmpImg;
		currentMapOfWalkability = walkMapTmpImg;
	}
	else {
		QString text = QString("Can't load file \n %1 \n or \n %2").arg(pathToMap, pathToWalkMap);
		Logger::logPotenialBug(text, "RouteCreator", "loadMap");
		return false;
	}

	return true;
}

void RouteCreator::repaintList(){
	QListWidget* list = ui->pointsList;
	list->clear();
	QStringList textOfAllPoints = route.toStringList();
	for each (QString var in textOfAllPoints){
		list->addItem(var);
	}
	list->repaint();
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
	QString text;

	text = isPl ? QString::fromLocal8Bit("[01] Zwyk³e pole") : "[01] Regular Field";
	box->addItem(text);
	text = isPl ? QString::fromLocal8Bit("[02] Schody do góry") : "[02] Stairs-up";
	box->addItem(text);
	text = isPl ? QString::fromLocal8Bit("[03] Schody do do³u") : "[03] Stairs-down";
	box->addItem(text);
	text = isPl ? QString::fromLocal8Bit("[04] Pole liny") : "[04] Rope field";
	box->addItem(text);
	text = isPl ? QString::fromLocal8Bit("[05] Dziura, otwarta") : "[05] Hole, open";
	box->addItem(text);
	text = isPl ? QString::fromLocal8Bit("[06] Dziura, zamknieta") : "[06] Hole, close";
	box->addItem(text);
	text = isPl ? QString::fromLocal8Bit("[07] Drabina w górê") : "[07] Ladder up";
	box->addItem(text);
	text = isPl ? QString::fromLocal8Bit("[08] Drabian w dó³") : "[08] Ladder down";
	box->addItem(text);
	text = isPl ? QString::fromLocal8Bit("[09] Teleport") : "[09] Teleport";
	box->addItem(text);
	text = isPl ? QString::fromLocal8Bit("[10] Punkt wyjscia : ") : "[10] Log out point";
	box->addItem(text);
	box->repaint();
}

void RouteCreator::TRANSLATE_gui(){
	QString left = isPl ? QString::fromLocal8Bit("lewo") : "left";
	ui->moveMapLeft_1->setText(left);
	ui->moveMapLeft_2->setText(left + " 5x");
	ui->moveMapLeft_3->setText(left + " 100x");
	ui->moveMapLeft_1->repaint();
	ui->moveMapLeft_2->repaint();
	ui->moveMapLeft_3->repaint();

	QString up = isPl ? QString::fromLocal8Bit("góra") : "up";
	ui->moveMapUp_1->setText(up);
	ui->moveMapUp_2->setText(up + " 5x");
	ui->moveMapUp_3->setText(up + " 100x");
	ui->moveMapUp_1->repaint();
	ui->moveMapUp_2->repaint();
	ui->moveMapUp_3->repaint();

	QString down = isPl ? QString::fromLocal8Bit("dó³") : "down";
	ui->moveMapDown_1->setText(down);
	ui->moveMapDown_2->setText(down + " 5x");
	ui->moveMapDown_3->setText(down + " 100x");
	ui->moveMapDown_1->repaint();
	ui->moveMapDown_2->repaint();
	ui->moveMapDown_3->repaint();

	QString right = isPl ? QString::fromLocal8Bit("prawo") : "right";
	ui->moveMapRight_1->setText(right);
	ui->moveMapRight_2->setText(right + " 5x");
	ui->moveMapRight_3->setText(right + " 100x");
	ui->moveMapRight_1->repaint();
	ui->moveMapRight_2->repaint();
	ui->moveMapRight_3->repaint();

	QString floorDown = isPl ? QString::fromLocal8Bit("piêtro ni¿ej") : "floor down";
	ui->lowerFloor->setText(floorDown);
	ui->lowerFloor->repaint();

	QString floorUp = isPl ? QString::fromLocal8Bit("piêtro wy¿ej") : "floor up";
	ui->upFloor->setText(floorUp);
	ui->upFloor->repaint();

	QString pointUp = isPl ? QString::fromLocal8Bit("w górê") : "move up";
	ui->movePointUpButton->setText(pointUp);
	ui->movePointUpButton->repaint();

	QString pointDown = isPl ? QString::fromLocal8Bit("w dó³") : "move down";
	ui->movePointDownButton->setText(pointDown);
	ui->movePointDownButton->repaint();

	QString deletePoint = isPl ? QString::fromLocal8Bit("usuñ punkt") : "delete point";
	ui->deletePointButton->setText(deletePoint);
	ui->deletePointButton->repaint();

	QString addPoint = isPl ? QString::fromLocal8Bit("dodaj punkt") : "add point";
	ui->addPointButton->setText(addPoint);
	ui->addPointButton->repaint();

	QString cancelButton = isPl ? QString::fromLocal8Bit("Anuluj") : "Cancel";
	ui->cancelButton->setText(cancelButton);
	ui->cancelButton->repaint();

	QString finishButton = isPl ? QString::fromLocal8Bit("Zakoñcz") : "Finish";
	ui->finishButton->setText(finishButton);
	ui->finishButton->repaint();

	QString loadButton = isPl ? QString::fromLocal8Bit("Wczytaj trase") : "Load route";
	ui->loadRouteButton->setText(loadButton);
	ui->loadRouteButton->repaint();

	QString helpButton = isPl ? QString::fromLocal8Bit("Pomoc") : "Help";
	ui->helpButton->setText(helpButton);
	ui->helpButton->repaint();

	QString checkRouteButton = isPl ? QString::fromLocal8Bit("SprawdŸ poprawnoœæ trasy") : "Check route correctness";
	ui->checkButton->setText(checkRouteButton);
	ui->checkButton->repaint();

}

bool RouteCreator::repaintMap(){
	const QSize sizeOfLabel = ui->imgLabel->size();
	const int WIDTH = currentMap.width();
	const int HEIGHT = currentMap.height();

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
	bool isWalkable = currentPixIsWalkable();
	QString textBase = isPl ? QString::fromLocal8Bit("   Pole przechodnie: ") : "   Walkable field: ";
	QString toAppendYes = isPl ? "Tak" : "Yes";
	QString toAppendNo = isPl ? "Nie" : "No";
	QString pointWakabilityStr = isWalkable ? (textBase + toAppendYes) : (textBase + toAppendNo);


	QString textToDisplay = currentChoosenPoint.toString() + pointWakabilityStr;
	ui->positionCordinateLabel->setText(textToDisplay);
	ui->positionCordinateLabel->repaint();
}

void RouteCreator::selectedItemOnListChanged(){
	QListWidget* list = ui->pointsList;
	int size = route.size();
	int curr = list->currentRow();
	bool somethingIsSelected = curr != -1;

	bool isFirstElement = curr == 0;
	bool elementCanBeMovedUp = somethingIsSelected && !isFirstElement && (size >= 2);
	ui->movePointUpButton->setEnabled(elementCanBeMovedUp);

	bool isLastElement = curr == (size - 1);
	bool elementCanBeMovedDown = somethingIsSelected && !isLastElement && (size >= 2);
	ui->movePointDownButton->setEnabled(elementCanBeMovedDown);

	bool thereAreElementsOnList = size > 0;
	bool desiredState = thereAreElementsOnList && somethingIsSelected;
	ui->deletePointButton->setEnabled(desiredState);
}

void RouteCreator::cancelButtonPressed(){
	this->reject();
}

void RouteCreator::finishButtonPressed(){
	bool routeOk = checkRouteButtonPressed();
	if (!routeOk)
		return;

	QString textToDisplay = isPl ? QString::fromLocal8Bit("Wybierz nazwê dla nowej trasy.") : "Set name for new route.";
	QString title = "CrackerJack";
	QString fileName;
	SetNameWidnow nameWindow(this, title, textToDisplay, &fileName);
	int retCode = nameWindow.exec();
	if (retCode == QDialog::Accepted) {
		fileName.push_back(".json");
		route.writeToJsonFile(dirWithRoutes.path(), fileName);
	}
}

void RouteCreator::loadRouteButtonPressed(){
	QFileDialog fileDialog;
	fileDialog.setNameFilter("*.json");
	fileDialog.setDirectory(dirWithRoutes);
	int retCode = fileDialog.exec();
	bool accepted = retCode == QDialog::Accepted;
	if (!accepted)
		return;
	QStringList fileList = fileDialog.selectedFiles();
	if (fileList.size() == 0)
		return;
	QString pathToFile = fileList.first();

	route.loadFromJsonFile(pathToFile);
	repaintList();
	selectedItemOnListChanged();
}

void RouteCreator::helpButtonPressed()
{
}

bool RouteCreator::checkRouteButtonPressed(){
	QString textToDisplayToUser;
	bool routeCorrect = route.checkRouteCorectness(textToDisplayToUser);
	if (routeCorrect)
		textToDisplayToUser = isPl ? QString::fromLocal8Bit("Trasa jest w porz¹dku.") : "Route is correct.";
	Utilities::showMessageBox("CrackerJack", textToDisplayToUser, QMessageBox::Ok);
	return routeCorrect;
}

bool RouteCreator::currentPixIsWalkable(){
	QPoint pt(currentChoosenPoint.x, currentChoosenPoint.y);
	uint pixCol = currentMapOfWalkability.pixel(pt);
	RGBstruct rgb(pixCol);
	bool isWalkable = (rgb.r == rgb.g) && (rgb.r == rgb.b);
	if (!isWalkable) {
		pixCol = currentMap.pixel(pt);
		rgb = RGBstruct(pixCol);
		bool isYellowOnNormalMap = rgb.r == 255 && rgb.g == 255 && rgb.b == 0;
		if (isYellowOnNormalMap)
			isWalkable = true;
	}
	return isWalkable;
}

void RouteCreator::moveMap(DIRECTIONS direction, int step){
	const int maxX = currentMap.width() - 1;
	const int maxY = currentMap.height() - 1;

	switch (direction)	{
	case DIRECTIONS::UP: {
		int tmpCordinateY = currentChoosenPoint.y - step;
		bool ptOutOfRange = tmpCordinateY  < 0;
		currentChoosenPoint.y = ptOutOfRange ? 0 : tmpCordinateY;
		break;
	}
	case DIRECTIONS::DOWN: {
		int tmpCordinateY = currentChoosenPoint.y + step;
		bool ptOutOfRange = tmpCordinateY  > maxY;
		currentChoosenPoint.y = ptOutOfRange ? maxY : tmpCordinateY;
		break;
	}
	case DIRECTIONS::RIGHT: {
		int tmpCordinateX = currentChoosenPoint.x + step;
		bool ptOutOfRange = tmpCordinateX  > maxX;
		currentChoosenPoint.x = ptOutOfRange ? maxX : tmpCordinateX;
		break;
	}
	case DIRECTIONS::LEFT: {
		int tmpCordinateX = currentChoosenPoint.x - step;
		bool ptOutOfRange = tmpCordinateX < 0;
		currentChoosenPoint.x = ptOutOfRange ? 0 : tmpCordinateX;
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
