#include "NewProfileConfiguartor.h"
#include "ui_NewProfileConfiguartor.h"
#include <Key.h>

NewProfileConfiguartor::NewProfileConfiguartor(Profile* prof, QWidget *parent)	: 
	QDialog(parent) {
	ui = new Ui::NewProfileConfiguartor();
	ui->setupUi(this); 
	MAX_PAGE = ui->stackedWidget->count();
	ui->stackedWidget->setCurrentIndex(0);
	fillGuiPtrs();
	additionalGuiSettings();
	profToEdit = prof;
	refreshGUI();
}

NewProfileConfiguartor::~NewProfileConfiguartor(){
	delete ui;
}

void NewProfileConfiguartor::refreshGUI(){
	QString page = tr("Page ");
	QString labelPageInfoText = page + QString::number(pageNumber) + "/" + QString::number(MAX_PAGE);
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

	for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) {
		guiPtrs.sliderOnPage_3[i]->setEnabled(false);
		guiPtrs.labelsOnPage_3[i]->setEnabled(false);
		guiPtrs.labelsOnPage_3[i]->setText("       ");
		guiPtrs.boxesOnPage_3[i]->setEnabled(false);
		guiPtrs.keyShortCutsOnPage_3[i]->setEnabled(false);
		guiPtrs.boxesOnPage_3[i]->setCurrentIndex(-1);

		guiPtrs.sliderOnPage_4[i]->setEnabled(false);
		guiPtrs.labelsOnPage_4[i]->setText("       ");
		guiPtrs.labelsOnPage_4[i]->setEnabled(false);
		guiPtrs.boxesOnPage_4[i]->setEnabled(false);
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
	bool toRet;
	switch (pageNumber) {
	case 1: {
		 toRet = checkCorrectnessOfPage_1();
		 break;
	}
	case 2: {
		toRet = checkCorrectnessOfPage_2();
		break;
	}
	case 3: {
		toRet = checkCorrectnessOfPage_3();
		break;
	}
	case 4: {
		toRet = checkCorrectnessOfPage_4();
		break;
	}
	case 5: {
		toRet = checkCorrectnessOfPage_5();
		break;
	}
	default: {
		toRet = true;
		break;
	}  
	} 
	return toRet;
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

		QList<QKeySequence> keys;
		keys.push_back(ui->_3_shortKey_1->keySequence());
		keys.push_back(ui->_3_shortKey_2->keySequence());
		keys.push_back(ui->_3_shortKey_3->keySequence());
		keys.push_back(ui->_3_shortKey_4->keySequence());
		keys.push_back(ui->_3_shortKey_5->keySequence());

		prof->healthKeys.clear();
		for (size_t i = 0; i < size; i++)
			prof->healthKeys.push_back(keys[i]);

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


		QList<QKeySequence> keys;
		keys.push_back(ui->_4_shortKey_1->keySequence());
		keys.push_back(ui->_4_shortKey_2->keySequence());
		keys.push_back(ui->_4_shortKey_3->keySequence());
		keys.push_back(ui->_4_shortKey_4->keySequence());
		keys.push_back(ui->_4_shortKey_5->keySequence());

		prof->ManaKeys.clear();
		for (size_t i = 0; i < size; i++)
			prof->ManaKeys.push_back(keys[i]);

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
	bool nameIsTooShort = nameOfProf.size() < 3;
	bool nameConsistForbiddenChars = false;
	if (nameOfProf.contains(QChar::LineFeed) || nameOfProf.contains(QChar::CarriageReturn))
		nameConsistForbiddenChars = true;
	for each (auto character in nameOfProf) {
		if (!character.isLetterOrNumber() && !character.isSpace())
			nameConsistForbiddenChars = true;
	}

	if (nameisTooLong) {
		QString text = tr("Profile name can't be longer than 50 characters.");
		Utilities::showMessageBox_INFO(text);
		return false;
	}
	if (nameIsTooShort) {
		QString text = tr("Profile name can't be shorter than 3 characters.");
		Utilities::showMessageBox_INFO(text);
		return false;
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
			QKeySequence keySequence = guiPtrs.keyShortCutsOnPage_3[i]->keySequence();
			if (keySequence.count() == 0)
				oneOfKeyFieldsHasNoValue = true;
			else if (keySequence.count() > 1)
				oneOfKeyFieldsHasManyValues = true;

			int keyValueToCheck = Key(keySequence).number;
			bool isProperKey = Key::checkIfnumberIsAloowed(keyValueToCheck);
			if (!isProperKey)
				oneOfKeyFieldsHasForbiddenChars = true;
		
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
		QMap<int, int> mapToDeleteReplicatedValues;
		int maxWidgetIndex = guiPtrs.activeElementsOnPage_3;
		for (size_t i = 0; i < maxWidgetIndex; i++) {
			QKeySequence keySeq = guiPtrs.keyShortCutsOnPage_3[i]->keySequence();
			int valueOfKey = Key(keySeq).number;
			mapToDeleteReplicatedValues.insert(valueOfKey, 0);
		}
		bool allWidgetsAreDiffrent = mapToDeleteReplicatedValues.size() == maxWidgetIndex;
		bool AtLeastOneIsActive = maxWidgetIndex > 0;
		if(AtLeastOneIsActive)
			theSameValueIsAssignedToMoreThanOneBox = !allWidgetsAreDiffrent;
	}

	bool theSameKeyIsAssignedToMoreThanOneKeyShortCut = false;
	if(guiPtrs.activeElementsOnPage_3 > 0){
		QMap<int, int> keyNumber_noImportnat_map;
		for (size_t i = 0; i < guiPtrs.activeElementsOnPage_3; i++){
			int keyNumber = Key(guiPtrs.keyShortCutsOnPage_3[i]->keySequence()).number;
			keyNumber_noImportnat_map.insert(keyNumber,0);
		}
		bool allKeysAreDiffrent = keyNumber_noImportnat_map.size() == guiPtrs.activeElementsOnPage_3;
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
			QKeySequence keySequence = guiPtrs.keyShortCutsOnPage_4[i]->keySequence();
			if (keySequence.count() == 0)
				oneOfKeyFieldsHasNoValue = true;
			else if (keySequence.count() > 1)
				oneOfKeyFieldsHasManyValues = true;

			int keyValueToCheck = Key(keySequence).number;
			bool isProperKey = Key::checkIfnumberIsAloowed(keyValueToCheck);
			if (!isProperKey)
				oneOfKeyFieldsHasForbiddenChars = true;

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
		QMap<int, int> mapToDeleteReplicatedValues;
		int maxWidgetIndex = guiPtrs.activeElementsOnPage_4;
		for (size_t i = 0; i < maxWidgetIndex; i++) {
			QKeySequence keySeq = guiPtrs.keyShortCutsOnPage_4[i]->keySequence();
			int valueOfKey = Key(keySeq).number;
			mapToDeleteReplicatedValues.insert(valueOfKey, 0);
		}
		bool allWidgetsAreDiffrent = mapToDeleteReplicatedValues.size() == maxWidgetIndex;
		bool AtLeastOneIsActive = maxWidgetIndex > 0;
		if (AtLeastOneIsActive)
			theSameValueIsAssignedToMoreThanOneBox = !allWidgetsAreDiffrent;
	}

	bool theSameKeyIsAssignedToMoreThanOneKeyShortCut = false;
	if(guiPtrs.activeElementsOnPage_4 > 0){
		QMap<int, int> keyNumber_noImportnat_map;
		for (size_t i = 0; i < guiPtrs.activeElementsOnPage_4; i++) {
			int keyNumber = Key(guiPtrs.keyShortCutsOnPage_4[i]->keySequence()).number;
			keyNumber_noImportnat_map.insert(keyNumber, 0);
		}
		bool allKeysAreDiffrent = keyNumber_noImportnat_map.size() == guiPtrs.activeElementsOnPage_4;
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
	Profile::PROFESSION toRet;
	if (ui->_2_RadButt_ED->isChecked())
		toRet = Profile::PROFESSION::ED;
	else if (ui->_2_RadButt_EK->isChecked())
		toRet = Profile::PROFESSION::EK;
	else if (ui->_2_RadButt_MS->isChecked())
		toRet = Profile::PROFESSION::MS;
	else if (ui->_2_RadButt_RP->isChecked())
		toRet = Profile::PROFESSION::RP;
	return toRet;
}

void NewProfileConfiguartor::fillGuiPtrs() {
	int tmpNumberOfActiveWidgets = 0;
	if (ui->_4_horizontalSlider_1->isEnabled())
		tmpNumberOfActiveWidgets++;
	if (ui->_4_horizontalSlider_2->isEnabled())
		tmpNumberOfActiveWidgets++;
	if (ui->_4_horizontalSlider_3->isEnabled())
		tmpNumberOfActiveWidgets++;
	if (ui->_4_horizontalSlider_4->isEnabled())
		tmpNumberOfActiveWidgets++;
	if (ui->_4_horizontalSlider_5->isEnabled())
		tmpNumberOfActiveWidgets++;
	this->guiPtrs.activeElementsOnPage_4 = tmpNumberOfActiveWidgets;

	tmpNumberOfActiveWidgets = 0;
	if (ui->_3_horizontalSlider_1->isEnabled())
		tmpNumberOfActiveWidgets++;
	if (ui->_3_horizontalSlider_2->isEnabled())
		tmpNumberOfActiveWidgets++;
	if (ui->_3_horizontalSlider_3->isEnabled())
		tmpNumberOfActiveWidgets++;
	if (ui->_3_horizontalSlider_4->isEnabled())
		tmpNumberOfActiveWidgets++;
	if (ui->_3_horizontalSlider_5->isEnabled())
		tmpNumberOfActiveWidgets++;
	this->guiPtrs.activeElementsOnPage_3 = tmpNumberOfActiveWidgets;


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
	case Profile::RP:	ui->_2_RadButt_RP->setChecked(true);	break;
	case Profile::EK:	ui->_2_RadButt_EK->setChecked(true);	break;
	case Profile::ED:	ui->_2_RadButt_ED->setChecked(true);	break;
	case Profile::MS:	ui->_2_RadButt_MS->setChecked(true);	break;
	default:	ui->_2_RadButt_RP->setChecked(true);	break;
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
				QKeySequence toSet(profToEdit->healthKeys[i].toQKeySequence(profToEdit->healthKeys[i]));
				guiPtrs.keyShortCutsOnPage_3[i]->setKeySequence(toSet);
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
				QKeySequence toSet(profToEdit->healthKeys[i].toQKeySequence(profToEdit->healthKeys[i]));
				guiPtrs.keyShortCutsOnPage_4[i]->setKeySequence(toSet);
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

void NewProfileConfiguartor::helpButtonAction() {
	QString helpText;
	switch (pageNumber)
	{
	case 1:
		helpText = tr("Enter profile name.");
		break;
	case 2:
		helpText = tr("Choose your profession.");
		break;
	case 3:
		helpText = tr("Choose how many methodes of restoring health do want, up to 5 diffrent methodes. ");
		helpText.append(tr("Move sliders to set value of missing health that will be used to auto heal. "));
		helpText.append(tr("First Slider should be used for smallest healing, last for biggest. "));
		helpText.append(tr("If healing would be needed, program will use appropriate healing to your remaining health. "));
		helpText.append(tr("To all used sliders you have to add the same hotkey that use in game. "));
		helpText.append(tr("If most effective healing methode is not avaible(no enough mana, no potion), program will try to another, less efficient methode."));
		break;
	case 4:
		helpText = (tr("Choose how many methodes of restoring mana do want, up to 5 diffrent methodes. "));
		helpText.append(tr("Move sliders to set value of missing mana that will be used to auto mana restore. "));
		helpText.append(tr("First Slider should be used for smallest mana regeneration, last for biggest. "));
		helpText.append(tr("If mana will be low, program will use appropriate methode to regenerate it. "));
		helpText.append(tr("To all used sliders you have to add the same hotkey that use in game. "));
		helpText.append(tr("Healing potions have priority over mana potions. "));
		break;
	default:
		break;
	}
	Utilities::showMessageBox_INFO(helpText);
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
			guiPtrs.keyShortCutsOnPage_3[i]->clear();
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
			guiPtrs.keyShortCutsOnPage_4[i]->clear();
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
