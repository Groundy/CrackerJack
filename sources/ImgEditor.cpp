#include "ImgEditor.h"

#include "CJ_Image.h"
ImgEditor::ImgEditor()  = default;
ImgEditor::~ImgEditor() = default;




void ImgEditor::saveMultipleImgs(const QImage& fullImg, QVector<QRect> frames, QString pathToFolder, QString fileNameWithoutExtension) {
  for (int i = 0; i < frames.size(); i++) {
    QImage  img      = fullImg.copy(frames[i]);
    QString fullPath = QString("%1\\%2_%3.png").arg(pathToFolder, QString::number(i), fileNameWithoutExtension);
    img.save(fullPath);
  }
}
void ImgEditor::saveMultipleImgs(const QList<QImage>& imgs, QString pathToFolder, QString fileNameWithoutExtension) {
  for (int i = 0; i < imgs.size(); i++) {
    QString fullPath = QString("%1\\%2_%3.png").arg(pathToFolder, QString::number(i), fileNameWithoutExtension);
    imgs[i].save(fullPath);
  }
}
