#pragma once
#include <qimage.h>
#include <qmap.h>
#include <qpoint.h>
#include <qrect.h>

#include <QMap>
#include <QObject>
#include <VariablesClass.hpp>

#include "Logger.hpp"
#include "PathResource.h"
#include "Profile.h"
#include "VariablesClass.hpp"
namespace CJ {
class Calibrator {
 public:
  Calibrator(QSharedPointer<VariablesClass> var);
  ~Calibrator();
  bool calibrateBasicAreas(const CJ_Image& fullImage);
  bool calibrateBattleArea(const CJ_Image& fullImage);
  bool calibrateStoreButton(const CJ_Image& fullImage);

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

  SlashesIndexes getIndexesOfImgsWithSlashes(const CJ_Image& fullScreen, const QVector<QRect>& importantFrames);
  bool           categorizeWindows(const CJ_Image& fullscreen, QVector<QRect>& importantRectangles);
  bool           findWindowsOnScreen(const CJ_Image& fullScreen, QVector<QRect>& importantRectangles);
  std::tuple<QVector<QPoint>, QVector<QPoint>> sortByXY(const QVector<QPoint>& points);
  std::tuple<QVector<QRect>, QVector<QRect>>   sortByXY(const QVector<QRect>& inputRects);
  Indexes         getIndexesOfHealthManaBars(const CJ_Image& fullscreen, const QVector<QRect>& listOfImportantRectangles);
  QVector<QPoint> getStartOfPossibleFrames(const CJ_Image& fullScreen, int minVal, int maxVal);
  QVector<QRect>  getAreasInsideFrames(const CJ_Image& fullScreen, const QVector<QPoint>& startOfFrames, const int MIN_DIM);
  QVector<QRect>  filterAreasCoveredByFrameFromBottomRight(const CJ_Image& fullScreen, const QVector<QRect>& areas);
  QVector<QRect>  getOutsideFramesOfOpenEntitiesOnSideBars(const CJ_Image& wholeScreen);

  QSharedPointer<BattleList> battle_list_;
  QSharedPointer<Equipment>  equipment_;
  QSharedPointer<GameWindow> game_window_;
  QSharedPointer<Vitallity>  vitalitty_;
  QSharedPointer<MiniMap>    minimap_;
};
}  // namespace CJ
