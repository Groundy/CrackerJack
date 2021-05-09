#include "NewProfileConfiguartor.h"
#include "ui_NewProfileConfiguartor.h"
#include <Key.h>

NewProfileConfiguartor::NewProfileConfiguartor(Profile* prof, QWidget *parent)	: 
	QDialog(parent) {
	ui = new Ui::NewProfileConfiguartor();
	ui->setupUi(this); 
	MAX_PAGE = ui->stackedWidget->count();
	ui->stackedWidget->setCurrentIndex(0);
	additionalGuiSettings();
	profToEdit = prof;
	refreshGUI();
}

NewProfileConfiguartor::~NewProfileConfiguartor(){
	delete ui;
}

void NewProfileConfiguartor::refreshGUI(){
	QString labelPageInfoText = "Page " + QString::number(pageNumber) + "/" + QString::number(MAX_PAGE);
	ui->pageLabel->setText(labelPageInfoText);
	ui->pageLabel->repaint();

	bool firstPage = pageNumber == 1 ? true : false;
	ui->previousButton->setEnabled(!firstPage);

	bool lastPage = pageNumber == MAX_PAGE ? true : false; 
	if (lastPage)
		ui->nextButton->setText("finish");
	else
		ui->nextButton->setText("Next Page");

	ui->stackedWidget->repaint();
}

bool NewProfileConfiguartor::finishAddingNewProfile(){
	bool accepted = Utilities::showMessageBox_NO_YES("finish creating new profile","Are you sure that you want finish creating new profile?");
	if (accepted) {
		saveDataToProfile(profToEdit);
		this->accept();
	}
	else
		return false;
}

void NewProfileConfiguartor::additionalGuiSettings(){
	ui->_3_spinGetNumberOfMethodes->setMinimum(0);
	ui->_3_spinGetNumberOfMethodes->setMaximum(5);
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
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_1_TooLongName(), QMessageBox::Ok);
		return false;
	}
	if (nameIsTooShort) {
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_1_TooShortName(), QMessageBox::Ok);
		return false;
	}
	if (nameConsistForbiddenChars) {
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_1_ForbiddenChars(), QMessageBox::Ok);
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
		Utilities::showMessageBox("CracerJack problem", StringResource::NewProfileConfig_2_anyProfIsChecked(), QMessageBox::Ok);
		return false;
	}

}

bool NewProfileConfiguartor::checkCorrectnessOfPage_3(){
	int numberOfSliders = ui->_3_spinGetNumberOfMethodes->value();

	QList<QAbstractSlider*> sliders;
	sliders.push_back(ui->_3_horizontalSlider_1);
	sliders.push_back(ui->_3_horizontalSlider_2);
	sliders.push_back(ui->_3_horizontalSlider_3);
	sliders.push_back(ui->_3_horizontalSlider_4);
	sliders.push_back(ui->_3_horizontalSlider_5);

	QList<QAbstractSlider*> activeSliders;
	for (size_t i = 0; i < numberOfSliders; i++)
		activeSliders.push_back(sliders[i]);

	bool slidersAreInCorrectOrder = true;
	bool everySliderHasDiffrentValue = true;
	int biggestValue = 101;

	for each (QAbstractSlider* slider in activeSliders) {
		if (slider->value() < biggestValue)
			biggestValue = slider->value();
		else if (slider->value() == biggestValue) 
			everySliderHasDiffrentValue = false;
		else 
			slidersAreInCorrectOrder = false;
	}


	QList<QKeySequence> keys;
	keys.push_back(ui->_3_shortKey_1->keySequence());
	keys.push_back(ui->_3_shortKey_2->keySequence());
	keys.push_back(ui->_3_shortKey_3->keySequence());
	keys.push_back(ui->_3_shortKey_4->keySequence());
	keys.push_back(ui->_3_shortKey_5->keySequence());

	bool oneOfKeyFieldsHasManyValues = false;
	bool oneOfKeyFieldsHasNoValue = false;
	bool oneOfKeyFieldsHasForbiddenChars = false;

	int size = ui->_3_spinGetNumberOfMethodes->value();
	for (int i = 0; i < size; i++) {
		if (keys[i].count() == 0) {
			oneOfKeyFieldsHasNoValue = true;
		}
		else if (keys[i].count() > 1) {
			oneOfKeyFieldsHasManyValues = true;
		}
	}

	for (int i = 0; i < size; i++) {
		int keyValurToCheck = Key(keys[i]).number;
		bool isProperKey = Key::checkIfnumberIsAloowed(keyValurToCheck);
		if (!isProperKey)
			oneOfKeyFieldsHasForbiddenChars = true;
	}

	if (!everySliderHasDiffrentValue) {
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_SlidersAreInTheSamePosition(), QMessageBox::Ok);
		return false;
	}
	if (!slidersAreInCorrectOrder) {
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_SlidersAreInWrongOrder(), QMessageBox::Ok);
		return false;
	}
	if (oneOfKeyFieldsHasManyValues) {
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_ShortcutManyValue(), QMessageBox::Ok);
		return false;
	}
	if (oneOfKeyFieldsHasNoValue) {
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_ShortcutNoValue(), QMessageBox::Ok);
		return false;
	}
	if (oneOfKeyFieldsHasForbiddenChars) {
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_ShortCutHasForbiddenChars(), QMessageBox::Ok);
		return false;
	}
	return true;
}

bool NewProfileConfiguartor::checkCorrectnessOfPage_4(){
	int numberOfSliders = ui->_4_spinGetNumberOfMethodes->value();

	QList<QAbstractSlider*> sliders;
	sliders.push_back(ui->_4_horizontalSlider_1);
	sliders.push_back(ui->_4_horizontalSlider_2);
	sliders.push_back(ui->_4_horizontalSlider_3);
	sliders.push_back(ui->_4_horizontalSlider_4);
	sliders.push_back(ui->_4_horizontalSlider_5);

	QList<QAbstractSlider*> activeSliders;
	for (size_t i = 0; i < numberOfSliders; i++)
		activeSliders.push_back(sliders[i]);

	bool slidersAreInCorrectOrder = true;
	bool everySliderHasDiffrentValue = true;
	int biggestValue = 101;
	for each (QAbstractSlider * slider in activeSliders) {
		if (slider->value() < biggestValue)
			biggestValue = slider->value();
		else if (slider->value() == biggestValue) {
			everySliderHasDiffrentValue = false;
		}
		else {
			slidersAreInCorrectOrder = false;
		}
	}

	QList<QKeySequence> keys;
	keys.push_back(ui->_4_shortKey_1->keySequence());
	keys.push_back(ui->_4_shortKey_2->keySequence());
	keys.push_back(ui->_4_shortKey_3->keySequence());
	keys.push_back(ui->_4_shortKey_4->keySequence());
	keys.push_back(ui->_4_shortKey_5->keySequence());

	bool oneOfKeyFieldHasManyValues = false;
	bool oneOfKeyFieldHasNoValue = false;
	bool oneOfKeyFieldsHasForbiddenChars = false;

	int size = ui->_4_spinGetNumberOfMethodes->value();
	for (int i = 0; i < size; i++) {
		if (keys[i].count() == 0) {
			oneOfKeyFieldHasNoValue = true;
		}
		else if (keys[i].count() > 1) {
			oneOfKeyFieldHasManyValues = true;
		}
	}

	for (int i = 0; i < size; i++) {
		int keyValurToCheck = Key(keys[i]).number;
		bool isProperKey = Key::checkIfnumberIsAloowed(keyValurToCheck);
		if (!isProperKey)
			oneOfKeyFieldsHasForbiddenChars = true;
	}


	if (!everySliderHasDiffrentValue) {
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_SlidersAreInTheSamePosition(), QMessageBox::Ok);
		return false;
	}
	if (!slidersAreInCorrectOrder) {
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_SlidersAreInWrongOrder(), QMessageBox::Ok);
		return false;
	}
	if (oneOfKeyFieldHasManyValues) {
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_ShortcutManyValue(), QMessageBox::Ok);
		return false;
	}
	if (oneOfKeyFieldHasNoValue) {
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_ShortcutNoValue(), QMessageBox::Ok);
		return false;
	}
	if (oneOfKeyFieldsHasForbiddenChars) {
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_ShortCutHasForbiddenChars(), QMessageBox::Ok);
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
	QList<Utilities::Potion> potions;
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
	QList<Utilities::Potion> potions;
	parser.getPotionsForProf(potions, &prof, JsonParser::TypeOfPotion::MANA);

	QStringList namesOfAvaibleManaRestoreMethodes;
	for each (auto var in potions)
		namesOfAvaibleManaRestoreMethodes.push_back(var.name);

	return namesOfAvaibleManaRestoreMethodes;
}

void NewProfileConfiguartor::fillGuiElementsWithNamesRestoreMethodes_Health(Profile::PROFESSION profession){
	QList<QComboBox*> listOfWidgets;
	listOfWidgets.push_back(ui->_3_comboBox);
	listOfWidgets.push_back(ui->_3_comboBox_2);
	listOfWidgets.push_back(ui->_3_comboBox_3);
	listOfWidgets.push_back(ui->_3_comboBox_4);
	listOfWidgets.push_back(ui->_3_comboBox_5);


	QStringList listOfMethodesNamesFromProf = profToEdit->healthRestoreMethodeNames;
	QStringList namesOfAllPosibleMethodes = getNamesOfHealthRestoringMethodes(profession);
	QStringList incantationsThatShouldBeDeleted{"utura", "utura gran", "exura sio \"name\"",
	"exana pox", "utura mas sio", "exana flam", "exana kor", "exana vis", "exura gran sio \"name\"",
	"exana mort"};

	for each (QString var in incantationsThatShouldBeDeleted)
		namesOfAllPosibleMethodes.removeOne(var);
	namesOfAllPosibleMethodes.sort();

	int numberOfComboBoxToSetActive = listOfMethodesNamesFromProf.size();
	for (size_t i = 0; i < 5; i++){
		QComboBox* box = listOfWidgets[i];
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
	QList<QComboBox*> listOfWidgets;
	listOfWidgets.push_back(ui->_4_comboBox);
	listOfWidgets.push_back(ui->_4_comboBox_2);
	listOfWidgets.push_back(ui->_4_comboBox_3);
	listOfWidgets.push_back(ui->_4_comboBox_4);
	listOfWidgets.push_back(ui->_4_comboBox_5);


	QStringList listOfMethodesNamesFromProf = profToEdit->manaRestoreMethodeNames;
	QStringList namesOfAllPosibleMethodes = getNamesOfManaRestoringMethodes(prof);
	namesOfAllPosibleMethodes.sort();

	int numberOfComboBoxToSetActive = listOfMethodesNamesFromProf.size();
	for (size_t i = 0; i < 5; i++) {
		QComboBox* box = listOfWidgets[i];
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
		int numberOfHealthSliders = profToEdit->healthRestorePercentages.size();
		QList<QAbstractSlider*> sliders;
		QList<QComboBox*> boxes;
		QList<QKeySequenceEdit*> keyEdit;
		QList<QLabel*> labels;

		{
			sliders.push_back(ui->_3_horizontalSlider_1);
			sliders.push_back(ui->_3_horizontalSlider_2);
			sliders.push_back(ui->_3_horizontalSlider_3);
			sliders.push_back(ui->_3_horizontalSlider_4);
			sliders.push_back(ui->_3_horizontalSlider_5);
			boxes.push_back(ui->_3_comboBox);
			boxes.push_back(ui->_3_comboBox_2);
			boxes.push_back(ui->_3_comboBox_3);
			boxes.push_back(ui->_3_comboBox_4);
			boxes.push_back(ui->_3_comboBox_5);
			keyEdit.push_back(ui->_3_shortKey_1);
			keyEdit.push_back(ui->_3_shortKey_2);
			keyEdit.push_back(ui->_3_shortKey_3);
			keyEdit.push_back(ui->_3_shortKey_4);
			keyEdit.push_back(ui->_3_shortKey_5);
			labels.push_back(ui->_3_label_1);
			labels.push_back(ui->_3_label_2);
			labels.push_back(ui->_3_label_3);
			labels.push_back(ui->_3_label_4);
			labels.push_back(ui->_3_label_5);
		}

		if (numberOfHealthSliders >= 1) {
			ui->_3_enableAutoHealing->setEnabled(true);
			ui->_3_enableAutoHealing->setChecked(true);
			ui->_3_spinGetNumberOfMethodes->setEnabled(true);
			ui->_3_spinGetNumberOfMethodes->setValue(numberOfHealthSliders);
		}
		for (size_t i = 0; i < numberOfHealthSliders; i++) {
			sliders[i]->setEnabled(true);
			labels[i]->setEnabled(true);
			keyEdit[i]->setEnabled(true);
			boxes[i]->setEnabled(true);

			sliders[i]->setValue(profToEdit->healthRestorePercentages[i]);
			labels[i]->setText(QString::number(profToEdit->healthRestorePercentages[i]) + "%");
			int indexToSet = profToEdit->healthRestoreMethodeNames.indexOf(profToEdit->healthRestoreMethodeNames[i]);
			boxes[i]->setCurrentIndex(indexToSet);
			QKeySequence toSet(profToEdit->healthKeys[i].toQKeySequence(profToEdit->healthKeys[i]));
			keyEdit[i]->setKeySequence(toSet);
		}
	}
	//4
	{
		int numberOfManaSliders = profToEdit->ManaRestoreMethodesPercentage.size();
		QList<QAbstractSlider*> sliders;
		QList<QComboBox*> boxes;
		QList<QKeySequenceEdit*> keyEdit;
		QList<QLabel*> labels;
		sliders.push_back(ui->_4_horizontalSlider_1);
		sliders.push_back(ui->_4_horizontalSlider_2);
		sliders.push_back(ui->_4_horizontalSlider_3);
		sliders.push_back(ui->_4_horizontalSlider_4);
		sliders.push_back(ui->_4_horizontalSlider_5);
		boxes.push_back(ui->_4_comboBox);
		boxes.push_back(ui->_4_comboBox_2);
		boxes.push_back(ui->_4_comboBox_3);
		boxes.push_back(ui->_4_comboBox_4);
		boxes.push_back(ui->_4_comboBox_5);
		keyEdit.push_back(ui->_4_shortKey_1);
		keyEdit.push_back(ui->_4_shortKey_2);
		keyEdit.push_back(ui->_4_shortKey_3);
		keyEdit.push_back(ui->_4_shortKey_4);
		keyEdit.push_back(ui->_4_shortKey_5);
		labels.push_back(ui->_4_label_1);
		labels.push_back(ui->_4_label_2);
		labels.push_back(ui->_4_label_3);
		labels.push_back(ui->_4_label_4);
		labels.push_back(ui->_4_label_5);
		if (numberOfManaSliders >= 1) {
			ui->_4_enableManaRestore->setEnabled(true);
			ui->_4_enableManaRestore->setChecked(true);
			ui->_4_spinGetNumberOfMethodes->setEnabled(true);
			ui->_4_spinGetNumberOfMethodes->setValue(numberOfManaSliders);
		}
		for (size_t i = 0; i < numberOfManaSliders; i++) {
			sliders[i]->setEnabled(true);
			labels[i]->setEnabled(true);
			keyEdit[i]->setEnabled(true);
			boxes[i]->setEnabled(true);

			sliders[i]->setValue(profToEdit->ManaRestoreMethodesPercentage[i]);
			labels[i]->setText(QString::number(profToEdit->ManaRestoreMethodesPercentage[i]) + "%");
			int indexToSet = profToEdit->manaRestoreMethodeNames.indexOf(profToEdit->manaRestoreMethodeNames[i]);
			boxes[i]->setCurrentIndex(indexToSet);
			QKeySequence toSet(profToEdit->ManaKeys[i].toQKeySequence(profToEdit->ManaKeys[i]));
			keyEdit[i]->setKeySequence(toSet);
		}
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
	QFlags<QMessageBox::StandardButton> flags = {QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No};
	int res = Utilities::showMessageBox("Cancel adding new profile", "Are you sure that you want cancel adding new profile?", flags);
	if(res == QMessageBox::StandardButton::Yes)
		this->reject();
}

void NewProfileConfiguartor::helpButtonAction() {
	QString helpText = StringResource::NewProfileConfigHelp(pageNumber);
	Utilities::showMessageBox("CrackerJack help", helpText, QMessageBox::StandardButton::Ok);
}

void NewProfileConfiguartor::_3_healingEnabledChanged(){
	bool enabled = ui->_3_enableAutoHealing->isChecked() ? true : false;
	ui->_3_spinGetNumberOfMethodes->setEnabled(enabled);
	if (!enabled) 
		ui->_3_spinGetNumberOfMethodes->setValue(0);
	
}

void NewProfileConfiguartor::_3_spinChanged() {
	QVector<QWidget*> guiElements;
	guiElements.push_back(ui->_3_horizontalSlider_1);
	guiElements.push_back(ui->_3_label_1);
	guiElements.push_back(ui->_3_shortKey_1);
	guiElements.push_back(ui->_3_comboBox);

	guiElements.push_back(ui->_3_label_2);
	guiElements.push_back(ui->_3_horizontalSlider_2);
	guiElements.push_back(ui->_3_shortKey_2);
	guiElements.push_back(ui->_3_comboBox_2);

	guiElements.push_back(ui->_3_label_3);
	guiElements.push_back(ui->_3_horizontalSlider_3);
	guiElements.push_back(ui->_3_shortKey_3);
	guiElements.push_back(ui->_3_comboBox_3);


	guiElements.push_back(ui->_3_label_4);
	guiElements.push_back(ui->_3_horizontalSlider_4);
	guiElements.push_back(ui->_3_shortKey_4);
	guiElements.push_back(ui->_3_comboBox_4);


	guiElements.push_back(ui->_3_label_5);
	guiElements.push_back(ui->_3_horizontalSlider_5);
	guiElements.push_back(ui->_3_shortKey_5);
	guiElements.push_back(ui->_3_comboBox_5);


	int valueOfSpin = ui->_3_spinGetNumberOfMethodes->value();

	for (int i = 0; i < guiElements.size(); i++) {
		bool enable = i < valueOfSpin * 4 ? true : false;
		guiElements[i]->setEnabled(enable);
	}
}
	
void NewProfileConfiguartor::_3_slidersChanged() {
	QVector<QLabel*> labels;
	labels.push_back(ui->_3_label_1);
	labels.push_back(ui->_3_label_2);
	labels.push_back(ui->_3_label_3);
	labels.push_back(ui->_3_label_4);
	labels.push_back(ui->_3_label_5);

	QVector<QAbstractSlider*> sliders;
	sliders.push_back(ui->_3_horizontalSlider_1);
	sliders.push_back(ui->_3_horizontalSlider_2);
	sliders.push_back(ui->_3_horizontalSlider_3);
	sliders.push_back(ui->_3_horizontalSlider_4);
	sliders.push_back(ui->_3_horizontalSlider_5);

	int valueOfSpin = ui->_3_spinGetNumberOfMethodes->value();
	for (int i = 0; i < labels.size(); i++){
		QString txt = QString::number(sliders[i]->value()) + "%";
		labels[i]->setText(txt);
	}
}

void NewProfileConfiguartor::_4_ManaEnabledChanged() {
	bool enabled = ui->_4_enableManaRestore ? true : false;
	ui->_4_spinGetNumberOfMethodes->setEnabled(enabled);
	if (!enabled)
		ui->_4_spinGetNumberOfMethodes->setValue(0);
}

void NewProfileConfiguartor::_4_spinChanged() {
	QVector<QWidget*> guiElements;
	guiElements.push_back(ui->_4_horizontalSlider_1);
	guiElements.push_back(ui->_4_label_1);
	guiElements.push_back(ui->_4_shortKey_1);

	guiElements.push_back(ui->_4_label_2);
	guiElements.push_back(ui->_4_horizontalSlider_2);
	guiElements.push_back(ui->_4_shortKey_2);

	guiElements.push_back(ui->_4_label_3);
	guiElements.push_back(ui->_4_horizontalSlider_3);
	guiElements.push_back(ui->_4_shortKey_3);

	guiElements.push_back(ui->_4_label_4);
	guiElements.push_back(ui->_4_horizontalSlider_4);
	guiElements.push_back(ui->_4_shortKey_4);

	guiElements.push_back(ui->_4_label_5);
	guiElements.push_back(ui->_4_horizontalSlider_5);
	guiElements.push_back(ui->_4_shortKey_5);

	int valueOfSpin = ui->_4_spinGetNumberOfMethodes->value();

	for (int i = 0; i < guiElements.size(); i++) {
		bool enable = i < valueOfSpin * 3 ? true : false;
		guiElements[i]->setEnabled(enable);
	}
}

void NewProfileConfiguartor::_4_slidersChanged() {
	QVector<QLabel*> labels;
	labels.push_back(ui->_4_label_1);
	labels.push_back(ui->_4_label_2);
	labels.push_back(ui->_4_label_3);
	labels.push_back(ui->_4_label_4);
	labels.push_back(ui->_4_label_5);

	QVector<QAbstractSlider*> sliders;
	sliders.push_back(ui->_4_horizontalSlider_1);
	sliders.push_back(ui->_4_horizontalSlider_2);
	sliders.push_back(ui->_4_horizontalSlider_3);
	sliders.push_back(ui->_4_horizontalSlider_4);
	sliders.push_back(ui->_4_horizontalSlider_5);

	int valueOfSpin = ui->_4_spinGetNumberOfMethodes->value();
	for (int i = 0; i < labels.size(); i++) {
		QString txt = QString::number(sliders[i]->value()) + "%";
		labels[i]->setText(txt);
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
