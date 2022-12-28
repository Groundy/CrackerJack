#include "MinimapAnalyzer.h"

MinimapAnalyzer::MinimapAnalyzer(QObject* parent, QSharedPointer<VariablesClass> var)
	: QThread(parent), var(var) {
}
MinimapAnalyzer::~MinimapAnalyzer() {};

void MinimapAnalyzer::run() {
	QMap<int, QImage*> floorsMaps;
	while (true) {
		msleep(SLEEP_TIME_BETWEEN_LOOPS);
		if (!var->getSettings().getKeepAnalyzeMiniMap()) {
			msleep(SLEEP_TIME_BETWEEN_LOOPS);
			continue;
		}
		QImage miniMap, miniMapLayer;
		var->getMiniMap().getImgMiniMap(miniMap);
		var->getMiniMap().getImgMiniMapLayer(miniMapLayer);
		if (miniMap.isNull() || miniMapLayer.isNull())
			continue;

		int currentLayer = getCurrentLayer(miniMapLayer);
		if (!floorsMaps.contains(currentLayer)) {
			QString path = PathResource::getPathToMap(currentLayer);
			floorsMaps.insert(currentLayer, new QImage(path));
		}
		auto currentPosition = findPlayerPosition(miniMap, floorsMaps[currentLayer]);
		previousPosition = currentPosition;

		if (!currentPosition.isNull())
			sendPostitionsToGUI(QString::number(currentPosition.x()), QString::number(currentPosition.y()), QString::number(currentLayer));
		else
			sendPostitionsToGUI("?", "?", QString::number(currentLayer));

		Point3D toSet(currentPosition.x(), currentPosition.y(), currentLayer);
		if (toSet.isValid())
			var->getPosition().setPlayerPos(toSet);
		else
			var->getPosition().clear();
	}
}
QImage MinimapAnalyzer::setSliderImg() {
	QString path = PathResource::getPathToMiniMapSliderImg();
	QImage img;
	img.load(path);
	return img;
}
int MinimapAnalyzer::getCurrentLayer(const QImage& layerImg) {
	QList<QPoint> startPostions = ImgEditor::findStartPositionInImg(LAYER_SLIDER_IMG, layerImg);
	if (startPostions.size() != 1)
		return -100;

	const int HIGHEST_LAYER_Y_POS = 2;
	const int HEIGHT_DIFF_FOR_ONE_LAYER = 4;
	int yPosition = startPostions[0].y() - HIGHEST_LAYER_Y_POS;
	int layerDiffToHighestLayer = yPosition / HEIGHT_DIFF_FOR_ONE_LAYER;
	int layer = MAX_LAYER - layerDiffToHighestLayer;

	if (layer >= MIN_LAYER && layer <= MAX_LAYER)
		return layer;
	else
		return -100;
}
QVector<QRect> MinimapAnalyzer::getMiniMapPartsPostions() {
	//5 is player mark
	QVector<QRect> imgParts;
	imgParts.push_back(QRect(0, 0, 53, 54));//1
	imgParts.push_back(QRect(53, 0, 2, 52));//2
	imgParts.push_back(QRect(55, 0, 51, 54));//3
	imgParts.push_back(QRect(0, 54, 51, 2));//4
	imgParts.push_back(QRect(57, 54, 49, 2));//6
	imgParts.push_back(QRect(0, 56, 53, 53));//7
	imgParts.push_back(QRect(53, 58, 2, 51));//8
	imgParts.push_back(QRect(55, 56, 51, 53));//9

	return imgParts;
}
QList<QImage> MinimapAnalyzer::splitMiniMap(const QImage& wholeMiniMap) {
	if (wholeMiniMap.size() != PROPER_MINIMAP_IMG_SIZE)
		return QList<QImage>();

	QList<QImage> toRet = {};
	for each (QRect rect in MINI_MAP_PARTS_POSITIONS) {
		toRet.push_back(wholeMiniMap.copy(rect));
	}
	return toRet;
}
QPoint MinimapAnalyzer::findPlayerPosition(const QImage& miniMap, const QImage* wholeMap) {
	const QList<QImage> miniMapParts = splitMiniMap(miniMap);
	for (size_t i = 0; i < miniMapParts.size(); i++) {
		QPoint startPositionOfImgPiece = ImgEditor::findExactStartPositionInImg(miniMapParts[i], *wholeMap, getFrameToLookByPreviousPos());
		if (startPositionOfImgPiece.isNull())
			continue;
		QPoint vectorToPlayerPos = getVectorToPlayerFromImgPieceIndex(i);
		QPoint playerPos = vectorToPlayerPos + startPositionOfImgPiece;
		return playerPos;
	}
	return QPoint();
}
QList<QPoint> MinimapAnalyzer::findStartPosOfImgMap(const QImage& imgToFind, const QImage& imgToSearchWithin, QRect frameInBigWindow) {
	try {
		const int WIDTH_SMALL_PIC = imgToFind.width();
		const int HEIGHT_SMALL_PIC = imgToFind.height();
		const int WIDTH_BIG_PIC = imgToSearchWithin.width();
		const int HEIGHT_BIG_PIC = imgToSearchWithin.height();

		if (imgToFind.isNull() || imgToSearchWithin.isNull())
			throw std::exception("Cant find postion, one of imgs is null");
		if (WIDTH_SMALL_PIC > WIDTH_BIG_PIC)
			throw std::exception("Cant find postion, Wrong imgs size");
		if (HEIGHT_SMALL_PIC > HEIGHT_BIG_PIC)
			throw std::exception("Cant find postion, Wrong imgs size");
		if (imgToFind.format() != imgToSearchWithin.format())
			throw std::exception("Cant find postion, wrong formats");
		if (frameInBigWindow.right() > WIDTH_BIG_PIC)
			throw std::exception("Cant find postion, Wrong frame size");
		if (frameInBigWindow.bottom() > HEIGHT_BIG_PIC)
			throw std::exception("Cant find postion, Wrong frame size");


		const int maxIndexToCheckX = frameInBigWindow.isEmpty() ? WIDTH_BIG_PIC - WIDTH_SMALL_PIC : frameInBigWindow.right();
		const int maxIndexToCheckY = frameInBigWindow.isEmpty() ? HEIGHT_BIG_PIC - HEIGHT_SMALL_PIC : frameInBigWindow.bottom();
		const int minIndexToCheckX = frameInBigWindow.isEmpty() ? 0 : frameInBigWindow.left();
		const int minIndexToCheckY = frameInBigWindow.isEmpty() ? 0 : frameInBigWindow.top();

		QList<QPoint> startPointsListToRet;
		for (int x = minIndexToCheckX; x <= maxIndexToCheckX; x++) {
			for (int y = minIndexToCheckY; y <= maxIndexToCheckY; y++) {
				uint pixSmallImg = imgToFind.pixel(0, 0);
				uint pixBigImg = imgToSearchWithin.pixel(x, y);
				if (pixSmallImg != pixBigImg)
					continue;

				bool foundPosition = true;
				for (int offsetX = 1; offsetX < WIDTH_SMALL_PIC; offsetX++) {
					for (int offsetY = 1; offsetY < HEIGHT_SMALL_PIC; offsetY++) {
						pixSmallImg = imgToFind.pixel(offsetX, offsetY);
						pixBigImg = imgToSearchWithin.pixel(x + offsetX, y + offsetY);
						if (pixBigImg == pixSmallImg)
							continue;
						//break both loops
						offsetX = WIDTH_SMALL_PIC;
						offsetY = HEIGHT_SMALL_PIC;
						foundPosition = false;
					}
				}
				if (foundPosition)
					startPointsListToRet.push_back(QPoint(x, y));

			}
		}
		return startPointsListToRet;
	}
	catch (const std::exception& e) {
		Logger::staticLog(e.what());
		return QList<QPoint>();
	}
}
QRect MinimapAnalyzer::getFrameToLookByPreviousPos() {
	if (previousPosition.isNull())
		return QRect();

	const int SIZE = 220 / 2;
	QRect frameToLookWithin = QRect(previousPosition.x() - SIZE, previousPosition.y() - SIZE, 2 * SIZE, 2 * SIZE);
	return frameToLookWithin;
}
QPoint MinimapAnalyzer::getVectorToPlayerFromImgPieceIndex(int index) {
	QPoint vectorToPlayerPos;
	switch (index)
	{
	case 0: vectorToPlayerPos = QPoint(53, 54); break;//topLeft
	case 1: vectorToPlayerPos = QPoint(0, 54); break;//top
	case 2: vectorToPlayerPos = QPoint(-55, 54); break;//topRight
	case 3: vectorToPlayerPos = QPoint(+53, 0); break;//left
	case 4: vectorToPlayerPos = QPoint(-4, 0); break;//right
	case 5: vectorToPlayerPos = QPoint(+53, -2); break;//downLeft
	case 6: vectorToPlayerPos = QPoint(0, -4); break;//down
	case 7: vectorToPlayerPos = QPoint(-2, -2); break;//downRight
	}
	return vectorToPlayerPos;
}









/*
QPoint MinimapAnalyzer::findPlayerPosition(const QImage& miniMap, const QImage* wholeMap) {
	try{
		const QList<QImage> miniMapParts = splitMiniMap(miniMap);
		QList<QPoint> pointsFromThreads;
		for (size_t i = 0; i < miniMapParts.size(); i++)
			pointsFromThreads.push_back(QPoint());

		QList<ImgShearcher*> threads;
		QRect frame = getFrameToLookByPreviousPos();
		for (size_t i = 0; i < miniMapParts.size(); i++) {
			auto thread = new ImgShearcher(this, miniMapParts[i], *wholeMap, pointsFromThreads[i], frame);
			threads.push_back(thread);
			thread->start();
		}

		bool keepWaiting = false;
		do {
			keepWaiting = false;
			msleep(50);
			for (size_t i = 0; i < threads.size(); i++) {
				if (threads[i]->isRunning())
					keepWaiting = true;
				else {
					bool foundPoint = pointsFromThreads[i] != QPoint();
					if (foundPoint) {
						keepWaiting = false;
						break;
					}
				}
			}
		} while (keepWaiting);

		for (size_t i = 0; i < threads.size(); i++)
			threads[i]->terminate();
		msleep(5);
		for (size_t i = 0; i < threads.size(); i++)
			delete threads[i];

		for (size_t i = 0; i < pointsFromThreads.size(); i++) {
			auto currentList = &pointsFromThreads[i];
			if (pointsFromThreads[i].isNull())
				continue;

			QSize vectorToPlayerPos = getVectorToPlayerFromImgPieceIndex(i);
			QPoint playerPos(vectorToPlayerPos.width() + currentList[0].x(), vectorToPlayerPos.height() + currentList[0].y());
			return playerPos;
		}
		return QPoint();
	}
	catch (const std::exception& e){
		Utilities::ring();
		var->logger.log("!!!!!!!!ERROR IN THREADS!!!!!!", true, true, true);
		return QPoint();
	}

}
*/