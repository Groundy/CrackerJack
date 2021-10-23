#include <QtWidgets/QApplication>
#include "MainMenu.h"
#include <qtranslator.h>
#include "Market.h"
#include "LoginWindow.h"
QString makeRandomProccessName();
void test();
QIcon getIcon();
void initSettings(QApplication& a);

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTranslator translator;
	bool ok1 = translator.load("Debug\\crackerjackclient_pl");
	bool ok2 = a.installTranslator(&translator);
	initSettings(a);
	test();
	if(false){
		LoginWindow w;
		int result = w.exec();
		if (result != QDialog::Accepted)
			return 0;
	}

	Profile* prof = new Profile();
	if (true) {
		SelectProfileWindow win(NULL, prof);
		bool accepted = win.exec() == QDialog::Accepted;
		if (!accepted)
			return 0;
	}
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

void initSettings(QApplication &a) {
	a.setQuitOnLastWindowClosed(true);
	a.setStyle("fusion");
	a.setWindowIcon(getIcon());
	QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
}
