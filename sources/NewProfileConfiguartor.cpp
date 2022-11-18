#include "NewProfileConfiguartor.h"
#include "ui_NewProfileConfiguartor.h"
#include <Key.h>

NewProfileConfiguartor::NewProfileConfiguartor(Profile* prof, QWidget *parent)	: 
	QDialog(parent), profToEdit(prof) {
	ui = new Ui::NewProfileConfiguartor();
	ui->setupUi(this); 
	MAX_PAGE = ui->stackedWidget->count();
	ui->stackedWidget->setCurrentIndex(0);
	fillGuiPtrs();
	additionalGuiSettings();
	refreshGUI();
}

NewProfileConfiguartor::~NewProfileConfiguartor(){
	delete ui;
}

void NewProfileConfiguartor::refreshGUI(){
	QString page = tr("Page ");
	QString currentPageStr = QString::number(pageNumber);
	QString maxPageStr = QString::number(MAX_PAGE);
	QString labelPageInfoText = page + currentPageStr + "/" + maxPageStr;
	ui->pageLabel->setText(labelPageInfoText);
	ui->pageLabel->repaint();

	bool firstPage = pageNumber == 1;
	ui->previousButton->setEnabled(!firstPage);

	bool lastPage = pageNumber == MAX_PAGE;
	QString strToSet = lastPage ? tr("Finish") : tr("Next page");
	ui->nextButton->setText(strToSet);
	ui->stackedWidget->repaint();
}

bool NewProfileConfiguartor::finishAddingNewProfile(){
	QString text = tr("Are you sure that you want finish creating new profile?");
	bool accepted = Utilities::showMessageBox_NO_YES(text);
	if (accepted) {
		saveDataToProfile(profToEdit);
		this->accept();
	}
	return false;
}

void NewProfileConfiguartor::additionalGuiSettings(){
	ui->_3_spinGetNumberOfMethodes->setMinimum(0);
	ui->_3_spinGetNumberOfMethodes->setMaximum(5);
	QStringList allPossibleKeys = Key::getListOfAllPossibleKeys();
	for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) {
		guiPtrs.sliderOnPage_3[i]->setEnabled(false);
		guiPtrs.labelsOnPage_3[i]->setEnabled(false);
		guiPtrs.labelsOnPage_3[i]->setText("       ");
		guiPtrs.boxesOnPage_3[i]->setEnabled(false);
		guiPtrs.keyShortCutsOnPage_3[i]->addItems(allPossibleKeys);
		guiPtrs.keyShortCutsOnPage_3[i]->setEnabled(false);
		guiPtrs.keyShortCutsOnPage_3[i]->repaint();
		guiPtrs.boxesOnPage_3[i]->setCurrentIndex(-1);

		guiPtrs.sliderOnPage_4[i]->setEnabled(false);
		guiPtrs.labelsOnPage_4[i]->setText("       ");
		guiPtrs.labelsOnPage_4[i]->setEnabled(false);
		guiPtrs.boxesOnPage_4[i]->setEnabled(false);
		guiPtrs.keyShortCutsOnPage_4[i]->addItems(allPossibleKeys);
		guiPtrs.keyShortCutsOnPage_4[i]->repaint();
		guiPtrs.keyShortCutsOnPage_4[i]->setEnabled(false);
		guiPtrs.boxesOnPage_4[i]->setCurrentIndex(-1);
	}

	ui->_4_spinGetNumberOfMethodes->setMinimum(0);
	ui->_4_spinGetNumberOfMethodes->setMaximum(5);

	ui->_5_LeftAddedBarsSpin->setMaximum(3);
	ui->_5_RightAddedBarsSpin->setMaximum(3);
	ui->_5_LeftAddedBarsSpin->setMinimum(0);
	ui->_5_RightAddedBarsSpin->setMinimum(0);
	
	ui->_5_ControlBox->addItem("Classic controls");
	ui->_5_ControlBox->addItem("Regular controls");
	ui->_5_ControlBox->addItem("Left smart click");
	ui->_5_AutoLootBox->addItem("Right");
	ui->_5_AutoLootBox->addItem("Shift+right");
	ui->_5_AutoLootBox->addItem("Left");

	ui->_5_ControlBox->setCurrentIndex(0);
	ui->_5_AutoLootBox->setCurrentIndex(0);

}

bool NewProfileConfiguartor::pageIsCorrectlyFilled() {
	switch (pageNumber) {
	case 1:
		 return checkCorrectnessOfPage_1();
	case 2:
		return checkCorrectnessOfPage_2();
	case 3:
		return checkCorrectnessOfPage_3();
	case 4:
		return checkCorrectnessOfPage_4();
	case 5: 
		return checkCorrectnessOfPage_5();
	default:
		return true;
	} 
}

void NewProfileConfiguartor::saveDataToProfile(Profile* prof) {
	prof->profileName = ui->_1_nameEdit->text();
	//2
	if (ui->_2_RadButt_ED->isChecked())
		prof->profession = Profile::PROFESSION::ED;
	else if (ui->_2_RadButt_EK->isChecked())
		prof->profession = Profile::PROFESSION::EK;
	else if (ui->_2_RadButt_MS->isChecked())
		prof->profession = Profile::PROFESSION::MS;
	else if (ui->_2_RadButt_RP->isChecked())
		prof->profession = Profile::PROFESSION::RP;
	//3
	if (ui->_3_enableAutoHealing->isChecked()) {
		int size = ui->_3_spinGetNumberOfMethodes->value();
		QList<QAbstractSlider*> sliders;
		sliders.push_back(ui->_3_horizontalSlider_1);
		sliders.push_back(ui->_3_horizontalSlider_2);
		sliders.push_back(ui->_3_horizontalSlider_3);
		sliders.push_back(ui->_3_horizontalSlider_4);
		sliders.push_back(ui->_3_horizontalSlider_5);

		prof->healthRestorePercentages.clear();
		for (size_t i = 0; i < size; i++){
			int thresholdOfSlider = sliders[i]->value();
			prof->healthRestorePercentages.push_back(thresholdOfSlider);
		}

		QList<QComboBox*> keysPtrs;
		keysPtrs.push_back(ui->_3_shortKey_1);
		keysPtrs.push_back(ui->_3_shortKey_2);
		keysPtrs.push_back(ui->_3_shortKey_3);
		keysPtrs.push_back(ui->_3_shortKey_4);
		keysPtrs.push_back(ui->_3_shortKey_5);

		prof->healthKeys.clear();
		for (size_t i = 0; i < size; i++)
			prof->healthKeys.push_back(keysPtrs[i]->currentText());

		QList<QString> helathRestorationNames;
		helathRestorationNames.push_back(ui->_3_comboBox->currentText());
		helathRestorationNames.push_back(ui->_3_comboBox_2->currentText());
		helathRestorationNames.push_back(ui->_3_comboBox_3->currentText());
		helathRestorationNames.push_back(ui->_3_comboBox_4->currentText());
		helathRestorationNames.push_back(ui->_3_comboBox_5->currentText());

		prof->healthRestoreMethodeNames.clear();
		for (size_t i = 0; i < size; i++)
			prof->healthRestoreMethodeNames.push_back(helathRestorationNames[i]);
	}
	//4
	if (ui->_4_enableManaRestore->isChecked()) {
		int size = ui->_4_spinGetNumberOfMethodes->value();
		QList<QAbstractSlider*> sliders;
		sliders.push_back(ui->_4_horizontalSlider_1);
		sliders.push_back(ui->_4_horizontalSlider_2);
		sliders.push_back(ui->_4_horizontalSlider_3);
		sliders.push_back(ui->_4_horizontalSlider_4);
		sliders.push_back(ui->_4_horizontalSlider_5);

		prof->ManaRestoreMethodesPercentage.clear();
		for (size_t i = 0; i < size; i++) {
			int thresholdOfSlider = sliders[i]->value();
			prof->ManaRestoreMethodesPercentage.push_back(thresholdOfSlider);
		}


		QList<QComboBox*> keysPtrs;
		keysPtrs.push_back(ui->_4_shortKey_1);
		keysPtrs.push_back(ui->_4_shortKey_2);
		keysPtrs.push_back(ui->_4_shortKey_3);
		keysPtrs.push_back(ui->_4_shortKey_4);
		keysPtrs.push_back(ui->_4_shortKey_5);

		prof->ManaKeys.clear();
		for (size_t i = 0; i < size; i++)
			prof->ManaKeys.push_back(keysPtrs[i]->currentText());

		QList<QString> manaRestoreNames;
		manaRestoreNames.push_back(ui->_4_comboBox->currentText());
		manaRestoreNames.push_back(ui->_4_comboBox_2->currentText());
		manaRestoreNames.push_back(ui->_4_comboBox_3->currentText());
		manaRestoreNames.push_back(ui->_4_comboBox_4->currentText());
		manaRestoreNames.push_back(ui->_4_comboBox_5->currentText());

		prof->manaRestoreMethodeNames.clear();
		for (size_t i = 0; i < size; i++)
			prof->manaRestoreMethodeNames.push_back(manaRestoreNames[i]);
	}
	//5
}

bool NewProfileConfiguartor::checkCorrectnessOfPage_1(){
	QString nameOfProf = ui->_1_nameEdit->text();
	bool nameisTooLong = nameOfProf.size() > 50;
	if (nameisTooLong) {
		QString text = tr("Profile name can't be longer than 50 characters.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}

	bool nameIsTooShort = nameOfProf.size() < 3;
	if (nameIsTooShort) {
		QString text = tr("Profile name can't be shorter than 3 characters.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}

	bool nameConsistForbiddenChars = false;
	if (nameOfProf.contains(QChar::LineFeed) || nameOfProf.contains(QChar::CarriageReturn))
		nameConsistForbiddenChars = true;
	for each (auto character in nameOfProf) {
		if (!character.isLetterOrNumber() && !character.isSpace())
			nameConsistForbiddenChars = true;
	}
	if (nameConsistForbiddenChars) {
		QString text = tr("Profile name can't have any special characters, please use only letters, numbers or spaces.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}

	return true;
}
bool NewProfileConfiguartor::checkCorrectnessOfPage_2(){
	bool oneOfButtonsIsChecked = false;
	QList<QRadioButton*> buttons;
	buttons.push_back(ui->_2_RadButt_ED);
	buttons.push_back(ui->_2_RadButt_EK);
	buttons.push_back(ui->_2_RadButt_RP);
	buttons.push_back(ui->_2_RadButt_MS);
	
	for each (QRadioButton * var in buttons) {
		if (var->isChecked()) {
			oneOfButtonsIsChecked = true;
			break;
		}
	}

	if (oneOfButtonsIsChecked)
		return true;
	else {
		QString text = tr("You have to choose profession.");
		Utilities::showMessageBox("CrackerJack", text, QMessageBox::Ok);
		return false;
	}

}
bool NewProfileConfiguartor::checkCorrectnessOfPage_3(){	
	bool slidersAreInCorrectOrder = true;
	bool everySliderHasDiffrentValue = true;
	{
		int biggestValue = 101;
		for (size_t i = 0; i < guiPtrs.activeElementsOnPage_3; i++){
			QAbstractSlider* slider = guiPtrs.sliderOnPage_3[i];
			if (slider->value() < biggestValue)
				biggestValue = slider->value();
			else if (slider->value() == biggestValue)
				everySliderHasDiffrentValue = false;
			else
				slidersAreInCorrectOrder = false;
		}
	}

	bool lastSliderIsZero = false;
	{
		int activeElements = guiPtrs.activeElementsOnPage_3;
		if (activeElements > 0) {
			int lastSliderIndex = activeElements - 1;
			int valOfLastSlider = guiPtrs.sliderOnPage_3[lastSliderIndex]->value();
			lastSliderIsZero = valOfLastSlider <= 0;
		}
	}

	bool oneOfKeyFieldsHasManyValues = false;
	bool oneOfKeyFieldsHasNoValue = false;
	bool oneOfKeyFieldsHasForbiddenChars = false;
	{
		for (size_t i = 0; i < guiPtrs.activeElementsOnPage_3; i++){
			QString keyText = guiPtrs.keyShortCutsOnPage_3[i]->currentText();
			int number = Key(keyText).number;
			bool error = number == -1;
			if (error)
				oneOfKeyFieldsHasNoValue = true;
		}
	}

	bool comboBoxHasNotChoosenValue = false;
	{
		for (size_t i = 0; i < guiPtrs.activeElementsOnPage_3; i++){
			QComboBox* comboBox = guiPtrs.boxesOnPage_3[i];
			int maxIndex = comboBox->count();
			int currentIndex = comboBox->currentIndex();
			bool indexIsInProperRange = currentIndex >= 0 && currentIndex < maxIndex;
			if (!indexIsInProperRange)
				comboBoxHasNotChoosenValue = true;
		}
	}

	bool theSameValueIsAssignedToMoreThanOneBox = false;
	if (guiPtrs.activeElementsOnPage_3 > 0) {
		QSet<int> setToPreventDuplicates;
		const int MAX_INDEX = guiPtrs.activeElementsOnPage_3;
		for (size_t i = 0; i < MAX_INDEX; i++) {
			QString keySrt = guiPtrs.keyShortCutsOnPage_3[i]->currentText();
			int valueOfKey = Key(keySrt).number;
			setToPreventDuplicates.insert(valueOfKey);
		}

		bool allWidgetsAreDiffrent = setToPreventDuplicates.size() == MAX_INDEX;
		bool AtLeastOneIsActive = MAX_INDEX > 0;
		if(AtLeastOneIsActive)
			theSameValueIsAssignedToMoreThanOneBox = !allWidgetsAreDiffrent;
	}

	bool theSameKeyIsAssignedToMoreThanOneKeyShortCut = false;
	if(guiPtrs.activeElementsOnPage_3 > 0){
		QSet<int> setToPreventDuplicates;
		for (size_t i = 0; i < guiPtrs.activeElementsOnPage_3; i++){
			QString codeStr = guiPtrs.keyShortCutsOnPage_3[i]->currentText();
			int code = Key::KeysAndCodesMap.value(codeStr, -1);
			setToPreventDuplicates.insert(code);
		}
		bool allKeysAreDiffrent = setToPreventDuplicates.size() == guiPtrs.activeElementsOnPage_3;
		theSameKeyIsAssignedToMoreThanOneKeyShortCut = !allKeysAreDiffrent;
	}

	if (!everySliderHasDiffrentValue) {
		QString text = tr("Two sliders can't be in the same position.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	if (!slidersAreInCorrectOrder) {
		QString text = tr("Sliders are in wrong order, please set it from biggest value to lowest.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	if (oneOfKeyFieldsHasManyValues) {
		QString text = tr("One of key field has more than one hotkey assigned to itself.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	if (lastSliderIsZero) {
		QString text = tr("Last slider has to have value above zero.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	if (oneOfKeyFieldsHasNoValue) {
		QString text = tr("One of key field doesn't have hotkey assigned to itself.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	if (oneOfKeyFieldsHasForbiddenChars) {
		QString text = tr("Key field has wrong value.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	if (comboBoxHasNotChoosenValue) {
		QString text = tr("Methode field can't be empty.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	if (theSameValueIsAssignedToMoreThanOneBox) {
		QString text = tr("Two  fields can't share the same value.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	if (theSameKeyIsAssignedToMoreThanOneKeyShortCut) {
		QString text = tr("Two key fields can't share the same key.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	return true;
}

bool NewProfileConfiguartor::checkCorrectnessOfPage_4(){
	bool slidersAreInCorrectOrder = true;
	bool everySliderHasDiffrentValue = true;
	{
		int biggestValue = 101;
		for (size_t i = 0; i < guiPtrs.activeElementsOnPage_4; i++) {
			QAbstractSlider* slider = guiPtrs.sliderOnPage_4[i];
			if (slider->value() < biggestValue)
				biggestValue = slider->value();
			else if (slider->value() == biggestValue)
				everySliderHasDiffrentValue = false;
			else
				slidersAreInCorrectOrder = false;
		}
	}

	bool lastSliderIsZero = false;
	{
		int activeElements = guiPtrs.activeElementsOnPage_4;
		if (activeElements > 0) {
			int lastSliderIndex = activeElements - 1;
			int valOfLastSlider = guiPtrs.sliderOnPage_4[lastSliderIndex]->value();
			lastSliderIsZero = valOfLastSlider <= 0;
		}
	}

	bool oneOfKeyFieldsHasManyValues = false;
	bool oneOfKeyFieldsHasNoValue = false;
	bool oneOfKeyFieldsHasForbiddenChars = false;
	{
		for (size_t i = 0; i < guiPtrs.activeElementsOnPage_4; i++) {
			QString keyText = guiPtrs.keyShortCutsOnPage_3[i]->currentText();
			int number = Key(keyText).number;
			bool error = number == -1;
			if (error)
				oneOfKeyFieldsHasNoValue = true;
		}
	}

	bool comboBoxHasNotChoosenValue = false;
	{
		for (size_t i = 0; i < guiPtrs.activeElementsOnPage_4; i++) {
			QComboBox* comboBox = guiPtrs.boxesOnPage_4[i];
			int maxIndex = comboBox->count();
			int currentIndex = comboBox->currentIndex();
			bool indexIsInProperRange = currentIndex >= 0 && currentIndex < maxIndex;
			if (!indexIsInProperRange)
				comboBoxHasNotChoosenValue = true;
		}
	}

	bool theSameValueIsAssignedToMoreThanOneBox = false;
	if (guiPtrs.activeElementsOnPage_4 > 0) {
		QSet<int> setToPreventDuplicates;
		int MAX_INDEX = guiPtrs.activeElementsOnPage_4;
		for (size_t i = 0; i < MAX_INDEX; i++) {
			QString keyStr = guiPtrs.keyShortCutsOnPage_4[i]->currentText();
			int valueOfKey = Key(keyStr).number;
			setToPreventDuplicates.insert(valueOfKey);
		}
		bool allWidgetsAreDiffrent = setToPreventDuplicates.size() == MAX_INDEX;
		bool AtLeastOneIsActive = MAX_INDEX > 0;
		if (AtLeastOneIsActive)
			theSameValueIsAssignedToMoreThanOneBox = !allWidgetsAreDiffrent;
	}

	bool theSameKeyIsAssignedToMoreThanOneKeyShortCut = false;
	if(guiPtrs.activeElementsOnPage_4 > 0){
		QSet<int> setToPreventDuplicates;
		for (size_t i = 0; i < guiPtrs.activeElementsOnPage_4; i++) {
			QString codeStr = Key(guiPtrs.keyShortCutsOnPage_4[i]->currentText()).number;
			int code = Key::KeysAndCodesMap.value(codeStr, -1);
			setToPreventDuplicates.insert(code);
		}
		bool allKeysAreDiffrent = setToPreventDuplicates.size() == guiPtrs.activeElementsOnPage_4;
		theSameKeyIsAssignedToMoreThanOneKeyShortCut = !allKeysAreDiffrent;
	}

	if (!everySliderHasDiffrentValue) {
		QString text = tr("Two sliders can't be in the same position.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	if (!slidersAreInCorrectOrder) {
		QString text = tr("Sliders are in wrong order, please set it from biggest value to lowest.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	if (oneOfKeyFieldsHasManyValues) {
		QString text = tr("One of key field has more than one hotkey assigned to itself.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	if (lastSliderIsZero) {
		QString text = tr("Last slider has to have value above zero.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	if (oneOfKeyFieldsHasNoValue) {
		QString text = tr("One of key field doesn't have hotkey assigned to itself.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	if (oneOfKeyFieldsHasForbiddenChars) {
		QString text = tr("Key field has wrong value.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	if (comboBoxHasNotChoosenValue) {
		QString text = tr("Methode field can't be empty.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	if (theSameValueIsAssignedToMoreThanOneBox) {
		QString text = tr("Two  fields can't share the same value.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	if (theSameKeyIsAssignedToMoreThanOneKeyShortCut) {
		QString text = tr("Two key fields can't share the same key.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	return true;
}
bool NewProfileConfiguartor::checkCorrectnessOfPage_5(){
	//change //TODO
	int indexOfAutoBox = ui->_5_AutoLootBox->currentIndex();
	int indexOfLootBox = ui->_5_AutoLootBox->currentIndex();
	bool indexAreInRage = true;
	if (indexOfAutoBox < 0 || indexOfAutoBox > 2)
		indexAreInRage = false;
	if (indexOfLootBox < 0 || indexOfLootBox > 2)
		indexAreInRage = false;
	return indexAreInRage;
}

QStringList NewProfileConfiguartor::getNamesOfHealthRestoringMethodes(Profile::PROFESSION prof){
	JsonParser parser;
	QList<Potion> potions;
	QList<Utilities::Spell> spells;
	parser.readSpellsJson(spells);
	auto typeOfSpell = Utilities::Spell::TYPE_OF_SPELL::HEALING;
	parser.filtrSpells(spells, &prof, &typeOfSpell);
	parser.getPotionsForProf(potions, &prof, JsonParser::TypeOfPotion::HEALTH);

	QStringList namesOfAvaibleHealthRestoreMethodes;
	for each (auto var in spells)
		namesOfAvaibleHealthRestoreMethodes.push_back(var.incantations);
	for each (auto var in potions)
		namesOfAvaibleHealthRestoreMethodes.push_back(var.name);

	return namesOfAvaibleHealthRestoreMethodes;
}

QStringList NewProfileConfiguartor::getNamesOfManaRestoringMethodes(Profile::PROFESSION prof){
	JsonParser parser;
	QList<Potion> potions;
	parser.getPotionsForProf(potions, &prof, JsonParser::TypeOfPotion::MANA);

	QStringList namesOfAvaibleManaRestoreMethodes;
	for each (auto var in potions)
		namesOfAvaibleManaRestoreMethodes.push_back(var.name);

	return namesOfAvaibleManaRestoreMethodes;
}

void NewProfileConfiguartor::fillGuiElementsWithNamesRestoreMethodes_Health(Profile::PROFESSION profession){
	QStringList listOfMethodesNamesFromProf = profToEdit->healthRestoreMethodeNames;
	QStringList namesOfAllPosibleMethodes = getNamesOfHealthRestoringMethodes(profession);
	QStringList incantationsThatShouldBeDeleted{"utura", "utura gran", "exura sio \"name\"",
	"exana pox", "utura mas sio", "exana flam", "exana kor", "exana vis", "exura gran sio \"name\"",
	"exana mort"};

	for each (QString var in incantationsThatShouldBeDeleted)
		namesOfAllPosibleMethodes.removeOne(var);
	namesOfAllPosibleMethodes.sort();

	int numberOfComboBoxToSetActive = listOfMethodesNamesFromProf.size();
	for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++){
		QComboBox* box = guiPtrs.boxesOnPage_3[i];
		box->clear();
		box->insertItems(0, namesOfAllPosibleMethodes);
		bool shouldBeActivated = i < numberOfComboBoxToSetActive;
		if (shouldBeActivated) {
			QString strToFind = listOfMethodesNamesFromProf[i];
			int indexOfWantedItem = namesOfAllPosibleMethodes.indexOf(strToFind);
			box->setCurrentIndex(indexOfWantedItem);
		}
	}
}

void NewProfileConfiguartor::fillGuiElementsWithNamesRestoreMethodes_Mana(Profile::PROFESSION prof){
	QStringList listOfMethodesNamesFromProf = profToEdit->manaRestoreMethodeNames;
	QStringList namesOfAllPosibleMethodes = getNamesOfManaRestoringMethodes(prof);
	namesOfAllPosibleMethodes.sort();

	int numberOfComboBoxToSetActive = listOfMethodesNamesFromProf.size();
	for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) {
		QComboBox* box = guiPtrs.boxesOnPage_4[i];
		box->clear();
		box->insertItems(0, namesOfAllPosibleMethodes);
		bool shouldBeActivated = i < numberOfComboBoxToSetActive;
		if (shouldBeActivated) {
			QString strToFind = listOfMethodesNamesFromProf[i];
			int indexOfWantedItem = namesOfAllPosibleMethodes.indexOf(strToFind);
			box->setCurrentIndex(indexOfWantedItem);
		}
	}
}

Profile::PROFESSION NewProfileConfiguartor::getProfessionFromRadioButtonOnPage2(){
	if (ui->_2_RadButt_ED->isChecked())
		return Profile::PROFESSION::ED;
	if (ui->_2_RadButt_EK->isChecked())
		return Profile::PROFESSION::EK;
	if (ui->_2_RadButt_MS->isChecked())
		return Profile::PROFESSION::MS;
	if (ui->_2_RadButt_RP->isChecked())
		return Profile::PROFESSION::RP;

	return Profile::PROFESSION::RP;
}

void NewProfileConfiguartor::fillGuiPtrs() {
	guiPtrs.sliderOnPage_3.push_back(ui->_3_horizontalSlider_1);
	guiPtrs.sliderOnPage_3.push_back(ui->_3_horizontalSlider_2);
	guiPtrs.sliderOnPage_3.push_back(ui->_3_horizontalSlider_3);
	guiPtrs.sliderOnPage_3.push_back(ui->_3_horizontalSlider_4);
	guiPtrs.sliderOnPage_3.push_back(ui->_3_horizontalSlider_5);

	guiPtrs.sliderOnPage_4.push_back(ui->_4_horizontalSlider_1);
	guiPtrs.sliderOnPage_4.push_back(ui->_4_horizontalSlider_2);
	guiPtrs.sliderOnPage_4.push_back(ui->_4_horizontalSlider_3);
	guiPtrs.sliderOnPage_4.push_back(ui->_4_horizontalSlider_4);
	guiPtrs.sliderOnPage_4.push_back(ui->_4_horizontalSlider_5);

	guiPtrs.labelsOnPage_3.push_back(ui->_3_label_1);
	guiPtrs.labelsOnPage_3.push_back(ui->_3_label_2);
	guiPtrs.labelsOnPage_3.push_back(ui->_3_label_3);
	guiPtrs.labelsOnPage_3.push_back(ui->_3_label_4);
	guiPtrs.labelsOnPage_3.push_back(ui->_3_label_5);

	guiPtrs.labelsOnPage_4.push_back(ui->_4_label_1);
	guiPtrs.labelsOnPage_4.push_back(ui->_4_label_2);
	guiPtrs.labelsOnPage_4.push_back(ui->_4_label_3);
	guiPtrs.labelsOnPage_4.push_back(ui->_4_label_4);
	guiPtrs.labelsOnPage_4.push_back(ui->_4_label_5);

	guiPtrs.boxesOnPage_3.push_back(ui->_3_comboBox);
	guiPtrs.boxesOnPage_3.push_back(ui->_3_comboBox_2);
	guiPtrs.boxesOnPage_3.push_back(ui->_3_comboBox_3);
	guiPtrs.boxesOnPage_3.push_back(ui->_3_comboBox_4);
	guiPtrs.boxesOnPage_3.push_back(ui->_3_comboBox_5);

	guiPtrs.boxesOnPage_4.push_back(ui->_4_comboBox);
	guiPtrs.boxesOnPage_4.push_back(ui->_4_comboBox_2);
	guiPtrs.boxesOnPage_4.push_back(ui->_4_comboBox_3);
	guiPtrs.boxesOnPage_4.push_back(ui->_4_comboBox_4);
	guiPtrs.boxesOnPage_4.push_back(ui->_4_comboBox_5);


	guiPtrs.keyShortCutsOnPage_3.push_back(ui->_3_shortKey_1);
	guiPtrs.keyShortCutsOnPage_3.push_back(ui->_3_shortKey_2);
	guiPtrs.keyShortCutsOnPage_3.push_back(ui->_3_shortKey_3);
	guiPtrs.keyShortCutsOnPage_3.push_back(ui->_3_shortKey_4);
	guiPtrs.keyShortCutsOnPage_3.push_back(ui->_3_shortKey_5);

	guiPtrs.keyShortCutsOnPage_4.push_back(ui->_4_shortKey_1);
	guiPtrs.keyShortCutsOnPage_4.push_back(ui->_4_shortKey_2);
	guiPtrs.keyShortCutsOnPage_4.push_back(ui->_4_shortKey_3);
	guiPtrs.keyShortCutsOnPage_4.push_back(ui->_4_shortKey_4);
	guiPtrs.keyShortCutsOnPage_4.push_back(ui->_4_shortKey_5);

	for each (QAbstractSlider* var in guiPtrs.sliderOnPage_4)
		guiPtrs.activeElementsOnPage_4 += var->isEnabled();
	
	for each (QAbstractSlider * var in guiPtrs.sliderOnPage_3)
		guiPtrs.activeElementsOnPage_3 += var->isEnabled();
}

void NewProfileConfiguartor::fillWidgetsWithDataFromProf(Profile* profToEdit) {
	//1
	{
		ui->_1_nameEdit->setText(profToEdit->profileName); 
	}
	//2
	{
	Profile::PROFESSION prof = Profile::PROFESSION(profToEdit->profession);
	switch (prof) {
	case Profile::RP:	
		ui->_2_RadButt_RP->setChecked(true);
		break;
	case Profile::EK:
		ui->_2_RadButt_EK->setChecked(true);
		break;
	case Profile::ED:
		ui->_2_RadButt_ED->setChecked(true);
		break;
	case Profile::MS:
		ui->_2_RadButt_MS->setChecked(true);
		break;
	default:	
		ui->_2_RadButt_RP->setChecked(true);
		break;
	}
	}
	//3
	{
		int methodesUsedByProfile = profToEdit->healthRestorePercentages.size();
		if (methodesUsedByProfile >= 1) {
			ui->_3_enableAutoHealing->setEnabled(true);
			ui->_3_enableAutoHealing->setChecked(true);
			ui->_3_spinGetNumberOfMethodes->setEnabled(true);
			ui->_3_spinGetNumberOfMethodes->setValue(guiPtrs.activeElementsOnPage_3);
		}
		
		for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) {
			bool shouldBeVisible = i < methodesUsedByProfile;

			guiPtrs.sliderOnPage_3[i]->setEnabled(shouldBeVisible);
			guiPtrs.labelsOnPage_3[i]->setEnabled(shouldBeVisible);
			guiPtrs.keyShortCutsOnPage_3[i]->setEnabled(shouldBeVisible);
			guiPtrs.boxesOnPage_3[i]->setEnabled(shouldBeVisible);
			
			if (shouldBeVisible) {
				guiPtrs.sliderOnPage_3[i]->setVisible(shouldBeVisible);
				guiPtrs.labelsOnPage_3[i] ->setVisible(shouldBeVisible);
				guiPtrs.keyShortCutsOnPage_3[i]->setVisible(shouldBeVisible);
				guiPtrs.boxesOnPage_3[i]->setVisible(shouldBeVisible);
				guiPtrs.sliderOnPage_3[i]->setValue(profToEdit->healthRestorePercentages[i]);
				int indexToSet = profToEdit->healthRestoreMethodeNames.indexOf(profToEdit->healthRestoreMethodeNames[i]);
				guiPtrs.boxesOnPage_3[i]->setCurrentIndex(indexToSet);
				int codeOfKey = profToEdit->healthKeys[i].number;
				QString textOfKey = Key::KeysAndCodesMap.key(codeOfKey, tr("Error"));
				guiPtrs.keyShortCutsOnPage_3[i]->setCurrentText(textOfKey);
			}
		}
		_3_slidersChanged();//setting proper text on labels;
	}
	//4
	{
		int methodesUsedByProfile = profToEdit->manaRestoreMethodeNames.size();
		if (methodesUsedByProfile >= 1) {
			ui->_4_enableManaRestore->setEnabled(true);
			ui->_4_enableManaRestore->setChecked(true);
			ui->_4_spinGetNumberOfMethodes->setEnabled(true);
			ui->_4_spinGetNumberOfMethodes->setValue(guiPtrs.activeElementsOnPage_4);
		}
		for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) {
			bool shouldBeVisible = i < methodesUsedByProfile;

			guiPtrs.sliderOnPage_4[i]->setEnabled(shouldBeVisible);
			guiPtrs.labelsOnPage_4[i]->setEnabled(shouldBeVisible);
			guiPtrs.keyShortCutsOnPage_4[i]->setEnabled(shouldBeVisible);
			guiPtrs.boxesOnPage_4[i]->setEnabled(shouldBeVisible);
			if (shouldBeVisible) {
				guiPtrs.sliderOnPage_4[i]->setVisible(shouldBeVisible);
				guiPtrs.labelsOnPage_4[i]->setVisible(shouldBeVisible);
				guiPtrs.keyShortCutsOnPage_4[i]->setVisible(shouldBeVisible);
				guiPtrs.boxesOnPage_4[i]->setVisible(shouldBeVisible);
				guiPtrs.sliderOnPage_4[i]->setValue(profToEdit->healthRestorePercentages[i]);
				int indexToSet = profToEdit->healthRestoreMethodeNames.indexOf(profToEdit->healthRestoreMethodeNames[i]);
				guiPtrs.boxesOnPage_4[i]->setCurrentIndex(indexToSet);
				int keyCode = profToEdit->healthKeys[i].number;
				QString textToFind = Key::KeysAndCodesMap.key(keyCode, tr("Error"));
				int index = guiPtrs.keyShortCutsOnPage_4[i]->findText(textToFind, Qt::MatchExactly);
				guiPtrs.keyShortCutsOnPage_4[i]->setCurrentIndex(index);
			}
		}
		_4_slidersChanged();//set proper text on labels;
	}
}
//SLOTS
void NewProfileConfiguartor::nextPageButtonAction() {
	if (!pageIsCorrectlyFilled())
		return;
	if (pageNumber == MAX_PAGE) {
		bool accepted = finishAddingNewProfile();	
		if (!accepted)
			return;
	}

	bool isNextPageHealthPageMethodes = pageNumber + 1 == 3;
	if (isNextPageHealthPageMethodes) {
		Profile::PROFESSION profesionForWhomSpellsWillBeSelected = getProfessionFromRadioButtonOnPage2();
		fillGuiElementsWithNamesRestoreMethodes_Health(profesionForWhomSpellsWillBeSelected);
	}
	bool isNextPageManaPageMethodes = pageNumber + 1 == 4;
	if (isNextPageManaPageMethodes) {
		Profile::PROFESSION profesionForWhomPotionsWillBeSelected = getProfessionFromRadioButtonOnPage2();
		fillGuiElementsWithNamesRestoreMethodes_Mana(profesionForWhomPotionsWillBeSelected);
	}
	
	pageNumber++;
	int index = ui->stackedWidget->currentIndex() + 1;
	ui->stackedWidget->setCurrentIndex(index);
	refreshGUI();
}

void NewProfileConfiguartor::previousPageButtonAction(){
	pageNumber--;
	/*
	if (pageNumber == 3)
		fillGuiElementsWithNamesOfHealthRestoreMethodesAvaibleForProf(getProfessionFromRadioButtonOnPage2());
	else if (pageNumber == 4)
		fillGuiElementsWithNamesOfManaRestoreMethodesAvaibleForProf(getProfessionFromRadioButtonOnPage2());
	*/
	int index = ui->stackedWidget->currentIndex() - 1;
	ui->stackedWidget->setCurrentIndex(index);

	refreshGUI();
}

void NewProfileConfiguartor::cancelButtonAction() {
	QString text = tr("Are you sure that you want cancel adding new profile?");
	bool accepted = Utilities::showMessageBox_NO_YES(text);
	if(accepted)
		this->reject();
}

void NewProfileConfiguartor::_3_healingEnabledChanged(){
	bool enabled = ui->_3_enableAutoHealing->isChecked();
	ui->_3_spinGetNumberOfMethodes->setEnabled(enabled);
	if (!enabled) {
		ui->_3_spinGetNumberOfMethodes->setValue(0);
		ui->_3_spinGetNumberOfMethodes->setEnabled(false);
	}
	_3_spinChanged();
}

void NewProfileConfiguartor::_3_spinChanged() {
	guiPtrs.activeElementsOnPage_3 = ui->_3_spinGetNumberOfMethodes->value();
	for (int i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) {
		bool enable = i < guiPtrs.activeElementsOnPage_3;
		guiPtrs.boxesOnPage_3[i]->setEnabled(enable);
		guiPtrs.keyShortCutsOnPage_3[i]->setEnabled(enable);
		guiPtrs.labelsOnPage_3[i]->setEnabled(enable);
		guiPtrs.sliderOnPage_3[i]->setEnabled(enable);
		if (!enable) {
			guiPtrs.sliderOnPage_3[i]->setValue(0);
			guiPtrs.labelsOnPage_3[i]->setText("");
			guiPtrs.boxesOnPage_3[i]->setCurrentIndex(-1);
			//guiPtrs.keyShortCutsOnPage_3[i]->clear();
		}
	}
}
	
void NewProfileConfiguartor::_3_slidersChanged() {
	QStringList rangesOfSlidersValue;
	for (int i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++){
		bool isEnabled = i < guiPtrs.activeElementsOnPage_3;
		if (!isEnabled) break;
		bool isBottomSlider = i == guiPtrs.activeElementsOnPage_3 - 1;
		int valueOfSlider = guiPtrs.sliderOnPage_3[i]->value();
		QString txtToSet;
		if (isBottomSlider) {
			txtToSet = QString::number(0) + "% - " + QString::number(valueOfSlider) + "%";
			rangesOfSlidersValue.push_back(txtToSet);
			continue;
		}
		else {
			int valueOfPreviousSlider = guiPtrs.sliderOnPage_3[i + 1]->value();
			txtToSet = QString::number(valueOfPreviousSlider) + "% - " + QString::number(valueOfSlider) + "%";
			rangesOfSlidersValue.push_back(txtToSet);
			continue;
		}
	}

	for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) {
		bool isEnabled = guiPtrs.labelsOnPage_3[i]->isEnabled();
		bool sliderExist = rangesOfSlidersValue.size() > i;
		QString strToSet = (isEnabled && sliderExist) ? rangesOfSlidersValue[i] : QString("");
		guiPtrs.labelsOnPage_3[i]->setText(strToSet);
	}
}

void NewProfileConfiguartor::_4_ManaEnabledChanged() {
	bool enabled = ui->_4_enableManaRestore;
	ui->_4_spinGetNumberOfMethodes->setEnabled(enabled);
	if (!enabled) {
		ui->_4_spinGetNumberOfMethodes->setValue(0);
		ui->_4_spinGetNumberOfMethodes->setEnabled(false);
	}
	_4_spinChanged();
}

void NewProfileConfiguartor::_4_spinChanged() {
	guiPtrs.activeElementsOnPage_4 = ui->_4_spinGetNumberOfMethodes->value();
	for (int i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) {
		bool enable = i < guiPtrs.activeElementsOnPage_4;
		guiPtrs.boxesOnPage_4[i]->setEnabled(enable);
		guiPtrs.keyShortCutsOnPage_4[i]->setEnabled(enable);
		guiPtrs.labelsOnPage_4[i]->setEnabled(enable);
		guiPtrs.sliderOnPage_4[i]->setEnabled(enable);
		if (!enable) {
			guiPtrs.sliderOnPage_4[i]->setValue(0);
			guiPtrs.labelsOnPage_4[i]->setText("");
			guiPtrs.boxesOnPage_4[i]->setCurrentIndex(-1);
			//guiPtrs.keyShortCutsOnPage_4[i]->clear();
		}
	}
}

void NewProfileConfiguartor::_4_slidersChanged() {
	QStringList rangesOfSlidersValue;
	for (int i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) {
		bool isEnabled = i < guiPtrs.activeElementsOnPage_4;
		if (!isEnabled) break;
		bool isBottomSlider = i == guiPtrs.activeElementsOnPage_4 - 1;
		int valueOfSlider = guiPtrs.sliderOnPage_4[i]->value();
		QString txtToSet;
		if (isBottomSlider) {
			txtToSet = QString::number(0) + "% - " + QString::number(valueOfSlider) + "%";
			rangesOfSlidersValue.push_back(txtToSet);
			continue;
		}
		else {
			int valueOfPreviousSlider = guiPtrs.sliderOnPage_4[i + 1]->value();
			txtToSet = QString::number(valueOfPreviousSlider) + "% - " + QString::number(valueOfSlider) + "%";
			rangesOfSlidersValue.push_back(txtToSet);
			continue;
		}
	}

	for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) {
		bool isEnabled = guiPtrs.labelsOnPage_4[i]->isEnabled();
		bool sliderExist = rangesOfSlidersValue.size() > i;
		QString strToSet = (isEnabled && sliderExist) ? rangesOfSlidersValue[i] : QString("");
		guiPtrs.labelsOnPage_4[i]->setText(strToSet);
	}
}

void NewProfileConfiguartor::_5_listAction(){
	if (ui->_5_ControlBox->currentIndex() == Profile::CONTROLS::CLSSIC_CONTROLS) {
		ui->_5_AutoLootBox->setEnabled(true);
		ui->_5_AutoLootBox->setCurrentIndex(0);
	}
	else {
		ui->_5_AutoLootBox->setEnabled(false);
		ui->_5_AutoLootBox->setCurrentIndex(0);
	}
	ui->_5_AutoLootBox->repaint();
}
