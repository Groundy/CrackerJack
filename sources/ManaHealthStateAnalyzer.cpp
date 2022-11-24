#include "ManaHealthStateAnalyzer.h"
#include <QChar>
#include <ImgEditor.h>

ManaHealthStateAnalyzer::ManaHealthStateAnalyzer(QObject *parent, Profile* profile, std::shared_ptr<VariablesClass> var)
	: QThread(parent), var(var){
	PopulateHealthManaMaps(profile);
	var->changeRestoringState(true);
}
ManaHealthStateAnalyzer::~ManaHealthStateAnalyzer(){
	this->terminate();
}
void ManaHealthStateAnalyzer::run(){
	while (true) {
		msleep(SLEEP_TIME);

		ValuesDoubles percentages = getCurrentPercentage();
		if (!percentages.isValid())
			continue;

		sendDataToGui(percentages);
		auto healthMetodes = findRestorationToUse(percentages.health, healthMap);
		auto manahMetodes = findRestorationToUse(percentages.mana, manaMap);

		for each (auto methode in healthMetodes)
			var->log("h: " + methode.getName(), false, true, true);
		for each (auto methode in manahMetodes)
			var->log("m: " + methode.getName(), false, true, true);
	}
}



/*
void ManaHealthStateAnalyzer::setupRestorationMethodes(QStringList listOfRestorationMethode_Health, QStringList listOfRestorationMethode_Mana){
	
	JsonParser parser;
	QList<Utilities::RestoreMethode> spellsAndPotions;
	QList<Item> manaPotions;

	parser.getHealthRestoreMethodes(listOfRestorationMethode_Health, spellsAndPotions);
	parser.getManaRestoreMethodes(listOfRestorationMethode_Mana, manaPotions);

	healthMethodes = spellsAndPotions;
	manaMethodes = manaPotions;
	
}
bool ManaHealthStateAnalyzer::getKeysForConnectorClass(Key& healthKey, Key& manaKey) {

		int indexOfHealthThreshold = findNearestThresholdIndex(healthPercentage, healthThresholds);
	int indexOfManaThreshold = findNearestThresholdIndex(manaPercentage, manaThresholds);

	bool potionSlotIsUsed = false;
	bool spellSlotIsUsed = false;
	Key potionKey = -1, spellKey = -1;

	int i = indexOfHealth;
	LONG64 currentTime = Utilities::getCurrentTimeInMiliSeconds();
	while (i >= 0) {
		Utilities::RestoreMethode::TypeOfMethode type = healthMethodes[i].type;

		if (type == Utilities::RestoreMethode::TypeOfMethode::POTION && !potionSlotIsUsed) {
			LONG64 nextTimeThisItemWillBeAvaible = ((LONG64)healthMethodes[i].cd * 1000) + var->lastTimeMethodeUsed_Healing[i];
			LONG64 nextTimeAnyItemWillBeAvaible = var->lastTimeUsed_item + 1000;
			bool useThis = max(nextTimeThisItemWillBeAvaible, nextTimeAnyItemWillBeAvaible) <= currentTime;
			if (useThis){
				//potionKey = healthKeys[i];//todo
				potionSlotIsUsed = true;
				var->lastTimeUsed_item = currentTime;
				var->lastTimeMethodeUsed_Healing[i] = currentTime;
			}
		}
		else if (type == Utilities::RestoreMethode::TypeOfMethode::SPELL && !spellSlotIsUsed) {
			LONG64 lastTimeUsed = var->lastTimeMethodeUsed_Healing[i];
			LONG64 minTimeCd = ((LONG64)healthMethodes[i].cd * 1000) + lastTimeUsed;
			LONG64 minTimeCdGroup = ((LONG64)healthMethodes[i].cdGroup * 1000) + lastTimeUsed;
			LONG64 nextTimeAnySpellWillBeAvaible = var->lastTimeUsed_spell_healing + 1000;
			LONG64 nextTimeThisSpellWillBeAvaible = max(max(minTimeCd, minTimeCdGroup), nextTimeAnySpellWillBeAvaible);
			bool canUseNow =  nextTimeThisSpellWillBeAvaible <= currentTime;
			bool haveManaForIt = mana >= healthMethodes[i].mana;
			if (canUseNow && haveManaForIt) {
				//spellKey = healthKeys[i];//todo
				spellSlotIsUsed = true;
				var->lastTimeUsed_spell_healing = currentTime;
				var->lastTimeMethodeUsed_Healing[i] = currentTime;
			}
		}
		i--;
	}

	i = potionSlotIsUsed ? -1 : indexOfMana;
	while (i >= 0){
		LONG64 nextTimeThisItemWillBeAvaible = var->lastTimeUsed_item + 1000;
		bool isReady = nextTimeThisItemWillBeAvaible <= currentTime;
		if (isReady) {
			//potionKey = manaKeys[i];//todo
			potionSlotIsUsed = true;
			var->lastTimeUsed_item = currentTime;
		}
		i--;
	}

	manaKey = potionKey;
	healthKey = spellKey;
	return OK;
	
	return true;
}
void ManaHealthStateAnalyzer::getAmountsOfPotions() {
	
	QMap<QString,QRect> map_copy = var->potionName_rectPosOnScreen_map;
	QList<int> amountOfPots;
	QStringList namesOfPots;
	QImage wholeImg;
	var->getCopyOfCurrentFullImg(wholeImg);
	for each (QString nameOfPot in map_copy.keys()) {
		QRect rect = map_copy[nameOfPot];
		if (rect.isEmpty())
			continue;
		QImage img = wholeImg.copy(rect);
		int amount = ImgEditor::getNumberFromBottomBar(img);
		amountOfPots.push_back(amount);
		namesOfPots.push_back(nameOfPot);
	}

	QStringList infoToSendToMainThread;
	for (size_t i = 0; i < namesOfPots.size(); i++){
		QString toAdd = namesOfPots[i] + ": " + QString::number(amountOfPots[i]);
		infoToSendToMainThread.push_back(toAdd);
	}
	sendInfoAboutPotAmountsToGUI(infoToSendToMainThread);
	
}
*/