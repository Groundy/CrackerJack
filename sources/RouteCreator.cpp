#include "RouteCreator.h"
#include "ui_RouteCreator.h"

RouteCreator::RouteCreator(QDialog* parent)
	: QDialog(parent){
	ui = new Ui::RouteCreator();
	ui->setupUi(this);
	repaintMap();

	/*
	ui->movePointUpButton->setEnabled(false);
	ui->movePointDownButton->setEnabled(false);
	ui->deletePointButton->setEnabled(false);
	TRANSLATE_addNamesOfFieldTypesToList();
	*/
}
RouteCreator::~RouteCreator() {
	for each (QImage * map in floorMaps.values()) {
		delete map;
	}
	delete ui;
}
void RouteCreator::repaintMap() {
	int floor = currentPosition.getFloor();
	if (!loadMap(floor))
		return;

	const int currFrameSize = ORG_SIZE_OF_FRAME / zoom;
	QRect frameToDisplay = QRect(
		currentPosition.getX() - (currFrameSize / 2),
		currentPosition.getY() - (currFrameSize / 2),
		currFrameSize,
		currFrameSize);
	QPixmap pixMapToShow = getScaledPixMapToPaint(frameToDisplay);
	ui->imgLabel->setPixmap(pixMapToShow);
	refreshPositionLabel();
}
bool RouteCreator::loadMap(int floor) {
	try {
		if (floor < -8 || floor > 7)
			throw std::exception("Wrong input for loading map function!");

		bool mapAlreadyLoaded = floorMaps.contains(floor);
		if (!mapAlreadyLoaded) {
			QString pathToMap = PathResource::getPathToMergedColorMap(floor);
			QImage* mapImg = new QImage(pathToMap);
			if (mapImg->isNull())
				throw std::exception("Map in Route creator could not be read!");
			floorMaps.insert(floor, mapImg);
		}
		return true;
	}
	catch (const std::exception& e) {
		Logger::staticLog(e.what());
		return false;
	}
}
QPixmap RouteCreator::getScaledPixMapToPaint(QRect frameWithinMap){
	double ratio = 1.0 * ui->imgLabel->width() / frameWithinMap.width();
	QImage imgToScale = floorMaps.value(currentPosition.getFloor())->copy(frameWithinMap);
	QPoint middlePt(imgToScale.width() / 2, imgToScale.height() / 2);
	imgToScale.setPixel(middlePt, POSITION_MARK_COLOR);
	QImage displayImg(ui->imgLabel->size(), QImage::Format::Format_ARGB32);
	for (int x = 0; x < displayImg.width(); x++){
		int xPixFromSmallImg = x / ratio;
		for (int y = 0; y < displayImg.height(); y++){
			int yPixFromSmallImg = y / ratio;
			uint smallImgPix = imgToScale.pixel(xPixFromSmallImg, yPixFromSmallImg);
			displayImg.setPixel(x, y, smallImgPix);
		}
	}
	return QPixmap::fromImage(displayImg);
}
void RouteCreator::refreshPositionLabel() {
	/*
	bool isWalkable = currentPixIsWalkable();
	QString textBase = tr("   Walkable field: ");
	QString toAppendYes = tr("Yes");
	QString toAppendNo = tr("No");
	QString pointWakabilityStr = isWalkable ? (textBase + toAppendYes) : (textBase + toAppendNo);
	*/

	QString textToDisplay = currentPosition.toString();
	ui->positionCordinateLabel->setText(textToDisplay);
	ui->positionCordinateLabel->repaint();
}
void RouteCreator::changePlayerPos(int x, int y) {
	const auto mapSize = floorMaps.value(currentPosition.getFloor());
	if (y > 0) {
		const int maxY = mapSize->height() - 1;
		int tmpCordinateY = currentPosition.getY() + y;
		bool ptOutOfRange = tmpCordinateY > maxY;
		int toSet = ptOutOfRange ? maxY : tmpCordinateY;
		currentPosition.setY(toSet);
	}
	if (y < 0) {
		int tmpCordinateY = currentPosition.getY() + y;
		bool ptOutOfRange = tmpCordinateY < 0;
		int toSet = ptOutOfRange ? 0 : tmpCordinateY;
		currentPosition.setY(toSet);
	}
	if (x > 0) {
		const int maxX = mapSize->width() - 1;
		int tmpCordinateX = currentPosition.getX() + x;
		bool ptOutOfRange = tmpCordinateX > maxX;
		int toSet = ptOutOfRange ? maxX : tmpCordinateX;
		currentPosition.setX(toSet);
	}
	if (x < 0) {
		int tmpCordinateX = currentPosition.getX() + x;
		bool ptOutOfRange = tmpCordinateX < 0;
		int toSet = ptOutOfRange ? 0 : tmpCordinateX;
		currentPosition.setX(toSet);
	}
};

//slots
void RouteCreator::zoomChanged() {
	zoom = (2 * zoom == 8) ? 1 : (2 * zoom);
	QString textOnButton = QString("Zoom %1x").arg(QString::number(zoom));
	ui->zoomButton->setText(textOnButton);
	repaintMap();
}
void RouteCreator::cancelButtonPressed(){
	this->reject();
}
void RouteCreator::mapMoved() {
	if (sender() == ui->moveMapDown_1)
		changePlayerPos(0, 1);
	if (sender() == ui->moveMapDown_2)
		changePlayerPos(0, 5);
	if (sender() == ui->moveMapDown_3)
		changePlayerPos(0, 100);
	if (sender() == ui->moveMapLeft_1)
		changePlayerPos(-1, 0);
	if (sender() == ui->moveMapLeft_2)
		changePlayerPos(-5, 0);
	if (sender() == ui->moveMapLeft_3)
		changePlayerPos(-100, 0);
	if (sender() == ui->moveMapUp_1)
		changePlayerPos(0, -1);
	if (sender() == ui->moveMapUp_2)
		changePlayerPos(0, -5);
	if (sender() == ui->moveMapUp_3)
		changePlayerPos(0, -100);
	if (sender() == ui->moveMapRight_1)
		changePlayerPos(1, 0);
	if (sender() == ui->moveMapRight_2)
		changePlayerPos(5, 0);
	if (sender() == ui->moveMapRight_3)
		changePlayerPos(100, 0);

	repaintMap();
}
void RouteCreator::floorChanged(){
	int currentF = currentPosition.getFloor();
	if (sender() == ui->upFloor) {
		if (currentF == 7)
			return;
		currentPosition.setFloor(currentF +1);

	}
	else if (sender() == ui->lowerFloor) {
		if (currentF == -8)
			return;
		currentPosition.setFloor(currentF - 1);
	}
	repaintMap();
}


/*

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

void RouteCreator::repaintList(){
	QListWidget* list = ui->pointsList;
	list->clear();
	QStringList textOfAllPoints = route.toStringList();
	for each (QString var in textOfAllPoints){
		list->addItem(var);
	}
	list->repaint();
}

void RouteCreator::TRANSLATE_addNamesOfFieldTypesToList(){
	QComboBox* box = ui->fieldTypesBox;
	QString text;

	text = tr("[01] Regular Field");
	box->addItem(text);
	text = tr("[02] Stairs-up");
	box->addItem(text);
	text = tr("[03] Stairs-down");
	box->addItem(text);
	text = tr("[04] Rope field");
	box->addItem(text);
	text = tr("[05] Hole, open");
	box->addItem(text);
	text = tr("[06] Hole, close");
	box->addItem(text);
	text = tr("[07] Ladder up");
	box->addItem(text);
	text = tr("[08] Ladder down");
	box->addItem(text);
	text = tr("[09] Teleport");
	box->addItem(text);
	text = tr("[10] Log out point");
	box->addItem(text);
	box->repaint();
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



void RouteCreator::finishButtonPressed(){
	bool routeOk = checkRouteButtonPressed();
	if (!routeOk)
		return;

	QString textToDisplay = tr("Set name for new route.");
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
		textToDisplayToUser = tr("Route is correct.");
	Utilities::showMessageBox_INFO(textToDisplayToUser);
	return routeCorrect;
}

bool RouteCreator::currentPixIsWalkable(){
	uint pixCol = currentMapOfWalkability.pixel(currentChoosenPoint.getXY());
	RGBstruct rgb(pixCol);
	bool isWalkable = rgb.isGrey();
	if (!isWalkable) {
		uint pixColFromNormalMap = currentMap.pixel(currentChoosenPoint.getXY());
		RGBstruct rgbFromNormalMap = RGBstruct(pixColFromNormalMap);
		isWalkable = rgbFromNormalMap.isYellow();
	}
	return isWalkable;
}

void RouteCreator::moveMap(DIRECTIONS direction, int step){
	const int maxX = currentMap.width() - 1;
	const int maxY = currentMap.height() - 1;

	switch (direction)	{
	case DIRECTIONS::UP: {
		int tmpCordinateY = currentChoosenPoint.getY() - step;
		bool ptOutOfRange = tmpCordinateY  < 0;
		int toSet = ptOutOfRange ? 0 : tmpCordinateY;
		currentChoosenPoint.setY(toSet);
		break;
	}
	case DIRECTIONS::DOWN: {
		int tmpCordinateY = currentChoosenPoint.getY() + step;
		bool ptOutOfRange = tmpCordinateY  > maxY;
		int toSet = ptOutOfRange ? maxY : tmpCordinateY;
		currentChoosenPoint.setY(toSet);
		break;
	}
	case DIRECTIONS::RIGHT: {
		int tmpCordinateX = currentChoosenPoint.getX() + step;
		bool ptOutOfRange = tmpCordinateX  > maxX;
		int toSet = ptOutOfRange ? maxX : tmpCordinateX;
		currentChoosenPoint.setX(toSet);
		break;
	}
	case DIRECTIONS::LEFT: {
		int tmpCordinateX = currentChoosenPoint.getX() - step;
		bool ptOutOfRange = tmpCordinateX < 0;
		int toSet = ptOutOfRange ? 0 : tmpCordinateX;
		currentChoosenPoint.setX(toSet);
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
*/