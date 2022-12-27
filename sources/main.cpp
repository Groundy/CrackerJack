#include <QtWidgets/QApplication>
#include "MainMenu.h"
#include <qtranslator.h>
#include <qicon.h>
void test(){

}
void setStyle(QApplication& a){
	a.setQuitOnLastWindowClosed(true);
	a.setStyle("fusion");
	a.setWindowIcon(QIcon(":imgs/logo"));
	QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
}
void setTranslation(QApplication& app) {
	QTranslator translator;
	bool ok1 = translator.load(":/translation");
	bool ok2 = app.installTranslator(&translator);
}
Profile* getProfile() {
	bool skip = false;
	if (skip) {
		Profile* prof = new Profile(JsonParser::loadProfile("mage"));
		return prof;
	}
	else {
		Profile* prof = new Profile();
		SelectProfileWindow win(prof);
		if (!(win.exec() == QDialog::Accepted))
			exit(0);
		return prof;
	}

}
int main(int argc, char* argv[])
{
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication app(argc, argv);
	setTranslation(app);
	setStyle(app);
	test();
	while (true) {
		Profile* profile = getProfile();
		auto mainMenu = new MainMenu(profile);
		bool endProgram = mainMenu->exec() != QDialog::Accepted;
		delete mainMenu;
		delete profile;
		if (endProgram)
			break;
	}
	return app.exec();
}
