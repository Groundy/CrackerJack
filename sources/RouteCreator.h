#pragma once
#include <qdialog.h>
#include "MinimapAnalyzer.h"
#include <qsize.h>
#include "Point3D.h"
#include "Route.h"
#include <qfiledialog.h>
#include "SetNameWidnow.h"
namespace Ui { class RouteCreator; };


class RouteCreator : public QDialog
{
	
	Q_OBJECT

public:
	RouteCreator(QDialog* parent = Q_NULLPTR);
	~RouteCreator();
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
	void cancelButtonPressed();
	void finishButtonPressed();
	void loadRouteButtonPressed();

private:
	enum class DIRECTIONS { UP, DOWN, LEFT, RIGHT };

	Ui::RouteCreator *ui;
	Point3D currentChoosenPoint = { 625,1265,0 };
	uint currentlyLoadedFloor;
	QImage currentMap;
	const QSize sizeToDisplay = { 108,108 };
	int zoom = 1;
	const uint CHOOSEN_POINT_COLOR = qRgb(60,60,60);
	Route route ;
	QDir dirWithRoutes;

	bool loadMap(int floor);
	bool repaintMap();
	void refreshPositionLabel();
	void moveMap(DIRECTIONS direction, int step);
	QPixmap getPixMapWithZoomAndCenterPix(QImage imgWithMap, QSize sizeToScale);
	Route::FIELDS_TYPE getFieldTypeComboBox();
	void TRANSLATE_addNamesOfFieldTypesToList();
	void TRANSLATE_gui();
};
