#include "Calibrator.h"
Calibrator::Calibrator(QImage& fullScreenImg, VariablesClass& varClass, Profile* profileToPass){
	var = &varClass;
	fullScreen = &fullScreenImg;
	profile = profileToPass;
}

Calibrator::~Calibrator()
{
}

void Calibrator::getMapWithPotionsImgCodes(QMap<QString, QString>& name_code_light, QMap<QString, QString>& name_code_dark) {
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

int Calibrator::fillRectWithPotsInVarClass(QImage& fullscreen, QStringList nameOfPotionsToLookFor) {
	QStringList foundPotions;
	QList<QRect> rectsWithPotions;
	findPotionsOnBottomBar(nameOfPotionsToLookFor, foundPotions, rectsWithPotions, fullscreen);

	bool allPotionsFound = true;
	for each (QString potNameThatShouldBeFound in nameOfPotionsToLookFor) {
		bool found = foundPotions.contains(potNameThatShouldBeFound);
		if (!found)
			allPotionsFound = false;
	}
	if (!allPotionsFound)
		return NOT_ALL_POTS_FOUND;

	for (size_t i = 0; i < rectsWithPotions.size(); i++) {
		QRect rectToAdd = rectsWithPotions[i];
		QString nameOfPot = foundPotions[i];
		var->potionName_rectPosOnScreen_map[nameOfPot] = rectToAdd;//[CHANGE]
	}

	return OK;
}

QList<QPoint> Calibrator::findStartPositionInImg_mulitpeImgs(QList<QImage*> imgsToFind, QImage& imgToShareWithin) {
	// this fun return starting points from imgToSharePoints than consist pixels from one of imgsToFind

	QImage::Format formatBig = imgToShareWithin.format();
	for each (QImage * var in imgsToFind) {
		if (formatBig != var->format()) {
			Logger::logPotenialBug("Not all imgs have the same format!","Calibrator","findStartPositionInImg_mulitpeImgs");
			return QList<QPoint>();//[TO DO zamienic ta funkcje zeby zwracala error code]
		}
	}

	const int WIDTH_OF_FIRST_IMG = imgsToFind[0]->width();
	const int HEIGHT_OF_FIRST_IMG = imgsToFind[0]->height();
	for each (QImage * var in imgsToFind) {
		bool widthIsGood = var->width() == WIDTH_OF_FIRST_IMG;
		bool heightIsGood = var->height() == HEIGHT_OF_FIRST_IMG;
		bool isAllRight = widthIsGood && heightIsGood;
		if (!isAllRight) {
			Logger::logPotenialBug("Not all imgs have the same size!", "Calibrator", "findStartPositionInImg_mulitpeImgs");
			return QList<QPoint>();//[TO DO zamienic ta funkcje zeby zwracala error code]
		}
	}

	const int MAX_WIDTH_VAL_TO_SHEARCH = imgToShareWithin.width() - WIDTH_OF_FIRST_IMG;
	const int MAX_HEIGHT_VAL_TO_SHEARCH = imgToShareWithin.height() - HEIGHT_OF_FIRST_IMG;

	QList<QPoint> startPointsListToRet;
	for (int x = 0; x <= MAX_WIDTH_VAL_TO_SHEARCH; x++) {
		for (int y = 0; y <= MAX_HEIGHT_VAL_TO_SHEARCH; y++) {
			bool atLeastOnePixIsMatched = false;
			uint pixFromImg_big = imgToShareWithin.pixel(x, y);

			for each (QImage * var in imgsToFind) {
				uint pixFromImg_small = var->pixel(0, 0);
				if (pixFromImg_big == pixFromImg_small) {
					atLeastOnePixIsMatched = true;
					break;
				}
			}

			if (atLeastOnePixIsMatched) {
				//first pix matched, looking for more
				int wrongPixels = WIDTH_OF_FIRST_IMG * HEIGHT_OF_FIRST_IMG;
				for (int x_TMP = 0; x_TMP < WIDTH_OF_FIRST_IMG; x_TMP++) {
					for (int y_TMP = 0; y_TMP < HEIGHT_OF_FIRST_IMG; y_TMP++) {
						uint pixFromImg_big2 = imgToShareWithin.pixel(x + x_TMP, y + y_TMP);
						bool metReq = false;
						for each (QImage * var in imgsToFind) {
							uint pixFromImg_small2 = var->pixel(x_TMP, y_TMP);
							bool pixMatched = pixFromImg_big2 == pixFromImg_small2;
							if (pixMatched) {
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

bool Calibrator::findPotionsOnBottomBar(QStringList namesOfPotionsToFind, QStringList& namesOfPotionosFound, QList<QRect>& rectsWithFoundPots, QImage& bottomBarImg) {
	bool wrongInput = namesOfPotionsToFind.size() == 0 || bottomBarImg.width() <= 0 || bottomBarImg.height() <= 0;
	if (wrongInput) {
		Logger::logPotenialBug("Wrong(empty) input", "Calibrator", "findPotionsOnBottomBar");
		return false;
	}

	QMap<QString, QString> map_light, map_dark;
	getMapWithPotionsImgCodes(map_light, map_dark);

	QStringList allPotionsNames = map_light.keys();
	for each (QString var in allPotionsNames) {
		bool shouldBeRemoved = !namesOfPotionsToFind.contains(var);
		if (shouldBeRemoved) {
			map_light.remove(var);
			map_dark.remove(var);
		}
	}

	QList<QImage> potionsImg_light, potionsImg_dark;
	for each (QString potName in map_light.keys()) {
		QString code_light = map_light[potName];
		QString code_dark = map_dark[potName];
		QImage imgToAdd;
		imgToAdd = ImgEditor::getImageFromAdvancedCode(code_light);
		potionsImg_light.push_back(imgToAdd);
		imgToAdd = ImgEditor::getImageFromAdvancedCode(code_dark);
		potionsImg_dark.push_back(imgToAdd);
	}

	QStringList namesOfFoundPotions;
	QList<QPoint> potionsOfStartingPotions;
	for (size_t i = 0; i < map_dark.size(); i++) {
		QList<QImage*> potionToLookFor;
		potionToLookFor.push_back(&potionsImg_light[i]);
		potionToLookFor.push_back(&potionsImg_dark[i]);
		QList<QPoint> pointsOfStart = findStartPositionInImg_mulitpeImgs(potionToLookFor, bottomBarImg);
		if (pointsOfStart.size() == 1) {
			potionsOfStartingPotions.push_back(pointsOfStart.first());
			namesOfFoundPotions.push_back(map_dark.keys()[i]);
		}
		else if(pointsOfStart.size() > 1)
			Logger::logPotenialBug("More than one occurance of Potion in Image", "Calibrator", "findPotionsOnBottomBar");
	}

	const int WIDTH_HEIGHT_PIC = 32;
	const RGBstruct LEFT_BOTTOM_COLOUR(66, 66, 66);
	const RGBstruct PIX_ABOVE_COLOUR(112, 113, 113);
	const int WIDTH = bottomBarImg.width();
	const int HEIGHT = bottomBarImg.height();
	QList<QRect> rectToRet;
	for (size_t i = 0; i < potionsOfStartingPotions.size(); i++) {
		QPoint& someWhereInPic = potionsOfStartingPotions[i];
		int curr_x = someWhereInPic.x();
		int curr_y = someWhereInPic.y();

		int min_x = max(curr_x - WIDTH_HEIGHT_PIC, 0);
		int min_y = curr_y;
		int max_x = min(curr_x + WIDTH_HEIGHT_PIC, WIDTH);
		int max_y = min(curr_y + WIDTH_HEIGHT_PIC, HEIGHT);

		for (size_t x = min_x; x < max_x; x++) {
			for (size_t y = min_y; y < max_y; y++) {
				RGBstruct rgb(bottomBarImg.pixel(x, y));
				RGBstruct pixAboveRgb(bottomBarImg.pixel(x, y - 1));
				bool goodPix1 = rgb == LEFT_BOTTOM_COLOUR;
				bool goodPix2 = pixAboveRgb == PIX_ABOVE_COLOUR;
				if (goodPix1 && goodPix2) {
					someWhereInPic.setX(x);
					someWhereInPic.setY(y);
					x = max_x;//break
					y = max_y;//break
				}
			}
		}

		int width = WIDTH_HEIGHT_PIC;
		int height = WIDTH_HEIGHT_PIC / 3;
		int start_x = someWhereInPic.x() + 1;
		int start_y = someWhereInPic.y() - WIDTH_HEIGHT_PIC + 2 * height + 1;

		bool err = start_x < 0 || (start_x + width) > WIDTH || start_y < 0 || (start_y + height) > HEIGHT;
		if(err)
			Logger::logPotenialBug("Out of range cordinates of rect to cut, only part of it will be cut", "Calibrator", "findPotionsOnBottomBar");

		QRect toAdd(start_x, start_y, width, height);
		rectToRet.push_back(toAdd);
	}

	namesOfPotionosFound = namesOfFoundPotions;
	rectsWithFoundPots = rectToRet;

	return true;
}

QList<QPoint> Calibrator::findStartPositionInImg(const QImage& imgToFind, const QImage& imgToSearchWithin) {
	//fun return list of start positions of imgToFind, position is lef down corner
	const int WIDTH_SMALL_PIC = imgToFind.width();
	const int HEIGHT_SMALL_PIC = imgToFind.height();
	const int WIDTH_BIG_PIC = imgToSearchWithin.width();
	const int HEIGHT_BIG_PIC = imgToSearchWithin.height();

	bool errEmpty = WIDTH_SMALL_PIC <= 0 || HEIGHT_SMALL_PIC <= 0 || WIDTH_BIG_PIC <= 0 || HEIGHT_BIG_PIC <= 0;
	bool errWidth = WIDTH_SMALL_PIC >= WIDTH_BIG_PIC;
	bool errHeight = HEIGHT_SMALL_PIC >= HEIGHT_BIG_PIC;
	bool errFormat = imgToFind.format() != imgToSearchWithin.format();
	bool anyErr = errWidth || errHeight || errFormat || errEmpty;
	if (anyErr) {
		Logger::logPotenialBug("Wrong input of two img", "Calibrator", "findStartPositionInImg");
		return QList<QPoint>();
	}

	const int MAX_X_INDEX_TO_CHECK = WIDTH_BIG_PIC - WIDTH_SMALL_PIC;
	const int MAX_Y_INDEX_TO_CHECK = HEIGHT_BIG_PIC - HEIGHT_SMALL_PIC;

	QList<QPoint> startPointsListToRet;
	for (int x = 0; x <= MAX_X_INDEX_TO_CHECK; x++) {
		for (int y = 0; y <= MAX_Y_INDEX_TO_CHECK; y++) {
			uint pixSmallImg = imgToFind.pixel(0, 0);
			uint pixBigImg = imgToSearchWithin.pixel(x, y);
			bool firstPixelMatched = pixSmallImg == pixBigImg;
			if (firstPixelMatched) {
				bool foundPosition = true;
				for (int x_TMP = 1; x_TMP < WIDTH_SMALL_PIC; x_TMP++) {
					for (int y_TMP = 1; y_TMP < HEIGHT_SMALL_PIC; y_TMP++) {
						pixSmallImg = imgToFind.pixel(x_TMP, y_TMP);
						pixBigImg = imgToSearchWithin.pixel(x + x_TMP, y + y_TMP);
						bool pixNotMatched = pixSmallImg != pixBigImg;
						if (pixNotMatched) {
							x_TMP = WIDTH_SMALL_PIC;
							y_TMP = HEIGHT_SMALL_PIC;
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

int Calibrator::calibrateManaAndHealthBar(){
	QList<QRect> importantRectangles;
	bool sucessfullyRead = getRectsFromProfile(importantRectangles);
	bool readValuesAreWrong = false;

	if(sucessfullyRead) {
		ERR res = (ERR)categorizeWindows(*fullScreen, importantRectangles);
		if (res == OK)
			return OK;
		else
			readValuesAreWrong = true;
	}
	bool scan = !sucessfullyRead || readValuesAreWrong;
	if(scan){
		ERR res = (ERR)findWindowsOnScreen(*this->fullScreen, importantRectangles);
		if (res == OK) {
			ERR res2 = (ERR)categorizeWindows(*fullScreen, importantRectangles);
			ERR toRet = res2 == OK ? OK : res;
			return toRet;
		}
	}
	return UNDEFINED_ERR;
}

int Calibrator::findIndexesOfRectangleThatContainsSlashes(QImage& fullScreen, QList<QRect> importantFrames, QList<int>& indexesOfFramesWithSlashesVert, QList<int>& indexesOfFramesWithSlashesHor, int& indexOfFrameCombined) {
	QImage vertSlashes = ImgEditor::fromCharToImg(QChar(47));
	QImage horSlashes = ImgEditor::fromCharToImg(QChar(92));
	QList<int> indexesVert, indexesHor;
	indexOfFrameCombined = -1;
	for (size_t i = 0; i < importantFrames.size(); i++) {
		QImage imgTmp = fullScreen.copy(importantFrames[i]);
		ImgEditor::imgToBlackAndWhiteAllColors(imgTmp, 250);

		QList<QPoint> pointsVert = findStartPositionInImg(vertSlashes, imgTmp);
		if (pointsVert.size() == 1)
			indexesVert.push_back(i);
		else if (pointsVert.size() == 2) {
			indexOfFrameCombined = i;
			indexesVert.push_back(i);
		}

		QList<QPoint> pointsHor = findStartPositionInImg(horSlashes, imgTmp);
		if (pointsHor.size() == 1)
			indexesHor.push_back(i);
		else if (pointsHor.size() == 2) {
			indexOfFrameCombined = i;
			indexesHor.push_back(i);
		}
	}
	bool noSlashesFound = indexesVert.size() == 0 && indexesHor.size() == 0;
	if (noSlashesFound)
		return NO_SLASHES_FOUND_IN_GAME_SCREEN;
	else {
		indexesOfFramesWithSlashesHor = indexesHor;
		indexesOfFramesWithSlashesVert = indexesVert;
		return OK;
	}
}

void Calibrator::sortByXAndYPoints(QList<QPoint>& points, QList<QPoint>& pointsSortedByX, QList<QPoint>& pointsSortedByY) {
	QMap<int, int> sortedByX; //x is key
	QMap<int, int> sortedByY; //y is key
	QList<QPoint> copyOfInput = points;
	for each (QPoint var in copyOfInput) {
		sortedByX.insert(var.x(), var.y());
		sortedByY.insert(var.y(), var.x());//inverted order of XY
	}
	QList<QPoint>sortedByXToRet;
	QList<QPoint>sortedByYToRet;
	for (auto mapEle : sortedByX.keys()) {
		int x = mapEle;
		int y = sortedByX.value(mapEle);
		sortedByXToRet.push_back(QPoint(x, y));
	}
	for (auto mapEle : sortedByY.keys()) {
		int y = mapEle;
		int x = sortedByY.value(mapEle);
		sortedByYToRet.push_back(QPoint(x, y));
	}

	//it's possibility that two points in vector have diffrent one of cordinates but
	//second cordinate differ by few pixels, next part of fun is designed to
	//make set for both cordinates the same value
	bool fixPoints = true; //turn on/off that fixing
	if (fixPoints) {
		bool needToReWriteLists = false;
		const int MAX_DIFFRENCE_IN_PIXELS = 4;
		for (int i = 1; i < sortedByXToRet.size(); i++) {
			bool areTheSame = sortedByXToRet[i].x() - sortedByXToRet[i - 1].x() <= MAX_DIFFRENCE_IN_PIXELS;
			if (areTheSame) {
				needToReWriteLists = true;
				int toSet = sortedByXToRet[i - 1].x();
				sortedByXToRet[i].setX(toSet);
				//making the same change in second container
				int yValueOfmodifiedPoint = sortedByXToRet[i].y();
				for (QPoint& point : sortedByYToRet) {
					if (point.y() == yValueOfmodifiedPoint)
						point.setX(toSet);
				}
			}
		}
		for (int i = 1; i < sortedByYToRet.size(); i++) {
			bool areTheSame = sortedByYToRet[i].y() - sortedByYToRet[i - 1].y() <= MAX_DIFFRENCE_IN_PIXELS;
			if (areTheSame) {
				needToReWriteLists = true;
				int toSet = sortedByYToRet[i - 1].y();
				sortedByYToRet[i].setY(toSet);
				int xValueOfmodifiedPoint = sortedByYToRet[i].x();
				for (QPoint& point : sortedByXToRet) {
					if (point.x() == xValueOfmodifiedPoint)
						point.setY(toSet);
				}
			}
		}
		if (needToReWriteLists) {
			//ReWriting begining of fun to eliminate duplicates
			QMap<int, int> sortedByX_TMP; //x is key
			QMap<int, int> sortedByY_TMP; //y is key
			QList<QPoint> copyOfInput = points;

			for each (QPoint var in sortedByXToRet)
				sortedByX_TMP.insert(var.x(), var.y());

			for each (QPoint var in sortedByYToRet)
				sortedByY_TMP.insert(var.y(), var.x());//inverted order

			QList<QPoint>sortedByXToRet_TMP;
			QList<QPoint>sortedByYToRet_TMP;
			for (auto mapEle : sortedByX_TMP.keys()) {
				int x = mapEle;
				int y = sortedByX.value(mapEle);
				sortedByXToRet_TMP.push_back(QPoint(x, y));
			}
			for (auto mapEle : sortedByY_TMP.keys()) {
				int y = mapEle;
				int x = sortedByY.value(mapEle);
				sortedByYToRet_TMP.push_back(QPoint(x, y));
			}
			pointsSortedByX = sortedByXToRet_TMP;
			pointsSortedByY = sortedByYToRet_TMP;
			return;
		}//end of rewriting
	}//end of slightly correcting

	pointsSortedByX = sortedByXToRet;
	pointsSortedByY = sortedByYToRet;
}

int Calibrator::sortByXAndYRects(QList<QRect>& inputRects, QList<QRect>& rectsSortedByPosX, QList<QRect>& rectsSortedByPosY) {
	if (inputRects.size() == 0)
		return UNDEFINED_ERR;

	QList<QRect> sortedByX, sortedByY;
	sortedByX.push_back(inputRects[0]);
	sortedByY.push_back(inputRects[0]);

	for (int i = 1; i < inputRects.size(); i++) {
		QRect rect = inputRects[i];
		int startPosX = rect.x();
		int startPosY = rect.y();

		int indexToInsertNewElement_X = -1;
		int indexToInsertNewElement_Y = -1;

		int size = sortedByX.size();
		for (int i = 0; i < size; i++) {
			if (startPosX <= sortedByX[i].x()) {
				indexToInsertNewElement_X = i;
				break;
			}
		}
		for (int i = 0; i < size; i++) {
			if (startPosY <= sortedByY[i].y()) {
				indexToInsertNewElement_Y = i;
				break;
			}
		}
		if (indexToInsertNewElement_X == -1)
			indexToInsertNewElement_X = size;
		if (indexToInsertNewElement_Y == -1)
			indexToInsertNewElement_Y = size;
		sortedByX.insert(indexToInsertNewElement_X, rect);
		sortedByY.insert(indexToInsertNewElement_Y, rect);
	}
	rectsSortedByPosX = sortedByX;
	rectsSortedByPosY = sortedByY;
	return OK;
}

int Calibrator::setPositionHealthImgs(QImage& fullscreen, QList<QRect> listOfImportantRectangles, int& indexOfHealth, int& indexOfMana, int& indexOfManaShield, int& indexOfManaAndManaShieldCombined, int& howTheyShouldBeRotated) {
	QList<int> indexesOfRectWithSlashVert;
	QList<int> indexesOfRectWithSlashHor;
	int indexOfFrameWithManaShieldAndMana;
	findIndexesOfRectangleThatContainsSlashes(fullscreen, listOfImportantRectangles, indexesOfRectWithSlashVert, indexesOfRectWithSlashHor, indexOfFrameWithManaShieldAndMana);
	
	{
		enum positionsOfInterface { TOP, RIGHT, DOWN, LEFT };
		positionsOfInterface position;
		QPoint midOfScreen(fullscreen.width() / 2, fullscreen.height() / 2);

		if (indexesOfRectWithSlashVert.size() != 0) {
			QRect first = listOfImportantRectangles[indexesOfRectWithSlashVert[0]];
			position = first.x() < midOfScreen.x() ? LEFT : RIGHT;
		}
		else if (indexesOfRectWithSlashHor.size() != 0) {
			QRect first = listOfImportantRectangles[indexesOfRectWithSlashHor[0]];
			position = first.y() < midOfScreen.y() ? TOP : DOWN;
		}
		else
			return NO_SLASHES_FOUND_IN_GAME_SCREEN;


		bool combined = indexOfFrameWithManaShieldAndMana != -1;
		switch (position) {
		case TOP: case DOWN: {
			int size = indexesOfRectWithSlashHor.size();
			howTheyShouldBeRotated = 0;
			QList<QRect> sortedByX, sortedByY, rectangles;
			for each (int var in indexesOfRectWithSlashHor)
				rectangles.push_back(listOfImportantRectangles[var]);
			sortByXAndYRects(rectangles, sortedByX, sortedByY);

			if (combined && size == 2) {
				bool isParallelStyle = sortedByY[0].y() != sortedByY[1].y();
				if (isParallelStyle) {
					indexOfHealth = listOfImportantRectangles.indexOf(sortedByY[0]);
					indexOfMana = -1;
					indexOfManaShield = -1;
					indexOfManaAndManaShieldCombined = listOfImportantRectangles.indexOf(sortedByY[1]);
				}
				else {
					indexOfHealth = listOfImportantRectangles.indexOf(sortedByX[0]);
					indexOfMana = -1;
					indexOfManaShield = -1;
					indexOfManaAndManaShieldCombined = listOfImportantRectangles.indexOf(sortedByX[1]);
				}
			}
			else if (!combined && size == 3) {
				indexOfHealth = listOfImportantRectangles.indexOf(sortedByX[0]);
				rectangles.removeOne(sortedByX[0]);
				sortByXAndYRects(rectangles, sortedByX, sortedByY);
				indexOfMana = listOfImportantRectangles.indexOf(sortedByY[0]);
				indexOfManaShield = listOfImportantRectangles.indexOf(sortedByY[1]);
				indexOfManaAndManaShieldCombined = -1;
			}
			else if (!combined && size == 2) {
				indexOfManaAndManaShieldCombined = -1;
				indexOfManaShield = -1;
				bool isParallelStyle = sortedByY[0].y() == sortedByY[1].y();
				if (isParallelStyle) {
					indexOfHealth = listOfImportantRectangles.indexOf(sortedByX[0]);
					indexOfMana = listOfImportantRectangles.indexOf(sortedByX[1]);
				}
				else {
					indexOfHealth = listOfImportantRectangles.indexOf(sortedByY[0]);
					indexOfMana = listOfImportantRectangles.indexOf(sortedByY[1]);
				}
			}
			else {
				return ERROR_IN_SETTING_POSITION_OF_INTERFACE;
			}

			for each (QRect var in sortedByX)
				listOfImportantRectangles.removeOne(var);

			break;
		};
		case LEFT: {
			int size = indexesOfRectWithSlashVert.size();
			howTheyShouldBeRotated = 1;//  1*90degree to right
			QList<QRect> sortedByX, sortedByY, rectangles;
			for each (int var in indexesOfRectWithSlashVert)
				rectangles.push_back(listOfImportantRectangles[var]);
			sortByXAndYRects(rectangles, sortedByX, sortedByY);

			if (size == 2 && !combined) {
				//no mana shield
				int w = listOfImportantRectangles[indexesOfRectWithSlashVert[0]].width();
				//25 is more less width of big bar, smaller is half of its width
				bool isParallelStyle = sortedByX[0].x() != sortedByX[1].x();
				if (isParallelStyle) {
					indexOfHealth = listOfImportantRectangles.indexOf(sortedByX[0]);
					indexOfMana = listOfImportantRectangles.indexOf(sortedByX[1]);
					indexOfManaShield = -1;
					indexOfManaAndManaShieldCombined = -1;
				}
				else {
					indexOfHealth = listOfImportantRectangles.indexOf(sortedByY[0]);
					indexOfMana = listOfImportantRectangles.indexOf(sortedByY[1]);
					indexOfManaShield = -1;
					indexOfManaAndManaShieldCombined = -1;
				}
			}
			else if (size == 2 && combined) {//mana shield, DEFAULT, PARALLEL AND COMPACT STYLE
				indexOfMana = -1;
				indexOfManaShield = -1;
				indexOfManaAndManaShieldCombined = indexOfFrameWithManaShieldAndMana;
				indexesOfRectWithSlashVert.removeOne(indexOfFrameWithManaShieldAndMana);
				indexOfHealth = indexesOfRectWithSlashVert[0];
			}
			else if (size == 3 && !combined) {//mana shields, LARGE
				indexOfHealth = listOfImportantRectangles.indexOf(sortedByX[1]);
				indexOfMana = listOfImportantRectangles.indexOf(sortedByX[0]);
				indexOfManaShield = listOfImportantRectangles.indexOf(sortedByX[2]);
				indexOfManaAndManaShieldCombined = -1;
			}
			else
				return ERROR_IN_SETTING_POSITION_OF_INTERFACE;

			for each (QRect var in sortedByX)
				listOfImportantRectangles.removeOne(var);

			break;
		};
		case RIGHT: {
			int size = indexesOfRectWithSlashVert.size();
			howTheyShouldBeRotated = -1;//  -1*90degree to right
			QList<QRect> sortedByX, sortedByY, rectangles;
			for each (int var in indexesOfRectWithSlashVert)
				rectangles.push_back(listOfImportantRectangles[var]);

			sortByXAndYRects(rectangles, sortedByX, sortedByY);
			if (size == 2 && !combined) {//no mana shield
				bool isParallelStyle = sortedByY[0].y() == sortedByY[1].y();

				if (isParallelStyle) {
					indexOfHealth = listOfImportantRectangles.indexOf(sortedByX[0]);
					indexOfMana = listOfImportantRectangles.indexOf(sortedByX[1]);
					indexOfManaShield = -1;
					indexOfManaAndManaShieldCombined = -1;
				}
				else {
					indexOfHealth = listOfImportantRectangles.indexOf(sortedByY[0]);
					indexOfMana = listOfImportantRectangles.indexOf(sortedByY[1]);
					indexOfManaShield = -1;
					indexOfManaAndManaShieldCombined = -1;
				}
			}
			else if (size == 2 && combined) {//mana shield, DEFAULT, PARALLEL AND COMPACT STYLE
				indexOfMana = -1;
				indexOfManaShield = -1;
				indexOfManaAndManaShieldCombined = indexOfFrameWithManaShieldAndMana;
				indexesOfRectWithSlashVert.removeOne(indexOfFrameWithManaShieldAndMana);
				indexOfHealth = indexesOfRectWithSlashVert[0];
			}
			else if (size == 3 && !combined) {//mana shields, LARGE
				indexOfHealth = listOfImportantRectangles.indexOf(sortedByX[1]);
				indexOfManaAndManaShieldCombined = -1;
				indexOfMana = listOfImportantRectangles.indexOf(sortedByX[2]);
				indexOfManaShield = listOfImportantRectangles.indexOf(sortedByX[0]);
			}
			else {
				return ERROR_IN_SETTING_POSITION_OF_INTERFACE;
			}

			for each (QRect var in sortedByX)
				listOfImportantRectangles.removeOne(var);

			break;
		};
		default: {
			return ERROR_IN_SETTING_POSITION_OF_INTERFACE;
			break;
		}
		}
	}

	return OK;
}

int Calibrator::categorizeWindows(QImage& fullscreen, QList<QRect>& importantRectangles) {
	//4 cause 1-minimap 2-gameScreen 3-health 4-mana, those 4 have to be found
	if (importantRectangles.size() < 4)
		return NO_ENOUGH_FRAMES_FOUND;

	//game Frame
	{
		int indexOfBiggestWindow = 0;
		int biggestSurface = 0;
		for (size_t i = 0; i < importantRectangles.size(); i++){
			QRect rect = importantRectangles[i];
			int surf = rect.width() * rect.height();
			if (surf > biggestSurface) {
				biggestSurface = surf;
				indexOfBiggestWindow = i;
			}
		}
		profile->frames.gameFrame = importantRectangles[indexOfBiggestWindow];
		importantRectangles.removeAt(indexOfBiggestWindow);
	}

	//indexOfHealth, indexOfMana, indexOfManaShield, howTheyShouldBeRotated, indexOfCombinedBox;
	{
		int indexOfHealth, indexOfMana, indexOfManaShield, howTheyShouldBeRotated, indexOfCombinedBox;
		setPositionHealthImgs(fullscreen, importantRectangles, indexOfHealth, indexOfMana, indexOfManaShield, indexOfCombinedBox, howTheyShouldBeRotated);
		profile->frames.howTheyShouldBeRotated = howTheyShouldBeRotated;

		int size = importantRectangles.size();
		bool healthFound = indexOfHealth != -1 && indexOfHealth >= 0 && indexOfHealth <= size;
		bool manaFound = indexOfMana != -1 && indexOfMana >= 0 && indexOfMana <= size;
		bool manaShieldFound = indexOfManaShield != -1 && indexOfManaShield >= 0 && indexOfManaShield <= size;
		bool combinedBoxFound = indexOfCombinedBox != -1 && indexOfCombinedBox >= 0 && indexOfCombinedBox <= size;

		bool enoughFramesFound = healthFound && (manaFound || combinedBoxFound);
		if (!enoughFramesFound)
			return ERROR_IN_SETTING_POSITION_OF_INTERFACE;

		bool deleteHelath = false, deleteMana = false, deleteManaShield = false, deleteCombined = false;
		if (healthFound) {
			profile->frames.healthFrame = importantRectangles[indexOfHealth];
			deleteHelath = true;
		}

		if (manaFound) {
			profile->frames.manaFrame = importantRectangles[indexOfMana];
			deleteHelath = true;
		}

		if (manaShieldFound) {
			profile->frames.manaShieldFrame = importantRectangles[indexOfManaShield];
			deleteManaShield = true;
		}

		if (combinedBoxFound) {
			profile->frames.combinedFrame = importantRectangles[indexOfCombinedBox];
			deleteCombined = true;
		}
		QList<QRect> rectsToDelete;
		if (deleteHelath) rectsToDelete.push_back(importantRectangles[indexOfHealth]);
		if (deleteMana) rectsToDelete.push_back(importantRectangles[indexOfMana]);
		if (deleteManaShield) rectsToDelete.push_back(importantRectangles[indexOfManaShield]);
		if (deleteCombined) rectsToDelete.push_back(importantRectangles[indexOfCombinedBox]);
		for each (QRect var in rectsToDelete)
			importantRectangles.removeOne(var);
	}

	//miniMap Frame
	{
		QMap<int, QRect> startX_Rect_map;
		for each (QRect var in importantRectangles)
			startX_Rect_map.insert(var.x(), var);
		QRect miniMapRect = startX_Rect_map.last();
		profile->frames.miniMapFrame = miniMapRect;
		importantRectangles.removeOne(miniMapRect);
	}

	return OK;
}

int Calibrator::findWindowsOnScreen(QImage& fullScreen, QList<QRect>& importantRectangles) {
	const int WIDTH = fullScreen.width();
	const int HEIGHT = fullScreen.height();
	const uint BLACK_PIX_COL = qRgb(0, 0, 0);
	const uint MIN_ACCEPTABLE_VAL = 17;
	const uint MAX_ACCEPTABLE_VAL = 29;
	QList<QPoint> startOfFrames;
	for (size_t x = 0; x < WIDTH - 2; x++) {
		for (size_t y = 0; y < HEIGHT - 2; y++) {
			bool isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x, y), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
			if (!isPixelOfFrame) continue;

			isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 1, y + 1), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, false);
			if (isPixelOfFrame) continue;

			isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 1, y + 2), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, false);
			if (isPixelOfFrame) continue;

			isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 2, y + 1), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, false);
			if (isPixelOfFrame) continue;

			isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 2, y + 2), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, false);
			if (isPixelOfFrame) continue;

			isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 1, y), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
			if (!isPixelOfFrame) continue;

			isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x + 2, y), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
			if (!isPixelOfFrame) continue;

			isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x, y + 1), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
			if (!isPixelOfFrame) continue;

			isPixelOfFrame = ImgEditor::isItPixelFromFrame(fullScreen.pixel(x, y + 2), MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
			if (!isPixelOfFrame) continue;

			startOfFrames.push_back(QPoint(x, y));
		}
	}
	//looking for rectangles from start points of frames
	QList<QRect> frameToRet;
	const int MIN_WIDTH = 5, MIN_HEIGHT = 5;
	for each (QPoint startPoint in startOfFrames) {
		int currentWidth = 0;
		for (size_t x = startPoint.x(); x < WIDTH; x++) {
			uint color = fullScreen.pixel(x, startPoint.y());
			bool isPixOfFrame = ImgEditor::isItPixelFromFrame(color, MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
			if (isPixOfFrame)
				currentWidth++;
			else
				break;
		}
		int currentHeight = 0;
		for (size_t y = startPoint.y(); y < HEIGHT; y++) {
			uint color = fullScreen.pixel(startPoint.x(), y);
			bool isPixOfFrame = ImgEditor::isItPixelFromFrame(color, MIN_ACCEPTABLE_VAL, MAX_ACCEPTABLE_VAL, true);
			if (isPixOfFrame)
				currentHeight++;
			else
				break;
		}
		bool accept = currentHeight > MIN_HEIGHT && currentWidth > MIN_WIDTH;
		if (accept) {
			int x = startPoint.x();
			int y = startPoint.y();
			int w = currentWidth;
			int h = currentHeight;
			frameToRet.push_back(QRect(x + 1, y + 1, w - 1, h - 1));
		}
	}

	importantRectangles = frameToRet;

	ERR toRet = frameToRet.size() >= 4 ? OK : NO_FRAMES_FOUND;
	return toRet;
}

void Calibrator::TEST_setPositionHealthImhs(QString pathToFolderWithDiffrentPositionsStylesScreen, QString pathToOutPutFolder) {
	QImage combined, health, mana, manaShield;
	QDir directory(pathToFolderWithDiffrentPositionsStylesScreen);
	QStringList listOfFIles = directory.entryList(QStringList() << "*.png", QDir::Files);
	for (int i = 0; i < listOfFIles.size(); i++) {
		QString nameOfFile = listOfFIles[i];
		qDebug() << QString::number(i);
		QString pathToPng = pathToFolderWithDiffrentPositionsStylesScreen + "\\" + nameOfFile;
		QImage img;
		img.load(pathToPng);
		QList<QRect> importantRects;
		findWindowsOnScreen(img, importantRects);
		int indCombined;
		int indHealth;
		int indMana;
		int indManaShield;
		int howTheyShouldBeRotated;

		setPositionHealthImgs(img, importantRects, indHealth, indMana, indManaShield, indCombined, howTheyShouldBeRotated);

		health = img.copy(importantRects[indHealth]);
		QString healthStr, manaStr, ManaShieldStr, combinedStr;

		ImgEditor::rotateImgToRight(health, howTheyShouldBeRotated);
		ImgEditor::imgToBlackAndWhiteAllColors(health, 240);
		healthStr = ImgEditor::imgWithStrToStr(health);
		qDebug() << "Health: " + healthStr;

		bool thereIsCombinedBox = indCombined != -1;
		bool manaFound = indMana != -1;
		bool manaShieldFound = indManaShield != -1;
		if (thereIsCombinedBox) {
			QRect rect = importantRects.at(indCombined);
			combined = img.copy(rect);

			ImgEditor::rotateImgToRight(combined, howTheyShouldBeRotated);
			ImgEditor::imgToBlackAndWhiteAllColors(combined, 240);
			QString combinedStr = ImgEditor::imgWithStrToStr(combined);
			qDebug() << "combinedStr: " + combinedStr;
		}
		else if (!thereIsCombinedBox && manaShieldFound && manaFound) {
			QRect rect = importantRects.at(indMana);
			mana = img.copy(rect);
			rect = importantRects.at(indManaShield);
			manaShield = img.copy(rect);

			ImgEditor::rotateImgToRight(mana, howTheyShouldBeRotated);
			ImgEditor::imgToBlackAndWhiteAllColors(mana, 240);
			QString manaStr = ImgEditor::imgWithStrToStr(mana);
			qDebug() << "manaStr: " + manaStr;

			ImgEditor::rotateImgToRight(manaShield, howTheyShouldBeRotated);
			ImgEditor::imgToBlackAndWhiteAllColors(manaShield, 240);
			QString ManaShieldStr = ImgEditor::imgWithStrToStr(manaShield);
			qDebug() << "ManaShieldStr: " + ManaShieldStr;
		}
		else if (!thereIsCombinedBox && !manaShieldFound && manaFound) {
			QRect rect = importantRects.at(indMana);
			mana = img.copy(rect);

			ImgEditor::rotateImgToRight(mana, howTheyShouldBeRotated);
			ImgEditor::imgToBlackAndWhiteAllColors(mana, 240);
			QString manaStr = ImgEditor::imgWithStrToStr(mana);
			qDebug() << "manaStr: " + manaStr;
		}
		else
			return;

		//QString name = QDateTime::currentDateTime().toString("mmss_mmm_");
		QString name = "";

		QString h = "Health_";
		QString m = "Mana_";
		QString c = "Combined_";
		QString ms = "ManaShield_";
		QString path = pathToOutPutFolder + "\\_";
		QString finalName_h = path + name + h + nameOfFile;
		QString finalName_m = path + name + m + nameOfFile;
		QString finalName_ms = path + name + ms + nameOfFile;
		QString finalName_c = path + name + c + nameOfFile;
		health.save(finalName_h);
		if (thereIsCombinedBox)
			combined.save(finalName_c);
		else if (!thereIsCombinedBox && manaShieldFound && manaFound) {
			mana.save(finalName_m);
			manaShield.save(finalName_ms);
		}
		else if (!thereIsCombinedBox && !manaShieldFound && manaFound)
			mana.save(finalName_m);
		else
			return;
	}
}

bool Calibrator::getRectsFromProfile(QList<QRect>& importRectsFromProf) {
	importRectsFromProf.clear();

	//they have to be;
	bool isEmpty_gameFram = profile->frames.gameFrame.isEmpty();
	bool isEmpty_healthFrame = profile->frames.healthFrame.isEmpty();
	bool isEmpty_minimapFrame = profile->frames.miniMapFrame.isEmpty();

	if (isEmpty_gameFram || isEmpty_healthFrame || isEmpty_minimapFrame)
		return false;

	//one of them have to be;
	bool isEmpty_manaFrame = profile->frames.manaFrame.isEmpty();
	bool isEmpty_combinedFrame = profile->frames.combinedFrame.isEmpty();

	if (isEmpty_manaFrame && isEmpty_combinedFrame)
		return false;

	//one that can be or not
	bool isEmpty_manaShieldFrame = profile->frames.manaShieldFrame.isEmpty();

	importRectsFromProf.push_back(profile->frames.gameFrame);
	importRectsFromProf.push_back(profile->frames.healthFrame);
	importRectsFromProf.push_back(profile->frames.miniMapFrame);

	if (!isEmpty_manaFrame)
		importRectsFromProf.push_back(profile->frames.manaFrame);
	if (!isEmpty_combinedFrame)
		importRectsFromProf.push_back(profile->frames.combinedFrame);
	if (!isEmpty_manaShieldFrame)
		importRectsFromProf.push_back(profile->frames.manaShieldFrame);

	return true;
}

