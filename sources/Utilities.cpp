#include "Utilities.h"

 int Utilities::showMessageBox(QString title = "CrackerJack", QString text = "", QFlags<QMessageBox::StandardButton> buttons = QMessageBox::Ok) {
	QMessageBox box;
	box.setText(text);
	box.setWindowTitle(title);
	box.setStandardButtons(buttons);
	//box.setDefaultButton(buttons);
	int ret = box.exec();
	return ret;
}

 bool Utilities::showMessageBox_NO_YES(QString title, QString text)
 {
	 QFlags<QMessageBox::StandardButton> flags = { QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No };
	 int res = Utilities::showMessageBox(title, text, flags);
	 if (res == QMessageBox::StandardButton::Yes)
		 return true;
	 else
		 return false;
 }

 void Utilities::sendKeyStrokeToProcess(Key key, unsigned int PID, QString nameOfWindow) {
    LPCWSTR nameOfWindowLPCWSTR = convert_StrToLPCWSTR(nameOfWindow);
    HWND handler = FindWindow(NULL, nameOfWindowLPCWSTR);
    if (handler == NULL)
        ;//diag err
    DWORD tmp = PID;
    DWORD hThread = GetWindowThreadProcessId(handler, &tmp);
    if (hThread != NULL) {
        PostMessage(handler, WM_KEYDOWN, key.number, 0);
    }
    else
        ;//errToDo
 }
 
 /*
 unsigned int Utilities::getPIDofProcess(QString nameOfProcess){
     QList<QString> names;
     QList<unsigned int> ids;
     unsigned int toRet;
     getListOfProcess(names, ids);
     for (int i = 0; i < names.size(); i++) {
         if (names[i].contains(nameOfProcess)) {
             if (ids.size() >= i) 
                 return ids[i];
             else
                 return 0;
         }
     }
     return 0;
 }
 */

 LPCWSTR Utilities::convert_StrToLPCWSTR(QString str){
     return (const wchar_t*)str.utf16();
 }

 