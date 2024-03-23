#pragma once
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qmutex.h>
#include <qstring.h>

#include <atomic>

#include "CJ_Image.h"
#include "Key.h"
#include "RGBstruct.h"
namespace CJ {
struct MutexImg {
 public:
  CJ_Image getImg() {
    QMutexLocker locker(&mutex_);
    return img_;
  }
  void setImg(const QImage& newImg) {
    QMutexLocker locker(&mutex_);
    img_ = newImg.copy();
  }
  void clear() {
    QMutexLocker locker(&mutex_);
    img_ = QImage();
  }

 private:
  QMutex   mutex_;
  CJ_Image img_;
};

struct MutexRect {
 public:
  QRect getRect() {
    QMutexLocker locker(&mutex_);
    return rect_;
  }
  void setRect(const QRect& newRect) {
    QMutexLocker locker(&mutex_);
    rect_ = newRect;
  }
  void clear() {
    QMutexLocker locker(&mutex_);
    rect_ = QRect();
  }

 private:
  QMutex mutex_;
  QRect  rect_;
};

class Utilities {
 public:
  static void showMessageBox_INFO(QString text) {
    QMessageBox box;
    box.setText(text);
    box.setWindowTitle("CrackerJack");
    box.setStandardButtons(QMessageBox::StandardButton::Ok);
    box.exec();
  }
  static bool showMessageBox_NO_YES(QString text) {
    QFlags<QMessageBox::StandardButton> flags = {QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No};
    int                                 res   = Utilities::showMessageBox("CrakerJack", text, flags);
    bool                                toRet = res == QMessageBox::StandardButton::Yes;
    return toRet;
  }
  static int showMessageBox(QString title, QString text, QFlags<QMessageBox::StandardButton> buttons) {
    QMessageBox box;
    box.setText(text);
    box.setWindowTitle(title);
    box.setStandardButtons(buttons);
    // box.setDefaultButton(buttons);
    return box.exec();
  }
  static QString getFileByDialog(QString filter, QString startDirect) {
    QFileDialog fileDialog;
    fileDialog.setNameFilter(filter);
    fileDialog.setDirectory(startDirect);
    int  retCode  = fileDialog.exec();
    bool accepted = retCode == QDialog::Accepted;
    if (!accepted) return QString();
    QStringList fileList = fileDialog.selectedFiles();
    if (fileList.size() == 0) return QString();
    QString pathToFile = fileList.first();
    return pathToFile;
  }
};
}  // namespace CJ
