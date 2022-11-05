#include <QtWidgets/QApplication>
#include "MainMenu.h"
#include <qtranslator.h>
#include "Market.h"
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

	Profile* prof = new Profile();
	if (true) {
		SelectProfileWindow win(NULL, prof);
		bool accepted = win.exec() == QDialog::Accepted;
		if (!accepted)
			return 0;
	}
	MainMenu* mainMenu = new MainMenu(prof, NULL);
	bool accepted = mainMenu->exec() == QDialog::Accepted;
	if (!accepted) {
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

void initSettings(QApplication &a) {
	a.setQuitOnLastWindowClosed(true);
	a.setStyle("fusion");
	a.setWindowIcon(getIcon());
	QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
}
