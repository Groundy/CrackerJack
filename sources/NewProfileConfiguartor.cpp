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

NewProfileConfiguartor::~NewProfileConfiguartor()
{
	delete ui;
}

void NewProfileConfiguartor::refreshGUI(){
	QString labelPageInfoText = "Page " + QString::number(pageNumber) + "/" + QString::number(MAX_PAGE);
	ui->pageLabel->setText(labelPageInfoText);
	ui->pageLabel->repaint();

	bool firstPage = pageNumber == 1 ? true : false;
	ui->previousButton->setEnabled(!firstPage);

	bool lastPage = pageNumber == MAX_PAGE ? true : false; 
	if (lastPage) {
		ui->nextButton->setText("finish");
	}
	else{
		ui->nextButton->setText("Next Page");
	}

	ui->stackedWidget->repaint();
}

void NewProfileConfiguartor::finishAddingNewProfile(){
	bool accepted = Utilities::showMessageBox_NO_YES("finish creating new profile","Are you sure that you want finish creating new profile?");
	if (accepted) {
		saveDataToProfile(profToEdit);
		this->accept();
	}
}

void NewProfileConfiguartor::additionalGuiSettings(){
	ui->_3_spinGetNumberOfMethodes->setMinimum(0);
	ui->_3_spinGetNumberOfMethodes->setMaximum(5);
	QStringList optionsHealth = { "POTION", "SPELL","CUPCAKE","OTHER" };
	ui->_3_comboBox->addItems(optionsHealth);
	ui->_3_comboBox_2->addItems(optionsHealth);
	ui->_3_comboBox_3->addItems(optionsHealth);
	ui->_3_comboBox_4->addItems(optionsHealth);
	ui->_3_comboBox_5->addItems(optionsHealth);
	//
	ui->_4_spinGetNumberOfMethodes->setMinimum(0);
	ui->_4_spinGetNumberOfMethodes->setMaximum(5);
	QStringList optionsMana =  { "POTION","CUPCAKE","OTHER" };
	ui->_4_comboBox->addItems(optionsMana);
	ui->_4_comboBox_2->addItems(optionsMana);
	ui->_4_comboBox_3->addItems(optionsMana);
	ui->_4_comboBox_4->addItems(optionsMana);
	ui->_4_comboBox_5->addItems(optionsMana);
	//
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
	//todo
	//return true;
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
	prof->profileName = ui->_1_nameEdit->toPlainText();
	//2
	if (ui->_2_RadButt_ED->isChecked())
		prof->profession = PROFESSION::ED;
	else if (ui->_2_RadButt_EK->isChecked())
		prof->profession = PROFESSION::EK;
	else if (ui->_2_RadButt_MS->isChecked())
		prof->profession = PROFESSION::MS;
	else if (ui->_2_RadButt_RP->isChecked())
		prof->profession = PROFESSION::RP;
	//3
	if (ui->_3_enableAutoHealing->isChecked()) {
		int size = ui->_3_spinGetNumberOfMethodes->value();
		QList<QAbstractSlider*> sliders;
		sliders.push_back(ui->_3_horizontalSlider_1);
		sliders.push_back(ui->_3_horizontalSlider_2);
		sliders.push_back(ui->_3_horizontalSlider_3);
		sliders.push_back(ui->_3_horizontalSlider_4);
		sliders.push_back(ui->_3_horizontalSlider_5);

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

		for (size_t i = 0; i < size; i++)
			prof->healthKeys.push_back(keys[i]);

		QList<int> key_Items;
		key_Items.push_back(ui->_3_comboBox->currentIndex());
		key_Items.push_back(ui->_3_comboBox_2->currentIndex());
		key_Items.push_back(ui->_3_comboBox_3->currentIndex());
		key_Items.push_back(ui->_3_comboBox_4->currentIndex());
		key_Items.push_back(ui->_3_comboBox_5->currentIndex());

		for (size_t i = 0; i < size; i++)
			prof->healthItems.push_back(KEY_ITEM(key_Items[i]));
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

		for (size_t i = 0; i < size; i++)
			prof->ManaKeys.push_back(keys[i]);

		QList<int> key_Items;
		key_Items.push_back(ui->_4_comboBox->currentIndex());
		key_Items.push_back(ui->_4_comboBox_2->currentIndex());
		key_Items.push_back(ui->_4_comboBox_3->currentIndex());
		key_Items.push_back(ui->_4_comboBox_4->currentIndex());
		key_Items.push_back(ui->_4_comboBox_5->currentIndex());

		for (size_t i = 0; i < size; i++)
			prof->ManaKeys.push_back(KEY_ITEM(key_Items[i]));
	}
	//5
}

bool NewProfileConfiguartor::checkCorrectnessOfPage_1(){
	QString nameOfProf = ui->_1_nameEdit->toPlainText();
	//QString nameOfCharacter = ui->_1_exactNameTextEdit->toPlainText();

	/*bool exactNameIsTooLong = nameOfCharacter.size() < 100 ? true : false;
	bool exactNameIsTooShort = nameOfCharacter.size() < 2  ? true : false;
	bool exactNameHasNewLineChars;
	if (nameOfProf.contains(QChar::LineFeed) || nameOfProf.contains(QChar::CarriageReturn))
		exactNameHasNewLineChars = true;
	else
		exactNameHasNewLineChars = false;
	*/
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
	/*
	if (exactNameIsTooShort) {
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_1_TooShortExactName(), QMessageBox::Ok);
		return false;
	}

	if (exactNameIsTooLong) {
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_1_TooLongExactName(), QMessageBox::Ok);
		return false;
	}

	if (exactNameHasNewLineChars) {
		Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_1_ExactNameHasNewLineChars(), QMessageBox::Ok);
		return false;
	}
	*/
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
	int indexOfAutoBox = ui->_5_AutoLootBox->currentIndex();
	int indexOfLootBox = ui->_5_AutoLootBox->currentIndex();
	bool indexAreInRage = true;
	if (indexOfAutoBox < 0 || indexOfAutoBox > 2)
		indexAreInRage = false;
	if (indexOfLootBox < 0 || indexOfLootBox > 2)
		indexAreInRage = false;
	return indexAreInRage;
}

void NewProfileConfiguartor::fillWidgetsWithDataFromProf() {
	//1
	ui->_1_nameEdit->setText(profToEdit->profileName);
	//2
	PROFESSION prof = PROFESSION(profToEdit->profession);
	switch (prof){
	case RP:	ui->_2_RadButt_RP->setChecked(true);	break;
	case EK:	ui->_2_RadButt_EK->setChecked(true);	break;
	case ED:	ui->_2_RadButt_ED->setChecked(true);	break;
	case MS:	ui->_2_RadButt_MS->setChecked(true);	break;
	default:	ui->_2_RadButt_RP->setChecked(true);	break;
	}
	//
	int numberOfHealthSliders = profToEdit->healthRestorePercentages.size();
	QList<QAbstractSlider*> sliders;
	QList<QComboBox*> boxes;
	QList<QKeySequenceEdit*> keyEdit;
	QList<QLabel*> labels;
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
	if (numberOfHealthSliders > 1) 
		ui->_3_enableAutoHealing->setEnabled(true);
	for (size_t i = 0; i < numberOfHealthSliders; i++){
		sliders[i]->setEnabled(true);
		labels[i]->setEnabled(true);
		keyEdit[i]->setEnabled(true);
		boxes[i]->setEnabled(true);

		sliders[i]->setValue(profToEdit->healthRestorePercentages[i]);
		labels[i]->setText(profToEdit->healthRestorePercentages[i] + "%");
		boxes[i]->setCurrentIndex(profToEdit->healthItems[i]);
	//	keyEdit[i]->setKeySequence(QKeySequence(profToEdit->));
	}
	
	//
}
//SLOTS
void NewProfileConfiguartor::nextPageButtonAction(){
	if (!pageIsCorrectlyFilled())
		return;

	pageNumber++;
	if (pageNumber > MAX_PAGE)
		finishAddingNewProfile();

	int index = ui->stackedWidget->currentIndex();
	index++;
	ui->stackedWidget->setCurrentIndex(index);

	refreshGUI();
}

void NewProfileConfiguartor::previousPageButtonAction(){
	pageNumber--;

	int index = ui->stackedWidget->currentIndex();
	index--;
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

	guiElements.push_back(ui->_3_label_2);
	guiElements.push_back(ui->_3_horizontalSlider_2);
	guiElements.push_back(ui->_3_shortKey_2);

	guiElements.push_back(ui->_3_label_3);
	guiElements.push_back(ui->_3_horizontalSlider_3);
	guiElements.push_back(ui->_3_shortKey_3);

	guiElements.push_back(ui->_3_label_4);
	guiElements.push_back(ui->_3_horizontalSlider_4);
	guiElements.push_back(ui->_3_shortKey_4);

	guiElements.push_back(ui->_3_label_5);
	guiElements.push_back(ui->_3_horizontalSlider_5);
	guiElements.push_back(ui->_3_shortKey_5);

	int valueOfSpin = ui->_3_spinGetNumberOfMethodes->value();

	for (int i = 0; i < guiElements.size(); i++) {
		bool enable = i < valueOfSpin * 3 ? true : false;
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
	if (ui->_5_ControlBox->currentIndex() == CONTROLS::CLSSIC_CONTROLS) {
		ui->_5_AutoLootBox->setEnabled(true);
		ui->_5_AutoLootBox->setCurrentIndex(0);
	}
	else {
		ui->_5_AutoLootBox->setEnabled(false);
		ui->_5_AutoLootBox->setCurrentIndex(0);
	}
	ui->_5_AutoLootBox->repaint();
}
