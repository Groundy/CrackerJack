#include "ScreenAnalyzer.h"

ScreenAnalyzer::ScreenAnalyzer(QObject *parent, std::shared_ptr<VariablesClass> var, Profile* profile)
	: QThread(parent), var(var), profile(profile){
	screenShotFolder = setUpScreenFolder();
	var->setSettingLoadingState(true);
}

ScreenAnalyzer::~ScreenAnalyzer(){
	this->terminate();
}

void ScreenAnalyzer::run() {
	mainLoop();
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
void ScreenAnalyzer::mainLoop(){
	while (true){
		msleep(SLEEP_TIME);
		if (!var->getSettingLoadingState())
			continue;

		QImage img;
		bool openCorrectly = loadScreen(img);
		if (!openCorrectly)
			continue;
		if (!var->framesAreValid()) {
			Calibrator(var).calibrate(img);
		}

		deleteScreenShotFolder();
		cutHealthManaImgs(img);

		if (var->getSettingKeepAnalyzeMiniMap()) {
			QRect frame = var->getFrameMiniMap();
			var->setImgMiniMap(img.copy(frame));

			const QPoint DIFF_DIST_SINCE_TOPLEFT_MINIMAP = QPoint(137,41);
			const QPoint TOP_LEFT_START_OF_MINIMAP_LAYER_FRAME = frame.topLeft() + DIFF_DIST_SINCE_TOPLEFT_MINIMAP;
			const QSize SIZE_MINIMAP_LAYER_FRAME = QSize(24, 71);
			QRect miniMapLayerFrame(TOP_LEFT_START_OF_MINIMAP_LAYER_FRAME, SIZE_MINIMAP_LAYER_FRAME);
			var->setImgMiniMapLayer(img.copy(miniMapLayerFrame));
		}

		var->setNewImg(img);			
	}
}
int ScreenAnalyzer::cutHealthManaImgs(const QImage& fullscreen){
	bool healthFrameFound = !var->getHealthArea().isEmpty();
	bool manaFrameFound = !var->getManaArea().isEmpty();
	bool manaShieldFound = !var->getMSArea().isEmpty();
	bool combinedBoxFound = !var->getCombinedArea().isEmpty();

	bool notEnoughFramesFound = !((manaFrameFound || combinedBoxFound) && healthFrameFound);
	if (notEnoughFramesFound)
		return false;

	const int ROTATION = var->getRotation();
	if (healthFrameFound) {
		QImage healthValueImg = fullscreen.copy(var->getHealthArea());
		if (ROTATION != 0)
			ImgEditor::rotateImgToRight(healthValueImg, ROTATION);
		var->setImageHealth(healthValueImg);
	}
	if (manaFrameFound) {
		QImage manaValueImg = fullscreen.copy(var->getManaArea());
		if (ROTATION != 0)
			ImgEditor::rotateImgToRight(manaValueImg, ROTATION);
		var->setImageMana(manaValueImg);
	}
	if (manaShieldFound) {
		QImage manaShieldValueImg = fullscreen.copy(var->getMSArea());
		if (ROTATION != 0)
			ImgEditor::rotateImgToRight(manaShieldValueImg, ROTATION);
		var->setImageMS(manaShieldValueImg);
	}
	if (combinedBoxFound) {
		QImage combinedValueImg = fullscreen.copy(var->getCombinedArea());
		if (ROTATION != 0)
			ImgEditor::rotateImgToRight(combinedValueImg, ROTATION);
		var->setImageCombined(combinedValueImg);
	}
	return true;
}
