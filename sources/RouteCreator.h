#pragma once
#include <qdialog.h>
#include "MinimapAnalyzer.h"
#include "PointInRootSetterWindow.h"
#include <qsize.h>
namespace Ui { class RouteCreator; };

class Point3D {
public:
	int x, y, floor;

QString toString() {
		QString xStr= QString::number(x);
		QString yStr = QString::number(y);
		QString fStr = QString::number(floor);// convert from scale 0-15 to -7 - +8
		QString text = QString("X=%1,Y=%2,F=%3").arg(xStr, yStr, fStr);
		return text;
	};
Point3D(int toSet_X, int toSet_Y, int toSet_F) {
		x = toSet_X;
		y = toSet_Y;
		floor = toSet_F;
	};
Point3D(QString str) {
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
bool operator==(Point3D toCompare){
	return (x == toCompare.x) && (y == toCompare.y) && (floor == toCompare.floor);
};
};


class RouteCreator : public QDialog
{
	
	Q_OBJECT

public:
	RouteCreator(QDialog* parent = Q_NULLPTR);
	~RouteCreator();
	enum class FIELDS_TYPE { REGULAR, STAIRS_UP, STAIRS_DOWN, ROPE_FIELD , SHOVEL_HOLE_ALWAYS_OPEN, SHOVEL_HOLE_NEED_SHOVEL, LADDER_UP, LADDER_DOWN, TELEPORT, EXIT_POINT };
public slots:
	void imgMoved_left();
	void imgMoved_left_fast();
	void imgMoved_left_very_fast();
	void imgMoved_right();
	void imgMoved_right_fast();
	void imgMoved_right_very_fast();
	void imgMoved_top();
	void imgMoved_top_fast();
	void imgMoved_top_very_fast();
	void imgMoved_down();
	void imgMoved_down_fast();
	void imgMoved_down_very_fast();
	void floorDown();
	void floorUp();
	void addPoint();
	void moveListItemUp();
	void moveListItemDown();
	void deletePoint();
	void routeTypeChanged();
	void zoomChanged();
	void selectedItemOnListChanged();

private:
	enum class ROUTE_TYPE { CIRCLE, BACK_AND_FORTH };
	enum class DIRECTIONS { UP, DOWN, LEFT, RIGHT };

	Ui::RouteCreator *ui;
	Point3D currentChoosenPoint = Point3D(625,1265,0);
	uint currentlyLoadedFloor;
	QImage currentMap;
	const QSize sizeToDisplay = { 108,108 };
	int zoom = 1;
	ROUTE_TYPE routeType;
	const uint CHOOSEN_POINT_COLOR = qRgb(255,0,255);

	bool loadMap(int floor);
	bool repaintMap();
	void refreshPositionLabel();
	void moveMap(DIRECTIONS direction, int step);
	QPixmap getPixMapWithZoomAndCenterPix(QImage imgWithMap, QSize sizeToScale);
};
