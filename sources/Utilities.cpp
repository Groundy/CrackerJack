#include "Utilities.h"
#include <QRgb>
#include <QtCore/qsettings.h>

int Utilities::showMessageBox(QString title = "CrackerJack", QString text = "", QFlags<QMessageBox::StandardButton> buttons = QMessageBox::Ok) {
	QMessageBox box;
	box.setText(text);
	box.setWindowTitle(title);
	box.setStandardButtons(buttons);
	//box.setDefaultButton(buttons);
	int ret = box.exec();
	return ret;
}

bool Utilities::showMessageBox_NO_YES(QString title, QString text)
 {
	 QFlags<QMessageBox::StandardButton> flags = { QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No };
	 int res = Utilities::showMessageBox(title, text, flags);
	 if (res == QMessageBox::StandardButton::Yes)
		 return true;
	 else
		 return false;
 }

bool Utilities::sendKeyStrokeToProcess(Key key, unsigned int PID, QString nameOfWindow) {
  
	LPCWSTR nameOfWindowLPCWSTR = convert_StrToLPCWSTR(nameOfWindow);
    HWND handler = FindWindow(NULL, nameOfWindowLPCWSTR);
    if (handler == NULL)
        return false;//diag err
    DWORD tmp = PID;
    DWORD hThread = GetWindowThreadProcessId(handler, &tmp);
    if (hThread != NULL) {
        int g =0x1470001;
        PostMessage(handler, WM_KEYDOWN, key.number, g);
        PostMessage(handler, WM_KEYUP, key.number, g);
		return true;
    }
    else
        return false;//errToDo

 }

void Utilities::imgToBlackAndWhiteOneColor(QImage& img, int threshold) {
	 int width = img.width();
	 int height = img.height();
	 for (size_t x = 0; x < width; x++) {
		 for (size_t y = 0; y < height; y++) {
			 uint pixelColor = img.pixel(x, y);
			 RGBstruct rgb(pixelColor);
			 bool redIsEnough = rgb.r >= threshold;
			 bool greenIsEnough = rgb.g >= threshold;
			 bool blueIsEnough = rgb.b >= threshold;
			 if (redIsEnough || greenIsEnough || blueIsEnough)
				 img.setPixel(x, y, qRgb(255, 255, 255));
			 else
				 img.setPixel(x, y, qRgb(0, 0, 0));
		 }
	 }
 }

void Utilities::imgToBlackAndWhiteAllColors(QImage& img, int threshold) {
	 int width = img.width();
	 int height = img.height();
	 for (size_t x = 0; x < width; x++) {
		 for (size_t y = 0; y < height; y++) {
			 uint pixelColor = img.pixel(x, y);
			 RGBstruct rgb(pixelColor);
			 bool redIsEnough = rgb.r >= threshold ? true : false;
			 bool greenIsEnough = rgb.g >= threshold ? true : false;
			 bool blueIsEnough = rgb.b >= threshold ? true : false;
			 if (redIsEnough && greenIsEnough && blueIsEnough)
				 img.setPixel(x, y, qRgb(255, 255, 255));
			 else
				 img.setPixel(x, y, qRgb(0, 0, 0));
		 }
	 }
 }

void Utilities::cutBlackBordersOfImg(QImage& img) {
	 int linesOfBlackrows_TOP = 0, linesOfBlackrows_DOWN = 0, linesOfBlackrows_RIGHT = 0, linesOfBlackrows_LEFT = 0;
	 int w = img.width();
	 int h = img.height();
	 for (int x = 0; x < w; x++) {
		 for (int y = 0; y < h; y++) {
			 bool isBlack = img.pixel(x, y) == qRgb(0, 0, 0);
			 if (!isBlack) {
				 linesOfBlackrows_LEFT = x;
				 x = w; //endOfLoop
				 y = h; //endOfLoop
			 }
		 }
	 }

	 for (int x = w - 1; x >= 0; x--) {
		 for (int y = 0; y < h; y++) {
			 bool isBlack = img.pixel(x, y) == qRgb(0, 0, 0);
			 if (!isBlack) {
				 linesOfBlackrows_RIGHT = w - x - 1;
				 x = -1;//endOfLoop
				 y = h;//endOfLoop
			 }
		 }
	 }

	 for (int y = 0; y < h; y++) {
		 for (int x = 0; x < w; x++) {
			 bool isBlack = img.pixel(x, y) == qRgb(0, 0, 0);
			 if (!isBlack) {
				 linesOfBlackrows_TOP = y;
				 x = w;//endOfLoop
				 y = h;//endOfLoop
			 }
		 }
	 }

	 for (int y = h - 1; y >= 0; y--) {
		 for (int x = 0; x < w; x++) {
			 bool isBlack = img.pixel(x, y) == qRgb(0, 0, 0);
			 if (!isBlack) {
				 linesOfBlackrows_DOWN = h - y - 1;
				 x = w;//endOfLoop
				 y = -1;//endOfLoop
			 }
		 }
	 }

	 int anotherParametr_x = w - linesOfBlackrows_RIGHT - linesOfBlackrows_LEFT;
	 int anotherParametr_y = h - linesOfBlackrows_TOP - linesOfBlackrows_DOWN;

	 int widthToCut = anotherParametr_x >= 0 ? anotherParametr_x : 0;
	 int heightToCut = anotherParametr_y >= 0 ? anotherParametr_y : 0;
	 img = img.copy(linesOfBlackrows_LEFT, linesOfBlackrows_TOP, widthToCut, heightToCut);
 }

void Utilities::cutImgWithLettersToSingleLettersImgList(QImage& img, QList<QImage>& letterImages) {
	 QList<int> colThatAreNotBlack;
	 int w = img.width();
	 int h = img.height();
	 for (int x = 0; x < w; x++) {
		 for (int y = 0; y < h; y++) {
			 if (img.pixel(x, y) != qRgb(0, 0, 0)) {
				 y = h; //ending inner loop
				 colThatAreNotBlack.push_back(x);
			 }
		 }
	 }
	 if (colThatAreNotBlack.size() == 0) {
		 letterImages.push_back(img);
		 return;
	 }

	 QList<int> indexesOfStartOfLetter, indexesOfEndsOfLetters;
	 indexesOfStartOfLetter.push_back(0);
	 for (int i = 0; i < colThatAreNotBlack.size() - 1; i++) {
		 if (colThatAreNotBlack[i + 1] - colThatAreNotBlack[i] > 1) {
			 indexesOfEndsOfLetters.push_back(colThatAreNotBlack[i]);
		 }
	 }
	 for (int i = 1; i < colThatAreNotBlack.size(); i++) {
		 if (colThatAreNotBlack[i] - colThatAreNotBlack[i - 1] > 1) {
			 indexesOfStartOfLetter.push_back(colThatAreNotBlack[i]);
		 }
	 }
	 indexesOfEndsOfLetters.push_back(colThatAreNotBlack.last());
	 QList<int> widths;

	 for (size_t i = 0; i < indexesOfStartOfLetter.size(); i++)
		 widths.push_back(indexesOfEndsOfLetters[i] - indexesOfStartOfLetter[i] + 1);

	 for (size_t i = 0; i < indexesOfStartOfLetter.size(); i++) {
		 QImage letter = img.copy(indexesOfStartOfLetter[i], 0, widths[i], img.height());
		 letterImages.push_back(letter);
	 }
 }

QString Utilities::imgWithStrToStr(QImage* img) {
	 imgToBlackAndWhiteAllColors(*img, 240);
	 cutBlackBordersOfImg(*img);
	 QList<QImage>* imgs = new QList<QImage>;
	 cutImgWithLettersToSingleLettersImgList(*img, *imgs);
	 QString toRet;
	 for (size_t i = 0; i < imgs->size(); i++) {
		 QImage tmp = imgs->at(i);
		 QImage* tmpPointer = &tmp;
		 cutBlackBordersOfImg(*tmpPointer);
		 QString letterCode = letterImgToLetterCodeStr(tmpPointer);
		 QChar letter = StrCodeToQChar(letterCode);
		 toRet.append(letter);
	 }
	 delete imgs;
	 return toRet;
 }

QString Utilities::letterImgToLetterCodeStr(QImage* SingleLetterImg) {
	 int width = SingleLetterImg->width();
	 int height = SingleLetterImg->height();
	 uint pixelColor;
	 QString toRet = QString::number(width) + QString("_") + QString::number(height) + QString("_");
	 int sum;
	 for (size_t x = 0; x < width; x++) {
		 for (size_t y = 0; y < height; y++) {
			 pixelColor = SingleLetterImg->pixel(x, y);
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

QChar Utilities::StrCodeToQChar(QString code){
	 //todo, it's terrible solution, it has to be changed
	QMap<QString, QChar> letters = getQmapWithCodes();
	return letters[code];
 }

QMap<QString, QChar>  Utilities::getQmapWithCodes() {
	 QMap<QString, QChar> letters;
	 //char 200 it's some weird char but here it's symbol of healthIcon
	 letters.insert("9_9_101110000011111000111111100111111110011111111111111110111111100111111000011110000",QChar(200));
	 //char 201 it's some weird char but here it's symbol of manaIcon
	 letters.insert("10_9_000100001001100111001111111111111110111111100111111100111110000111110000100100000100000000", QChar(201));
	 letters.insert("6_8_011111101111111110000001100000011111111101111110", '0');
	 letters.insert("4_8_01000001111111111111111100000001", '1');
	 letters.insert("6_8_010000111100011110001101100110011111000101100001", '2');
	 letters.insert("6_8_010000101100001110010001100100011111111101101110", '3');
	 letters.insert("6_8_000011000001010000100100011111111111111100000100", '4');
	 letters.insert("6_8_000000101111001111110001100100011001111110001110", '5');
	 letters.insert("6_8_001111100111111111010001100100011001111100001110", '6');
	 letters.insert("6_8_100000001000001110001111101111001111000011000000", '7');
	 letters.insert("6_8_011011101111111110010001100100011111111101101110", '8');
	 letters.insert("6_8_011100001111100110001001100010111111111001111100", '9');
	 letters.insert("6_6_000110101111101001101001111111011111", 'a');
	 letters.insert("6_9_111111111111111111000100001000100001000111111000011110", 'b');
	 letters.insert("5_6_011110111111100001100001100001", 'c');
	 letters.insert("6_9_000011110000111111000100001000100001111111111111111111", 'd');
	 letters.insert("6_6_011110111111101001101001111001011010", 'e');
	 letters.insert("5_9_000100000011111111111111111100100000100100000", 'f');
	 letters.insert("6_8_011110001111110110000101100001011111111111111110", 'g');
	 letters.insert("6_9_111111111111111111000100000000100000000111111000011111", 'h');
	 letters.insert("2_8_1011111110111111", 'i');
	 letters.insert("4_10_0000000001001000000110111111111011111110", 'j');
	 letters.insert("6_9_111111111111111111000001100000011110000110011000100001", 'k');
	 letters.insert("2_9_111111111111111111", 'l');
	 letters.insert("10_6_111111111111100000100000111111011111100000100000111111011111", 'm');
	 letters.insert("6_6_111111111111100000100000111111011111", 'n');
	 letters.insert("6_6_011110111111100001100001111111011110", 'o');
	 letters.insert("6_8_111111111111111110000100100001001111110001111000", 'p');
	 letters.insert("5_6_111111111111010000110000110000", 'r');
	 letters.insert("5_6_011001111101101101101111100110", 's');
	 letters.insert("5_8_0010000011111110111111110010000100100001", 't');
	 letters.insert("6_6_111110111111000001000001111111111111", 'u');
	 letters.insert("6_6_111000111110000111000111111110111000", 'v');
	 letters.insert("8_6_111100111111000011111100111100000011111111111100", 'w');
	 letters.insert("6_6_110011111111001100001100111111110011", 'x');
	 letters.insert("6_8_110000001111001100111111001111001111000011000000", 'y');
	 letters.insert("5_6_100011100111101101111001110001", 'z');
	 letters.insert("7_8_00000111001111111111110011000100111111000011111100000111", 'A');
	 letters.insert("6_8_111111111111111110010001100100011111111101101110", 'B');
	 letters.insert("6_8_011111101111111110000001100000011000000101000010", 'C');
	 letters.insert("7_8_11111111111111111000000110000001110000110111111000111100", 'D');
	 letters.insert("6_8_111111111111111110010001100100011001000110010001", 'E');
	 letters.insert("6_8_111111111111111110010000100100001001000010010000", 'F');
	 letters.insert("7_8_01111110111111111000000110000001100010011000111101001111", 'G');
	 letters.insert("7_8_11111111111111110001000000010000000100001111111111111111", 'H');
	 letters.insert("4_8_10000001111111111111111110000001", 'I');
	 letters.insert("5_8_0000000110000001100000011111111111111110", 'J');
	 letters.insert("6_8_111111111111111100111100011001101100001110000001", 'K');
	 letters.insert("6_8_111111111111111100000001000000010000000100000001", 'L');
	 letters.insert("8_8_1111111111100000011100000011100000010000001000000111111111111111", 'M');
	 letters.insert("7_8_11111111011000000011000000011000000011000000011011111111", 'N');
	 letters.insert("7_8_01111110111111111000000110000001100000011111111101111110", 'O');
	 letters.insert("6_8_111111111111111110001000100010001111100001110000", 'P');
	 letters.insert("7_8_11111111111111111000100010001100111111100111001100000001", 'R');
	 letters.insert("6_8_011100101111100110011001100110011001111101001110", 'S');
	 letters.insert("8_8_1000000010000000100000001111111111111111100000001000000010000000", 'T');
	 letters.insert("7_8_11111110111111110000000100000001000000011111111111111110", 'U');
	 letters.insert("6_8_111000001111110000011111000111111111110011100000", 'V');
	 letters.insert("0_8_11100000111111000001111100001111111110001111100000001111000111111111110011100000", 'W');
	 letters.insert("6_8_110000111110011100111100001111001110011111000011", 'X');
	 letters.insert("6_8_110000001111000000111111001111111111000011000000", 'Y');
	 letters.insert("6_8_100001111000111110011101101110011111000111100001", 'Z');
	 //slash, in program it's slash between current health and max health
	 letters.insert("11_6_110000110000011000011000001100001100001100000110000110000011000011", QChar(47));
	 //backslash char but in program it's slash above but rotated
	 letters.insert("6_11_000000000110000000111100001111100001111100001111000000011000000000", QChar(92));
	 //opening bracket
	 letters.insert("4_11_00011111000011111111101110000011110000000001", QChar(40));
	 //closing bracket
	 letters.insert("4_11_10000000001111000001110111111111000011111000", QChar(41));
	 
	 return letters;
 }

void Utilities::getMapWithPotionsImgCodes(QMap<QString, QString>& name_code_light, QMap<QString, QString>& name_code_dark){
	name_code_light.clear();
	name_code_light.insert("Great Health Potion", "17_2_#139#7#0_#50#3#0_#170#6#0_#109#6#0_#199#2#0_#205#4#0_#228#2#0_#222#2#0_#247#33#39_#242#2#0_#243#105#129_#244#33#39_#236#86#105_#219#58#66_#157#8#0_#163#38#39_#183#5#0_#160#5#0_#171#5#0_#199#6#0_#144#7#0_#160#6#0_#146#7#0_#124#9#0_#147#8#0_#137#7#0_#150#3#0_#142#7#0_#146#4#0_#147#4#0_#117#3#0_#106#4#0_#117#4#0_#101#4#0_");
	name_code_light.insert("Great Mana Potion", "18_2_#136#46#100_#46#12#35_#158#53#118_#110#37#82_#175#69#125_#180#67#130_#184#68#134_#168#61#123_#227#123#174_#184#67#132_#213#113#165_#199#93#151_#148#44#112_#163#53#122_#136#40#103_#131#40#99_#123#27#100_#115#30#91_#131#40#99_#147#40#114_#137#44#103_#135#40#103_#143#48#106_#138#46#102_#140#44#105_#131#41#98_#131#48#96_#121#32#95_#90#18#72_#128#44#95_#59#5#51_#58#5#49_#63#5#54_#55#5#47_#18#2#15_#13#1#11_");
	name_code_light.insert("Great Spirit Potion", "4_2_#168#115#59_#191#146#89_#151#92#34_#188#143#89_#128#68#0_#119#63#0_#121#58#0_#112#59#0_");
	name_code_light.insert("Health Potion", "9_2_#201#27#21_#177#35#0_#248#68#117_#194#4#13_#252#75#114_#220#17#74_#208#10#38_#195#7#55_#151#0#1_#158#0#10_#126#16#0_#130#9#0_#131#19#0_#134#16#0_#93#13#0_#92#17#0_#83#14#0_#98#16#0_");
	name_code_light.insert("Mana Potion", "9_2_#201#21#194_#177#0#142_#199#68#248_#184#4#194_#213#75#252_#162#17#220_#179#10#208_#147#7#195_#149#0#151_#148#0#158_#126#0#110_#130#0#120_#131#0#112_#134#0#118_#93#0#80_#92#0#75_#83#0#69_#98#0#82_");
	name_code_light.insert("Strong Health Potion", "13_2_#197#0#10_#177#38#0_#238#0#66_#194#0#7_#254#0#28_#229#0#46_#231#0#16_#234#0#17_#206#2#1_#187#0#2_#228#1#3_#170#8#1_#215#4#2_#136#11#0_#148#10#0_#165#0#21_#143#10#0_#143#10#0_#143#10#0_#136#11#0_#136#11#0_#112#15#0_#92#18#0_#92#18#0_#92#18#0_#92#18#0_");
	name_code_light.insert("Strong Mana Potion", "13_2_#183#0#126_#177#0#77_#213#0#246_#194#0#133_#221#0#250_#218#0#209_#206#0#167_#193#0#176_#153#0#96_#158#0#106_#143#0#82_#139#0#79_#145#0#80_#136#0#78_#136#0#78_#165#0#128_#143#0#83_#143#0#83_#143#0#83_#136#0#78_#136#0#78_#112#0#58_#92#0#42_#92#0#42_#92#0#42_#92#0#42_");
	name_code_light.insert("Supreme Health Potion", "3_3_#212#2#0_#221#33#39_#53#43#40_#211#2#0_#174#6#0_#157#8#0_#48#44#44_#48#44#44_#48#44#44_");
	name_code_light.insert("Ultimate Health Potion", "3_3_#221#33#39_#236#86#105_#219#58#66_#174#6#0_#157#8#0_#163#38#39_#48#44#44_#48#44#44_#160#5#0_");
	name_code_light.insert("Ultimate Mana Potion", "4_2_#213#113#165_#199#93#151_#148#44#112_#163#53#122_#136#40#103_#131#40#99_#48#44#44_#115#30#91_");
	name_code_light.insert("Ultimate Spirit Potion", "4_2_#168#115#59_#191#146#89_#151#92#34_#188#143#89_#128#68#0_#119#63#0_#48#44#44_#112#59#0_");
	
	name_code_dark.clear();
	name_code_dark.insert("Great Mana Potion", "18_2_#41#14#30_#14#4#10_#47#16#35_#33#11#24_#52#21#37_#54#20#39_#55#20#40_#50#18#37_#68#37#52_#55#20#39_#63#34#49_#59#28#45_#44#13#33_#49#16#36_#41#12#31_#39#12#30_#37#8#30_#34#9#27_#39#12#30_#44#12#34_#41#13#31_#40#12#31_#43#14#32_#41#14#30_#42#13#31_#39#12#29_#39#14#29_#36#10#28_#27#5#21_#38#13#28_#18#1#15_#17#1#15_#19#1#16_#16#1#14_#5#1#4_#4#0#3_");
	name_code_dark.insert("Great Spirit Potion", "4_2_#50#34#18_#57#44#27_#45#27#10_#56#43#27_#38#20#0_#35#19#0_#36#17#0_#33#18#0_");
	name_code_dark.insert("Health Potion", "9_2_#60#8#6_#53#10#0_#74#20#35_#58#1#4_#75#22#34_#66#5#22_#62#3#11_#58#2#16_#45#0#0_#47#0#3_#38#5#0_#39#3#0_#39#6#0_#40#5#0_#28#4#0_#27#5#0_#25#4#0_#29#5#0_");
	name_code_dark.insert("Mana Potion", "9_2_#60#6#58_#53#0#42_#59#20#74_#55#1#58_#63#22#75_#48#5#66_#53#3#62_#44#2#58_#44#0#45_#44#0#47_#38#0#33_#39#0#36_#39#0#33_#40#0#35_#28#0#24_#27#0#22_#25#0#21_#29#0#24_");
	name_code_dark.insert("Strong Health Potion", "13_2_#59#0#3_#53#11#0_#71#0#20_#58#0#2_#76#0#8_#68#0#14_#69#0#5_#70#0#5_#61#1#0_#56#0#1_#68#0#1_#51#2#0_#64#1#1_#41#3#0_#44#3#0_#49#0#6_#43#3#0_#43#3#0_#43#3#0_#41#3#0_#41#3#0_#33#4#0_#27#5#0_#27#5#0_#27#5#0_#27#5#0_");
	name_code_dark.insert("Strong Mana Potion", "13_2_#55#0#38_#53#0#23_#63#0#73_#58#0#40_#66#0#75_#65#0#62_#61#0#50_#58#0#52_#46#0#29_#47#0#32_#43#0#24_#41#0#24_#43#0#24_#41#0#23_#41#0#23_#49#0#38_#43#0#25_#43#0#25_#43#0#25_#41#0#23_#41#0#23_#33#0#17_#27#0#13_#27#0#13_#27#0#13_#27#0#13_");
	name_code_dark.insert("Supreme Health Potion", "3_3_#63#1#0_#66#10#12_#16#13#12_#63#1#0_#52#2#0_#47#2#0_#14#13#13_#14#13#13_#14#13#13_");
	name_code_dark.insert("Ultimate Health Potion", "3_3_#66#10#12_#70#26#31_#65#17#20_#52#2#0_#47#2#0_#49#11#12_#14#13#13_#14#13#13_#48#1#0_");
	name_code_dark.insert("Ultimate Mana Potion", "4_2_#63#34#49_#59#28#45_#44#13#33_#49#16#36_#41#12#31_#39#12#30_#14#13#13_#34#9#27_");
	name_code_dark.insert("Ultimate Spirit Potion", "4_2_#50#34#18_#57#44#27_#45#27#10_#56#43#27_#38#20#0_#35#19#0_#14#13#13_#33#18#0_");
	name_code_dark.insert("Great Health Potion", "17_2_#41#2#0_#15#1#0_#51#2#0_#32#2#0_#59#1#0_#61#1#0_#68#1#0_#66#1#0_#74#10#12_#72#1#0_#72#31#38_#73#10#12_#70#26#31_#65#17#20_#47#2#0_#49#11#12_#55#1#0_#48#1#0_#51#1#0_#59#2#0_#43#2#0_#48#2#0_#44#2#0_#37#3#0_#44#2#0_#41#2#0_#45#1#0_#42#2#0_#44#1#0_#44#1#0_#35#1#0_#32#1#0_#35#1#0_#30#1#0_");
}

QList<QPoint> Utilities::findStartPositionInImg(QImage* imgToFind, QImage* imgToSearchWithin){
	 //it can be opitimalized, now we compare the same pixels few times
	 //fun return list of start positions of imgToFind, position is lef down corner
	 QImage::Format format1 = imgToFind->format();
	 QImage::Format format2 = imgToSearchWithin->format();

	 const int widthSmall = imgToFind->width();
	 const int heightSmall = imgToFind->height();
	 const int widthBig = imgToSearchWithin->width();
	 const int heightBig = imgToSearchWithin->height();


	 bool errWidth = widthSmall >= widthBig;
	 bool errHeight = heightSmall >= heightBig;
	 bool errFormat = format1 != format2;
	 bool anyErr = errWidth || errHeight || errFormat;
	 if (anyErr);//diag /err

	 const int maxPixIndThatShoudldBeTested_X = widthBig - widthSmall;
	 const int maxPixIndThatShoudldBeTested_Y = heightBig - heightSmall;

	 QList<QPoint> startPointsListToRet;
	 QRgb pixFirst, pixSecond;
	 bool foundPosition;

	 for (int x = 0; x <= maxPixIndThatShoudldBeTested_X; x++){
		 for (int y = 0; y <= maxPixIndThatShoudldBeTested_Y; y++){
			 pixFirst = imgToFind->pixel(0, 0);
			 pixSecond = imgToSearchWithin->pixel(x, y);
			 if (pixFirst == pixSecond){
				 //first pix matched, looking for more
				 foundPosition = true;
				 for (int x_TMP = 1; x_TMP < widthSmall; x_TMP++){
					 for (int y_TMP = 1; y_TMP < heightSmall; y_TMP++){
						 pixFirst = imgToFind->pixel(x_TMP,y_TMP);
						 pixSecond = imgToSearchWithin->pixel(x + x_TMP, y + y_TMP);
						 if (pixFirst != pixSecond) {
							 x_TMP = widthSmall;
							 y_TMP = heightSmall;
							 foundPosition = false;
						 }
					 }
				 }
				 if (foundPosition)
					 startPointsListToRet.push_back(QPoint(x, y));
			 }
		 }
	 }
	 return startPointsListToRet;
 }

QList<QPoint> Utilities::findStartPositionInImg_mulitpeImgs(QList<QImage*> imgsToFind, QImage* imgToShareWithin){
	 // this fun return starting points from imgToSharePoints than consist pixels from one of imgsToFind


	 bool properFormats = true;
	 QImage::Format formatBig = imgToShareWithin->format();

	 for each (QImage * var in imgsToFind){
		 if (formatBig != var->format()) {
			 properFormats = false;
			 break;
		 }
	 }
	 if (!properFormats)
		 ;//diag /err

	 bool allPicAreInTheSameSize = true;
	 int widthOfFirst = imgsToFind[0]->width();
	 int heightOfFirst = imgsToFind[0]->height();
	 for each (QImage* var in imgsToFind){
		 bool widthIsGood = var->width() == widthOfFirst;
		 bool heightIsGood = var->height() == heightOfFirst;
		 if (!(widthIsGood && heightIsGood)) {
			 allPicAreInTheSameSize = false;
			 break;//diag /err
		 }
	 }
	 bool allGood = allPicAreInTheSameSize && properFormats;

	 const int maxPixIndThatShoudldBeTested_X = imgToShareWithin->width() - widthOfFirst;
	 const int maxPixIndThatShoudldBeTested_Y = imgToShareWithin->height() - heightOfFirst;

	 QList<QPoint> startPointsListToRet;
	 for (int x = 0; x <= maxPixIndThatShoudldBeTested_X; x++) {
		 for (int y = 0; y <= maxPixIndThatShoudldBeTested_Y; y++) {
			 bool atLeastOnePixIsMatched = false;
			 QRgb pixFromImg_big = imgToShareWithin->pixel(x, y);

			 for each (QImage * var in imgsToFind) {
				 QRgb pixFromImg_small = var->pixel(0, 0);
				 if (pixFromImg_big == pixFromImg_small) {
					 atLeastOnePixIsMatched = true;
					 break;
				 }
			 }

			 if (atLeastOnePixIsMatched) {
				 //first pix matched, looking for more
				 int wrongPixels = widthOfFirst * heightOfFirst;
				 for (int x_TMP = 0; x_TMP < widthOfFirst; x_TMP++) {
					 for (int y_TMP = 0; y_TMP < heightOfFirst; y_TMP++) {
						 QRgb pixFromImg_big2 = imgToShareWithin->pixel(x + x_TMP, y + y_TMP);
					     bool metReq = false;
						 for each (QImage* var in imgsToFind) {
							 QRgb pixFromImg_small2 = var->pixel(x_TMP, y_TMP);
							 if (pixFromImg_big2 == pixFromImg_small2) {
								 metReq = true;
								 break;
							 }
						 }
						 if (metReq)
							 wrongPixels--;
					 }
				 }
				 if (wrongPixels == 0)
					 startPointsListToRet.push_back(QPoint(x, y));
			 }
		 }
	 }
	 return startPointsListToRet;
 }

QImage Utilities::fromCharToImg(QChar CharToImg){
	auto mapWithCodes = getQmapWithCodes();
	QString keyCode = mapWithCodes.key(CharToImg);
	QStringList parts;
	parts = keyCode.split("_");
	int widthOfLetter = parts[0].toInt();
	int heightOfLetter = parts[1].toInt();
	QString code = parts[2];
	QImage imgToRet(widthOfLetter, heightOfLetter, QImage::Format::Format_ARGB32);
	int i = 0;
	int sizeOfStr = code.size();
	for (size_t x = 0; x < widthOfLetter; x++) {
		for (size_t y = 0; y < heightOfLetter; y++) {
			if (code[i] == '1')
				imgToRet.setPixel(x, y, qRgba(255,255,255,255));
			else if (code[i] == '0')
				imgToRet.setPixel(x, y, qRgba(0, 0, 0, 255));
			else
				;//diag err
			i++;
		}
	}
	return imgToRet;
}

void Utilities::rotateImgToRight(QImage* imgToRotate, int timesToRotateRight){
	QTransform rotating;
	qreal degreeToRotateToRight = timesToRotateRight * 90;
	rotating.rotate(degreeToRotateToRight,Qt::Axis::ZAxis);
	QImage tmp = imgToRotate->transformed(rotating);
	*imgToRotate = tmp;
}

bool Utilities::isItPixelFromFrame(uint color, int minValueAcceptable, int maxValueAcceptable, bool requireSameValuesOfRGB){
	RGBstruct rgb(color);
	int minValueFound = min(min(rgb.r, rgb.g), rgb.b);
	int maxValueFound = max(max(rgb.r, rgb.g), rgb.b);
	if (requireSameValuesOfRGB) {
		if (maxValueFound - minValueFound > 1)
			return false;
	}
	if (minValueFound >= minValueAcceptable && maxValueFound <= maxValueAcceptable)
		return true;
	else
		return false;
}

long long Utilities::getCurrentTimeInMiliSeconds() {
	QDateTime date = QDateTime::currentDateTime();
	qint64 mseconds = date.currentMSecsSinceEpoch();
	return mseconds;
}

QImage Utilities::getImageFromAdvancedCode(QString codeOfImg){
	QStringList partsOfCode = codeOfImg.split(QString("_"),Qt::SkipEmptyParts);
	int width = partsOfCode[0].toInt();
	int height = partsOfCode[1].toInt();
	partsOfCode.removeFirst();
	partsOfCode.removeFirst();
	QImage imgToCreate(width, height, QImage::Format::Format_ARGB32);
	int i = 0;
	for (size_t x = 0; x < width; x++){
		for (size_t y = 0; y < height; y++){
			QStringList rgb = partsOfCode[i].split(QString("#"), Qt::SkipEmptyParts);
			i++;
			int r = rgb[0].toInt();
			int g = rgb[1].toInt();
			int b = rgb[2].toInt();
			auto pixToSet = qRgb(r, g, b);
			imgToCreate.setPixel(x, y, pixToSet);
		}
	}
	return imgToCreate;
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

int Utilities::getNumberFromBottomBar(QImage* bottomBar){
	QMap<QString, int> lightMap, darkMap;
	Utilities::getMapWithNumbersFromBottomBar(lightMap, darkMap);
	QStringList lightCodes = lightMap.keys();
	QStringList darkCodes = darkMap.keys();

	QMap<int, int> anotherMap; // <positionX, value>

	for (size_t i = 0; i < lightCodes.size(); i++){
		QList<QImage*> listWithLightAndDarkLetterImg;
		QImage darkLetter = Utilities::getImageFromAdvancedCode(darkCodes[i]);
		QImage lightLetter = Utilities::getImageFromAdvancedCode(lightCodes[i]);
		listWithLightAndDarkLetterImg.push_back(&darkLetter);
		listWithLightAndDarkLetterImg.push_back(&lightLetter);

		int digit = lightMap[lightCodes[i]];
		auto listOfStartingPoints = findStartPositionInImg_mulitpeImgs(listWithLightAndDarkLetterImg, bottomBar);
		for each (QPoint var in listOfStartingPoints)
			anotherMap.insert(var.x(), digit);
	}

	QString strToRe;
	for each (auto key in anotherMap.keys())
		strToRe.push_back(QString::number(anotherMap[key]));
	 
	return strToRe.toInt();
}

QMap<Utilities::FieldsOfIniFile, QString> Utilities::get_Field_NamesFromIni_map(){
	typedef Utilities::FieldsOfIniFile Field;
	QMap<Field, QString> toRet;

	toRet.insert(Field::LANGUAGE, "language");
	toRet.insert(Field::LAST_USED_LOGIN, "testLogin");

	return toRet;
}

QString Utilities::readFromIniFile(FieldsOfIniFile nameOfField){
	QSettings setttings("settings.ini", QSettings::IniFormat);
	QMap<Utilities::FieldsOfIniFile, QString> map = get_Field_NamesFromIni_map();
	QString nameOfFieldAsStr = map[nameOfField];
	QString readVal = setttings.value(nameOfFieldAsStr).toString();
	return readVal;
}

void Utilities::writeIniFile(FieldsOfIniFile nameOfField, QString value){
	QSettings setttings("settings.ini", QSettings::IniFormat);
	QMap<Utilities::FieldsOfIniFile, QString> map = get_Field_NamesFromIni_map();
	QString nameOfFieldAsStr = map[nameOfField];
	setttings.setValue(nameOfFieldAsStr, value);
}


bool Utilities::findPotionsOnBottomBar(QStringList namesOfPotionsToFind, QStringList& namesOfPotionosFound, QList<QRect>& rectsWithFoundPots, QImage& bottomBarImg){
	QMap<QString, QString> map_light, map_dark;
	getMapWithPotionsImgCodes(map_light, map_dark);

	QStringList allPotionsNames = map_light.keys();
	for each (QString var in allPotionsNames) {
		bool shouldBeOnListToFind = namesOfPotionsToFind.contains(var);
		if (!shouldBeOnListToFind) {
			map_light.remove(var);
			map_dark.remove(var);
		}
	}
	bool err = (map_light.size() != map_dark.size()) || (map_dark.size() < 0);
	if (err)
		return false;

	QList<QImage> potionsImg_light, potionsImg_dark;
	for each (QString potName in map_light.keys()) {
		QString code_light = map_light[potName];
		QString code_dark = map_dark[potName];
		QImage imgToAdd;
		imgToAdd = getImageFromAdvancedCode(code_light);
		potionsImg_light.push_back(imgToAdd);
		imgToAdd = getImageFromAdvancedCode(code_dark);
		potionsImg_dark.push_back(imgToAdd);
	}

	QStringList namesOfFoundPotions;
	QList<QPoint> potionsOfStartingPotions;
	for (size_t i = 0; i < map_dark.size(); i++){
		QList<QImage*> potionToLookFor;
		potionToLookFor.push_back(&potionsImg_light[i]);
		potionToLookFor.push_back(&potionsImg_dark[i]);
		QList<QPoint> pointsOfStart = findStartPositionInImg_mulitpeImgs(potionToLookFor, &bottomBarImg);
		if (pointsOfStart.size() == 1) {
			potionsOfStartingPotions.push_back(pointsOfStart.first());
			namesOfFoundPotions.push_back(map_dark.keys()[i]);
		}
		else
			;//diag err
	}

	const int WIDTH_HEIGHT_PIC = 32;
	const RGBstruct LEFT_BOTTOM_COLOUR(66, 66, 66);
	const RGBstruct PIX_ABOVE_COLOUR(112, 113, 113);
	const int WIDTH = bottomBarImg.width();
	const int HEIGHT = bottomBarImg.height();
	QList<QRect> rectToRet;
	for (size_t i = 0; i < potionsOfStartingPotions.size(); i++){
		QPoint& someWhereInPic = potionsOfStartingPotions[i];
		int min_x, max_x, min_y, max_y;
		int curr_x = someWhereInPic.x();
		int curr_y = someWhereInPic.y();
		
		min_x = max(curr_x - WIDTH_HEIGHT_PIC, 0);
		min_y = curr_y;
		max_x = min(curr_x + WIDTH_HEIGHT_PIC, WIDTH);
		max_y = min(curr_y + WIDTH_HEIGHT_PIC, HEIGHT);
		
		for (size_t x = min_x; x < max_x; x++)	{
			for (size_t y = min_y; y < max_y; y++){
				RGBstruct rgb(bottomBarImg.pixel(x, y));
				RGBstruct pixAboveRgb(bottomBarImg.pixel(x, y-1));
				bool goodPix1 = rgb == LEFT_BOTTOM_COLOUR;
				bool goodPix2 = pixAboveRgb == PIX_ABOVE_COLOUR;
				if(goodPix1 && goodPix2){
					someWhereInPic.setX(x);
					someWhereInPic.setY(y);
					x = max_x;//break
					y = max_y;//break
				}
			}
		}

		int width = WIDTH_HEIGHT_PIC;
		int height = WIDTH_HEIGHT_PIC / 3;
		int start_x = someWhereInPic.x() +1;
		int start_y = someWhereInPic.y() - WIDTH_HEIGHT_PIC + 2 * height +1;

		bool err = start_x < 0 || (start_x + width) > WIDTH || start_y < 0 || (start_y + height) > HEIGHT;
		QRect toAdd(start_x, start_y, width, height);
		rectToRet.push_back(toAdd);
	}

	namesOfPotionosFound = namesOfFoundPotions;
	rectsWithFoundPots = rectToRet;

	return true;
}

void Utilities::saveImgToOutPutFolder(QImage* img, QString *extraName){
	QString prefixOfName;
	if (extraName == NULL)
		prefixOfName = QDateTime::currentDateTime().toString("mm_ss_zzz");
	else
		prefixOfName = *extraName;
	QString fullname = "C:\\Users\\ADMIN\\Desktop\\output\\_" + prefixOfName + ".png";
	img->save(fullname);
}

LPCWSTR Utilities::convert_StrToLPCWSTR(QString str){
     return (const wchar_t*)str.utf16();
 }

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
		QString hash = QString("#");
		QString floor = QString("_");
		QString strWithCode = QString::number(width) + floor + QString::number(height) + floor;
		
		for (size_t x = 0; x < width; x++){
			for (size_t y = 0; y < height; y++){
				uint pixVal = (uint)img->pixel(x, y);
				RGBstruct rgb(pixVal);
				QString r = QString::number(rgb.r);
				QString g = QString::number(rgb.g);
				QString b = QString::number(rgb.b);
				QString strToAppend = hash + r + hash + g + hash + b + floor;
				strWithCode.append(strToAppend);
			}
		}
		list.push_back(strWithCode);
		QString toDisplay = litOfFIles[i] + "______" + strWithCode;
		qDebug() << toDisplay;
	}

	
	return list;
}

QStringList Utilities::TOOL_getCodesOfAllInFolder_regular(QString pathToInputFolder, QString pathToOutputFolder) {
	 QDir directory(pathToInputFolder);
	 QStringList litOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
	 QList<QString> list;
	 for (size_t i = 0; i < litOfFIles.size(); i++) {
		 QString pathToFile = pathToInputFolder + "\\" + litOfFIles[i];
		 QImage img = QImage(pathToFile);
		 Utilities::imgToBlackAndWhiteOneColor(img, 200);
		 QString name = pathToOutputFolder + "\\" + litOfFIles[i];
		 img.save(name);
		 QString code = letterImgToLetterCodeStr(&img);

		 //changing "slash.png" to "slash"
		 QString tmp = litOfFIles[i].left(litOfFIles[i].size() - 4);
		 litOfFIles[i] = tmp;

		 list.push_back(litOfFIles[i] + QString("___") + code);
	 }

	 for each (QString var in list) {
		 qDebug() << var;
	 }
	 return list;
 }

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
 
void Utilities::UNUSED_imgAvoideOneColor(QImage& img, QRgb minimalColorValues, QRgb maxColorValues, bool allOfThem){
	 int width = img.width();
	 int height = img.height();
	 RGBstruct minV((uint)minimalColorValues);
	 RGBstruct maxV((uint)maxColorValues);

	 for (size_t x = 0; x < width; x++) {
		 for (size_t y = 0; y < height; y++) {
			 QRgb colorOfPixel = img.pixel(x, y);
			 RGBstruct current(colorOfPixel);

			 bool redIsEnough = (current.r >= minV.r) && (current.r <= maxV.r) ? true : false;
			 bool greenIsEnough = (current.g >= minV.g) && (current.g <= maxV.g) ? true : false;
			 bool blueIsEnough = (current.b >= minV.b) && (current.b <= maxV.b) ? true : false;

			 bool setGiveColor;
			 allOfThem = allOfThem ? (redIsEnough && greenIsEnough && blueIsEnough) : (redIsEnough || greenIsEnough || blueIsEnough);

			 if (setGiveColor)
				 img.setPixel(x, y, qRgb(0, 0, 0));
		 }
	 }
 }

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

 int Utilities::UNUSED_modifyBit(int numberToEdit, int postition, int zeroOrOne){
	int mask = 1 << postition;
	return (numberToEdit & ~mask) | ((zeroOrOne << postition) & mask);
}
*/ 