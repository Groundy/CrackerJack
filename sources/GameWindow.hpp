#pragma once
#include <qimage.h>
#include <qrect.h>

#include "Utilities.hpp"
class GameWindow {
 public:
  GameWindow()  = default;
  ~GameWindow() = default;
  void getImgMainGameWindow(QImage& imgToRet) {
    imgToRet = this->gameWindow.getImgCopy();
  }
  void setImgMainGameWindow(const QImage& mainGameWindow) {
    this->gameWindow.setImg(mainGameWindow);
  }
  QRect getFrameMainGameWindow() {
    return gameWindowFrame.getRect();
  }
  void setFrameMainGameWindow(const QRect& newGameWindowFrame) {
    gameWindowFrame.setRect(newGameWindowFrame);
  }
  QVector<QPoint> getPointsOfFieldsAroundPlayer() {
    QRect     frame                 = getFrameMainGameWindow();
    const int TITLE_WIDTH           = frame.width() / TITLES_X;
    const int TITLE_HEIGHT          = frame.height() / TITLES_Y;
    QPoint    middleOfTitleZeroZero = frame.topLeft() + QPoint(TITLE_WIDTH / 2, TITLE_HEIGHT / 2);
    QPoint    middleOfPlayerTitle =
        middleOfTitleZeroZero + QPoint(7 * TITLE_WIDTH,
                                       5 * TITLE_HEIGHT + 12);  // that way cursor is little too high, 12 to lower in down.
    QVector<QPoint> toRet = {};
    for (int x = -1; x <= 1; x++) {
      for (int y = -1; y <= 1; y++) {
        toRet.push_back(middleOfPlayerTitle + QPoint(x * TITLE_WIDTH, y * TITLE_HEIGHT));
      }
    }
    toRet.swapItemsAt(1, 4);
    return toRet;
  }

 private:
  const int TITLES_X = 15, TITLES_Y = 11;
  MutexImg  gameWindow;
  MutexRect gameWindowFrame;
};
