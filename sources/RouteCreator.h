#pragma once
#include <qdialog.h>
#include <qsize.h>
//#include <qfiledialog.h>
#include <qpixmap.h>
#include <qimage.h>

#include "Point3D.h"
#include "Route.h"
#include "RGBstruct.h"
//#include "Spell.h"

namespace Ui { class RouteCreator; };


class RouteCreator : public QDialog {
	Q_OBJECT
public:
	RouteCreator(QDialog* parent = Q_NULLPTR);
	~RouteCreator();

public slots:
	void mapMoved();
	void floorChanged();
	void zoomChanged();
	void cancelButtonPressed();
private:
	Ui::RouteCreator* ui;
	QMap<int, QImage*> floorMaps = {};
	Point3D currentPosition = Point3D(625, 1265, 0);//thais temple
	const int ORG_SIZE_OF_FRAME = 112;
	const uint POSITION_MARK_COLOR = qRgb(0, 255, 255);
	int zoom = 1;

	void repaintMap();
	bool loadMap(int floor);
	QPixmap getScaledPixMapToPaint(QRect frameWithinMap);
	void changePlayerPos(int x, int y);
	void refreshPositionLabel();
};

/*
public:

public slots:
	~RouteCreator();
	void addPoint();
	void moveListItemUp();
	void moveListItemDown();
	void deletePoint();
	void selectedItemOnListChanged();

	void finishButtonPressed();
	void loadRouteButtonPressed();
	void helpButtonPressed();
	bool checkRouteButtonPressed();
	bool currentPixIsWalkable();

private:
	Route route;
	QDir dirWithRoutes;

	QPixmap getPixMapWithZoomAndCenterPix(QImage imgWithMap, QSize sizeToScale);
	Route::FIELDS_TYPE getFieldTypeComboBox();
	void TRANSLATE_addNamesOfFieldTypesToList();
};
*/