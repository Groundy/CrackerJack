#include "LoginWindow.h"
#include <QtWidgets/QApplication>
#include "ScreenSaver.h"
#include "MainMenu.h"
#include <memory.h>
#include "JsonParser.h"
#include "qsettings.h"
/*This function is designed to make ranom name of Process to avoid giving the same name the name that could be easily detected*/
QString makeRandomProccessName() {
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

void test() {
    QStringList potions{ "Ultimate Spirit Potion",
        "Great Health Potion",
        "Ultimate Mana Potion",
        "Ultimate Health Potion",
        "Supreme Health Potion",
        "Strong Mana Potion",
        "Strong Health Potion",
        "Mana Potion",
        "Health Potion",
        "Great Mana Potion",
        "Great Spirit Potion" };



    QString path = "C:\\Users\\ADMIN\\Desktop\\z1.png";
    QString path2 = "C:\\Users\\ADMIN\\Desktop\\z2.png";
    QImage img;
    img.load(path);
    QStringList potionFound;
    QList<QRect> rects;
    Utilities::findPotionsOnBottomBar(potions, potionFound, rects, img);
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);
    test();
    /*
    LoginWindow w;
    int result = w.exec();
    if (result != QDialog::Accepted)
        return 0;
    */
    Profile prof;
    SelectProfileWindow win(NULL, &prof);
    int result2 = win.exec();
    if (result2 != QDialog::Accepted)
        return 0;
    MainMenu* mainMenu = new MainMenu(&prof, NULL);
    mainMenu->exec();
    //delete mainMenu;
    return a.exec();
}



