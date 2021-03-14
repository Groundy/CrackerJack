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
	TMP();
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

void ScreenAnalyzer::TMP_save(QImage* img){
	QString toSave = "C:\\Users\\ADMIN\\Desktop\\tested\\";
	QString timeString = QDateTime::currentDateTime().toString("hh-mm-ss-mmm");
	toSave.append(timeString);
	toSave.append("kurwa.png");
	bool kurwa = img->save(toSave);

}

void ScreenAnalyzer::TMP(){
	const QString path = "C:\\Users\\ADMIN\\Desktop\\LETTERS";
	QDir directory(path);
	QStringList litOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
	const QString path2 = "C:\\Users\\ADMIN\\Desktop\\blackLetters";
	QList<QString> list;
	for (size_t i = 0; i < litOfFIles.size(); i++){
		QString pathToFile = path + "\\" + litOfFIles[i];
		QImage* img = new QImage(pathToFile);
		imgToBlackAndWhite(img, 200);
		QString name = path2 + "\\" + litOfFIles[i];
		img->save(name);
		QString code = TMP_letterImgToString(img);
		list.push_back(litOfFIles[i].left(1) + QString("___") + code);
	}
	for each (QString var in list){
		qDebug() << var;
	}
}

void ScreenAnalyzer::changeGreyPixelsToBlack(QImage* img, int minVal, int maxVal){
	int width = img->size().width();
	int height = img->size().height();
	QPoint point;
	for (size_t x = 0; x < width; x++)	{
		for (size_t y = 0; y < height; y++)		{
			point.setX(x);
			point.setY(y);
			uint pixVal = img->pixel(point);
			bool shouldBeBlacked = RGBstruct::isPixelInRangeOfGrey(pixVal, minVal, maxVal);
			if(shouldBeBlacked)
				img->setPixel(point, 0);
		}
	}
}

void ScreenAnalyzer::deleteScreenShotFolder(){
	QDir dir(pathToScreenFolder);
	dir.setNameFilters(QStringList() << "*.*");
	dir.setFilter(QDir::Files);
	foreach(QString dirFile, dir.entryList())
		dir.remove(dirFile);
}

QChar ScreenAnalyzer::getCharFromStrMadeFromImg(QString imgEncoded){
	return '4';
}

void ScreenAnalyzer::imgToBlackAndWhite(QImage* img, int threshold){
	int width = img->width();
	int height = img->height();
	uint pixelColor;
	bool toWhite ;
	for (size_t x = 0; x < width; x++){
		for (size_t y = 0; y < height; y++){
			toWhite = false;
			pixelColor = img->pixel(QPoint(x, y));
			RGBstruct rgb(pixelColor);
			if (rgb.b >= threshold)
				toWhite = true;
			else if (rgb.r >= threshold)
				toWhite = true;
			else if (rgb.b >= threshold)
				toWhite = true;
			if(toWhite)
				img->setPixel(x, y, 0xffffff);
			else
				img->setPixel(x, y, 0);
		}
	}
}

QString ScreenAnalyzer::TMP_letterImgToString(QImage* img){
	int width = img->width();
	int height = img->height();
	uint pixelColor;
	QString toRet = QString::number(width) + QString("_") + QString::number(height) + QString("_");
	int sum;
	for (size_t x = 0; x < width; x++) {
		for (size_t y = 0; y < height; y++) {
			pixelColor = img->pixel(QPoint(x, y));
			RGBstruct rgb(pixelColor);
			sum = rgb.b + rgb.r + rgb.g;
			if (sum == 0)
				toRet.append(QString("0"));
			else
				toRet.append(QString("1"));
		}
	}
	return toRet;
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

		changeGreyPixelsToBlack(var->var_img, 40, 120);

		TMP_save(var->var_img);
		delete img;
	}
}
