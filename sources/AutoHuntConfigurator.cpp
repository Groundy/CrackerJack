#include "AutoHuntConfigurator.h"

AutoHuntConfigurator::AutoHuntConfigurator(QWidget *parent, Profile* profile)
	: QDialog(parent), prof(profile), ui(new Ui::AutoHuntConfiguratorClass()){
	ui->setupUi(this);
	fillPointersGUI();
	fillGuiFromProfileData(profile);
}

AutoHuntConfigurator::~AutoHuntConfigurator()
{
	delete ui;
}
