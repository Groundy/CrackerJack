#pragma once
#include <QObject>
#include <QMap>
#include <qrect.h>
#include <qimage.h>
#include <qmap.h>
#include <memory>

#include "ImgEditor.h"
#include "VariablesClass.h"
#include "Utilities.h"
#include "Profile.h"
class Calibrator {
public:
	Calibrator(std::shared_ptr<VariablesClass> var);
	~Calibrator();
	bool calibrateManaAndHealthBar(const QImage& fullImage);
	static void test() {
		auto var = std::shared_ptr<VariablesClass>();
		int totalTime = 0;
		try {	
			QStringList namesOfScreenShots;
			QDir screenShotFolder("C:\\Moje\\pliki\\repos\\CrackerJackClient\\TMP");
			QStringList listOfFIlesNames = screenShotFolder.entryList(QDir::Files);

			for each (QString fileName in listOfFIlesNames) {
				QString pathToFile = screenShotFolder.absoluteFilePath(fileName);
				QImage fullScreen;
				bool loaded = fullScreen.load(pathToFile);
				if (!loaded)
					throw std::exception("cant load img!");

				auto calibrator = Calibrator(var);
				auto start = QDateTime::currentMSecsSinceEpoch();
				bool categroized = calibrator.calibrateManaAndHealthBar(fullScreen);
				auto time = QDateTime::currentMSecsSinceEpoch() - start;
				if (categroized)
					qDebug() << "analyzed in : " + QString::number(time);
				else {
					QString msg = QString("cant sort windows in %1").arg(fileName);
					throw std::exception(msg.toStdString().c_str());
				}
				totalTime += time;
			}
			qDebug() << "avg time : " + QString::number(totalTime/ listOfFIlesNames.size());
		}
		catch (const std::exception& e) {
			qDebug() << e.what();
		}
	}

private:
	enum BarsPostion { LEFT, RIGHT, DOWN, TOP };
	enum ManaShieldType { NO_SHIELD, COMBINED, SEPARATE };
	struct Indexes {
		int health = -1;
		int mana = -1;
		int shield = -1;
		int combined = -1;
		int rotation = -2;
		bool rotationOk() const { return (rotation >= -1) && (rotation <= 1); }
		bool healthFound(int maxSize = 100) const { return (health >= 0) && (health < maxSize); }
		bool manaFound(int maxSize = 100) const { return (mana >= 0) && (mana < maxSize); }
		bool shieldFound(int maxSize = 100) const { return (shield >= 0) && (shield < maxSize); }
		bool combinedFound(int maxSize = 100) const { return (combined >= 0) && (combined < maxSize); }
		bool isValid(int maxSize = 100) const {
			return healthFound(maxSize) && (manaFound(maxSize) || combinedFound(maxSize));
		}
	};
	struct SlashesIndexes {
		QVector<int> slashesX = {}, slashesY = {};
		int combinedIndex = -1;
		int sizeX() const { return slashesX.size(); }
		int sizeY() const { return slashesY.size(); }
		bool isValid() {
			int xSize = slashesX.size();
			int ySize = slashesY.size();

			return
				((xSize == 2 || xSize == 3) && ySize == 0) ||
				((ySize == 2 || ySize == 3) && xSize == 0);
		}
	};

	std::shared_ptr<VariablesClass> var;

	SlashesIndexes getIndexesOfImgsWithSlashes(const QImage& fullScreen, const QList<QRect>& importantFrames);
	bool categorizeWindows(const QImage& fullscreen, QList<QRect>& importantRectangles);
	bool findWindowsOnScreen(const QImage& fullScreen, QList<QRect>& importantRectangles);
	void sortByXY(QList<QPoint>& points, QList<QPoint>& sortedByX, QList<QPoint>& sortedByY);
	void sortByXY(QList<QRect>& inputRects, QList<QRect>& sortedByX, QList<QRect>& sortedByY);
	Indexes getIndexesOfHealthManaBars(const QImage& fullscreen, const QList<QRect>& listOfImportantRectangles);
	QList<QPoint> getStartOfPossibleFrames(const QImage& fullScreen, int minVal, int maxVal);
	QList<QRect> getAreasInsideFrames(const QImage& fullScreen, const QList<QPoint>& startOfFrames, const int MIN_DIM);
	QList<QRect> filterAreasCoveredByFrameFromBottomRight(const QImage& fullScreen, const QList<QRect>& areas);
};
	/*
	void TEST_setPositionHealthImhs(QString pathToFolderWithDiffrentPositionsStylesScreen, QString pathToOutPutFolder);
	bool getRectsFromProfile(QList<QRect>& importRectsFromProf);	
	bool fillRectWithPotsInVarClass(QImage& fullscreen, QStringList nameOfPotionsToLookFor);
	bool findPotionsOnBottomBar(QStringList namesOfPotionsToFind, QStringList& namesOfPotionosFound, QList<QRect>& rectsWithFoundPots, QImage& bottomBarImg);
	void getMapWithPotionsImgCodes(QMap<QString, QString>& name_code_light, QMap<QString, QString>& name_code_dark);
	*/