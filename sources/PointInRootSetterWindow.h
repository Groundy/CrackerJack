#pragma once
#include <QObject>
#include <QDialog>
#include "Route.h"
namespace Ui { class PointInRootSetterWindow; };

class PointInRootSetterWindow : public QDialog 
{

	Q_OBJECT
public:
	PointInRootSetterWindow(QDialog* parent, QString pointStr, Route::FIELDS_TYPE* fieldType);
	~PointInRootSetterWindow();
public slots:
	void cancelButtonClicked();
	void okButtonClicked();
private:
	Ui::PointInRootSetterWindow *ui;
	Route::FIELDS_TYPE* fieldTypeToRet;
	Route::FIELDS_TYPE getFieldTypeFromButtons();
	void TRANSLATE_setRadioButtonDescrpitions();
};
