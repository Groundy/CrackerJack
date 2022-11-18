#include <QtWidgets/QApplication>
#include "MainMenu.h"
#include <qtranslator.h>
#include "Market.h"
void test(){
}
QIcon getIcon(){
	QDir dir = QDir::current();
	dir.cd("Resources");
	QString iconPath = dir.absoluteFilePath("logo.png");
	return QIcon(iconPath);
}
void initSettings(QApplication& a){
	a.setQuitOnLastWindowClosed(true);
	a.setStyle("fusion");
	a.setWindowIcon(getIcon());
	QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QTranslator translator;
	bool ok1 = translator.load("Debug\\crackerjackclient_pl");
	bool ok2 = app.installTranslator(&translator);
	initSettings(app);
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
	return app.exec();
}
