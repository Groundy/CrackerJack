#include <QtWidgets/QApplication>
#include "MainMenu.h"
#include <qtranslator.h>
#include "Market.h"
#include <qicon.h>
void test(){
}
QIcon getIcon(){
	QDir dir = QDir::current();
	dir.cd("Resources");
	QString iconPath = dir.absoluteFilePath("logo.png");
	return QIcon(iconPath);
}
void setStyle(QApplication& a){
	a.setQuitOnLastWindowClosed(true);
	a.setStyle("fusion");
	a.setWindowIcon(getIcon());
	QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
}
void setTranslation(QApplication& app) {
	QTranslator translator;
	bool ok1 = translator.load("Debug\\crackerjackclient_pl");
	bool ok2 = app.installTranslator(&translator);
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	setTranslation(app);
	setStyle(app);
	test();

	Profile* prof = new Profile();
	if (true) {
		SelectProfileWindow win(NULL, prof);
		bool accepted = win.exec() == QDialog::Accepted;
		if (!accepted)
			return 0;
	}
	auto mainMenu = std::make_unique<MainMenu>(prof);
	bool accepted = mainMenu->exec() == QDialog::Accepted;
	if (!accepted) {
		delete prof;
		return 0;
	}
	return app.exec();
}
