#pragma once
#include <QDialog>
#include "StringResource.h"
namespace Ui { class SetNameWidnow; };

class SetNameWidnow : public QDialog
{
	Q_OBJECT

public:
	SetNameWidnow(QDialog *parent, QString title, QString textToDisplay, QString* insertedLine);
	~SetNameWidnow();
	int minLength = 3;
	int maxLength = 40;
public slots:
	void okButtonPressed();
	void textChanged();
	void cancelButtonPressed();
private:
	Ui::SetNameWidnow *ui;
	QString* ptToRetrunStr;
};
