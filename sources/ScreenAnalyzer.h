#pragma once
#include <QThread>
#include <memory>
#include <qdir.h>
#include <qimage.h>
#include <qmap.h>
#include <qdebug.h>
#include <qdatetime.h>
#include <qdebug.h>

#include "Calibrator.h"
#include "Profile.h"
#include "Utilities.h"
#include "RGBstruct.h"
#include "ScreenAnalyzer.h"
#include "VariablesClass.h"
class ScreenAnalyzer : public QThread
{
	Q_OBJECT

public:
	ScreenAnalyzer( QObject *parent, std::shared_ptr<VariablesClass> var, Profile* prof);
	~ScreenAnalyzer();
	void run();

private:
	const int SLEEP_TIME = 50;

	std::shared_ptr<VariablesClass> var;
	QDir screenShotFolder;
	Profile* profile;

	void deleteScreenShotFolder();
	QDir setUpScreenFolder();
	QString getNameOfLastTakenScreenShot();
	bool loadScreen(QImage& img);
	int cutHealthManaImgs(const QImage& fullscreen);
	void cutBattleList(const QImage& fullscreen);
	void analyzeBattleList(const QImage& fullscreen) {
		if (!var->getSettings().getKeepHuntingAutomaticly())
			return;
		if (var->getBattleList().getFrame().isEmpty()) {
			bool foundBattleArea = Calibrator(var).calibrateBattleArea(fullscreen);
			if (!foundBattleArea) {
				QString msg = "Nie mozna otworzy battle listy";
				var->log(msg, true, true, false);
			}
		}
		cutBattleList(fullscreen);
	}
	void analyzeMiniMap(const QImage& fullscreen) {
		if (!var->getSettings().getKeepAnalyzeMiniMap())
			return;

		QRect frame = var->getMiniMap().getFrameMiniMap();
		var->getMiniMap().setImgMiniMap(fullscreen.copy(frame));
		const QPoint DIFF_DIST_SINCE_TOPLEFT_MINIMAP = QPoint(137, 41);
		const QPoint TOP_LEFT_START_OF_MINIMAP_LAYER_FRAME = frame.topLeft() + DIFF_DIST_SINCE_TOPLEFT_MINIMAP;
		const QSize SIZE_MINIMAP_LAYER_FRAME = QSize(24, 71);
		QRect miniMapLayerFrame(TOP_LEFT_START_OF_MINIMAP_LAYER_FRAME, SIZE_MINIMAP_LAYER_FRAME);
		var->getMiniMap().setImgMiniMapLayer(fullscreen.copy(miniMapLayerFrame));
	}
};
