#include "ScreenAnalyzer.h"


ScreenAnalyzer::ScreenAnalyzer(QObject *parent, VariablesClass *varClass)
	: QThread(parent){
	var = varClass;
	setPriority(QThread::Priority::HighestPriority);
}

ScreenAnalyzer::~ScreenAnalyzer()
{
}

void ScreenAnalyzer::run() {
	//tmp
	QImage fullScreenImg = QImage("C:\\Users\\ADMIN\\Desktop\\ttt.png");
	calibrate(fullScreenImg);
	sleep(1);
	//mainLoop();
}

QString ScreenAnalyzer::getNameOfLastTakenScreenShot(){
	QDir directory(pathToScreenFolder);
	QStringList litOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
	if (litOfFIles.size() == 0) 
		return QString(); //diag err

	QList<long long> listOfOnlyDatesAndTimes;
	//geting QStrin with last digit of year, 2 digits of month, 2 digits of day
	//2 digits of hour, 2 digits of minute, 2 of seconds, 3 of miliseconds
	for each (QString var in litOfFIles)
		listOfOnlyDatesAndTimes << var.remove("-").remove("_").mid(3,14).toLongLong();

	int index = 0;
	long long biggestValue = 0;
	for (int i = 0; i < listOfOnlyDatesAndTimes.size(); i++) {
		if (listOfOnlyDatesAndTimes[i] > biggestValue) {
			biggestValue = listOfOnlyDatesAndTimes[i];
			index = i;
		}
	}
	if (litOfFIles.size() - 1 < index)
		;//diag err

	return litOfFIles[index];
}

void ScreenAnalyzer::deleteScreenShotFolder(){
	QDir dir(pathToScreenFolder);
	dir.setNameFilters(QStringList() << "*.*");
	dir.setFilter(QDir::Files);
	foreach(QString dirFile, dir.entryList())
		dir.remove(dirFile);
}

void ScreenAnalyzer::mainLoop(){
	while (enableScreenAnalyzer){
		QString nameOfImgToCapture = getNameOfLastTakenScreenShot();
		QString pathToImg = pathToScreenFolder + QString("\\") + nameOfImgToCapture;
		QImage* img = new QImage;
		bool openCorrectly = img->load(pathToImg);
		if (openCorrectly) {
			var->var_img = img;
			//deleteScreenShotFolder();
		}
		else{
			delete img;
			continue;
		}


		delete img;
	}
}

void ScreenAnalyzer::calibrate(QImage fullScreenImg){
	//this fun looking for certain pixel sequences and save their locations for
	//further img splitting
	Utilities::imgToBlackAndWhiteAllColors(&fullScreenImg, 240);
	bool interfaceIsVert = setManaHealthManaShieldPosVer(&fullScreenImg);
	bool interfaceIsHor = setManaHealthManaShieldPosHor(&fullScreenImg);
	if (!(interfaceIsVert || interfaceIsHor)) {
		;//diag err
	}

	/*
	for (size_t i = 0; i < imgsOfValue.size(); i++){
		QString name = QString::number(i) + ".png";
		imgsOfValue[i].save(name);
		qDebug() << Utilities::imgWithStrToStr(&imgsOfValue[i]);
	}
	int zero = 0;
	*/
}

void sortByXAndY(QList<QPoint>* points, QList<QPoint>* pointsSortedByX, QList<QPoint>* pointsSortedByY) {
	QMap<int, int> sortedByX; //x is key
	QMap<int, int> sortedByY; //y is key
	QList<QPoint> copyOfInput = *points;
	for each (QPoint var in copyOfInput) {
		sortedByX.insert(var.x(), var.y());
		sortedByY.insert(var.y(), var.x());//inverted order of XY
	}
	QList<QPoint>sortedByXToRet;
	QList<QPoint>sortedByYToRet;
	for (auto mapEle : sortedByX.keys()){
		int x = mapEle;
		int y = sortedByX.value(mapEle);
		sortedByXToRet.push_back(QPoint(x, y));
	}
	for (auto mapEle : sortedByY.keys()) {
		int y = mapEle;
		int x = sortedByY.value(mapEle);
		sortedByYToRet.push_back(QPoint(x, y));
	}
	*pointsSortedByX = sortedByXToRet;
	*pointsSortedByY = sortedByYToRet;
}

bool ScreenAnalyzer::setManaHealthManaShieldPosVer(QImage* imgFull) {
	QImage slash = Utilities::fromCharToImg(47);
	QList<QPoint> slashesPos = Utilities::findStartPositionInImg(&slash, imgFull);
	if (slashesPos.size() == 0)
		return false;

	QSet<int> uncialXPosVer;
	QSet<int> uncialYPosVer;
	for each (QPoint var in slashesPos) {
		uncialXPosVer.insert(var.x());
		uncialYPosVer.insert(var.y());
	}
	int numberOfXPosVer = uncialXPosVer.size();
	int numberOfPosVer = uncialYPosVer.size();

	QList<QPoint>* sortedByX = new QList<QPoint>;
	QList<QPoint>* sortedByY = new QList<QPoint>;
	sortByXAndY(&slashesPos, sortedByX, sortedByY);

	const int halfWidthOfScreen = imgFull->width() / 2;	
	const bool isManaShield = slashesPos.size() == 3 ? true : false;
	const int sizeX = sortedByX->size();
	const int sizeY = sortedByY->size();
	if (slashesPos.first().x() < halfWidthOfScreen) {
		//Position LEFT
		if (sizeX == 1 && sizeY == 3) {//compact or default style
			healthSlash = sortedByY->at(2);
			manaSlash = sortedByY->at(0);
			manaShieldSlash = sortedByY->at(1);
		}
		else if (sizeX == 3 && sizeY == 2) {//large style
			healthSlash = sortedByY->at(0);
			manaSlash = sortedByX->at(0);
			manaShieldSlash = sortedByX->at(2);
		}
		else if (sizeX == 2 && sizeY == 3) {//parallel style
			healthSlash = sortedByX->at(0);
			manaSlash = sortedByY->at(2);
			manaShieldSlash = sortedByY->at(0);
		}
	}
	else {
		//Position RIGHT
		if (sizeX == 1 && sizeY == 3) {//compact or default style
			healthSlash = sortedByY->at(0);
			manaSlash = sortedByY->at(1);
			manaShieldSlash = sortedByY->at(2);
		}
		else if (sizeX == 3 && sizeY == 2) {//large style
			healthSlash = sortedByY->at(0);
			manaSlash = sortedByX->at(0);
			manaShieldSlash = sortedByX->at(2);
		}
		else if (sizeX == 2 && sizeY == 3) {//parallel style
			healthSlash = sortedByX->at(0);
			manaSlash = sortedByY->at(0);
			manaShieldSlash = sortedByY->at(2);
		}
	}



	delete sortedByX;
	delete sortedByY;
	return true;
}

bool ScreenAnalyzer::setManaHealthManaShieldPosHor(QImage *imgFull) {
	QImage slashes = Utilities::fromCharToImg(92);
	QList<QPoint> slashesPos = Utilities::findStartPositionInImg(&slashes, imgFull);
	if (slashesPos.size() == 0)
		return false;

	QSet<int> uncialXPosHor;
	QSet<int> uncialYPosHor;
	for each (QPoint var in slashesPos) {
		uncialXPosHor.insert(var.x());
		uncialYPosHor.insert(var.y());
	}
	int numberOfXPosHor = uncialXPosHor.size();
	int numberOfYPosHor = uncialYPosHor.size();

	const int halfHeighOfScreen = imgFull->height() / 2;
	QList<QPoint> *sortedByX, *sortedByY;
	sortByXAndY(&slashesPos, sortedByX, sortedByY);

	if (slashesPos.first().y() < halfHeighOfScreen) {

	//Position TOP
		if (numberOfXPosHor == 3) {
			//Large or Compact Style
			
		}
	}
	else {
		//Position DOWN
	}
}