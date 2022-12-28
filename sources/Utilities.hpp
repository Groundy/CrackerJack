#pragma once
#include <qstring.h>
#include <qsound.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include "RGBstruct.h"
#include "Key.h"
#include <atomic>
#include <mutex>

struct MutexImg {
public:
	QImage getImgCopy() {
		mutex.lock();
		QImage returnImg = QImage();
		if (!img.isNull())
			returnImg = img.copy();
		mutex.unlock();
		return returnImg;
	}
	void setImg(const QImage& newImg) {
		mutex.lock();
		img = newImg.copy();
		mutex.unlock();
	}
	void clear() {
		mutex.lock();
		img = QImage();
		mutex.unlock();
	}
private:
	std::mutex mutex;
	QImage img;
};
struct MutexRect {
public:
	QRect getRect() {
		mutex.lock();
		QRect toRet = rect;
		mutex.unlock();
		return toRet;
	}
	void setRect(const QRect& newRect) {
		mutex.lock();
		rect = newRect;
		mutex.unlock();
	}
	void clear() {
		mutex.lock();
		rect = QRect();
		mutex.unlock();
	}
private:
	std::mutex mutex;
	QRect rect;
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
		QFlags<QMessageBox::StandardButton> flags = { QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No };
		int res = Utilities::showMessageBox("CrakerJack", text, flags);
		bool toRet = res == QMessageBox::StandardButton::Yes;
		return toRet;
	}
	static int showMessageBox(QString title, QString text, QFlags<QMessageBox::StandardButton> buttons) {
		QMessageBox box;
		box.setText(text);
		box.setWindowTitle(title);
		box.setStandardButtons(buttons);
		//box.setDefaultButton(buttons);
		return box.exec();
	}
	static QString getFileByDialog(QString filter, QString startDirect) {
		QFileDialog fileDialog;
		fileDialog.setNameFilter(filter);
		fileDialog.setDirectory(startDirect);
		int retCode = fileDialog.exec();
		bool accepted = retCode == QDialog::Accepted;
		if (!accepted)
			return QString();
		QStringList fileList = fileDialog.selectedFiles();
		if (fileList.size() == 0)
			return QString();
		QString pathToFile = fileList.first();
		return pathToFile;			
	}
};
