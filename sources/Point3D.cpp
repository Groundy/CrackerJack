#include "Point3D.h"

Point3D::Point3D(){
	//Thais temple
	x = 625;
	y = 1265;
	floor = 0;
}
Point3D::~Point3D()
{
}
Point3D::Point3D(int X, int Y, int F) {
	x = X;
	y = Y;
	floor = F;
};
Point3D::Point3D(QString str) {
	QStringList parts = str.split(",", Qt::SplitBehaviorFlags::SkipEmptyParts);
	if (parts.size() != 3) {
		//todo log
		return;
	}

	QString xStr = parts[0].right(parts[0].size() - 2);
	QString yStr = parts[1].right(parts[1].size() - 2);
	QString fStr = parts[2].right(parts[2].size() - 2);

	bool ok1, ok2, ok3;
	x = xStr.toInt(&ok1);
	y = yStr.toInt(&ok2);
	floor = fStr.toInt(&ok3);

	bool xReadOk = x > 0 && ok1;
	bool yReadOk = y > 0 && ok2;
	bool fReadOk = floor >= -7 && floor <= 8 && ok3;
	bool readAllOk = xReadOk && yReadOk && fReadOk;
	if (!readAllOk) {
		//todo log
		return;
	}
};
QString Point3D::toString() {
	QString xStr = QString::number(x);
	QString yStr = QString::number(y);
	QString fStr = QString::number(floor);
	QString text = QString("X=%1,Y=%2,F=%3").arg(xStr, yStr, fStr);
	return text;
}
bool Point3D::operator==(Point3D pt){
	bool x_OK = x == pt.x;
	bool y_OK = y == pt.y;
	bool f_OK = floor == pt.floor;
	return x_OK && y_OK && f_OK;
}
