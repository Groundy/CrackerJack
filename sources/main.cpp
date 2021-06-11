#include "LoginWindow.h"
#include <QtWidgets/QApplication>
#include "ScreenSaver.h"
#include "MainMenu.h"
#include <memory.h>
#include "JsonParser.h"
#include "qsettings.h"
#include "VariablesClass.h"
#include "Calibrator.h"
#include "Logger.h"
#include "Cryptography.h"
#include "ServerConnector.h"
QString makeRandomProccessName();
void test();
QIcon getIcon();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);
    a.setStyle("fusion"); 

    a.setWindowIcon(getIcon());

    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
    test();
   
    LoginWindow w;
    int result = w.exec();
    if (result != QDialog::Accepted)
        return 0;
    Profile* prof = new Profile();
    SelectProfileWindow win(NULL, prof);
    bool accepted = win.exec() == QDialog::Accepted;
    if (!accepted)
        return 0;
    MainMenu* mainMenu = new MainMenu(prof, NULL);
    bool accepted2 = mainMenu->exec() == QDialog::Accepted;
    if (!accepted2) {
        delete mainMenu;
        delete prof;
        return 0;
    }
    return a.exec();
}

void test(){
    ServerConnector con;
    //exit(0);
};

QIcon getIcon() {
    QDir dir = QDir::current();
    dir.cd("Resources");
    QString iconPath = dir.absoluteFilePath("logo.png");
    return QIcon(iconPath);
};

QString makeRandomProccessName() {
    /*This function is designed to make ranom name of Process to avoid giving the same name, which could be easily detected*/
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ");
    const int charactersAmount = possibleCharacters.length();
    const int randomWordsNumber = (qrand() % 3) + 1;
    QString ProccessName = "";
    QString wordTmp;
    for (int i = 0; i < randomWordsNumber; i++){
        int wordLength = (qrand() % 10) + 1;
        for (int t = 0; t < wordLength; ++t){
            int index = qrand() % charactersAmount;
            QChar nextChar = possibleCharacters.at(index);
            wordTmp.append(nextChar);
        }
        ProccessName.append(wordTmp);
        ProccessName.append(' ');
        wordTmp.clear();
    }
    return ProccessName;
}