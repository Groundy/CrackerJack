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
int main(int argc, char* argv[]) {
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication app(argc, argv);
  setStyle(app);

  QSharedPointer<CJ::Profile> profile(new CJ::Profile());
  CJ::SelectProfileWindow     win(profile);
  if (!(win.exec() == QDialog::Accepted)) {
    exit(0);
  }
  CJ::MainMenu* mainMenu = new CJ::MainMenu(profile);
  mainMenu->exec();
  delete mainMenu;
  exit(0);
  return app.exec();
}
