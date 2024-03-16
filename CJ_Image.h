#pragma once
#include <qimage.h>
class CJ_Image {
 public:
  CJ_Image() = delete;
  CJ_Image(const QImage& img) {
    img_ = img;
  }

 private:
  QImage img_;
};
