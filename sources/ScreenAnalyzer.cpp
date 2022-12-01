#include "ScreenAnalyzer.h"

ScreenAnalyzer::ScreenAnalyzer(QObject *parent, std::shared_ptr<VariablesClass> var, Profile* profile)
	: QThread(parent), var(var), profile(profile){
	screenShotFolder = setUpScreenFolder();
	var->getSettings().setLoadingState(true);
}

ScreenAnalyzer::~ScreenAnalyzer(){
	this->terminate();
}

void ScreenAnalyzer::run() {
	while (true) {
		msleep(SLEEP_TIME);
		if (!var->getSettings().getLoadingState())
			continue;

		QImage img;
		bool openCorrectly = loadScreen(img);
		if (!openCorrectly)
			continue;
		if (var->getVitalitty().needCalibration()) {
			bool basicCalibrationOk = Calibrator(var).calibrateBasicAreas(img);
			if (!basicCalibrationOk) {
				QString msg = "Problem z kalibracja!";
				var->log(msg, true, true, false);
				deleteScreenShotFolder();
				continue;
			}
		}
		if (var->getSettings().getKeepHuntingAutomaticly()) {
			if (var->getBattleList().getFrame().isEmpty()) {
				bool foundBattleArea = Calibrator(var).calibrateBattleArea(img);
				if (!foundBattleArea) {
					QString msg = "Nie mozna otworzy battle listy";
					var->log(msg, true, true, false);
				}
			}
			cutBattleList(img);
			int enemies = getAmountOfEnemiesOnBattleList();
			emit updateEnemiesAmountInGUI(enemies);
		}

		deleteScreenShotFolder();
		cutHealthManaImgs(img);

		if (var->getSettings().getKeepAnalyzeMiniMap()) {
			QRect frame = var->getMiniMap().getFrameMiniMap();
			var->getMiniMap().setImgMiniMap(img.copy(frame));

			const QPoint DIFF_DIST_SINCE_TOPLEFT_MINIMAP = QPoint(137, 41);
			const QPoint TOP_LEFT_START_OF_MINIMAP_LAYER_FRAME = frame.topLeft() + DIFF_DIST_SINCE_TOPLEFT_MINIMAP;
			const QSize SIZE_MINIMAP_LAYER_FRAME = QSize(24, 71);
			QRect miniMapLayerFrame(TOP_LEFT_START_OF_MINIMAP_LAYER_FRAME, SIZE_MINIMAP_LAYER_FRAME);
			var->getMiniMap().setImgMiniMapLayer(img.copy(miniMapLayerFrame));
		}
		var->setNewImg(img);
	}
}

bool ScreenAnalyzer::loadScreen(QImage& img){
	QString nameOfImgToCapture = getNameOfLastTakenScreenShot();
	if (nameOfImgToCapture.isEmpty())
		return false;
	QString pathToImg = screenShotFolder.absoluteFilePath(nameOfImgToCapture);
	bool openCorrectly = img.load(pathToImg);
	return openCorrectly;
}
QString ScreenAnalyzer::getNameOfLastTakenScreenShot(){
	QStringList filtr = QStringList() << "*_*_*_Hotkey.png";
	screenShotFolder.refresh();
	QStringList listOfFIlesNames = screenShotFolder.entryList(filtr, QDir::Files);
	if (listOfFIlesNames.isEmpty())
		return QString();

	return listOfFIlesNames[0];
}
QDir ScreenAnalyzer::setUpScreenFolder(){
	try{
		QDir dir = QDir::tempPath();
		if (!dir.cdUp())
			throw std::exception("Error in setting up screenshot folder");
		if(!dir.cd("Tibia"))
			throw std::exception(QString("Error in finding screenshot folder, No Tibia folder in : %1").arg(dir.path()).toStdString().c_str());
		if (!dir.cd("packages"))
			throw std::exception(QString("Error in finding screenshot folder, No packages folder in : %1").arg(dir.path()).toStdString().c_str());
		if (!dir.cd("Tibia"))
			throw std::exception(QString("Error in finding screenshot folder, No Tibia folder in : %1").arg(dir.path()).toStdString().c_str());
		if (!dir.cd("screenshots"))
			throw std::exception(QString("Error in finding screenshot folder, No screenshots folder in : %1").arg(dir.path()).toStdString().c_str());
		return dir;
	}
	catch (const std::exception& e){
		qDebug() << e.what();
		return QDir();
	}
}
void ScreenAnalyzer::deleteScreenShotFolder(){
	QStringList filterPatter("*_*_*_Hotkey.png");//yyyy-MM-dd_hhmmsszzzz_CharNick_Methode.png
	screenShotFolder.setNameFilters(filterPatter);
	screenShotFolder.setFilter(QDir::Files);

	QStringList namesOfFoundFiles = screenShotFolder.entryList();
	foreach(QString dirFile, namesOfFoundFiles)
		screenShotFolder.remove(dirFile);
}
int ScreenAnalyzer::cutHealthManaImgs(const QImage& fullscreen){
	bool healthFrameFound = !var->getVitalitty().getHealthArea().isEmpty();
	bool manaFrameFound = !var->getVitalitty().getManaArea().isEmpty();
	bool manaShieldFound = !var->getVitalitty().getMSArea().isEmpty();
	bool combinedBoxFound = !var->getVitalitty().getCombinedArea().isEmpty();

	bool notEnoughFramesFound = !((manaFrameFound || combinedBoxFound) && healthFrameFound);
	if (notEnoughFramesFound)
		return false;

	const int ROTATION = var->getVitalitty().getRotation();
	if (healthFrameFound) {
		QImage healthValueImg = fullscreen.copy(var->getVitalitty().getHealthArea());
		if (ROTATION != 0)
			ImgEditor::rotateImgToRight(healthValueImg, ROTATION);
		var->getVitalitty().setImageHealth(healthValueImg);
	}
	if (manaFrameFound) {
		QImage manaValueImg = fullscreen.copy(var->getVitalitty().getManaArea());
		if (ROTATION != 0)
			ImgEditor::rotateImgToRight(manaValueImg, ROTATION);
		var->getVitalitty().setImageMana(manaValueImg);
	}
	if (manaShieldFound) {
		QImage manaShieldValueImg = fullscreen.copy(var->getVitalitty().getMSArea());
		if (ROTATION != 0)
			ImgEditor::rotateImgToRight(manaShieldValueImg, ROTATION);
		var->getVitalitty().setImageMS(manaShieldValueImg);
	}
	if (combinedBoxFound) {
		QImage combinedValueImg = fullscreen.copy(var->getVitalitty().getCombinedArea());
		if (ROTATION != 0)
			ImgEditor::rotateImgToRight(combinedValueImg, ROTATION);
		var->getVitalitty().setImageCombined(combinedValueImg);
	}
	return true;
}
int ScreenAnalyzer::getAmountOfEnemiesOnBattleList() {
	if (var->getSettings().getKeepAnalyzeMainGameWindow())
		return -1;
	if (var->getBattleList().getFrame().isEmpty())
		return -1;

	bool checkCorrectness = qrand() % 10 == 0;
	if (checkCorrectness) {
		QImage battleMark = QImage(PathResource::getPathToBattleList());
		QImage battleList;
		var->getBattleList().getImg(battleList);
		bool properImg = ImgEditor::findStartPositionInImg(battleMark, battleList).size() == 1;
		if (!properImg) {
			var->getBattleList().setFrame(QRect());
			return -1;
		}
	}
	QImage battleListLImg;
	var->getBattleList().getImg(battleListLImg);
	if (battleListLImg.isNull())
		return false;
	QRect inneBattleListArea = QRect(4,15,156, battleListLImg.height());
	QImage innerBattleList = battleListLImg.copy(inneBattleListArea);
	const int middleX = inneBattleListArea.width() / 2;
	int blackDots = 0;
	const uint BLACK = qRgb(0, 0, 0);
	for (int y = 0; y < innerBattleList.height(); y++){
		if (innerBattleList.pixel(middleX, y) == BLACK)
			blackDots++;
	}
	return blackDots/2;
}
void ScreenAnalyzer::cutBattleList(const QImage& fullscreen){
	if (var->getSettings().getKeepAnalyzeMainGameWindow())
		return;
	if (var->getBattleList().getFrame().isEmpty())
		return;

	var->getBattleList().setImg(fullscreen.copy(var->getBattleList().getFrame()));
}
