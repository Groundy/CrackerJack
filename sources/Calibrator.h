#pragma once
#include <qimage.h>
#include <qmap.h>
#include <qpoint.h>
#include <qrect.h>

#include <QMap>
#include <QObject>
#include <VariablesClass.hpp>

#include "ImgEditor.h"
#include "Logger.hpp"
#include "PathResource.h"
#include "Profile.h"
#include "VariablesClass.hpp"
class Calibrator {
 public:
  Calibrator(QSharedPointer<VariablesClass> var);
  ~Calibrator();
  bool        calibrateBasicAreas(const QImage& fullImage);
  bool        calibrateBattleArea(const QImage& fullImage);
  bool        calibrateStoreButton(const QImage& fullImage);
  static void test(QString pathToFilesWithScreens);

 private:
  enum BarsPostion { LEFT, RIGHT, DOWN, TOP };
  enum ManaShieldType { NO_SHIELD, COMBINED, SEPARATE };
  struct Indexes {
    int  health   = -1;
    int  mana     = -1;
    int  shield   = -1;
    int  combined = -1;
    int  rotation = -2;
    bool rotationOk() const {
      return (rotation >= -1) && (rotation <= 1);
    }
    bool healthFound(int maxSize = 100) const {
      return (health >= 0) && (health < maxSize);
    }
    bool manaFound(int maxSize = 100) const {
      return (mana >= 0) && (mana < maxSize);
    }
    bool shieldFound(int maxSize = 100) const {
      return (shield >= 0) && (shield < maxSize);
    }
    bool combinedFound(int maxSize = 100) const {
      return (combined >= 0) && (combined < maxSize);
    }
    bool isValid(int maxSize = 100) const {
      return healthFound(maxSize) && (manaFound(maxSize) || combinedFound(maxSize));
    }
  };
  struct SlashesIndexes {
    QVector<int> slashesX = {}, slashesY = {};
    int          combinedIndex = -1;
    int          sizeX() const {
      return slashesX.size();
    }
    int sizeY() const {
      return slashesY.size();
    }
    bool isValid() {
      int xSize = slashesX.size();
      int ySize = slashesY.size();

      return ((xSize == 2 || xSize == 3) && ySize == 0) || ((ySize == 2 || ySize == 3) && xSize == 0);
    }
  };

  QSharedPointer<VariablesClass> var;

  SlashesIndexes                               getIndexesOfImgsWithSlashes(const QImage& fullScreen, const QVector<QRect>& importantFrames);
  bool                                         categorizeWindows(const QImage& fullscreen, QVector<QRect>& importantRectangles);
  bool                                         findWindowsOnScreen(const QImage& fullScreen, QVector<QRect>& importantRectangles);
  std::tuple<QVector<QPoint>, QVector<QPoint>> Calibrator::sortByXY(QVector<QPoint>& const points);
  void                                         sortByXY(QVector<QRect>& inputRects, QVector<QRect>& sortedByX, QVector<QRect>& sortedByY);
  Indexes         getIndexesOfHealthManaBars(const QImage& fullscreen, const QVector<QRect>& listOfImportantRectangles);
  QVector<QPoint> getStartOfPossibleFrames(const QImage& fullScreen, int minVal, int maxVal);
  QVector<QRect>  getAreasInsideFrames(const QImage& fullScreen, const QVector<QPoint>& startOfFrames, const int MIN_DIM);
  QVector<QRect>  filterAreasCoveredByFrameFromBottomRight(const QImage& fullScreen, const QVector<QRect>& areas);
  QVector<QRect>  getOutsideFramesOfOpenEntitiesOnSideBars(const QImage& wholeScreen);
};
