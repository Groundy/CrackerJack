#include "RouteCreator.h"

#include "ui_RouteCreator.h"
namespace CJ {
RouteCreator::RouteCreator(QDialog* parent, Route* route) : QDialog(parent) {
  ui = new Ui::RouteCreator();
  ui->setupUi(this);
  repaintMap();
  fillNamesOfFieldTypesToList();
  ui->fieldTypesBox->insertItems(0, listOfRoutePointsType);
  ui->fieldTypesBox->repaint();
  loadFromRoute(route);
}
RouteCreator::~RouteCreator() {
  foreach (QImage* map, floorMaps.values()) {
    delete map;
  }
  delete ui;
}

//slots
void RouteCreator::mapMoved() {
  if (sender() == ui->moveMapDown_1) changePlayerPos(0, 1);
  if (sender() == ui->moveMapDown_2) changePlayerPos(0, 5);
  if (sender() == ui->moveMapDown_3) changePlayerPos(0, 100);
  if (sender() == ui->moveMapLeft_1) changePlayerPos(-1, 0);
  if (sender() == ui->moveMapLeft_2) changePlayerPos(-5, 0);
  if (sender() == ui->moveMapLeft_3) changePlayerPos(-100, 0);
  if (sender() == ui->moveMapUp_1) changePlayerPos(0, -1);
  if (sender() == ui->moveMapUp_2) changePlayerPos(0, -5);
  if (sender() == ui->moveMapUp_3) changePlayerPos(0, -100);
  if (sender() == ui->moveMapRight_1) changePlayerPos(1, 0);
  if (sender() == ui->moveMapRight_2) changePlayerPos(5, 0);
  if (sender() == ui->moveMapRight_3) changePlayerPos(100, 0);

  repaintMap();
}
void RouteCreator::floorChanged() {
  int currentF = currentPosition.getFloor();
  if (sender() == ui->upFloor) {
    if (currentF == 7) return;
    currentPosition.setFloor(currentF + 1);

  } else if (sender() == ui->lowerFloor) {
    if (currentF == -8) return;
    currentPosition.setFloor(currentF - 1);
  }
  repaintMap();
}
void RouteCreator::zoomChanged() {
  zoom                 = (2 * zoom == 8) ? 1 : (2 * zoom);
  QString textOnButton = QString("Zoom %1x").arg(QString::number(zoom));
  ui->zoomButton->setText(textOnButton);
  repaintMap();
}
void RouteCreator::cancelButtonPressed() {
  this->reject();
}
void RouteCreator::addPoint() {
  auto                  list        = ui->pointsList;
  RoutePoint::FieldType typeOfPoint = getFieldOfCurrentPt();
  route.addPoint(RoutePoint(currentPosition, typeOfPoint));
  selectedItemOnListChanged();
  repaintList();

  int newSize = list->count();
  if (newSize == 1) ui->fieldTypesBox->setCurrentIndex(0);
  ui->fieldTypesBox->repaint();
}
void RouteCreator::deletePoint() {
  auto list        = ui->pointsList;
  int  rowToDelete = list->currentRow();
  if (rowToDelete == -1) return;
  route.removePoint(rowToDelete);
  selectedItemOnListChanged();
  repaintList();

  bool deleltedRowWasntLast = rowToDelete < route.size();
  int  rowToSet             = deleltedRowWasntLast ? rowToDelete : route.size() - 1;
  list->setCurrentRow(rowToDelete);
}
void RouteCreator::movePointOnList() {
  auto list       = ui->pointsList;
  int  currentRow = list->currentRow();
  if (sender() == ui->movePointUpButton) {
    if (currentRow == -1 || currentRow == 0) return;
    bool pointMoved = route.movePointUp(currentRow);
    if (pointMoved) {
      selectedItemOnListChanged();
      repaintList();
      list->setCurrentRow(currentRow - 1);
    }
  } else if (sender() == ui->movePointDownButton) {
    if (currentRow == -1 || currentRow == route.size() - 1) return;
    bool pointMoved = route.movePointDown(currentRow);
    if (pointMoved) {
      selectedItemOnListChanged();
      repaintList();
      list->setCurrentRow(currentRow + 1);
    }
  }
}
void RouteCreator::finishButtonPressed() {
  bool routeOk = checkRouteButtonPressed();
  if (!routeOk) return;

  QString name = ui->nameEditField->text();
  route.setName(name);
  if (name.size() < 3 || name.size() > 50) {
    QString msg = "Route name must be between 3 and 50 characters length";
    Utilities::showMessageBox_INFO(msg);
    return;
  }

  JsonParser::saveRouteToJson(name, route);
  Utilities::showMessageBox_INFO("Saved!");
}
bool RouteCreator::loadMap(int floor) {
  if (floor < -8 || floor > 7) {
    qWarning() << "Wrong input for loading map function";
    return false;
  }
  bool mapAlreadyLoaded = floorMaps.contains(floor);
  if (!mapAlreadyLoaded) {
    QImage* mapImg = new QImage(PathResource::getPathToMap(floor));
    if (mapImg->isNull()) {
      qWarning() << "Cant read map from qrc";
      return false;
    }
    floorMaps.insert(floor, mapImg);
  }
  return true;
}
void RouteCreator::loadRouteButtonPressed() {
  QString pathToFile = Utilities::getFileByDialog("*.json", PathResource::getRouteFolder().absolutePath());
  route              = JsonParser::readRoute(pathToFile);
  ui->nameEditField->setText(route.getName());
  repaintList();
  selectedItemOnListChanged();
}

//funcs
void RouteCreator::fillNamesOfFieldTypesToList() {
  auto strList = &listOfRoutePointsType;
  strList->push_back("Regular Field");
  strList->push_back("Stairs-up");
  strList->push_back("Stairs-down");
  strList->push_back("Rope field");
  strList->push_back("Hole, open");
  strList->push_back("Hole, close");
  strList->push_back("Ladder up");
  strList->push_back("Ladder down");
  strList->push_back("Teleport");
  strList->push_back("Log out point");
};
void RouteCreator::changePlayerPos(int x, int y) {
  const auto mapSize = floorMaps.value(currentPosition.getFloor());
  if (y > 0) {
    const int maxY          = mapSize->height() - 1;
    int       tmpCordinateY = currentPosition.getY() + y;
    bool      ptOutOfRange  = tmpCordinateY > maxY;
    int       toSet         = ptOutOfRange ? maxY : tmpCordinateY;
    currentPosition.setY(toSet);
  }
  if (y < 0) {
    int  tmpCordinateY = currentPosition.getY() + y;
    bool ptOutOfRange  = tmpCordinateY < 0;
    int  toSet         = ptOutOfRange ? 0 : tmpCordinateY;
    currentPosition.setY(toSet);
  }
  if (x > 0) {
    const int maxX          = mapSize->width() - 1;
    int       tmpCordinateX = currentPosition.getX() + x;
    bool      ptOutOfRange  = tmpCordinateX > maxX;
    int       toSet         = ptOutOfRange ? maxX : tmpCordinateX;
    currentPosition.setX(toSet);
  }
  if (x < 0) {
    int  tmpCordinateX = currentPosition.getX() + x;
    bool ptOutOfRange  = tmpCordinateX < 0;
    int  toSet         = ptOutOfRange ? 0 : tmpCordinateX;
    currentPosition.setX(toSet);
  }
};
void RouteCreator::repaintMap() {
  int floor = currentPosition.getFloor();
  if (!loadMap(floor)) return;

  const int currFrameSize = ORG_SIZE_OF_FRAME / zoom;
  QRect     frameToDisplay =
      QRect(currentPosition.getX() - (currFrameSize / 2), currentPosition.getY() - (currFrameSize / 2), currFrameSize, currFrameSize);
  QPixmap pixMapToShow = getScaledPixMapToPaint(frameToDisplay);
  ui->imgLabel->setPixmap(pixMapToShow);
  refreshPositionLabel();
}
QPixmap RouteCreator::getScaledPixMapToPaint(QRect frameWithinMap) {
  double ratio      = 1.0 * ui->imgLabel->width() / frameWithinMap.width();
  QImage imgToScale = floorMaps.value(currentPosition.getFloor())->copy(frameWithinMap);
  QPoint middlePt(imgToScale.width() / 2, imgToScale.height() / 2);
  imgToScale.setPixel(middlePt, POSITION_MARK_COLOR);
  QImage displayImg(ui->imgLabel->size(), QImage::Format::Format_ARGB32);
  for (int x = 0; x < displayImg.width(); x++) {
    int xPixFromSmallImg = x / ratio;
    for (int y = 0; y < displayImg.height(); y++) {
      int  yPixFromSmallImg = y / ratio;
      uint smallImgPix      = imgToScale.pixel(xPixFromSmallImg, yPixFromSmallImg);
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
RoutePoint::FieldType RouteCreator::getFieldOfCurrentPt() {
  QString textFromBox = ui->fieldTypesBox->currentText();
  int     index       = this->listOfRoutePointsType.indexOf(textFromBox);
  if (index == -1) return RoutePoint::FieldType(0);
  return RoutePoint::FieldType(index);
}
void RouteCreator::repaintList() {
  QListWidget* list = ui->pointsList;
  list->clear();
  QStringList textOfAllPoints = route.toStringList();
  foreach (QString var, textOfAllPoints) {
    list->addItem(var);
  }
  list->repaint();
}
void RouteCreator::selectedItemOnListChanged() {
  auto list                = ui->pointsList;
  int  size                = route.size();
  int  curr                = list->currentRow();
  bool somethingIsSelected = curr != -1;

  bool isFirstElement      = curr == 0;
  bool elementCanBeMovedUp = somethingIsSelected && !isFirstElement && (size >= 2);
  ui->movePointUpButton->setEnabled(elementCanBeMovedUp);

  bool isLastElement         = curr == (size - 1);
  bool elementCanBeMovedDown = somethingIsSelected && !isLastElement && (size >= 2);
  ui->movePointDownButton->setEnabled(elementCanBeMovedDown);

  bool thereAreElementsOnList = size > 0;
  bool desiredState           = thereAreElementsOnList && somethingIsSelected;
  ui->deletePointButton->setEnabled(desiredState);
}
bool RouteCreator::checkRouteButtonPressed() {
  QString errorText;
  bool    routeCorrect = route.checkRouteCorectness(errorText);
  if (routeCorrect) return true;

  Utilities::showMessageBox_INFO(errorText);
  return routeCorrect;
}
void RouteCreator::loadFromRoute(Route* route) {
  if (route == nullptr) {
    return;
  }
  this->route           = *route;
  this->currentPosition = route->getPoint(0).getPosition();
  repaintList();
  repaintMap();
}
}  // namespace CJ
