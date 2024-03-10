#include "Calibrator.h"
Calibrator::Calibrator(QSharedPointer<VariablesClass> var) : var(var) {}
Calibrator::~Calibrator() {}

// public
bool Calibrator::calibrateBasicAreas(const QImage& fullscreen) {
  QList<QRect> importantRectangles;
  bool         windowsFound = findWindowsOnScreen(fullscreen, importantRectangles);
  if (!windowsFound) return false;

  bool windowsCategorized = categorizeWindows(fullscreen, importantRectangles);
  if (!windowsCategorized) return false;

  return true;
}
bool Calibrator::calibrateBattleArea(const QImage& fullscreen) {
  auto rectangles = getOutsideFramesOfOpenEntitiesOnSideBars(fullscreen);
  if (rectangles.size() == 0) return false;

  QImage battleListMark(PathResource::getPathToBattleList());
  if (battleListMark.isNull()) return false;

  for each (QRect rect in rectangles) {
    auto battleMarksPos = ImgEditor::findStartPositionInImg(battleListMark, fullscreen, rect);
    if (battleMarksPos.size() != 1) continue;

    var->getBattleList().setFrame(rect);
    return true;
  }

  var->getBattleList().setFrame(QRect());
  return false;
}
bool Calibrator::calibrateStoreButton(const QImage& fullImage) {
  QImage img(PathResource::getPathToStoreButton());
  QRect  rect = fullImage.rect();
  rect.setLeft(rect.width() * 0.75);  // shearch only witing last 25% of fulll screen, on right side
  QPoint storeButtonPosition = ImgEditor::findExactStartPositionInImg(img, fullImage, rect);
  if (storeButtonPosition.isNull()) return false;
  QRect storeButtonRect(storeButtonPosition, img.size());
  var->getEquipment().setStoreRect(storeButtonRect);
  return true;
}
void Calibrator::test(QString pathToFilesWithScreens) {
  auto var       = QSharedPointer<VariablesClass>();
  int  totalTime = 0;

  QStringList namesOfScreenShots;
  QDir        screenShotFolder(pathToFilesWithScreens);
  QStringList listOfFIlesNames = screenShotFolder.entryList(QDir::Files);

  for each (QString fileName in listOfFIlesNames) {
    QString pathToFile = screenShotFolder.absoluteFilePath(fileName);
    QImage  fullScreen;
    bool    loaded = fullScreen.load(pathToFile);
    if (!loaded) {
      qDebug() << "cant load img!";
      return;
    }
    auto calibrator  = Calibrator(var);
    auto start       = QDateTime::currentMSecsSinceEpoch();
    bool categroized = calibrator.calibrateBasicAreas(fullScreen);
    auto time        = QDateTime::currentMSecsSinceEpoch() - start;
    if (categroized) {
      qDebug() << fileName << "analyzed in : " << time << "ms";
    } else {
      qDebug() << "cant sort windows in" << fileName;
      return;
    }
    totalTime += time;
  }
  qDebug() << "avg time : " + QString::number(totalTime / listOfFIlesNames.size());
}

// private
Calibrator::SlashesIndexes Calibrator::getIndexesOfImgsWithSlashes(const QImage& fullScreen, const QList<QRect>& importantFrames) {
  const QImage   slashYImg = var->getImgEditorObj().fromCharToImg(QChar(47));
  const QImage   slashXImg = var->getImgEditorObj().fromCharToImg(QChar(92));
  SlashesIndexes indexes;
  for (size_t i = 0; i < importantFrames.size(); i++) {
    QImage imgTmp = fullScreen.copy(importantFrames[i]);
    ImgEditor::imgToBlackAndWhiteAllColors(imgTmp, 250);

    QList<QPoint> pointsX = ImgEditor::findStartPositionInImg(slashXImg, imgTmp);
    if (pointsX.size() == 1)
      indexes.slashesX.push_back(i);
    else if (pointsX.size() == 2) {
      indexes.combinedIndex = i;
      indexes.slashesX.push_back(i);
    }
    if (indexes.isValid()) break;

    QList<QPoint> pointsY = ImgEditor::findStartPositionInImg(slashYImg, imgTmp);
    if (pointsY.size() == 1)
      indexes.slashesY.push_back(i);
    else if (pointsY.size() == 2) {
      indexes.combinedIndex = i;
      indexes.slashesY.push_back(i);
    }
    if (indexes.isValid()) break;
  }
  return indexes;
}
Calibrator::Indexes Calibrator::getIndexesOfHealthManaBars(const QImage& fullscreen, const QList<QRect>& listOfImportantRectangles) {
  Indexes        indexes;
  SlashesIndexes slashesIndexes = getIndexesOfImgsWithSlashes(fullscreen, listOfImportantRectangles);
  if (!slashesIndexes.isValid()) {
    qWarning() << "No enough frames with slashes on full screen";
    return Indexes();
  }
  indexes.combined = slashesIndexes.combinedIndex;

  int  SlashesXSize  = slashesIndexes.sizeX();
  int  SlashesYSize  = slashesIndexes.sizeY();
  bool foundSlsahesX = SlashesXSize >= 2 && SlashesYSize == 0;
  bool foundSlsahesY = SlashesYSize >= 2 && SlashesXSize == 0;
  if (!foundSlsahesY && !foundSlsahesX) {
    qWarning() << "Wrong slsashes frame in configuration";
    return Indexes();
  }

  BarsPostion position;
  QPoint      midOfScreen(fullscreen.width() / 2, fullscreen.height() / 2);
  if (foundSlsahesY) {
    QRect firstRectangle = listOfImportantRectangles[slashesIndexes.slashesY[0]];
    position             = firstRectangle.x() < midOfScreen.x() ? LEFT : RIGHT;
  } else {
    QRect firstRectangle = listOfImportantRectangles[slashesIndexes.slashesX[0]];
    position             = firstRectangle.y() < midOfScreen.y() ? TOP : DOWN;
  }

  ManaShieldType shieldType;
  if (indexes.combined != -1)
    shieldType = COMBINED;
  else {
    if (SlashesXSize == 3 || SlashesYSize == 3)
      shieldType = SEPARATE;
    else if (SlashesXSize == 2 || SlashesYSize == 2)
      shieldType = NO_SHIELD;
  }

  switch (position) {
    case LEFT: {
      indexes.rotation = 1;
      QList<QRect> sortedByX, sortedByY, rectangles;
      for each (int var in slashesIndexes.slashesY) rectangles.push_back(listOfImportantRectangles[var]);
      sortByXY(rectangles, sortedByX, sortedByY);
      switch (shieldType) {
        case NO_SHIELD: {
          // 25 is more less width of big bar, smaller is half of its width
          int w                = listOfImportantRectangles[slashesIndexes.slashesY[0]].width();
          indexes.shield       = -1;
          indexes.combined     = -1;
          bool isParallelStyle = sortedByX[0].x() != sortedByX[1].x();
          if (isParallelStyle) {
            indexes.health = listOfImportantRectangles.indexOf(sortedByX[0]);
            indexes.mana   = listOfImportantRectangles.indexOf(sortedByX[1]);
          } else {
            indexes.health = listOfImportantRectangles.indexOf(sortedByY[0]);
            indexes.mana   = listOfImportantRectangles.indexOf(sortedByY[1]);
          }
          break;
        }
        case COMBINED: {
          indexes.mana   = -1;
          indexes.shield = -1;
          indexes.health = slashesIndexes.slashesY[1];
          break;
        }
        case SEPARATE: {
          indexes.health   = listOfImportantRectangles.indexOf(sortedByX[1]);
          indexes.mana     = listOfImportantRectangles.indexOf(sortedByX[0]);
          indexes.shield   = listOfImportantRectangles.indexOf(sortedByX[2]);
          indexes.combined = -1;
          break;
        }
      }
      break;
    }
    case RIGHT: {
      indexes.rotation = -1;
      QList<QRect> sortedByX, sortedByY, rectangles;
      for each (int var in slashesIndexes.slashesY) rectangles.push_back(listOfImportantRectangles[var]);
      sortByXY(rectangles, sortedByX, sortedByY);
      switch (shieldType) {
        case NO_SHIELD: {
          bool isParallelStyle = sortedByY[0].y() == sortedByY[1].y();
          indexes.shield       = -1;
          indexes.combined     = -1;
          if (isParallelStyle) {
            indexes.health = listOfImportantRectangles.indexOf(sortedByX[0]);
            indexes.mana   = listOfImportantRectangles.indexOf(sortedByX[1]);
          } else {
            indexes.health = listOfImportantRectangles.indexOf(sortedByY[0]);
            indexes.mana   = listOfImportantRectangles.indexOf(sortedByY[1]);
          }
          break;
        }
        case COMBINED: {
          indexes.mana   = -1;
          indexes.shield = -1;
          indexes.health = slashesIndexes.slashesY[1];
          break;
        }
        case SEPARATE: {
          indexes.health   = listOfImportantRectangles.indexOf(sortedByX[1]);
          indexes.combined = -1;
          indexes.mana     = listOfImportantRectangles.indexOf(sortedByX[2]);
          indexes.shield   = listOfImportantRectangles.indexOf(sortedByX[0]);
          break;
        }
      }
      break;
    }
    case DOWN:
    case TOP: {
      indexes.rotation = 0;
      QList<QRect> sortedByX, sortedByY, rectangles;
      for each (int var in slashesIndexes.slashesX) rectangles.push_back(listOfImportantRectangles[var]);
      sortByXY(rectangles, sortedByX, sortedByY);
      switch (shieldType) {
        case NO_SHIELD: {
          indexes.combined     = -1;
          indexes.shield       = -1;
          bool isParallelStyle = sortedByY[0].y() == sortedByY[1].y();
          if (isParallelStyle) {
            indexes.health = listOfImportantRectangles.indexOf(sortedByX[0]);
            indexes.mana   = listOfImportantRectangles.indexOf(sortedByX[1]);
          } else {
            indexes.health = listOfImportantRectangles.indexOf(sortedByY[0]);
            indexes.mana   = listOfImportantRectangles.indexOf(sortedByY[1]);
          }
          break;
        }
        case COMBINED: {
          bool isParallelStyle = sortedByY[0].y() != sortedByY[1].y();
          indexes.mana         = -1;
          indexes.shield       = -1;
          if (isParallelStyle) {
            indexes.health   = listOfImportantRectangles.indexOf(sortedByY[0]);
            indexes.combined = listOfImportantRectangles.indexOf(sortedByY[1]);
          } else {
            indexes.health   = listOfImportantRectangles.indexOf(sortedByX[0]);
            indexes.combined = listOfImportantRectangles.indexOf(sortedByX[1]);
          }
          break;
        }
        case SEPARATE: {
          indexes.health = listOfImportantRectangles.indexOf(sortedByX[0]);
          rectangles.removeOne(sortedByX[0]);
          sortByXY(rectangles, sortedByX, sortedByY);
          indexes.mana     = listOfImportantRectangles.indexOf(sortedByY[0]);
          indexes.shield   = listOfImportantRectangles.indexOf(sortedByY[1]);
          indexes.combined = -1;
          break;
        }
      }
      break;
    }
  }
  return indexes;
}
bool Calibrator::categorizeWindows(const QImage& fullscreen, QList<QRect>& importantRectangles) {
  // 4 cause 1-minimap 2-gameScreen 3-health 4-mana, those 4 have to be found
  if (importantRectangles.size() < 4) return false;

  // game Frame
  {
    QRect biggestRect;
    int   biggestSurface = 0;
    for (size_t i = 0; i < importantRectangles.size(); i++) {
      int surf = importantRectangles[i].width() * importantRectangles[i].height();
      if (surf > biggestSurface) {
        biggestSurface = surf;
        biggestRect    = importantRectangles[i];
      }
    }
    var->getGameWindow().setFrameMainGameWindow(biggestRect);
    importantRectangles.removeOne(biggestRect);
  }

  // indexOfHealth, indexOfMana, indexOfManaShield, howTheyShouldBeRotated,
  // indexOfCombinedBox;
  {
    Indexes indexes = getIndexesOfHealthManaBars(fullscreen, importantRectangles);
    int     size    = importantRectangles.size();
    if (!indexes.isValid(size)) return false;

    var->getVitalitty().setRotation(indexes.rotation);
    QVector<QRect> rectsToDelete;
    if (indexes.healthFound(size)) {
      var->getVitalitty().setHealthArea(importantRectangles[indexes.health]);
      rectsToDelete.push_back(importantRectangles[indexes.health]);
    }

    if (indexes.manaFound(size)) {
      var->getVitalitty().setManaArea(importantRectangles[indexes.mana]);
      rectsToDelete.push_back(importantRectangles[indexes.mana]);
    }

    if (indexes.shieldFound(size)) {
      var->getVitalitty().setMSArea(importantRectangles[indexes.shield]);
      rectsToDelete.push_back(importantRectangles[indexes.shield]);
    }

    if (indexes.combinedFound(size)) {
      var->getVitalitty().setCombinedArea(importantRectangles[indexes.combined]);
      rectsToDelete.push_back(importantRectangles[indexes.combined]);
    }

    for each (QRect var in rectsToDelete) importantRectangles.removeOne(var);
  }

  // miniMap Frame
  {
    QList<QRect> sortedX, sortedY;
    sortByXY(importantRectangles, sortedX, sortedY);
    var->getMiniMap().setFrameMiniMap(sortedX.last());
    importantRectangles.removeOne(sortedX.last());
  }

  return true;
}
bool Calibrator::findWindowsOnScreen(const QImage& fullScreen, QList<QRect>& importantRectangles) {
  const int  WIDTH              = fullScreen.width();
  const int  HEIGHT             = fullScreen.height();
  const uint MIN_ACCEPTABLE_VAL = 17;
  const uint MAX_ACCEPTABLE_VAL = 29;

  QList<QPoint> startOfFrames         = getStartOfPossibleFrames(fullScreen, MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL);
  QList<QRect>  areasWithoutFrames    = getAreasInsideFrames(fullScreen, startOfFrames, 10);
  QList<QRect>  areasWithinFullFrames = filterAreasCoveredByFrameFromBottomRight(fullScreen, areasWithoutFrames);
  importantRectangles                 = areasWithinFullFrames;
  return areasWithoutFrames.size() >= 4;
}
void Calibrator::sortByXY(QList<QPoint>& points, QList<QPoint>& sortedByX, QList<QPoint>& sortedByY) {
  QMultiMap<int, QPoint> mapX;
  QMultiMap<int, QPoint> mapY;
  for each (QPoint point in points) {
    mapX.insert(point.x(), point);
    mapY.insert(point.y(), point);
  }
  sortedByX = mapX.values();
  sortedByY = mapY.values();
}
void Calibrator::sortByXY(QList<QRect>& inputRects, QList<QRect>& sortedByX, QList<QRect>& sortedByY) {
  QMultiMap<int, QRect> mapX;
  QMultiMap<int, QRect> mapY;
  for each (QRect rect in inputRects) {
    mapX.insert(rect.x(), rect);
    mapY.insert(rect.y(), rect);
  }
  sortedByX = mapX.values();
  sortedByY = mapY.values();
}
QList<QPoint> Calibrator::getStartOfPossibleFrames(const QImage& fullScreen, int minVal, int maxVal) {
  const int     WIDTH              = fullScreen.width();
  const int     HEIGHT             = fullScreen.height();
  const uint    MIN_ACCEPTABLE_VAL = 17;
  const uint    MAX_ACCEPTABLE_VAL = 29;
  QList<QPoint> topLeftCorners;
  for (size_t x = 1; x < WIDTH - 2; x++) {
    for (size_t y = 1; y < HEIGHT - 2; y++) {
      bool isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x, y), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
      if (!isPixelOfFrame) continue;

      isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x - 1, y - 1), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, false);
      if (isPixelOfFrame) continue;

      isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 1, y + 1), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, false);
      if (isPixelOfFrame) continue;

      isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 1, y + 2), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, false);
      if (isPixelOfFrame) continue;

      isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 2, y + 1), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, false);
      if (isPixelOfFrame) continue;

      isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 2, y + 2), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, false);
      if (isPixelOfFrame) continue;

      isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 1, y), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
      if (!isPixelOfFrame) continue;

      isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 2, y), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
      if (!isPixelOfFrame) continue;

      isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x, y + 1), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
      if (!isPixelOfFrame) continue;

      isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x, y + 2), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
      if (!isPixelOfFrame) continue;

      topLeftCorners.push_back(QPoint(x, y));
    }
  }
  return topLeftCorners;
}
QList<QRect> Calibrator::getAreasInsideFrames(const QImage& fullScreen, const QList<QPoint>& startOfFrames, const int MIN_DIM) {
  const int    WIDTH              = fullScreen.width();
  const int    HEIGHT             = fullScreen.height();
  const uint   MIN_ACCEPTABLE_VAL = 17;
  const uint   MAX_ACCEPTABLE_VAL = 29;
  QList<QRect> frameToRet;
  for each (QPoint startPoint in startOfFrames) {
    int currentWidth = 0;
    for (size_t x = startPoint.x(); x < WIDTH; x++) {
      uint color        = fullScreen.pixel(x, startPoint.y());
      bool isPixOfFrame = ImgEditor::isItPixelFromFrame(color, MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
      if (isPixOfFrame)
        currentWidth++;
      else
        break;
    }
    if (currentWidth < MIN_DIM) continue;

    int currentHeight = 0;
    for (size_t y = startPoint.y(); y < HEIGHT; y++) {
      uint color        = fullScreen.pixel(startPoint.x(), y);
      bool isPixOfFrame = ImgEditor::isItPixelFromFrame(color, MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
      if (isPixOfFrame)
        currentHeight++;
      else
        break;
    }
    if (currentHeight < MIN_DIM) continue;

    int x = startPoint.x() + 1;
    int y = startPoint.y() + 1;
    int w = currentWidth - 2;
    int h = currentHeight - 2;
    frameToRet.push_back(QRect(x, y, w, h));
  }
  return frameToRet;
}
QList<QRect> Calibrator::filterAreasCoveredByFrameFromBottomRight(const QImage& fullScreen, const QList<QRect>& areas) {
  const int    WIDTH              = fullScreen.width();
  const int    HEIGHT             = fullScreen.height();
  const uint   MIN_ACCEPTABLE_VAL = 100;
  const uint   MAX_ACCEPTABLE_VAL = 130;
  QList<QRect> toRet;
  for each (QRect area in areas) {
    if (area.bottom() + 1 >= fullScreen.height()) {
      qWarning() << "????";
      return toRet;
    }
    if (area.right() + 1 >= fullScreen.width()) {
      qWarning() << "????";
      return toRet;
    }

    // botom of area
    bool skip = false;
    for (size_t x = 0; x < area.width(); x++) {
      uint color   = fullScreen.pixel(area.left() + x, area.bottom() + 1);
      bool framPix = ImgEditor::isItPixelFromFrame(color, MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
      if (!framPix) {
        skip = true;
        break;
      }
    }
    if (skip) {
      continue;
    }

    // right
    for (size_t y = 0; y < area.height(); y++) {
      uint color   = fullScreen.pixel(area.right() + 1, area.top() + y);
      bool framPix = ImgEditor::isItPixelFromFrame(color, MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
      if (!framPix) {
        skip = true;
        break;
      }
    }
    if (skip) {
      continue;
    }
    toRet.push_back(area);
  }
  return toRet;
}
QList<QRect> Calibrator::getOutsideFramesOfOpenEntitiesOnSideBars(const QImage& wholeScreen) {
  const QImage startOfSideBarEntity(PathResource::getPathToSideBarEntityStart());
  if (startOfSideBarEntity.isNull()) return {};

  QList<QRect>  outerFramesOfSideBarsEntity = {};
  QList<QPoint> startPoints                 = ImgEditor::findStartPositionInImg(startOfSideBarEntity, wholeScreen);
  const QImage  endOfSideBarEntity(PathResource::getPathToSideBarEntityEnd());
  for each (QPoint currPt in startPoints) {
    QRect rectToSheachWithIn(currPt, QSize(wholeScreen.width() - currPt.x(), wholeScreen.height() - currPt.y()));
    auto  endsOfFrame = ImgEditor::findStartPositionInImg(endOfSideBarEntity, wholeScreen, rectToSheachWithIn);
    if (endsOfFrame.size() == 0) continue;
    QList<QPoint> sortedByX, sortedByY;
    sortByXY(endsOfFrame, sortedByX, sortedByY);
    QPoint pt = sortedByY[0];
    QSize  size(pt.x() + endOfSideBarEntity.width() - currPt.x(), pt.y() + endOfSideBarEntity.height() - currPt.y());
    QRect  toAdd(currPt, size);
    // wholeScreen.copy(toAdd).save(QString("C:\\Users\\szczy\\Desktop\\inne\\%1.png").arg(QString::number(QDateTime::currentMSecsSinceEpoch())));
    outerFramesOfSideBarsEntity.push_back(toAdd);
  }
  return outerFramesOfSideBarsEntity;
}

