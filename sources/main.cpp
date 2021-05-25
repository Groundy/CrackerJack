#include "LoginWindow.h"
#include <QtWidgets/QApplication>
#include "ScreenSaver.h"
#include "MainMenu.h"
#include <memory.h>
#include "JsonParser.h"
#include "qsettings.h"
#include "VariablesClass.h"
#include "Calibrator.h"

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
    /*
    LoginWindow w;
    int result = w.exec();
    if (result != QDialog::Accepted)
        return 0;
    */
    Profile* prof = new Profile();
    SelectProfileWindow win(NULL, prof);
    int result2 = win.exec();
    if (result2 != QDialog::Accepted)
        return 0;
    MainMenu* mainMenu = new MainMenu(prof, NULL);
    int result3 = mainMenu->exec();
    if (result3 != QDialog::Accepted) {
        delete mainMenu;
        delete prof;
        return 0;
    }
    return a.exec();
}

QIcon getIcon() {
    QDir dir = QDir::current();
    dir.cd("Resources");
    QString iconPath = dir.absoluteFilePath("logo.png");
    return QIcon(iconPath);
};

void test() {
    QString path = "C:\\Users\\ADMIN\\Desktop\\maps";
    Utilities::TOOL_generateMapAsText(path);

    int brea = 3;
};

QString makeRandomProccessName() {
    /*This function is designed to make ranom name of Process to avoid giving the same name the name that could be easily detected*/
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ");
    const int charactersAmount = possibleCharacters.length();
    const int randomWordsNumber = (qrand() % 3) + 1;
    QString ProccessName = "";
    QString wordTmp;
    for (int i = 0; i < randomWordsNumber; i++)
    {
        int wordLength = (qrand() % 10) + 1;
        for (int t = 0; t < wordLength; ++t)
        {
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