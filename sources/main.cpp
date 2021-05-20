#include "LoginWindow.h"
#include <QtWidgets/QApplication>
#include "ScreenSaver.h"
#include "MainMenu.h"
#include <memory.h>
#include "JsonParser.h"
#include "qsettings.h"
#include "VariablesClass.h"
#include "Calibrator.h"
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
    Profile prof;
    SelectProfileWindow win(NULL, &prof);
    int result2 = win.exec();
    if (result2 != QDialog::Accepted)
        return 0;
    MainMenu* mainMenu = new MainMenu(&prof, NULL);
    int result3 = mainMenu->exec();
    if (result3 != QDialog::Accepted) {
        delete mainMenu;
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
    VariablesClass var;
    QImage img;
    QString path = "C:\\Users\\ADMIN\\Desktop\\z.png";
    img.load(path);
    QString pr = "cycki";
    Profile testProf;
   // Calibrator cali(img, var, &pr);
    //auto ret = cali.calibrateManaAndHealthBar();
    int g = 5;
};