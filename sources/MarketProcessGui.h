#pragma once
#include <QDialog>
#include <qtranslator.h>
#include "Utilities.h"
namespace Ui { class MarketProcessGui; };

class MarketProcessGui : public QDialog
{
	Q_OBJECT

public:
	MarketProcessGui(QDialog*parent = Q_NULLPTR);
	~MarketProcessGui();
signals:
	void startMarketProcess();
	void stopMarketProcess();
public slots:
	void printValueToProgressBar(int currentVal, int maxVal);
	void repaintLabel(QString str);
	void addTextToDisplayList(QString str);
	void displayNoMarketSignFoundEffect(QString str);

	void cancelButtonClicked();
	void startButtonClicker();
private:
	Ui::MarketProcessGui *ui;
};