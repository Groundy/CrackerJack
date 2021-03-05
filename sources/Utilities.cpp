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
 /*
 void Utilities::sendKeyStroke(Key key, int sleepTimeBetweenPressAndRelease) {
     LPCWSTR name = L"Tibia - Gomezb";
     HWND tt = FindWindow(NULL, name);
     DWORD PID = 13308;
     DWORD hThread = GetWindowThreadProcessId(tt, &PID);
     if ( hThread != NULL) {
         PostMessage(tt, WM_KEYDOWN, 'A', 0);
         Sleep(1);
         PostMessage(tt, WM_KEYUP, 'A', 0);
     }
 }*/

 void Utilities::sendKeyStrokeToProcess(Key key, int sleepTimeBetweenPressAndRelease, unsigned int PID, QString nameOfWindow) {
     LPCWSTR nameOfWindowLPCWSTR = StrToLPCWSTR(nameOfWindow);
     HWND handler = FindWindow(NULL, nameOfWindowLPCWSTR);
     DWORD tmp = PID;
     DWORD hThread = GetWindowThreadProcessId(handler, &tmp);
     if (hThread != NULL) {
         unsigned int keyAsInt = key.letter.toLatin1();
         switch (key.keyType)
         {
         case(Key::TYPE_OF_KEY::ALT): {
             
             break;
         }
         case(Key::TYPE_OF_KEY::CONTROL): {

             break;
         }
         case(Key::TYPE_OF_KEY::FKEY): {

            break;
         }
         case(Key::TYPE_OF_KEY::NORMAL): {
            PostMessage(handler, WM_KEYDOWN, keyAsInt, 0);
            //PostMessage(handler, WM_KEYUP, keyAsInt, 0);
            break;
         }
         case(Key::TYPE_OF_KEY::SHIFT): {
             keybd_event(VK_SHIFT, 0, KEYEVENTF_EXTENDEDKEY, 0);
             PostMessage(handler, WM_KEYDOWN, keyAsInt, 0);
             break;
         }
         }

     }
 }
 void Utilities::getListOfProcess(QList<QString>& names, QList<unsigned int>& IDs) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot) {
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &pe32)) {
            do {
                unsigned int ID = (unsigned int)pe32.th32ProcessID;
                WCHAR* name = pe32.szExeFile;
                _bstr_t b(name);
                const char* nameCC = b;
                IDs.push_back(ID);
                names.push_back(QString(nameCC));
            } while (Process32Next(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
    }
 }

 void Utilities::getHandlerToProcess(QString procName){

 }

 void Utilities::getPIDofProcess(QString nameOfProcess, unsigned int& ID, QList<QString> names, QList<unsigned int> IDs){
     int index;
     for (size_t i = 0; i < names.length(); i++){
         if (names[i] == nameOfProcess) {
             index = i;
             break;
         }
     }
     ID = IDs[index];
 }

 unsigned int Utilities::getPIDofProcess(QString nameOfProcess){
     QList<QString> names;
     QList<unsigned int> ids;
     unsigned int toRet;
     getListOfProcess(names, ids);
     int index;
     for (size_t i = 0; i < names.length(); i++) {
         if (names[i] == nameOfProcess) {
             index = i;
             break;
         }
     }
     return ids[index];
 }

 LPCWSTR Utilities::StrToLPCWSTR(QString str){
     return (const wchar_t*)str.utf16();
 }

