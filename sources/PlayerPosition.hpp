#pragma once
#include <qpoint.h>
#include "Point3D.h"
class PlayerPosition
{
public:
	PlayerPosition() {};
	~PlayerPosition() {};
	void setPlayerPos(const QPoint xyToSet, int floorToSet) {
		positionMutex.lock();
		xy = xyToSet;
		positionMutex.unlock();
		floor = floorToSet;
	}
	void setPlayerPos(const Point3D toSet) {
		positionMutex.lock();
		xy = QPoint(toSet.getX(), toSet.getY());
		positionMutex.unlock();
		floor = toSet.getFloor();
	}
	void setPlayerPos(const int x, const int y, const int f) {
		positionMutex.lock();
		xy = QPoint(x, y);
		positionMutex.unlock();
		floor = f;
	}
	Point3D getPlayerPos() {
		positionMutex.lock();
		Point3D toRet = Point3D(xy.x(), xy.y(), floor);
		positionMutex.unlock();
		return toRet;
	}
	void clear() {
		xy = QPoint(-1, -1);
		floor = -20;
	}
	int getFloor() { return floor; }
	QPoint getXY() {
		positionMutex.lock();
		QPoint toRet = xy;
		positionMutex.unlock();
		return xy;
	}
private:
	QMutex positionMutex;
	std::atomic<int> floor = 0;
	QPoint xy;
};