#include "ScreenAnalyzer.h"

ScreenAnalyzer::ScreenAnalyzer(QObject *parent, std::shared_ptr<VariablesClass> var, Profile* profile)
	: QThread(parent), var(var), profile(profile){
	screenShotFolder = setUpScreenFolder();
	var->getSettings().setLoadingState(true);
	deleteScreenShotFolder();
}

ScreenAnalyzer::~ScreenAnalyzer(){
	deleteScreenShotFolder();
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
		analyzeBattleList(img);
		deleteScreenShotFolder();
		cutHealthManaImgs(img);
		analyzeMiniMap(img);
		analyzeEquipment(img);
		var->setNewImg(img);
	}
}

bool ScreenAnalyzer::loadScreen(QImage& img){
	QString nameOfImgToCapture = getNameOfLastTakenScreenShot();
	if (nameOfImgToCapture.isEmpty())
		return false;
	QString pathToImg = screenShotFolder.absoluteFilePath(nameOfImgToCapture);
	if (!img.load(pathToImg))
		return false;
	return !img.isNull();
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
void ScreenAnalyzer::cutBattleList(const QImage& fullscreen){
	if (var->getSettings().getKeepAnalyzeMainGameWindow())
		return;
	if (var->getBattleList().getFrame().isEmpty())
		return;

	QRect frame = var->getBattleList().getFrame();
	var->getBattleList().setImg(fullscreen.copy(frame));
}
