#pragma once
#include "qstring.h"
#include "qmessagebox.h"
class Utilities {
public:
	static int showMessageBox(QString title, QString text, QFlags<QMessageBox::StandardButton> buttons);
	static bool showMessageBox_NO_YES(QString title, QString text);
};

