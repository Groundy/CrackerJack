#include "NewProfileConfiguartor.h"

//const dest
NewProfileConfiguartor::NewProfileConfiguartor(Profile* prof, QWidget* parent) :
	QDialog(parent), profToEdit(prof) {
	ui = new Ui::NewProfileConfiguartor();
	ui->setupUi(this);
	fillGuiPtrs();
}
NewProfileConfiguartor::~NewProfileConfiguartor() {
	delete ui;
}

//correctnessCheck
bool NewProfileConfiguartor::checkNameGroup(){
	try {
		QString nameOfProf = ui->_1_nameEdit->text();
		bool wrongLength = nameOfProf.size() > 50 || nameOfProf.size() < 3;
		if (wrongLength)
			throw std::exception("Profile name must be between 3 and 50 characters long");

		bool nameConsistForbiddenChars =
			nameOfProf.contains(QChar::LineFeed) ||
			nameOfProf.contains(QChar::CarriageReturn);

		if (nameConsistForbiddenChars)
			throw std::exception("Profile name can't have any special characters, please use only letters, numbers or spaces.");

		for each (auto character in nameOfProf) {
			bool allowed = character.isLetterOrNumber() || character.isSpace();
			if(!allowed)
				throw std::exception("Profile name can't have any special characters, please use only letters, numbers or spaces.");
		}
		return true;
	}
	catch (const std::exception& e){
		qDebug() << e.what();
		Utilities::showMessageBox_INFO(e.what());
		return false;
	}
}
bool NewProfileConfiguartor::checkControlsGroup() {
	//change //TODO
	try{
		int indexOfControlBox = ui->_5_ControlBox->currentIndex();
		bool controlsWrong = indexOfControlBox < 0 ||
			indexOfControlBox > ui->_5_ControlBox->count() - 1 ||
			indexOfControlBox < 0;
		if (controlsWrong)
			throw("Something is wrong with selected controls");

		int indexOfAutoLootlBox = ui->_5_AutoLootBox->currentIndex();
		bool autoLootWrongs = indexOfControlBox < 0 ||
			indexOfAutoLootlBox > ui->_5_AutoLootBox->count() - 1 ||
			indexOfAutoLootlBox < 0;
		if (autoLootWrongs)
			throw("Something is wrong with selected auto loot key");

		return true;
	}
	catch (const std::exception& e){
		qDebug() << e.what();
		Utilities::showMessageBox_INFO(e.what());
		return false;
	}
}
bool NewProfileConfiguartor::checkSlidersGroup(GuiPointers guiPtrs){
	try {
		int activeElements = guiPtrs.activeElementsCounter->value();
		if (activeElements == 0)
			return true;

		QVector<int> slidersValue;
		QStringList keyNames, methodeNames;
		for (size_t i = 0; i < activeElements; i++) {
			slidersValue.push_back(guiPtrs.sliders[i]->value());
			keyNames.push_back(guiPtrs.keyShortCuts[i]->currentText());
			methodeNames.push_back(guiPtrs.methodeNames[i]->currentText());
		}

		if (activeElements > 1) {
			for (int i = 1; i < activeElements; i++) {
				if (slidersValue[i - 1] < slidersValue[i])
					throw std::exception("Sliders are in wrong order, please set it from biggest value to lowest.");
				if (slidersValue[i - 1] == slidersValue[i])
					throw(tr("Two sliders can't be in the same position."));
			}
		}

		if (slidersValue.last() == 0)
			throw(tr("Last slider has to have value above zero."));

		for (size_t i = 0; i < activeElements; i++) {
			if (keyNames[i].isEmpty())
				throw(tr("One of key field doesn't have hotkey assigned to itself."));
			if (methodeNames[i].isEmpty())
				throw (tr("Methode field can't be empty."));
		}

		if (keyNames.removeDuplicates() > 0)
			throw (tr("Two key fields can't share the same key."));

		if (methodeNames.removeDuplicates() > 0)
			throw (tr("Two  fields can't share the same value."));

		return true;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		Utilities::showMessageBox_INFO(e.what());
		return false;
	}
}
bool NewProfileConfiguartor::checkHealthGroup() {
	return checkSlidersGroup(healthPtrs);
}
bool NewProfileConfiguartor::checkManaGroup(){
	return checkSlidersGroup(manaPtrs);
}
bool NewProfileConfiguartor::checkAllForms(){
	if (!checkNameGroup())
		return false;
	if (!checkHealthGroup())
		return false;
	if (!checkManaGroup())
		return false;
	if (!checkControlsGroup())
		return false;
	return true;
}

//slots
void NewProfileConfiguartor::finishButtonAction(){
	if (!checkAllForms())
		return;

	QString text = tr("Are you sure that you want finish creating new profile?");
	bool accepted = Utilities::showMessageBox_NO_YES(text);
	if (accepted) {
		saveDataToProfile();
		JsonParser().saveProfiles(profToEdit);
		this->accept();
	}
}
void NewProfileConfiguartor::cancelButtonAction() {
	QString text = tr("Are you sure that you want cancel edit/adding new profile?");
	bool accepted = Utilities::showMessageBox_NO_YES(text);
	if(accepted)
		this->reject();
}
void NewProfileConfiguartor::healingOrManaGroupCounterChanged() {
	fillRestoriationMethodesForProfession(getSelectedProf());
}
void NewProfileConfiguartor::healingSlidersChanged(){
	refreshSliders(healthPtrs);
}
void NewProfileConfiguartor::manaSlidersChanged(){
	refreshSliders(manaPtrs);
}
void NewProfileConfiguartor::controlsOrAutoLootListAction(){
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

//funcs
Profession NewProfileConfiguartor::getSelectedProf(){
	QString profName = ui->professionCombBox->currentText();
	return Profession(profName);
}
void NewProfileConfiguartor::refreshSliders(GuiPointers guiPointers){
	int activeElements = guiPointers.activeElementsCounter->value();
	if (activeElements == 0){
		for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++)
			guiPointers.percentageLabels[i]->setText(QString());
		return;
	}

	QStringList textToSetOnSliders;
	for (int i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) {
		bool shouldBeEnabled = i < activeElements;
		if (shouldBeEnabled) {
			textToSetOnSliders.push_back(QString());
			continue;
		}

		bool isLastActiveSlider = i == activeElements - 1;
		QString valueOfSliderStr = QString::number(guiPointers.sliders[i]->value());
		if (isLastActiveSlider) {
			QString txtToSet = QString::number(0) + "% - " + valueOfSliderStr + "%";
			textToSetOnSliders.push_back(txtToSet);
		}
		else {
			int valueOfPreviousSlider = guiPointers.sliders[i + 1]->value();
			QString txtToSet = QString::number(valueOfPreviousSlider) + "% - " + valueOfSliderStr + "%";
			textToSetOnSliders.push_back(txtToSet);
		}
	}
	if (textToSetOnSliders.size() != MAX_NUMBER_OF_ACTIVE_WIDGETS) {
		//todo 
	}

	for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++)
		guiPointers.percentageLabels[i]->setText(textToSetOnSliders[i]);
}
void NewProfileConfiguartor::fillSlidersGroup(GuiPointers guiPointers, QList<RestorationStruct> restorationsMethodes){
	int size = restorationsMethodes.size();
	guiPointers.activeElementsCounter->setValue(size);
	for (size_t i = 0; i < size; i++){
		RestorationStruct* current = &restorationsMethodes[i];
		guiPointers.keyShortCuts[i]->setCurrentText(current->getKeyName());
		guiPointers.methodeNames[i]->setCurrentText(current->getName());
		guiPointers.sliders[i]->setValue(current->getThreshold());
	}
}
void NewProfileConfiguartor::fillRestoriationMethodesForProfession(Profession profession){
	QStringList healthMethodes = JsonParser().getNamesOfHealingPotsAndSpellsForProf(profession);
	int healthSlotsToFill = healthPtrs.activeElementsCounter->value();
	for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++){
		QComboBox* box = healthPtrs.methodeNames[i];
		box->clear();
		bool healthSlotShouldBeActive = i < healthSlotsToFill;
		box->setEnabled(healthSlotShouldBeActive);
		if (healthSlotShouldBeActive) {
			box->insertItems(0, healthMethodes);
			box->setCurrentIndex(0);
		}
	}

	QStringList manaMethodes = JsonParser().getNamesOManaPotsForProf(profession);
	int manaSlotsToFill = manaPtrs.activeElementsCounter->value();
	for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) {
		QComboBox* box = manaPtrs.methodeNames[i];
		box->clear();
		bool healthSlotShouldBeActive = i < manaSlotsToFill;
		box->setEnabled(healthSlotShouldBeActive);
		if (healthSlotShouldBeActive) {
			box->insertItems(0, healthMethodes);
			box->setCurrentIndex(0);
		}
	}
}
void NewProfileConfiguartor::fillGuiPtrs() {
	healthPtrs.sliders.push_back(ui->_3_horizontalSlider_1);
	healthPtrs.sliders.push_back(ui->_3_horizontalSlider_2);
	healthPtrs.sliders.push_back(ui->_3_horizontalSlider_3);
	healthPtrs.sliders.push_back(ui->_3_horizontalSlider_4);
	healthPtrs.sliders.push_back(ui->_3_horizontalSlider_5);

	manaPtrs.sliders.push_back(ui->_4_horizontalSlider_1);
	manaPtrs.sliders.push_back(ui->_4_horizontalSlider_2);
	manaPtrs.sliders.push_back(ui->_4_horizontalSlider_3);
	manaPtrs.sliders.push_back(ui->_4_horizontalSlider_4);
	manaPtrs.sliders.push_back(ui->_4_horizontalSlider_5);

	healthPtrs.percentageLabels.push_back(ui->_3_label_1);
	healthPtrs.percentageLabels.push_back(ui->_3_label_2);
	healthPtrs.percentageLabels.push_back(ui->_3_label_3);
	healthPtrs.percentageLabels.push_back(ui->_3_label_4);
	healthPtrs.percentageLabels.push_back(ui->_3_label_5);

	manaPtrs.percentageLabels.push_back(ui->_4_label_1);
	manaPtrs.percentageLabels.push_back(ui->_4_label_2);
	manaPtrs.percentageLabels.push_back(ui->_4_label_3);
	manaPtrs.percentageLabels.push_back(ui->_4_label_4);
	manaPtrs.percentageLabels.push_back(ui->_4_label_5);

	healthPtrs.methodeNames.push_back(ui->_3_comboBox);
	healthPtrs.methodeNames.push_back(ui->_3_comboBox_2);
	healthPtrs.methodeNames.push_back(ui->_3_comboBox_3);
	healthPtrs.methodeNames.push_back(ui->_3_comboBox_4);
	healthPtrs.methodeNames.push_back(ui->_3_comboBox_5);

	manaPtrs.methodeNames.push_back(ui->_4_comboBox);
	manaPtrs.methodeNames.push_back(ui->_4_comboBox_2);
	manaPtrs.methodeNames.push_back(ui->_4_comboBox_3);
	manaPtrs.methodeNames.push_back(ui->_4_comboBox_4);
	manaPtrs.methodeNames.push_back(ui->_4_comboBox_5);


	healthPtrs.keyShortCuts.push_back(ui->_3_shortKey_1);
	healthPtrs.keyShortCuts.push_back(ui->_3_shortKey_2);
	healthPtrs.keyShortCuts.push_back(ui->_3_shortKey_3);
	healthPtrs.keyShortCuts.push_back(ui->_3_shortKey_4);
	healthPtrs.keyShortCuts.push_back(ui->_3_shortKey_5);

	manaPtrs.keyShortCuts.push_back(ui->_4_shortKey_1);
	manaPtrs.keyShortCuts.push_back(ui->_4_shortKey_2);
	manaPtrs.keyShortCuts.push_back(ui->_4_shortKey_3);
	manaPtrs.keyShortCuts.push_back(ui->_4_shortKey_4);
	manaPtrs.keyShortCuts.push_back(ui->_4_shortKey_5);

	healthPtrs.activeElementsCounter = ui->_3_spinGetNumberOfMethodes;
	manaPtrs.activeElementsCounter = ui->_4_spinGetNumberOfMethodes;
}
void NewProfileConfiguartor::fillFormsFromDataFromProf(const Profile& profToEdit){
	ui->_1_nameEdit->setText(profToEdit.profileName); 
	fillSlidersGroup(healthPtrs, profToEdit.healthRestorations);
	fillSlidersGroup(manaPtrs, profToEdit.manaRestorations);

	//ToDo fill loot controls loot
}
QList<RestorationStruct> NewProfileConfiguartor::getRestorationMethodesFromGUI(GuiPointers guiPotiners){
	QList<RestorationStruct> toRet = {};
	int activeElements = guiPotiners.activeElementsCounter->value();
	for (size_t i = 0; i < activeElements; i++){
		int threshold = guiPotiners.sliders[i]->value();
		QString keyName = guiPotiners.keyShortCuts[i]->currentText();
		QString methodeName = guiPotiners.methodeNames[i]->currentText();
		toRet.push_back(RestorationStruct(threshold, Key(keyName), methodeName));
	}
	return toRet;
}
void NewProfileConfiguartor::saveDataToProfile(){
	profToEdit->profileName = ui->_1_nameEdit->text();
	profToEdit->profession = getSelectedProf();
	profToEdit->healthRestorations = getRestorationMethodesFromGUI(healthPtrs);
	profToEdit->manaRestorations = getRestorationMethodesFromGUI(manaPtrs);
	//todo fill controls loot
}



/*
void NewProfileConfiguartor::saveDataToProfile(Profile* prof) {
	//3
	if (ui->_3_enableAutoHealing->isChecked()) {
		QList<QAbstractSlider*> sliders;
		sliders.push_back(ui->_3_horizontalSlider_1);
		sliders.push_back(ui->_3_horizontalSlider_2);
		sliders.push_back(ui->_3_horizontalSlider_3);
		sliders.push_back(ui->_3_horizontalSlider_4);
		sliders.push_back(ui->_3_horizontalSlider_5);
		QList<QComboBox*> keysPtrs;
		keysPtrs.push_back(ui->_3_shortKey_1);
		keysPtrs.push_back(ui->_3_shortKey_2);
		keysPtrs.push_back(ui->_3_shortKey_3);
		keysPtrs.push_back(ui->_3_shortKey_4);
		keysPtrs.push_back(ui->_3_shortKey_5);
		QStringList helathRestorationNames;
		helathRestorationNames.push_back(ui->_3_comboBox->currentText());
		helathRestorationNames.push_back(ui->_3_comboBox_2->currentText());
		helathRestorationNames.push_back(ui->_3_comboBox_3->currentText());
		helathRestorationNames.push_back(ui->_3_comboBox_4->currentText());
		helathRestorationNames.push_back(ui->_3_comboBox_5->currentText());

		prof->healthRestorations.clear();
		int size = ui->_3_spinGetNumberOfMethodes->value();
		for (size_t i = 0; i < size; i++) {
			int thresholdOfSlider = sliders[i]->value();
			Key key = Key(keysPtrs[i]->currentText());
			QString restorationName = helathRestorationNames[i];
			RestorationStruct restorationMethode(thresholdOfSlider, key, restorationName);
			prof->healthRestorations.push_back(restorationMethode);
		}
	}
	//4
	if (ui->_4_enableManaRestore->isChecked()) {
		QList<QAbstractSlider*> sliders;
		sliders.push_back(ui->_4_horizontalSlider_1);
		sliders.push_back(ui->_4_horizontalSlider_2);
		sliders.push_back(ui->_4_horizontalSlider_3);
		sliders.push_back(ui->_4_horizontalSlider_4);
		sliders.push_back(ui->_4_horizontalSlider_5);
		QList<QComboBox*> keysPtrs;
		keysPtrs.push_back(ui->_4_shortKey_1);
		keysPtrs.push_back(ui->_4_shortKey_2);
		keysPtrs.push_back(ui->_4_shortKey_3);
		keysPtrs.push_back(ui->_4_shortKey_4);
		keysPtrs.push_back(ui->_4_shortKey_5);
		QList<QString> manaRestoreNames;
		manaRestoreNames.push_back(ui->_4_comboBox->currentText());
		manaRestoreNames.push_back(ui->_4_comboBox_2->currentText());
		manaRestoreNames.push_back(ui->_4_comboBox_3->currentText());
		manaRestoreNames.push_back(ui->_4_comboBox_4->currentText());
		manaRestoreNames.push_back(ui->_4_comboBox_5->currentText());

		int size = ui->_4_spinGetNumberOfMethodes->value();
		for (size_t i = 0; i < size; i++) {
			int thresholdOfSlider = sliders[i]->value();
			Key key = Key(keysPtrs[i]->currentText());
			QString restorationName = manaRestoreNames[i];
			RestorationStruct restorationMethode(thresholdOfSlider, key, restorationName);
			prof->manaRestorations.push_back(restorationMethode);
		}
	}
	//5
}
*/