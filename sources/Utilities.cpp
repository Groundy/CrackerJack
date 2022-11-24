#include "Utilities.h"

QString Utilities::StrCodeToLetter(QString code){
    //todo, it's terrible solution, it has to be changed
	QMap<QString, QString> letters = getQmapWithCodes();
	return letters[code];
 }
QMap<QString, QString> Utilities::getQmapWithCodes() {
    QMap<QString, QString> letters;
    letters.insert("6_8_011111101111111110000001100000011111111101111110", "0");
    letters.insert("4_8_01000001111111111111111100000001", "1");
    letters.insert("6_8_010000111100011110001101100110011111000101100001", "2");
    letters.insert("6_8_010000101100001110010001100100011111111101101110", "3");
    letters.insert("6_8_000011000001010000100100011111111111111100000100", "4");
    letters.insert("6_8_000000101111001111110001100100011001111110001110", "5");
    letters.insert("6_8_001111100111111111010001100100011001111100001110", "6");
    letters.insert("6_8_100000001000001110001111101111001111000011000000", "7");
    letters.insert("6_8_011011101111111110010001100100011111111101101110", "8");
    letters.insert("6_8_011100001111100110001001100010111111111001111100", "9");
    letters.insert("6_6_000110101111101001101001111111011111", "a");
    letters.insert("6_9_111111111111111111000100001000100001000111111000011110", "b");
    letters.insert("5_6_011110111111100001100001100001", "c");
    letters.insert("6_9_000011110000111111000100001000100001111111111111111111", "d");
    letters.insert("6_6_011110111111101001101001111001011010", "e");
    letters.insert("5_9_000100000011111111111111111100100000100100000", "f");
    letters.insert("6_8_011110001111110110000101100001011111111111111110", "g");
    letters.insert("6_9_111111111111111111000100000000100000000111111000011111", "h");
    letters.insert("2_8_1011111110111111", "i");
    letters.insert("4_10_0000000001001000000110111111111011111110", "j");
    letters.insert("6_9_111111111111111111000001100000011110000110011000100001", "k");
    letters.insert("2_9_111111111111111111", "l");
    letters.insert("10_6_111111111111100000100000111111011111100000100000111111011111", "m");
    letters.insert("6_6_111111111111100000100000111111011111", "n");
    letters.insert("6_6_011110111111100001100001111111011110", "o");
    letters.insert("6_8_111111111111111110000100100001001111110001111000", "p");
    letters.insert("5_6_111111111111010000110000110000", "r");
    letters.insert("5_6_011001111101101101101111100110", "s");
    letters.insert("5_8_0010000011111110111111110010000100100001", "t");
    letters.insert("6_6_111110111111000001000001111111111111", "u");
    letters.insert("6_6_111000111110000111000111111110111000", "v");
    letters.insert("8_6_111100111111000011111100111100000011111111111100", "w");
    letters.insert("6_6_110011111111001100001100111111110011", "x");
    letters.insert("6_8_110000001111001100111111001111001111000011000000", "y");
    letters.insert("5_6_100011100111101101111001110001", "z");
    letters.insert("7_8_00000111001111111111110011000100111111000011111100000111", "A");
    letters.insert("6_8_111111111111111110010001100100011111111101101110", "B");
    letters.insert("6_8_011111101111111110000001100000011000000101000010", "C");
    letters.insert("7_8_11111111111111111000000110000001110000110111111000111100", "D");
    letters.insert("6_8_111111111111111110010001100100011001000110010001", "E");
    letters.insert("6_8_111111111111111110010000100100001001000010010000", "F");
    letters.insert("7_8_01111110111111111000000110000001100010011000111101001111", "G");
    letters.insert("7_8_11111111111111110001000000010000000100001111111111111111", "H");
    letters.insert("4_8_10000001111111111111111110000001", "I");
    letters.insert("5_8_0000000110000001100000011111111111111110", "J");
    letters.insert("6_8_111111111111111100111100011001101100001110000001", "K");
    letters.insert("6_8_111111111111111100000001000000010000000100000001", "L");
    letters.insert("8_8_1111111111100000011100000011100000010000001000000111111111111111", "M");
    letters.insert("7_8_11111111011000000011000000011000000011000000011011111111", "N");
    letters.insert("7_8_01111110111111111000000110000001100000011111111101111110", "O");
    letters.insert("6_8_111111111111111110001000100010001111100001110000", "P");
    letters.insert("7_8_11111111111111111000100010001100111111100111001100000001", "R");
    letters.insert("6_8_011100101111100110011001100110011001111101001110", "S");
    letters.insert("8_8_1000000010000000100000001111111111111111100000001000000010000000", "T");
    letters.insert("7_8_11111110111111110000000100000001000000011111111111111110", "U");
    letters.insert("6_8_111000001111110000011111000111111111110011100000", "V");
    letters.insert("0_8_11100000111111000001111100001111111110001111100000001111000111111111110011100000", "W");
    letters.insert("6_8_110000111110011100111100001111001110011111000011", "X");
    letters.insert("6_8_110000001111000000111111001111111111000011000000", "Y");
    letters.insert("6_8_100001111000111110011101101110011111000111100001", "Z");
    //slash, in program it"s slash between current health and max health
    letters.insert("11_6_110000110000011000011000001100001100001100000110000110000011000011", QChar(47));
    //backslash char but in program it"s slash above but rotated
    letters.insert("6_11_000000000110000000111100001111100001111100001111000000011000000000", QChar(92));
    //opening bracket
    letters.insert("4_11_00011111000011111111101110000011110000000001", QChar(40));
    //closing bracket
    letters.insert("4_11_10000000001111000001110111111111000011111000", QChar(41));
    //char 200 it"s some weird char but here it"s symbol of healthIcon
    letters.insert("9_9_101110000011111000111111100111111110011111111111111110111111100111111000011110000",QChar(200));
    //char 201 it"s some weird char but here it"s symbol of manaIcon
    letters.insert("10_9_000100001001100111001111111111111110111111100111111100111110000111110000100100000100000000", QChar(201));
    //there are no empty line between letter r and next letter in those cases;
	letters.insert("10_9_000111111000111111000010000000110000000110000000100000011111111111111111100100000100100000","rf");
	letters.insert("9_10_001111110000111111000001000000001100000000110000000000000001001000000110111111111011111110", "rj");
	letters.insert("10_8_00111111001111110001000000110000001100000010000011111110111111110010000100100001", "rt");
	letters.insert("10_9_000100000011111111111111111100100000100100000000100000011111111111111111100100000100100000", "ff");
	letters.insert("10_9_000100000011111111111111111100100000100100000000100000011111110011111111000100001000100001", "ft");
	letters.insert("2_3_111111", "'");
    return letters;
 }
bool Utilities::showMessageBox_NO_YES(QString text)
 {
    QFlags<QMessageBox::StandardButton> flags = { QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No };
    int res = Utilities::showMessageBox("CrakerJack", text, flags);
    bool toRet = res == QMessageBox::StandardButton::Yes;
    return toRet;
 }
int Utilities::showMessageBox(QString title, QString text = "", QFlags<QMessageBox::StandardButton> buttons = QMessageBox::Ok) {
	QMessageBox box;
	box.setText(text);
	box.setWindowTitle(title);
	box.setStandardButtons(buttons);
	//box.setDefaultButton(buttons);
	return box.exec();
}
void Utilities::showMessageBox_INFO(QString text){
	QMessageBox box;
	box.setText(text);
	box.setWindowTitle("CrackerJack");
	box.setStandardButtons(QMessageBox::StandardButton::Ok);
	box.exec();
}
void Utilities::getMapWithNumbersFromBottomBar(QMap<QString, int>& lightToRet, QMap<QString, int>& darkToRet){
	QMap<QString, int> lightVersion;
	lightVersion.insert("4_6_#125#125#125_#223#223#223_#223#223#223_#223#223#223_#223#223#223_#180#180#180_#208#208#208_#83#83#83_#0#0#0_#0#0#0_#29#29#29_#223#223#223_#194#194#194_#125#125#125_#56#56#56_#56#56#56_#111#111#111_#223#223#223_#83#83#83_#223#223#223_#223#223#223_#223#223#223_#223#223#223_#111#111#111_", 0);
	lightVersion.insert("2_6_#125#125#125_#223#223#223_#167#167#167_#167#167#167_#167#167#167_#223#223#223_#125#125#125_#167#167#167_#167#167#167_#167#167#167_#167#167#167_#223#223#223_", 1);
	lightVersion.insert("2_4_#56#56#56_#0#0#0_#83#83#83_#208#208#208_#153#153#153_#139#139#139_#223#223#223_#56#56#56_", 2);
	lightVersion.insert("2_4_#153#153#153_#194#194#194_#180#180#180_#139#139#139_#223#223#223_#111#111#111_#180#180#180_#223#223#223_", 3);
	lightVersion.insert("3_4_#194#194#194_#83#83#83_#111#111#111_#167#167#167_#194#194#194_#167#167#167_#194#194#194_#208#208#208_#111#111#111_#111#111#111_#167#167#167_#194#194#194_", 4);
	lightVersion.insert("4_6_#167#167#167_#223#223#223_#223#223#223_#83#83#83_#41#41#41_#223#223#223_#167#167#167_#97#97#97_#180#180#180_#56#56#56_#0#0#0_#223#223#223_#167#167#167_#56#56#56_#167#167#167_#167#167#167_#125#125#125_#223#223#223_#167#167#167_#56#56#56_#69#69#69_#223#223#223_#223#223#223_#111#111#111_", 5);
	lightVersion.insert("4_5_#208#208#208_#223#223#223_#223#223#223_#223#223#223_#167#167#167_#139#139#139_#167#167#167_#56#56#56_#29#29#29_#223#223#223_#56#56#56_#167#167#167_#111#111#111_#69#69#69_#223#223#223_#41#41#41_#125#125#125_#223#223#223_#223#223#223_#139#139#139_", 6);
	lightVersion.insert("2_4_#167#167#167_#56#56#56_#29#29#29_#153#153#153_#167#167#167_#153#153#153_#223#223#223_#208#208#208_", 7);
	lightVersion.insert("4_6_#139#139#139_#223#223#223_#208#208#208_#194#194#194_#194#194#194_#194#194#194_#223#223#223_#29#29#29_#194#194#194_#111#111#111_#0#0#0_#223#223#223_#208#208#208_#97#97#97_#167#167#167_#194#194#194_#56#56#56_#223#223#223_#125#125#125_#223#223#223_#139#139#139_#208#208#208_#223#223#223_#167#167#167_", 8);
	lightVersion.insert("2_6_#223#223#223_#29#29#29_#0#0#0_#223#223#223_#0#0#0_#223#223#223_#194#194#194_#125#125#125_#56#56#56_#208#208#208_#153#153#153_#194#194#194_", 9);
	lightToRet = lightVersion;

	QMap<QString, int> darkVersion;
	darkVersion.insert("4_6_#37#37#37_#66#66#66_#66#66#66_#66#66#66_#66#66#66_#54#54#54_#62#62#62_#25#25#25_#0#0#0_#0#0#0_#9#9#9_#66#66#66_#58#58#58_#37#37#37_#17#17#17_#17#17#17_#33#33#33_#66#66#66_#25#25#25_#66#66#66_#66#66#66_#66#66#66_#66#66#66_#33#33#33_",0);
	darkVersion.insert("2_6_#37#37#37_#66#66#66_#50#50#50_#50#50#50_#50#50#50_#66#66#66_#37#37#37_#50#50#50_#50#50#50_#50#50#50_#50#50#50_#66#66#66_", 1);
	darkVersion.insert("2_4_#17#17#17_#0#0#0_#25#25#25_#62#62#62_#46#46#46_#41#41#41_#66#66#66_#17#17#17_", 2);
	darkVersion.insert("2_4_#46#46#46_#58#58#58_#54#54#54_#41#41#41_#66#66#66_#33#33#33_#54#54#54_#66#66#66_", 3);
	darkVersion.insert("3_4_#58#58#58_#25#25#25_#33#33#33_#50#50#50_#58#58#58_#50#50#50_#58#58#58_#62#62#62_#33#33#33_#33#33#33_#50#50#50_#58#58#58_", 4);
	darkVersion.insert("4_6_#50#50#50_#66#66#66_#66#66#66_#25#25#25_#12#12#12_#66#66#66_#50#50#50_#29#29#29_#54#54#54_#17#17#17_#0#0#0_#66#66#66_#50#50#50_#17#17#17_#50#50#50_#50#50#50_#37#37#37_#66#66#66_#50#50#50_#17#17#17_#21#21#21_#66#66#66_#66#66#66_#33#33#33_", 5);
	darkVersion.insert("4_5_#62#62#62_#66#66#66_#66#66#66_#66#66#66_#50#50#50_#41#41#41_#50#50#50_#17#17#17_#9#9#9_#66#66#66_#17#17#17_#50#50#50_#33#33#33_#21#21#21_#66#66#66_#12#12#12_#37#37#37_#66#66#66_#66#66#66_#41#41#41_", 6);
	darkVersion.insert("2_4_#50#50#50_#17#17#17_#9#9#9_#46#46#46_#50#50#50_#46#46#46_#66#66#66_#62#62#62_", 7);
	darkVersion.insert("4_6_#41#41#41_#66#66#66_#62#62#62_#58#58#58_#58#58#58_#58#58#58_#66#66#66_#9#9#9_#58#58#58_#33#33#33_#0#0#0_#66#66#66_#62#62#62_#29#29#29_#50#50#50_#58#58#58_#17#17#17_#66#66#66_#37#37#37_#66#66#66_#41#41#41_#62#62#62_#66#66#66_#50#50#50_", 8);
	darkVersion.insert("2_6_#66#66#66_#9#9#9_#0#0#0_#66#66#66_#0#0#0_#66#66#66_#58#58#58_#37#37#37_#17#17#17_#62#62#62_#46#46#46_#58#58#58_", 9);
	darkToRet = darkVersion;
}




/*
qint64 Utilities::getCurrentTimeInMiliSeconds() {
	QDateTime date = QDateTime::currentDateTime();
	qint64 mseconds = date.currentMSecsSinceEpoch();
	return mseconds;
}
*/

/*
QDir Utilities::getDirWithCrackerJackTmpFolder(FOLDERS_OF_TMP_FOLDER folderType){
	typedef FOLDERS_OF_TMP_FOLDER Sub;
	QDir dir = QDir::tempPath();
	dir.cdUp();
	bool crackerJackFolderExist = dir.cd("CrackerJack");
	if (!crackerJackFolderExist) {
		bool dirCreationFailed = dir.mkdir("CrackerJack");
		if (dirCreationFailed) {
			QString text = QString("Failed to create sub-dir %1 in %2").arg("CrackerJack", dir.absolutePath());
			//Logger::logPotenialBug(text, "Utilities", "getDirWithCrackerJackTmpFolder");
			return QDir::current();
		}
	}
	if (folderType == Sub::Main)
		return dir;

	QString nameOfSubFolder;
	if (folderType == Sub::Logs)
		nameOfSubFolder = "Logs";
	else if (folderType == Sub::MarketLists)
		nameOfSubFolder = "MarketLists";
	else if (folderType == Sub::Profiles)
		nameOfSubFolder = "Profiles";
	else if (folderType == Sub::Routes)
		nameOfSubFolder = "Routes";
	else if (folderType == Sub::TradeReports)
		nameOfSubFolder = "Trading logs";

	bool subFolderExist = dir.cd(nameOfSubFolder);
	if (!subFolderExist) {
		bool dirCreationFailed = dir.mkdir(nameOfSubFolder);
		if (dirCreationFailed) {
			QString text = QString("Failed to create sub-dir %1 in %2").arg(nameOfSubFolder, dir.absolutePath());
			//Logger::logPotenialBug(text, "Utilities", "getDirWithCrackerJackTmpFolder");
			return QDir::current();
		}
	}
	return dir;
}
*/
/*
QMap<Utilities::FieldsOfIniFile, QString> Utilities::get_Field_NamesFromIni_map(){
	typedef Utilities::FieldsOfIniFile Field;
	QMap<Field, QString> toRet;

	toRet.insert(Field::LANGUAGE, "language");
	toRet.insert(Field::LAST_USED_LOGIN, "lastLogin");
	toRet.insert(Field::LAST_USED_PROFILE_NAME, "lastProfile");

	return toRet;
}
*/
/*
QString Utilities::readFromIniFile(FieldsOfIniFile nameOfField){
	QString filePath = getPathToSettingsFile();
	QSettings setttings(filePath, QSettings::IniFormat);
	auto map = get_Field_NamesFromIni_map();
	QString nameOfFieldAsStr = map[nameOfField];
	QString readVal = setttings.value(nameOfFieldAsStr).toString();
	return readVal;
}
*/
/*
void Utilities::TOOL_convertMapsFromOrgNameToSqrName(QString inputFolder) {
	for (size_t i = 0; i < 16; i++) {
		qDebug() << QString::number(i);
		QString path = inputFolder + "\\" + QString::number(i);
		QDir directory(path);
		QStringList litOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
		for each (QString var in litOfFIles) {
			QStringList partOfNames = var.split("_");
			partOfNames[2] = QString::number(partOfNames[2].toInt() / 256);
			partOfNames[3] = QString::number(partOfNames[3].toInt() / 256);
			QString newName = partOfNames.join("_");
			QString pathToFileOld = path + "\\" + var;
			QString pathToFileNew = path + "\\" + newName;
			QFile::rename(pathToFileOld, pathToFileNew);
		}
	}
}
*/
/*
QStringList Utilities::TOOL_getCodesOfAllInFolder_regular(QString pathToInputFolder, QString pathToOutputFolder) {
    QDir directory(pathToInputFolder);
    QStringList litOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
    QList<QString> list;
    for (size_t i = 0; i < litOfFIles.size(); i++) {
	    QString pathToFile = pathToInputFolder + "\\" + litOfFIles[i];
	    QImage img = QImage(pathToFile);
		ImgEditor::imgToBlackAndWhiteOneColor(img, 200);
		QString name = pathToOutputFolder + "\\" + litOfFIles[i];
		TOOL_saveImgToOutPutFolder(img,name);
	    QString code = ImgEditor::letterImgToLetterCodeStr(&img);

	    //changing "slash.png" to "slash"
	    QString tmp = litOfFIles[i].left(litOfFIles[i].size() - 4);
	    litOfFIles[i] = tmp;

		QString codeToDisplay = litOfFIles[i] + QString("___") + code;
	    list.push_back(codeToDisplay);
		qDebug() << codeToDisplay;
    }
    return list;
 }
*/
/*
QString Utilities::getPathToSettingsFile() {
	QDir dir = QDir::temp();
	dir.cdUp();
	QString pathToCrackerJackTmpFolder = dir.path() + "/CrackerJack";
	bool crackerJackDirExist = QDir(pathToCrackerJackTmpFolder).exists();
	if (!crackerJackDirExist) {
		bool sucess = dir.rmpath(pathToCrackerJackTmpFolder);
		if (!sucess) {
			//Logger::logPotenialBug("Can't make path to crackerJack main tmp folder","Utilities","getPathToSettingsFile");
			;//TODO fatal error
		}
	}
	pathToCrackerJackTmpFolder.append("/settings.ini");
	return pathToCrackerJackTmpFolder;
}
*/
/*
QStringList Utilities::TOOL_getCodesOfAllInFolder_bottom(QString pathToInputFolder) {
	QDir directory(pathToInputFolder);
	QStringList litOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
	QList<QString> list;
	for (size_t i = 0; i < litOfFIles.size(); i++) {
		QString pathToFile = pathToInputFolder + "\\" + litOfFIles[i];
		QImage* img = new QImage(pathToFile);
		int width = img->width();
		int height = img->height();
		// patern width_height_digits
		// digits = #r#g#b_#r#g#b_#r#g#b_#r#g#b_#r#g#b_#r#g#b 
		const QString HASH = QString("#");
		const QString FLOOR = QString("_");
		QString strWithCode = QString::number(width) + HASH + QString::number(height) + HASH;
		
		for (size_t x = 0; x < width; x++){
			for (size_t y = 0; y < height; y++){
				uint pixVal = (uint)img->pixel(x, y);
				QString strToAppend = RGBstruct(pixVal).toString();
				strWithCode.append(strToAppend);
			}
		}
		list.push_back(strWithCode);
		QString toDisplay = litOfFIles[i] + "______" + strWithCode;
		qDebug() << toDisplay;
	}
	return list;
}

*/
/*
void Utilities::TOOL_saveImgToOutPutFolder(QImage& img, QString extraName){
	QString prefixOfName;
	if (extraName.isEmpty())
		prefixOfName = QDateTime::currentDateTime().toString("mm_ss_zzz");
	else
		prefixOfName = extraName;
	QString fullname = "C:\\Users\\ADMIN\\Desktop\\output\\_" + prefixOfName + ".png";
	img.save(fullname);
}
*/
/*
void Utilities::writeIniFile(FieldsOfIniFile nameOfField, QString value){
	QString filePath = getPathToSettingsFile();
	QSettings setttings(filePath, QSettings::IniFormat);
	auto map = get_Field_NamesFromIni_map();
	QString nameOfFieldAsStr = map[nameOfField];
	setttings.setValue(nameOfFieldAsStr, value);
}
*/
/*
void Utilities::TOOL_generateMapAsText(QString inputFolder){
	//it takes so fucking long approximately 210s
	const long start_xPos = 124, start_yPos = 120;
	const long end_xPos_included = 133, end_yPos_included = 128;
	const long width = 256 * (end_xPos_included - start_xPos + 1);
	const long height = 256 * (end_yPos_included - start_yPos + 1);
	const QString floor("_");
	const QString minimapStr = "Minimap";//_Color_126_124_2.png"
	const QString color = "Color";
	const QString end = ".png";

	for (long level = 0; level < 16; level++) {
		QString name = QString::number(level) + ".txt";
		QFile fOut(name);
		fOut.open(QFile::WriteOnly | QFile::Text);
		QTextStream s(&fOut);
		for (long fileNamePosX = start_xPos; fileNamePosX <= end_xPos_included; fileNamePosX++) {
			for (long fileNamePosY = start_yPos; fileNamePosY <= end_yPos_included; fileNamePosY++) {
				qDebug() << "[" + QString::number(fileNamePosX - start_xPos) + "," + QString::number(fileNamePosY - start_yPos) + "]_"+ QString::number(level);

				QString fileXPos = QString::number(fileNamePosX);
				QString fileYPos = QString::number(fileNamePosY);
				QString fileName = minimapStr + floor + color + floor + fileXPos + floor + fileYPos + floor + QString::number(level) + end;
				QString nameOfFileToReadFrom = inputFolder + "\\" + QString::number(level) + "\\" + fileName;
				QImage img;
				bool sucess = img.load(nameOfFileToReadFrom);
				if (sucess) {
					for (long x = 0; x < 256; x++){
						for (long y = 0; y < 256; y++){
							RGBstruct rgb(img.pixel(x, y));
							bool isBlack = rgb.r == 0 && rgb.g == 0 && rgb.b == 0;
							if (isBlack) continue;
							bool isGrass = rgb.r == 0 && rgb.g == 204 && rgb.b == 0;
							if (isGrass) continue;
							bool isWater = rgb.r == 51 && rgb.g == 102 && rgb.b == 153;
							if (isWater) continue;

							QString xPosStr = QString::number(256 * (fileNamePosX - start_xPos) + x);
							QString yPosStr = QString::number(256 * (fileNamePosY - start_yPos) + y);
							QString str = QString::number(rgb.r) + "_" + QString::number(rgb.g) + "_" + QString::number(rgb.b);
							QString toAppend = "[" + xPosStr + "," + yPosStr +"] "+ str;
							s << toAppend << '\n';
						}
					}
				}
			}
		}
		fOut.close();
	}
	
	int endLoop = 4;
}
*/
/*
void Utilities::UNSUED_findBoredersOfFrames(QImage fullScreen){
	QImage screeOfFrames(fullScreen);
	auto  minBlack = qRgb(19, 19, 19);
	auto  maxBlack = qRgb(29, 29, 29);
	auto minBlack2 = qRgb(108, 108, 108);
	auto maxBlack2 = qRgb(125, 125, 125);
	auto black = qRgb(0, 0, 0);
	auto white = qRgb(255, 255, 255);
	Utilities::UNUSED_imgToOneColor(screeOfFrames, minBlack, maxBlack, white, true);

	int width = screeOfFrames.width();
	int height = screeOfFrames.height();
	uint pix;
	bool blueIsEnough, redIsEnough, greenIsEnough;
	bool blueIsEnough1, redIsEnough1, greenIsEnough1;
	bool finalBool;
	RGBstruct minV(minBlack);
	RGBstruct maxV(maxBlack);
	RGBstruct minV2(minBlack2);
	RGBstruct maxV2(maxBlack2);
	uint toSet;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			pix = fullScreen.pixel(x, y);
			RGBstruct rgb(pix);
			blueIsEnough = (rgb.b >= minV.b) && (rgb.b <= maxV.b);
			blueIsEnough1 = (rgb.b >= minV2.b) && (rgb.b <= maxV2.b);
			redIsEnough = (rgb.r >= minV.r) && (rgb.r <= maxV.r);
			redIsEnough1 = (rgb.r >= minV2.r) && (rgb.r <= maxV2.r);
			greenIsEnough = (rgb.g >= minV.g) && (rgb.g <= maxV.g);
			greenIsEnough1 = (rgb.g >= minV2.g) && (rgb.g <= maxV2.g);
			finalBool = (blueIsEnough || blueIsEnough1) && (redIsEnough || redIsEnough1) && (greenIsEnough || greenIsEnough1);
			toSet = finalBool ? white : black;
			screeOfFrames.setPixel(x, y, toSet);
		}
	}
	Utilities::saveImgToOutPutFolder(&screeOfFrames, NULL);

	QImage startOfFrame = QImage(3, 3, QImage::Format::Format_RGB32);
}
*/
/*
void Utilities::UNUSED_changeGreyPixelsToBlack(QImage& img, int minGreyVal, int maxGreyVal) {
    int width = img.width();
    int height = img.height();
    for (size_t x = 0; x < width; x++) {
	    for (size_t y = 0; y < height; y++) {
		    uint pixVal = img.pixel(x,y);
		    bool shouldBeBlacked = RGBstruct::isPixelInRangeOfGrey(pixVal, minGreyVal, maxGreyVal);
		    if (shouldBeBlacked)
			    img.setPixel(x, y, qRgb(0,0,0));
	    }
    }
 }
*/
/*
 int Utilities::UNUSED_modifyBit(int numberToEdit, int postition, int zeroOrOne){
	int mask = 1 << postition;
	return (numberToEdit & ~mask) | ((zeroOrOne << postition) & mask);
}
*/
/*
void Utilities::UNUSED_imgAvoideOneColor(QImage& img, QRgb minimalColorValues, QRgb maxColorValues, bool allOfThem){
    int width = img.width();
    int height = img.height();
    RGBstruct minV((uint)minimalColorValues);
    RGBstruct maxV((uint)maxColorValues);

    for (size_t x = 0; x < width; x++) {
	    for (size_t y = 0; y < height; y++) {
		    QRgb colorOfPixel = img.pixel(x, y);
		    RGBstruct current(colorOfPixel);

		    bool redIsEnough = (current.r >= minV.r) && (current.r <= maxV.r);
		    bool greenIsEnough = (current.g >= minV.g) && (current.g <= maxV.g);
		    bool blueIsEnough = (current.b >= minV.b) && (current.b <= maxV.b);

		    bool setGiveColor;
		    allOfThem = allOfThem ? (redIsEnough && greenIsEnough && blueIsEnough) : (redIsEnough || greenIsEnough || blueIsEnough);

		    if (setGiveColor)
			    img.setPixel(x, y, qRgb(0, 0, 0));
	    }
    }
 }
*/
/*
void Utilities::UNUSED_imgToOneColor(QImage& img, QRgb minimalColorValues, QRgb maxColorValues, QRgb colorToSet, bool allOfThem){
    int width = img.width();
    int height = img.height();
    auto black = qRgb(0, 0, 0);
    uint minValue = (uint)minimalColorValues;
    uint maxValue = (uint)maxColorValues;
    RGBstruct minV(minValue);
    RGBstruct maxV(maxValue);
    bool setGiveColor;

    for (size_t x = 0; x < width; x++) {
	    for (size_t y = 0; y < height; y++) {
		    QRgb colorOfPixel = img.pixel(x, y);
		    RGBstruct current(colorOfPixel);

		    bool redIsEnough = (current.r >= minV.r) && (current.r <= maxV.r);
		    bool greenIsEnough = (current.g >= minV.g) && (current.g <= maxV.g);
		    bool blueIsEnough = (current.b >= minV.b) && (current.b <= maxV.b);
		    
		    if (allOfThem)
			    setGiveColor = (redIsEnough && greenIsEnough && blueIsEnough);
		    else
			    setGiveColor = (redIsEnough || greenIsEnough || blueIsEnough);
	    
		    if (setGiveColor)
			    img.setPixel(x, y, colorToSet);
		    else
			    img.setPixel(x, y, black);
	    }
    }
 }
*/
/*
void convertMapToNoise(QImage& img, quint32 seed) {
	auto generator = QRandomGenerator(seed);
	//that fun needs #include "qrandom.h" to be used;
	quint64 AMOUNT_OF_POSSIBLE_COLOURS = 15;
	for (int x = 0; x < img.width(); x++) {
		for (int y = 0; y < img.height(); y++) {
			uint basic = generator.generate64() % (256 - AMOUNT_OF_POSSIBLE_COLOURS);
			RGBstruct rgb(img.pixel(x,y));
			uint sumOfColorsToAdd = rgb.r + rgb.g + rgb.b;
			uint toSet = basic + sumOfColorsToAdd;
			uint colourToSet= RGBstruct(toSet, toSet, toSet).toUint();
			img.setPixelColor(x, y, colourToSet);
		}
	}
}
*/