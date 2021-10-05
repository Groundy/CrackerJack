#include "MarketProcessGui.h"
#include "ui_MarketProcessGui.h"

MarketProcessGui::MarketProcessGui(QDialog *parent)
	: QDialog(parent){
	ui = new Ui::MarketProcessGui();
	ui->setupUi(this);
}

MarketProcessGui::~MarketProcessGui()
{
	delete ui;
}

void MarketProcessGui::printValueToProgressBar(int currentVal, int maxVal){
	int progressInt = (100 * currentVal / maxVal);
	if(progressInt == 100)
		ui->startButton->setEnabled(true);
}

void MarketProcessGui::repaintLabel(QString str){
	ui->label->setText(str);
	ui->label->repaint();
}

void MarketProcessGui::addTextToDisplayList(QString str){
	ui->listWidget->addItem(str);
	ui->listWidget->repaint();
}

void MarketProcessGui::displayNoMarketSignFoundEffect(QString text) {
	Utilities::showMessageBox_INFO(text);
}

void MarketProcessGui::cancelButtonClicked(){
	emit stopMarketProcess();
	ui->startButton->setEnabled(true);
}

void MarketProcessGui::startButtonClicker(){
	emit startMarketProcess();
	ui->startButton->setEnabled(false);
}
