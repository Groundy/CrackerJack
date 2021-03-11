#pragma once
#include "qstring.h"
#include "qmessagebox.h"
#include <Key.h>
#include "qprocess.h"
#include "Windows.h"
#include <psapi.h>
#include "qlist.h"
#include "tlhelp32.h"
#include <comdef.h> 
#include <iostream>


class Utilities {
public:
	static int showMessageBox(QString title, QString text, QFlags<QMessageBox::StandardButton> buttons);
	static bool showMessageBox_NO_YES(QString title, QString text);
	static void sendKeyStrokeToProcess(Key key, unsigned int PID, QString WindowName);
	static int modifyBit(int numberToEdit, int postition, int zeroOrOne)
	{
		int mask = 1 << postition;
		return (numberToEdit & ~mask) | ((zeroOrOne << postition) & mask);
	}
	static void test() {
		;
	};
	static LPCWSTR convert_StrToLPCWSTR(QString str);
};