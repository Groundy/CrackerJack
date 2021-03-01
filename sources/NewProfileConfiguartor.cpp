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
	bool accepted = Utilities::showMessageBox_NO_YES("finish creating new profile","Are you sure that you want finish creating new profile?\nYou will be able to edit it later.");
	if (accepted) {
		saveDataToProfile(profToEdit);
		this->accept();
	}
	else
		this->accept();
}

void NewProfileConfiguartor::additionalGuiSettings(){
	ui->_3_spinGetNumberOfMethodes->setMinimum(0);
	ui->_3_spinGetNumberOfMethodes->setMaximum(5);
	ui->_4_spinGetNumberOfMethodes->setMinimum(0);
	ui->_4_spinGetNumberOfMethodes->setMaximum(5);
}

bool NewProfileConfiguartor::pageIsCorrectlyFilled() {
	bool toRet = false;
	switch (pageNumber) {
	case 1: {
		QString nameOfProf = ui->_1_nameEdit->toPlainText();

		if (nameOfProf.size() > 50) {
			Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_1_TooLongName(), QMessageBox::Ok);
			break;
		}

		if (nameOfProf.size() < 3) {
			Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_1_EmptyName(), QMessageBox::Ok);
			break;
		}

		bool allCharsAreAllowed = true;
		//checking for new lines chars
		if (nameOfProf.contains(QChar::LineFeed) || nameOfProf.contains(QChar::CarriageReturn))
			allCharsAreAllowed=false;
		
		for each (auto character in nameOfProf){
			if (!character.isLetterOrNumber() && !character.isSpace())
				allCharsAreAllowed = false;
		}

		if (!allCharsAreAllowed) {
			Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_1_ForbiddenChars(), QMessageBox::Ok);
			break;
		}

		toRet = true;
		break;
	}
	case 2: {
		QList<QRadioButton*> buttons;
		buttons.push_back(ui->_2_RadButt_ED);
		buttons.push_back(ui->_2_RadButt_EK);
		buttons.push_back(ui->_2_RadButt_RP);
		buttons.push_back(ui->_2_RadButt_MS);
		bool oneOfButtonsIsChecked = false;
		for each (QRadioButton * var in buttons) {
			if (var->isChecked()) {
				oneOfButtonsIsChecked = true;
				break;
			}
		}
		if (oneOfButtonsIsChecked) 
			toRet = true;
		else{
			Utilities::showMessageBox("CracerJack problem", StringResource::NewProfileConfig_2_anyProfIsChecked(), QMessageBox::Ok);
			break;
		}
		break;
	}
	case 3: {
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
		toRet = true;
		for each (QAbstractSlider* slider in activeSliders){
			if (slider->value() < biggestValue) 
				biggestValue = slider->value();
			else if (slider->value() == biggestValue) {
				everySliderHasDiffrentValue = false;
				toRet = false;
				break;
			}
			else {
				slidersAreInCorrectOrder = false;
				toRet = false;
				break;
			}
		}
		if (!everySliderHasDiffrentValue) {
			Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_SlidersAreInTheSamePosition(), QMessageBox::Ok);
			break;
		}
		if (!slidersAreInCorrectOrder) {
			Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_SlidersAreInWrongOrder(), QMessageBox::Ok);
			break;
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
		for (int i = 0; i < size; i++){
			if (keys[i].count() == 0) {
				oneOfKeyFieldsHasNoValue = true;
				toRet = false;
			}
			else if (keys[i].count() > 1) {
				oneOfKeyFieldsHasManyValues = true;
				toRet = false;
			}
		}

		for each (QKeySequence var in keys) {
			if (var.toString() == "+" || var.toString() == "#")
				oneOfKeyFieldsHasForbiddenChars = true;
		}



		if (oneOfKeyFieldsHasManyValues) {
			Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_ShortcutManyValue(), QMessageBox::Ok);
			break;
		}
		if (oneOfKeyFieldsHasNoValue) {
			Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_ShortcutNoValue(), QMessageBox::Ok);
			break;
		}
		if (oneOfKeyFieldsHasForbiddenChars) {
			Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_ShortCutHasForbiddenChars(), QMessageBox::Ok);
			break;
		}

		break;
	}
	case 4: {
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
		toRet = true;
		for each (QAbstractSlider * slider in activeSliders) {
			if (slider->value() < biggestValue)
				biggestValue = slider->value();
			else if (slider->value() == biggestValue) {
				everySliderHasDiffrentValue = false;
				toRet = false;
				break;
			}
			else {
				slidersAreInCorrectOrder = false;
				toRet = false;
				break;
			}
		}
		if (!everySliderHasDiffrentValue) {
			Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_SlidersAreInTheSamePosition(), QMessageBox::Ok);
			break;
		}
		if (!slidersAreInCorrectOrder) {
			Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_SlidersAreInWrongOrder(), QMessageBox::Ok);
			break;
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
				toRet = false;
			}
			else if (keys[i].count() > 1) {
				oneOfKeyFieldHasManyValues = true;
				toRet = false;
			}
		}

		for each (QKeySequence var in keys){
			if (var.toString() == "+" || var.toString() == "#")
				oneOfKeyFieldsHasForbiddenChars = true;
		}


		if (oneOfKeyFieldHasManyValues) {
			Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_ShortcutManyValue(), QMessageBox::Ok);
			break;
		}
		if (oneOfKeyFieldHasNoValue) {
			Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_ShortcutNoValue(), QMessageBox::Ok);
			break;
		}
		if (oneOfKeyFieldsHasForbiddenChars) {
			Utilities::showMessageBox(StringResource::WindowTitle_CrackerJackProblem(), StringResource::NewProfileConfig_3_ShortCutHasForbiddenChars(), QMessageBox::Ok);
			break;
		}

		break;
	}
	default: {

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
	if (ui->_2_RadButt_EK->isChecked())
		prof->profession = PROFESSION::EK;
	if (ui->_2_RadButt_MS->isChecked())
		prof->profession = PROFESSION::MS;
	if (ui->_2_RadButt_RP->isChecked())
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
		
	}
	//5
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
