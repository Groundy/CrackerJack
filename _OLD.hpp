/*
bool Calibrator::getRectsFromProfile(QList<QRect>& importRectsFromProf) {
        importRectsFromProf.clear();
        //they have to be;
        bool isEmpty_gameFram = var->frames.gameFrame.isEmpty();
        bool isEmpty_healthFrame = var->frames.healthFrame.isEmpty();
        bool isEmpty_minimapFrame = var->frames.miniMapFrame.isEmpty();

        if (isEmpty_gameFram || isEmpty_healthFrame || isEmpty_minimapFrame)
                return false;

        //one of them have to be;
        bool isEmpty_manaFrame = var->frames.manaFrame.isEmpty();
        bool isEmpty_combinedFrame = var->frames.combinedFrame.isEmpty();

        if (isEmpty_manaFrame && isEmpty_combinedFrame)
                return false;

        //one that can be or not
        bool isEmpty_manaShieldFrame = var->frames.manaShieldFrame.isEmpty();

        importRectsFromProf.push_back(var->frames.gameFrame);
        importRectsFromProf.push_back(var->frames.healthFrame);
        importRectsFromProf.push_back(var->frames.miniMapFrame);

        if (!isEmpty_manaFrame)
                importRectsFromProf.push_back(var->frames.manaFrame);
        if (!isEmpty_combinedFrame)
                importRectsFromProf.push_back(var->frames.combinedFrame);
        if (!isEmpty_manaShieldFrame)
                importRectsFromProf.push_back(var->frames.manaShieldFrame);

        return true;
}
*/
/*
void Calibrator::TEST_setPositionHealthImhs(QString
pathToFolderWithDiffrentPositionsStylesScreen, QString pathToOutPutFolder) {
        QImage combined, health, mana, manaShield;
        QDir directory(pathToFolderWithDiffrentPositionsStylesScreen);
        QStringList listOfFIles = directory.entryList(QStringList() << "*.png",
QDir::Files); for (int i = 0; i < listOfFIles.size(); i++) { QString nameOfFile
= listOfFIles[i]; qDebug() << QString::number(i); QString pathToPng =
pathToFolderWithDiffrentPositionsStylesScreen + "\\" + nameOfFile; QImage img;
                img.load(pathToPng);
                QList<QRect> importantRects;
                findWindowsOnScreen(img, importantRects);
                int indCombined;
                int indHealth;
                int indMana;
                int indManaShield;
                int howTheyShouldBeRotated;

                setPositionHealthImgs(img, importantRects, indHealth, indMana,
indManaShield, indCombined, howTheyShouldBeRotated);

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

                        ImgEditor::rotateImgToRight(combined,
howTheyShouldBeRotated); ImgEditor::imgToBlackAndWhiteAllColors(combined, 240);
                        QString combinedStr =
ImgEditor::imgWithStrToStr(combined); qDebug() << "combinedStr: " + combinedStr;
                }
                else if (!thereIsCombinedBox && manaShieldFound && manaFound) {
                        QRect rect = importantRects.at(indMana);
                        mana = img.copy(rect);
                        rect = importantRects.at(indManaShield);
                        manaShield = img.copy(rect);

                        ImgEditor::rotateImgToRight(mana,
howTheyShouldBeRotated); ImgEditor::imgToBlackAndWhiteAllColors(mana, 240);
                        QString manaStr = ImgEditor::imgWithStrToStr(mana);
                        qDebug() << "manaStr: " + manaStr;

                        ImgEditor::rotateImgToRight(manaShield,
howTheyShouldBeRotated); ImgEditor::imgToBlackAndWhiteAllColors(manaShield,
240); QString ManaShieldStr = ImgEditor::imgWithStrToStr(manaShield); qDebug()
<< "ManaShieldStr: " + ManaShieldStr;
                }
                else if (!thereIsCombinedBox && !manaShieldFound && manaFound) {
                        QRect rect = importantRects.at(indMana);
                        mana = img.copy(rect);

                        ImgEditor::rotateImgToRight(mana,
howTheyShouldBeRotated); ImgEditor::imgToBlackAndWhiteAllColors(mana, 240);
                        QString manaStr = ImgEditor::imgWithStrToStr(mana);
                        qDebug() << "manaStr: " + manaStr;
                }
                else
                        return;

                //QString name =
QDateTime::currentDateTime().toString("mmss_mmm_"); QString name = "";

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
*/
/*void Calibrator::getMapWithPotionsImgCodes(QMap<QString, QString>&
name_code_light, QMap<QString, QString>& name_code_dark) {
        name_code_light.clear();
        name_code_light.insert("Great Health Potion",
"17_2_#139#7#0_#50#3#0_#170#6#0_#109#6#0_#199#2#0_#205#4#0_#228#2#0_#222#2#0_#247#33#39_#242#2#0_#243#105#129_#244#33#39_#236#86#105_#219#58#66_#157#8#0_#163#38#39_#183#5#0_#160#5#0_#171#5#0_#199#6#0_#144#7#0_#160#6#0_#146#7#0_#124#9#0_#147#8#0_#137#7#0_#150#3#0_#142#7#0_#146#4#0_#147#4#0_#117#3#0_#106#4#0_#117#4#0_#101#4#0_");
        name_code_light.insert("Great Mana Potion",
"18_2_#136#46#100_#46#12#35_#158#53#118_#110#37#82_#175#69#125_#180#67#130_#184#68#134_#168#61#123_#227#123#174_#184#67#132_#213#113#165_#199#93#151_#148#44#112_#163#53#122_#136#40#103_#131#40#99_#123#27#100_#115#30#91_#131#40#99_#147#40#114_#137#44#103_#135#40#103_#143#48#106_#138#46#102_#140#44#105_#131#41#98_#131#48#96_#121#32#95_#90#18#72_#128#44#95_#59#5#51_#58#5#49_#63#5#54_#55#5#47_#18#2#15_#13#1#11_");
        name_code_light.insert("Great Spirit Potion",
"4_2_#168#115#59_#191#146#89_#151#92#34_#188#143#89_#128#68#0_#119#63#0_#121#58#0_#112#59#0_");
        name_code_light.insert("Health Potion",
"9_2_#201#27#21_#177#35#0_#248#68#117_#194#4#13_#252#75#114_#220#17#74_#208#10#38_#195#7#55_#151#0#1_#158#0#10_#126#16#0_#130#9#0_#131#19#0_#134#16#0_#93#13#0_#92#17#0_#83#14#0_#98#16#0_");
        name_code_light.insert("Mana Potion",
"9_2_#201#21#194_#177#0#142_#199#68#248_#184#4#194_#213#75#252_#162#17#220_#179#10#208_#147#7#195_#149#0#151_#148#0#158_#126#0#110_#130#0#120_#131#0#112_#134#0#118_#93#0#80_#92#0#75_#83#0#69_#98#0#82_");
        name_code_light.insert("Strong Health Potion",
"13_2_#197#0#10_#177#38#0_#238#0#66_#194#0#7_#254#0#28_#229#0#46_#231#0#16_#234#0#17_#206#2#1_#187#0#2_#228#1#3_#170#8#1_#215#4#2_#136#11#0_#148#10#0_#165#0#21_#143#10#0_#143#10#0_#143#10#0_#136#11#0_#136#11#0_#112#15#0_#92#18#0_#92#18#0_#92#18#0_#92#18#0_");
        name_code_light.insert("Strong Mana Potion",
"13_2_#183#0#126_#177#0#77_#213#0#246_#194#0#133_#221#0#250_#218#0#209_#206#0#167_#193#0#176_#153#0#96_#158#0#106_#143#0#82_#139#0#79_#145#0#80_#136#0#78_#136#0#78_#165#0#128_#143#0#83_#143#0#83_#143#0#83_#136#0#78_#136#0#78_#112#0#58_#92#0#42_#92#0#42_#92#0#42_#92#0#42_");
        name_code_light.insert("Supreme Health Potion",
"3_3_#212#2#0_#221#33#39_#53#43#40_#211#2#0_#174#6#0_#157#8#0_#48#44#44_#48#44#44_#48#44#44_");
        name_code_light.insert("Ultimate Health Potion",
"3_3_#221#33#39_#236#86#105_#219#58#66_#174#6#0_#157#8#0_#163#38#39_#48#44#44_#48#44#44_#160#5#0_");
        name_code_light.insert("Ultimate Mana Potion",
"4_2_#213#113#165_#199#93#151_#148#44#112_#163#53#122_#136#40#103_#131#40#99_#48#44#44_#115#30#91_");
        name_code_light.insert("Ultimate Spirit Potion",
"4_2_#168#115#59_#191#146#89_#151#92#34_#188#143#89_#128#68#0_#119#63#0_#48#44#44_#112#59#0_");

        name_code_dark.clear();
        name_code_dark.insert("Great Mana Potion",
"18_2_#41#14#30_#14#4#10_#47#16#35_#33#11#24_#52#21#37_#54#20#39_#55#20#40_#50#18#37_#68#37#52_#55#20#39_#63#34#49_#59#28#45_#44#13#33_#49#16#36_#41#12#31_#39#12#30_#37#8#30_#34#9#27_#39#12#30_#44#12#34_#41#13#31_#40#12#31_#43#14#32_#41#14#30_#42#13#31_#39#12#29_#39#14#29_#36#10#28_#27#5#21_#38#13#28_#18#1#15_#17#1#15_#19#1#16_#16#1#14_#5#1#4_#4#0#3_");
        name_code_dark.insert("Great Spirit Potion",
"4_2_#50#34#18_#57#44#27_#45#27#10_#56#43#27_#38#20#0_#35#19#0_#36#17#0_#33#18#0_");
        name_code_dark.insert("Health Potion",
"9_2_#60#8#6_#53#10#0_#74#20#35_#58#1#4_#75#22#34_#66#5#22_#62#3#11_#58#2#16_#45#0#0_#47#0#3_#38#5#0_#39#3#0_#39#6#0_#40#5#0_#28#4#0_#27#5#0_#25#4#0_#29#5#0_");
        name_code_dark.insert("Mana Potion",
"9_2_#60#6#58_#53#0#42_#59#20#74_#55#1#58_#63#22#75_#48#5#66_#53#3#62_#44#2#58_#44#0#45_#44#0#47_#38#0#33_#39#0#36_#39#0#33_#40#0#35_#28#0#24_#27#0#22_#25#0#21_#29#0#24_");
        name_code_dark.insert("Strong Health Potion",
"13_2_#59#0#3_#53#11#0_#71#0#20_#58#0#2_#76#0#8_#68#0#14_#69#0#5_#70#0#5_#61#1#0_#56#0#1_#68#0#1_#51#2#0_#64#1#1_#41#3#0_#44#3#0_#49#0#6_#43#3#0_#43#3#0_#43#3#0_#41#3#0_#41#3#0_#33#4#0_#27#5#0_#27#5#0_#27#5#0_#27#5#0_");
        name_code_dark.insert("Strong Mana Potion",
"13_2_#55#0#38_#53#0#23_#63#0#73_#58#0#40_#66#0#75_#65#0#62_#61#0#50_#58#0#52_#46#0#29_#47#0#32_#43#0#24_#41#0#24_#43#0#24_#41#0#23_#41#0#23_#49#0#38_#43#0#25_#43#0#25_#43#0#25_#41#0#23_#41#0#23_#33#0#17_#27#0#13_#27#0#13_#27#0#13_#27#0#13_");
        name_code_dark.insert("Supreme Health Potion",
"3_3_#63#1#0_#66#10#12_#16#13#12_#63#1#0_#52#2#0_#47#2#0_#14#13#13_#14#13#13_#14#13#13_");
        name_code_dark.insert("Ultimate Health Potion",
"3_3_#66#10#12_#70#26#31_#65#17#20_#52#2#0_#47#2#0_#49#11#12_#14#13#13_#14#13#13_#48#1#0_");
        name_code_dark.insert("Ultimate Mana Potion",
"4_2_#63#34#49_#59#28#45_#44#13#33_#49#16#36_#41#12#31_#39#12#30_#14#13#13_#34#9#27_");
        name_code_dark.insert("Ultimate Spirit Potion",
"4_2_#50#34#18_#57#44#27_#45#27#10_#56#43#27_#38#20#0_#35#19#0_#14#13#13_#33#18#0_");
        name_code_dark.insert("Great Health Potion",
"17_2_#41#2#0_#15#1#0_#51#2#0_#32#2#0_#59#1#0_#61#1#0_#68#1#0_#66#1#0_#74#10#12_#72#1#0_#72#31#38_#73#10#12_#70#26#31_#65#17#20_#47#2#0_#49#11#12_#55#1#0_#48#1#0_#51#1#0_#59#2#0_#43#2#0_#48#2#0_#44#2#0_#37#3#0_#44#2#0_#41#2#0_#45#1#0_#42#2#0_#44#1#0_#44#1#0_#35#1#0_#32#1#0_#35#1#0_#30#1#0_");
}*/
/*bool Calibrator::fillRectWithPotsInVarClass(QImage& fullscreen, QStringList
nameOfPotionsToLookFor) { QStringList foundPotions; QList<QRect>
rectsWithPotions; findPotionsOnBottomBar(nameOfPotionsToLookFor, foundPotions,
rectsWithPotions, fullscreen);

        bool allPotionsFound = true;
        for each (QString potNameThatShouldBeFound in nameOfPotionsToLookFor) {
                bool found = foundPotions.contains(potNameThatShouldBeFound);
                if (!found)
                        allPotionsFound = false;
        }
        if (!allPotionsFound)
                return false;

        for (size_t i = 0; i < rectsWithPotions.size(); i++) {
                QRect rectToAdd = rectsWithPotions[i];
                QString nameOfPot = foundPotions[i];
                var->potionName_rectPosOnScreen_map[nameOfPot] =
rectToAdd;//[CHANGE]
        }

        return true;
}*/
/*bool Calibrator::findPotionsOnBottomBar(QStringList namesOfPotionsToFind,
QStringList& namesOfPotionosFound, QList<QRect>& rectsWithFoundPots, QImage&
bottomBarImg) { bool wrongInput = namesOfPotionsToFind.size() == 0 ||
bottomBarImg.width() <= 0 || bottomBarImg.height() <= 0; if (wrongInput) {
                //Logger::logPotenialBug("Wrong(empty) input", "Calibrator",
"findPotionsOnBottomBar"); return false;
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
                QList<QPoint> pointsOfStart =
ImgEditor::findStartPositionInImg_mulitpeImgs(potionToLookFor, bottomBarImg); if
(pointsOfStart.size() == 1) {
                        potionsOfStartingPotions.push_back(pointsOfStart.first());
                        namesOfFoundPotions.push_back(map_dark.keys()[i]);
                }
                //else if(pointsOfStart.size() > 1)
                        //Logger::logPotenialBug("More than one occurance of
Potion in Image", "Calibrator", "findPotionsOnBottomBar");
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
                                RGBstruct pixAboveRgb(bottomBarImg.pixel(x, y -
1)); bool goodPix1 = rgb == LEFT_BOTTOM_COLOUR; bool goodPix2 = pixAboveRgb ==
PIX_ABOVE_COLOUR; if (goodPix1 && goodPix2) { someWhereInPic.setX(x);
                                        someWhereInPic.setY(y);
                                        x = max_x;//break
                                        y = max_y;//break
                                }
                        }
                }

                int width = WIDTH_HEIGHT_PIC;
                int height = WIDTH_HEIGHT_PIC / 3;
                int start_x = someWhereInPic.x() + 1;
                int start_y = someWhereInPic.y() - WIDTH_HEIGHT_PIC + 2 * height
+ 1;

                bool err = start_x < 0 || (start_x + width) > WIDTH || start_y <
0 || (start_y + height) > HEIGHT;
                //if(err)
                        //Logger::logPotenialBug("Out of range cordinates of
rect to cut, only part of it will be cut", "Calibrator",
"findPotionsOnBottomBar");

                QRect toAdd(start_x, start_y, width, height);
                rectToRet.push_back(toAdd);
        }

        namesOfPotionosFound = namesOfFoundPotions;
        rectsWithFoundPots = rectToRet;

        return true;
}*/
/*
bool ManaHealthStateAnalyzer::populareMapsWithBottomBarsLetters(QMap<QString, int>& lightMap, QMap<QString, int>& darkMap) {
  try {
    QString     path = PathResource::getPathToFileWithBottomsBarDigitsCodes();
    QJsonObject obj;
    bool        readCorrectly = JsonParser::openJsonFile(obj, path);
    if (!readCorrectly) throw std::exception("Error, can't find bottomBarsDigits.json file");

    QJsonValue value = obj["darkNumbers"];
    if (value.isUndefined() || !value.isArray()) throw std::exception("No darkNumbers field in bottomBar json file!");
    QJsonArray array = value.toArray();
    for each (auto singleValue in array) {
      QJsonObject singleObject = singleValue.toObject();
      int         intVal       = singleObject["name"].toString().toInt();
      QString     pixValues    = singleObject["value"].toString();
      darkMap.insert(pixValues, intVal);
    }

    value = obj["lightNumbers"];
    if (value.isUndefined() || !value.isArray()) throw std::exception("No lightNumbers field in bottomBar json file!");
    array = value.toArray();
    for each (auto singleValue in array) {
      QJsonObject singleObject = singleValue.toObject();
      int         intVal       = singleObject["name"].toInt();
      QString     pixValues    = singleObject["value"].toString();
      lightMap.insert(pixValues, intVal);
    }
    bool toRet = lightMap.size() == 10 && darkMap.size() == 10;
    return toRet;
  } catch (const std::exception& e) {
    var->log(e.what(), true, false, false);
    return false;
  }
};
*/

/*
int ManaHealthStateAnalyzer::getNumberFromBottomBar(QImage& imgToShearchWithin) {
  QMap<QString, int> lightMap, darkMap;
  Utilities::getMapWithNumbersFromBottomBar(lightMap, darkMap);
  QStringList lightCodes = lightMap.keys();
  QStringList darkCodes  = darkMap.keys();

  QMap<int, int> anotherMap;  // <positionX, value>

  for (size_t i = 0; i < lightCodes.size(); i++) {
    QList<QImage*> listWithLightAndDarkLetterImg;
    QImage         darkLetter  = ImgEditor::getImageFromAdvancedCode(darkCodes[i]);
    QImage         lightLetter = ImgEditor::getImageFromAdvancedCode(lightCodes[i]);
    listWithLightAndDarkLetterImg.push_back(&darkLetter);
    listWithLightAndDarkLetterImg.push_back(&lightLetter);

    int           digit                = lightMap[lightCodes[i]];
    QList<QPoint> listOfStartingPoints = ImgEditor::findStartPositionInImg_mulitpeImgs(
        listWithLightAndDarkLetterImg, imgToShearchWithin);  // (listWithLightAndDarkLetterImg, imgToShearchWithin);
    for each (QPoint var in listOfStartingPoints) anotherMap.insert(var.x(), digit);
  }

  QString strToRe;
  for each (auto key in anotherMap.keys()) strToRe.push_back(QString::number(anotherMap[key]));

  return strToRe.toInt();
}
*/

/*
void getAmountsOfPotions() {
  QMap<QString, QRect> map_copy = var->potionName_rectPosOnScreen_map;
  QList<int>           amountOfPots;
  QStringList          namesOfPots;
  QImage               wholeImg;
  var->getCopyOfCurrentFullImg(wholeImg);
  for each (QString nameOfPot in map_copy.keys()) {
    QRect rect = map_copy[nameOfPot];
    if (rect.isEmpty()) continue;
    QImage img    = wholeImg.copy(rect);
    int    amount = ImgEditor::getNumberFromBottomBar(img);
    amountOfPots.push_back(amount);
    namesOfPots.push_back(nameOfPot);
  }

  QStringList infoToSendToMainThread;
  for (size_t i = 0; i < namesOfPots.size(); i++) {
    QString toAdd = namesOfPots[i] + ": " + QString::number(amountOfPots[i]);
    infoToSendToMainThread.push_back(toAdd);
  }
  sendInfoAboutPotAmountsToGUI(infoToSendToMainThread);
}
*/

/*
QPoint MinimapAnalyzer::findPlayerPosition(const QImage& miniMap, const QImage* wholeMap) {
	try{
		const QList<QImage> miniMapParts = splitMiniMap(miniMap);
		QList<QPoint> pointsFromThreads;
		for (size_t i = 0; i < miniMapParts.size(); i++)
			pointsFromThreads.push_back(QPoint());

		QList<ImgShearcher*> threads;
		QRect frame = getFrameToLookByPreviousPos();
		for (size_t i = 0; i < miniMapParts.size(); i++) {
			auto thread = new ImgShearcher(this, miniMapParts[i], *wholeMap, pointsFromThreads[i], frame);
			threads.push_back(thread);
			thread->start();
		}

		bool keepWaiting = false;
		do {
			keepWaiting = false;
			msleep(50);
			for (size_t i = 0; i < threads.size(); i++) {
				if (threads[i]->isRunning())
					keepWaiting = true;
				else {
					bool foundPoint = pointsFromThreads[i] != QPoint();
					if (foundPoint) {
						keepWaiting = false;
						break;
					}
				}
			}
		} while (keepWaiting);

		for (size_t i = 0; i < threads.size(); i++)
			threads[i]->terminate();
		msleep(5);
		for (size_t i = 0; i < threads.size(); i++)
			delete threads[i];

		for (size_t i = 0; i < pointsFromThreads.size(); i++) {
			auto currentList = &pointsFromThreads[i];
			if (pointsFromThreads[i].isNull())
				continue;

			QSize vectorToPlayerPos = getVectorToPlayerFromImgPieceIndex(i);
			QPoint playerPos(vectorToPlayerPos.width() + currentList[0].x(), vectorToPlayerPos.height() + currentList[0].y());
			return playerPos;
		}
		return QPoint();
	}
	catch (const std::exception& e){
		Utilities::ring();
		var->logger.log("!!!!!!!!ERROR IN THREADS!!!!!!", true, true, true);
		return QPoint();
	}

}
*/

/*
bool RouteCreator::currentPixIsWalkable(){
	uint pixCol = currentMapOfWalkability.pixel(currentChoosenPoint.getXY());
	RGBstruct rgb(pixCol);
	bool isWalkable = rgb.isGrey();
	if (!isWalkable) {
		uint pixColFromNormalMap = currentMap.pixel(currentChoosenPoint.getXY());
		RGBstruct rgbFromNormalMap = RGBstruct(pixColFromNormalMap);
		isWalkable = rgbFromNormalMap.isYellow();
	}
	return isWalkable;
}
*/
/*
void SelectProfileWindow::readAndSetLastUsedProFromINI() {
  
	if (lastUsedProfName.size() == 0)
		return;
	QStringList listOfDisplayNames;
	if (!ok)
		return;
	int indexToSet = 0;
	for (size_t i = 0; i < listOfDisplayNames.size(); i++){
		QString nameToSplit = listOfDisplayNames[i];
		QStringList partOfName = nameToSplit.split("] ",Qt::SkipEmptyParts);
		if (partOfName.size() >= 2) {
			QString exactProfName = partOfName[1];
			bool matched = exactProfName == lastUsedProfName;
			if (matched) {
				indexToSet = i;
				break;
			}
		}
	}
	ui->listOfProfs->setCurrentRow(indexToSet);
	ui->listOfProfs->repaint();

}	
*/
