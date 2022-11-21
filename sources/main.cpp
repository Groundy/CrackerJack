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
	SelectProfileWindow win(NULL, prof);
	if (!(win.exec() == QDialog::Accepted))
		return 0;
	auto mainMenu = std::make_unique<MainMenu>(prof);
	if (!(mainMenu->exec() == QDialog::Accepted))
		return 0;
	return app.exec();
}
