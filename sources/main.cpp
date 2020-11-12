#include "LoginWindow.h"
#include <QtWidgets/QApplication>

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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QString proccessName = makeRandomProccessName();
    //a.setApplicationName(proccessName);
    LoginWindow w;
    w.show();
    return a.exec();
}

