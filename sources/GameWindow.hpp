#pragma once
#include <qimage.h>
#include <qrect.h>

#include "Utilities.hpp"
namespace CJ {
class GameWindow {
 public:
  GameWindow()  = default;
  ~GameWindow() = default;

  CJ_Image getImgMainGameWindow() {
    return game_window_.getImg();
  }

  void setImgMainGameWindow(const CJ_Image& mainGameWindow) {
    game_window_.setImg(mainGameWindow);
  }

  QRect getFrameMainGameWindow() {
    return game_window_frame_.getRect();
  }

  void setFrameMainGameWindow(const QRect& newGameWindowFrame) {
    game_window_frame_.setRect(newGameWindowFrame);
  }

  QVector<QPoint> getPointsOfFieldsAroundPlayer() {
    QRect     frame                 = getFrameMainGameWindow();
    const int TITLE_WIDTH           = frame.width() / titles_amount_x_;
    const int TITLE_HEIGHT          = frame.height() / titles_amount_y_;
    QPoint    middleOfTitleZeroZero = frame.topLeft() + QPoint(TITLE_WIDTH / 2, TITLE_HEIGHT / 2);
    QPoint    middleOfPlayerTitle =
        middleOfTitleZeroZero + QPoint(7 * TITLE_WIDTH,
                                       5 * TITLE_HEIGHT + 12);  // that way cursor is little too high, 12 to lower in down.
    QVector<QPoint> toRet{};
    for (int x = -1; x <= 1; x++) {
      for (int y = -1; y <= 1; y++) {
        toRet.push_back(middleOfPlayerTitle + QPoint(x * TITLE_WIDTH, y * TITLE_HEIGHT));
      }
    }
    toRet.swapItemsAt(1, 4);
    return toRet;
  }

 private:
  const uint titles_amount_x_ = 15, titles_amount_y_ = 11;
  MutexImg   game_window_;
  MutexRect  game_window_frame_;
};
}  // namespace CJ
