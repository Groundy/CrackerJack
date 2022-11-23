#include "NewProfileConfiguartor.h"

//const dest
NewProfileConfiguartor::NewProfileConfiguartor(Profile* prof, QWidget* parent) :
	QDialog(parent), profToEdit(prof) {
	ui = new Ui::NewProfileConfiguartor();
	ui->setupUi(this);
	fillGuiPtrs();
	ui->screenShotBox->insertItems(0, Key::getListOfAllPossibleKeys());
	ui->screenShotBox->setCurrentIndex(-1);
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
			throw std::exception("Something is wrong with selected controls");

		int indexOfAutoLootlBox = ui->_5_AutoLootBox->currentIndex();
		bool autoLootWrongs = indexOfControlBox < 0 ||
			indexOfAutoLootlBox > ui->_5_AutoLootBox->count() - 1 ||
			indexOfAutoLootlBox < 0;
		if (autoLootWrongs)
			throw std::exception("Something is wrong with selected auto loot key");

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
					throw std::exception("Two sliders can't be in the same position.");
			}
		}

		if (slidersValue.last() == 0)
			throw std::exception("Last slider has to have value above zero.");

		for (size_t i = 0; i < activeElements; i++) {
			if (keyNames[i].isEmpty())
				throw std::exception("One of key field doesn't have hotkey assigned to itself.");
			if (methodeNames[i].isEmpty())
				throw std::exception("Methode field can't be empty.");
		}

		if (keyNames.removeDuplicates() > 0)
			throw std::exception("Two key fields can't share the same key.");

		if (methodeNames.removeDuplicates() > 0)
			throw std::exception("Two  fields can't share the same value.");

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
bool NewProfileConfiguartor::checkAllKeyBoxes(){
	QVector<QComboBox*> keyBoxes;
	keyBoxes.append(healthPtrs.keyShortCuts);
	keyBoxes.append(manaPtrs.keyShortCuts);
	keyBoxes.append(ui->screenShotBox);
	QStringList keysName;
	for each (auto keyBox in keyBoxes){
		if (!keyBox->isEnabled())
			continue;
		if (keyBox->currentIndex() == -1) {
			Utilities::showMessageBox_INFO("Found one key box without value!");
			return false;
		}
		keysName.append(keyBox->currentText());
	}
	int duplicatedKeys = keysName.removeDuplicates();
	if (duplicatedKeys != 0) {
		Utilities::showMessageBox_INFO("some key boxes have the same value!");
		return false;
	}
	return true;
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
	if (!checkAllKeyBoxes())
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
		JsonParser().saveProfile(profToEdit);
		this->accept();
	}
}
void NewProfileConfiguartor::cancelButtonAction() {
	QString text = tr("Are you sure that you want cancel edit/adding new profile?");
	bool accepted = Utilities::showMessageBox_NO_YES(text);
	if(accepted)
		this->reject();
}
void NewProfileConfiguartor::healingGroupCounterChanged() {
	counterChanged(healthPtrs);
	fillRestoriationMethodes(getSelectedProf(), healthPtrs);
	refreshSlidersLabels(healthPtrs);
}
void NewProfileConfiguartor::manaGroupCounterChanged(){
	counterChanged(manaPtrs);
	fillRestoriationMethodes(getSelectedProf(), manaPtrs);
	refreshSlidersLabels(manaPtrs);
}
void NewProfileConfiguartor::healingSlidersChanged(){
	refreshSlidersLabels(healthPtrs);
}
void NewProfileConfiguartor::manaSlidersChanged(){
	refreshSlidersLabels(manaPtrs);
}
void NewProfileConfiguartor::controlsOrAutoLootListAction(){
	bool enableAutoLoot = ui->_5_ControlBox->currentIndex() == Profile::Controls::CLSSIC_CONTROLS;
	ui->_5_AutoLootBox->setEnabled(enableAutoLoot);
	ui->_5_AutoLootBox->setCurrentIndex(0);
	ui->_5_AutoLootBox->repaint();
}
void NewProfileConfiguartor::professionChanged(){
	Profession profession = getSelectedProf();
	fillRestoriationMethodes(profession, manaPtrs);
	fillRestoriationMethodes(profession, healthPtrs);
	for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++){
		healthPtrs.methodeNames[i]->setCurrentIndex(-1);
		manaPtrs.methodeNames[i]->setCurrentIndex(-1);
	}
}

//funcs
Profession NewProfileConfiguartor::getSelectedProf(){
	QString profName = ui->professionCombBox->currentText();
	return Profession(profName);
}
void NewProfileConfiguartor::refreshSlidersLabels(GuiPointers guiPointers){
	int activeElements = guiPointers.activeElementsCounter->value();
	if (activeElements == 0){
		for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++)
			guiPointers.percentageLabels[i]->setText(QString());
		return;
	}

	QStringList textToSetOnSliders;
	for (int i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++) {
		bool shouldBeEnabled = i < activeElements;
		if (!shouldBeEnabled) {
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
void NewProfileConfiguartor::fillSlidersGroup(GuiPointers guiPointers, QVector<RestorationMethode> restorationsMethodes){
	int size = restorationsMethodes.size();
	guiPointers.activeElementsCounter->setValue(size);
	for (size_t i = 0; i < size; i++){
		RestorationMethode* current = &restorationsMethodes[i];
		guiPointers.sliders[i]->setValue(current->getThreshold());

		auto keyBox = guiPointers.keyShortCuts[i];
		keyBox->setEnabled(true);
		int keyIndex = keyBox->findText(current->getKeyName());
		keyBox->setCurrentIndex(keyIndex);

		auto methodeBox = guiPointers.methodeNames[i];
		methodeBox->setEnabled(true);
		int methodeIndex = methodeBox->findText(current->getName());
		methodeBox->setCurrentIndex(methodeIndex);
	}
}
void NewProfileConfiguartor::fillRestoriationMethodes(Profession profession, GuiPointers& guiPointers){
	QStringList methodesNames = {};
	if (&guiPointers == &healthPtrs)
		methodesNames = JsonParser().getNamesOfHealingPotsAndSpellsForProf(profession);
	else if (&guiPointers == &manaPtrs)
		methodesNames = JsonParser().getNamesOManaPotsForProf(profession);
	else
		return;

	int slotsToFill = guiPointers.activeElementsCounter->value();
	QStringList allPossibleKeys = Key::getListOfAllPossibleKeys();
	for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++){
		QComboBox* methodeBox = guiPointers.methodeNames[i];
		QComboBox* keyBox = guiPointers.keyShortCuts[i];
		methodeBox->clear();
		keyBox->clear();
		bool enable = i < slotsToFill;
		if (enable) {
			methodeBox->insertItems(0, methodesNames);
			methodeBox->setCurrentIndex(-1);
			keyBox->insertItems(0, allPossibleKeys);
			keyBox->setCurrentIndex(-1);
		}
	}
}
void NewProfileConfiguartor::fillGuiPtrs() {
	healthPtrs.sliders.push_back(ui->healthSlider_1);
	healthPtrs.sliders.push_back(ui->healthSlider_2);
	healthPtrs.sliders.push_back(ui->healthSlider_3);
	healthPtrs.sliders.push_back(ui->healthSlider_4);
	healthPtrs.sliders.push_back(ui->healthSlider_5);

	manaPtrs.sliders.push_back(ui->manaSlider_1);
	manaPtrs.sliders.push_back(ui->manaSlider_2);
	manaPtrs.sliders.push_back(ui->manaSlider_3);
	manaPtrs.sliders.push_back(ui->manaSlider_4);
	manaPtrs.sliders.push_back(ui->manaSlider_5);

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

	healthPtrs.activeElementsCounter = ui->amountHealthRegMethodes;
	manaPtrs.activeElementsCounter = ui->amountManaRegMethodes;
}
void NewProfileConfiguartor::counterChanged(GuiPointers guiPointers){
	for (size_t i = 0; i < MAX_NUMBER_OF_ACTIVE_WIDGETS; i++){
		bool enable = i < guiPointers.activeElementsCounter->value();
		guiPointers.keyShortCuts[i]->setEnabled(enable);
		guiPointers.methodeNames[i]->setEnabled(enable);
		guiPointers.sliders[i]->setEnabled(enable);
		guiPointers.percentageLabels[i]->setEnabled(enable);
		if (!enable) {
			//guiPointers.keyShortCuts[i]->setEnabled(enable);
			//guiPointers.methodeNames[i]->s(enable);
			guiPointers.sliders[i]->setValue(0);
			guiPointers.percentageLabels[i]->setText(QString());
		}
	}
}
void NewProfileConfiguartor::fillFormsFromDataFromProf(Profile& prof){
	ui->_1_nameEdit->setText(prof.getName());

	auto profComboBox = ui->professionCombBox;
	QString professionName = prof.getProfession().getTypeName();
	int professionIndex = profComboBox->findText(professionName);
	profComboBox->setCurrentIndex(professionIndex);

	auto screenBox = ui->screenShotBox;
	int screenShotKeyIndex = screenBox->findText(prof.getScreenShotKey().getKeyName());
	screenBox->setCurrentIndex(screenShotKeyIndex);

	fillSlidersGroup(healthPtrs, prof.getRestMethodesHealth());
	fillSlidersGroup(manaPtrs, prof.getRestMethodesMana());

	ui->_5_ControlBox->setCurrentIndex(prof.getControls());
	ui->_5_AutoLootBox->setCurrentIndex(prof.getAutoLoot());
	ui->_5_LeftAddedBarsSpin->setValue(prof.getBarsLeft());
	ui->_5_RightAddedBarsSpin->setValue(prof.getBarsRight());
}
QVector<RestorationMethode> NewProfileConfiguartor::getRestorationMethodesFromGUI(GuiPointers guiPotiners){
	QVector<RestorationMethode> toRet = {};
	int activeElements = guiPotiners.activeElementsCounter->value();
	for (size_t i = 0; i < activeElements; i++){
		int threshold = guiPotiners.sliders[i]->value();
		QString keyName = guiPotiners.keyShortCuts[i]->currentText();
		QString methodeName = guiPotiners.methodeNames[i]->currentText();
		toRet.push_back(RestorationMethode(threshold, Key(keyName), methodeName));
	}
	fillRestorationMethodesDetails(toRet);
	return toRet;
}
void NewProfileConfiguartor::saveDataToProfile(){
	profToEdit->setName(ui->_1_nameEdit->text());
	profToEdit->setProfession(getSelectedProf());
	profToEdit->setRestMethodesHealth(getRestorationMethodesFromGUI(healthPtrs));
	profToEdit->setRestMethodesMana(getRestorationMethodesFromGUI(manaPtrs));
	profToEdit->setScreenShotKey(Key(ui->screenShotBox->currentText()));
	profToEdit->setBars(ui->_5_LeftAddedBarsSpin->value(), ui->_5_RightAddedBarsSpin->value());
	profToEdit->setControls(ui->_5_ControlBox->currentIndex());
	profToEdit->setAutoLoot(ui->_5_AutoLootBox->currentIndex());
}
