#pragma once
#include <QObject>
#include <QDialog>

namespace Ui { class PointInRootSetterWindow; };

class PointInRootSetterWindow : public QDialog 
{

	Q_OBJECT
public:
	PointInRootSetterWindow(QDialog* parent, QString pointStr, RouteCreator::FIELDS_TYPE &type);
	~PointInRootSetterWindow();
public slots:
	void cancelButtonClicked();
	void okButtonClicked();
private:
	Ui::PointInRootSetterWindow *ui;
	RouteCreator::FIELDS_TYPE* fieldInfoPtr;
	RouteCreator::FIELDS_TYPE getFieldTypeFromButtons();
};
