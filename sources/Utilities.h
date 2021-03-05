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


class Utilities {
public:
	static int showMessageBox(QString title, QString text, QFlags<QMessageBox::StandardButton> buttons);
	static bool showMessageBox_NO_YES(QString title, QString text);
	static void sendKeyStrokeToProcess(Key key, int sleepTimeBetweenPressAndRelease, unsigned int PID, QString WindowName);
		
	static void getListOfProcess(QList<QString> &names, QList<unsigned int> &IDs);
	static void getHandlerToProcess(QString procName);
	static void getPIDofProcess(QString nameOfProcess,unsigned int& ID, QList<QString> names, QList<unsigned int> IDs);
	static  unsigned int getPIDofProcess(QString nameOfProcess);
	
	static void test() {
		Key key(Key::TYPE_OF_KEY::SHIFT,'A');
		while (true)
		{
		sendKeyStrokeToProcess(key, 10, getPIDofProcess("client.exe"), "Tibia - Gomezb");

		}
	};

	static LPCWSTR StrToLPCWSTR(QString str);

	//      LPCWSTR name = L"Tibia - Gomezb";
      //HWND tt = FindWindow(NULL, name);
	//
};

