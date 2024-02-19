#include <qicon.h>
#include <qtranslator.h>

#include <QtWidgets/QApplication>

#include "MainMenu.h"

void setStyle(QApplication& app) {
  app.setQuitOnLastWindowClosed(true);
  app.setStyle("fusion");
  app.setWindowIcon(QIcon(":imgs/logo"));
  QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
}
void setTranslation(QApplication& app) {
  QTranslator translator;
  translator.load(":/translation");
  app.installTranslator(&translator);
}
int main(int argc, char* argv[]) {
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication app(argc, argv);
  setTranslation(app);
  setStyle(app);

  QSharedPointer<Profile> profile;
  SelectProfileWindow     win(profile);
  if (!(win.exec() == QDialog::Accepted)) {
    exit(0);
  }
  MainMenu mainMenu(profile);
  mainMenu.exec();
  return app.exec();
}
