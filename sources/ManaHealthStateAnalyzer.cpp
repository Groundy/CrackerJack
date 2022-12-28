#include "ManaHealthStateAnalyzer.h"

ManaHealthStateAnalyzer::ManaHealthStateAnalyzer(QObject *parent, Profile* profile, QSharedPointer<VariablesClass> var, QSharedPointer<GameConnecter> gameConnector)
	: QThread(parent), var(var), gameConnector(gameConnector){
	populateHealthManaMaps(profile);
	imgEditor = &var->getImgEditorObj();
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
		for each (auto methode in healthMetodes) {
			gameConnector->useRestorationMethode(methode);
			//var->getVitalitty().clearHealth();
		}
		auto manahMetodes = findRestorationToUse(percentages.mana, manaMap);
		for each (auto methode in manahMetodes) {
			int additionalManaBreak = calcTimeBetweenManaPots(percentages.mana);
			gameConnector->useRestorationMethode(methode, additionalManaBreak);
			//var->getVitalitty().clearMana();
		}

		handleStates();
	}
}


ManaHealthStateAnalyzer::ValuesDoubles ManaHealthStateAnalyzer::toDoubles(ValuesInts currentValues) {
	double healthPercentage;
	if (currentValues.isHealthOk())
		healthPercentage = 100.0 * currentValues.health / currentValues.maxHealth;
	else
		healthPercentage = NULL;

	double manaPercentage;
	if (currentValues.isManaOk())
		manaPercentage = 100.0 * currentValues.mana / currentValues.maxMana;
	else
		manaPercentage = NULL;

	double manaShieldPercentage;
	if (currentValues.isShieldOk())
		manaShieldPercentage = 100.0 * currentValues.shield / currentValues.maxShield;
	else
		manaShieldPercentage = NULL;
	ValuesDoubles toRet(healthPercentage, manaPercentage, manaShieldPercentage);
	return toRet;
}
bool ManaHealthStateAnalyzer::populateHealthManaMaps(const Profile* profile) {
	try {
		auto healthRestorationsMethode = profile->getRestMethodesHealth();
		QVector<int> healthThresholds;
		for each (auto var in healthRestorationsMethode)
			healthThresholds.push_back(var.getThreshold());
		if (healthThresholds.size() != healthRestorationsMethode.size())
			throw std::exception("wrong sizes of health restoration methodes passed to ManaHealthAnalyzer!");
		for (size_t i = 0; i < healthThresholds.size(); i++)
			healthMap.insert(healthThresholds[i], healthRestorationsMethode[i]);

		auto manaRestorationsMethode = profile->getRestMethodesMana();
		QVector<int> manaThresholds;
		for each (auto var in manaRestorationsMethode)
			manaThresholds.push_back(var.getThreshold());
		if (manaThresholds.size() != manaRestorationsMethode.size())
			throw std::exception("wrong sizes of health restoration methodes passed to ManaHealthAnalyzer!");
		for (size_t i = 0; i < manaThresholds.size(); i++)
			manaMap.insert(manaThresholds[i], manaRestorationsMethode[i]);

		return true;
	}
	catch (const std::exception& e) {
		qDebug() << e.what();
		return false;
	}
}
ManaHealthStateAnalyzer::ValuesStrs ManaHealthStateAnalyzer::toStrsValues(FoundFlags foundFlags, ImageValues imgVals) {
	try {
		ValuesStrs strVals;
		if (foundFlags.health)
			strVals.health = imgEditor->imgWithStrToStr(imgVals.health).remove("\0");
		else
			strVals.health = QString();

		if (foundFlags.mana)
			strVals.mana = imgEditor->imgWithStrToStr(imgVals.mana).remove("\0");
		else
			strVals.mana = QString();


		if (foundFlags.shield)
			strVals.manaShield = imgEditor->imgWithStrToStr(imgVals.manaShield).remove("\0");
		else
			strVals.manaShield = QString();

		if (foundFlags.combined)
			strVals.combined = imgEditor->imgWithStrToStr(imgVals.combined).remove("\0");
		else
			strVals.combined = QString();

		const int MIN_LENGTH_FOR_CORR_STR = 3;
		if (strVals.health.length() < MIN_LENGTH_FOR_CORR_STR)
			throw std::exception("Error in converting ims with health bar to str form");
		if (strVals.mana.length() < MIN_LENGTH_FOR_CORR_STR && strVals.combined.length() < MIN_LENGTH_FOR_CORR_STR)
			throw std::exception("Error in converting ims with mana bar to str form");

		return strVals;
	}
	catch (const std::exception& e) {
		qDebug() << e.what();
		return ValuesStrs();
	}
}
ManaHealthStateAnalyzer::ValuesInts ManaHealthStateAnalyzer::toIntsValues(FoundFlags foundFlags, ValuesStrs valueStrs) {
	ValuesInts valuesInts;
	if (foundFlags.health)
		getValuesFromStringRegularCase(valueStrs.health, valuesInts.health, valuesInts.maxHealth);

	if (foundFlags.combined)
		getValuesFromStringOfCombinedBox(valueStrs.combined, valuesInts.mana, valuesInts.maxMana, valuesInts.shield, valuesInts.maxShield);

	if (foundFlags.shield)
		getValuesFromStringRegularCase(valueStrs.manaShield, valuesInts.shield, valuesInts.maxShield);

	if (foundFlags.mana)
		getValuesFromStringRegularCase(valueStrs.mana, valuesInts.mana, valuesInts.maxMana);
	return valuesInts;
};
ManaHealthStateAnalyzer::FoundFlags ManaHealthStateAnalyzer::getFoundFlags() {
	FoundFlags flags;
	flags.health = !var->getVitalitty().getHealthArea().isEmpty();
	flags.mana = !var->getVitalitty().getManaArea().isEmpty();
	flags.combined = !var->getVitalitty().getCombinedArea().isEmpty();
	flags.shield = !var->getVitalitty().getMSArea().isEmpty();
	return flags;
}
bool ManaHealthStateAnalyzer::getValuesFromStringRegularCase(QString in, int& current, int& max) {
	try {
		const QStringList partOfStr = in.split("\\");//wanted form of input currentVal/maxVal
		if (partOfStr.size() != 2)
			throw std::exception(QString("ManaHealthAnalyzer recived wrong input, input = " + in).toStdString().c_str());
		const int currentVal = partOfStr[0].toInt();
		const int maxVal = partOfStr[1].toInt();
		const int MAX_POSSIBLE_VALUE = 100000;
		const bool wrongValues = currentVal > MAX_POSSIBLE_VALUE || currentVal > maxVal || currentVal < 0 || maxVal < 0;
		if (wrongValues)
			throw std::exception("Wrong int values in splittling str currentVal/maxVal");
		current = currentVal;
		max = maxVal;
		return true;
	}
	catch (const std::exception& e) {
		qDebug() << e.what();
		return false;
	}
}
bool ManaHealthStateAnalyzer::getValuesFromStringOfCombinedBox(QString in, int& currentMana, int& maxMana, int& currentManaShield, int& maxManaShield) {
	//wanted form of input manaMinVal/maxVal(minShieldValue/maxShieldValue)
	try {
		bool inputOk = in.count("\\") == 2 && in.count("(") == 1 && in.count(")") == 1;
		if (!inputOk) {
			QString msg = "Wrong input in splitting str with values of mana combined with magic shield, input = " + in;
			throw std::exception(msg.toStdString().c_str());
		}

		in.remove(")");
		QStringList parts = in.split("(");
		if (parts.size() != 2)
			throw std::exception("error in splitting  str with values of mana combined with magic shield");

		QString manaStr = parts[0];
		QString shieldStr = parts[1];
		int currentManaTMP, maxManaTMP, currentManaShieldTMP, maxManaShieldTMP;
		bool splittingOk1 = getValuesFromStringRegularCase(manaStr, currentManaTMP, maxManaTMP);
		bool splittingOk2 = getValuesFromStringRegularCase(shieldStr, currentManaShieldTMP, maxManaShieldTMP);
		bool okResult = splittingOk1 && splittingOk2 && maxManaTMP > 0;
		if (!okResult)
			throw std::exception("error in splitting  str with values of mana combined with magic shield");

		currentMana = currentManaTMP;
		maxMana = maxManaTMP;
		currentManaShield = currentManaShieldTMP;
		maxManaShield = maxManaShieldTMP;
		return true;
	}
	catch (const std::exception& e) {
		qDebug() << e.what();
		return false;
	}
}
QVector<RestorationMethode> ManaHealthStateAnalyzer::findRestorationToUse(double currentValue, const QMap<int, RestorationMethode>& methodes) {
	QVector<RestorationMethode> toRet = {};
	if (methodes.size() == 0)
		return toRet;

	try {
		bool wrongInput = currentValue < 0.0 || currentValue > 100.0 || methodes.size() > 5;
		if (wrongInput)
			throw std::exception("Wrong input passed to fun looking for neareast threshold");

		auto thresholds = methodes.keys();
		for (int i = 0; i < thresholds.size(); i++) {
			if (currentValue > thresholds[i])
				continue;

			if (toRet.size() == 2)
				break;

			auto currentMethode = methodes[thresholds[i]];
			bool suchTypeIsAlreadyOnList = false;
			for each (auto var in toRet){
				if (var.isType(currentMethode.getType())){
					suchTypeIsAlreadyOnList = true;
					break;
				}
			}
			if (suchTypeIsAlreadyOnList)
				continue;

			if(!restMethodeCanBeUsed(currentMethode))
				continue;

			toRet.push_back(currentMethode);
		}
		return toRet;
	}
	catch (const std::exception& e) {
		qDebug() << e.what();
		return toRet;
	}
}
ManaHealthStateAnalyzer::ValuesDoubles ManaHealthStateAnalyzer::getCurrentPercentage() {
	if (!var->getSettings().getRestoringState())
		return ValuesDoubles();
	ImageValues imgs = getImages();
	if (!imgs.isValid())
		return ValuesDoubles();
	FoundFlags flags = getFoundFlags();
	if (!flags.isValid())
		return ValuesDoubles();
	ValuesStrs strsValues = toStrsValues(flags, imgs);
	if (!strsValues.isValid())
		return ValuesDoubles();
	ValuesInts intsValues = toIntsValues(flags, strsValues);
	if (!intsValues.isValid())
		return ValuesDoubles();
	ValuesDoubles percentages = toDoubles(intsValues);
	if (!percentages.isValid())
		return ValuesDoubles();

	writeDataToVariableClass(percentages);
	writeDataToVariableClass(intsValues);

	return percentages;
}
void ManaHealthStateAnalyzer::sendDataToGui(ValuesDoubles currentValues) {
	emit sendValueToMainThread(currentValues.health, currentValues.mana, currentValues.manaShield);
}
void ManaHealthStateAnalyzer::writeDataToVariableClass(ValuesDoubles values) {
	var->getVitalitty().setCurrentPercentage(values.health, values.mana, values.manaShield);
}
void ManaHealthStateAnalyzer::writeDataToVariableClass(ValuesInts values) {
	var->getVitalitty().setCurrentRawValues(values.health, values.mana, values.shield);
}
ManaHealthStateAnalyzer::ImageValues ManaHealthStateAnalyzer::getImages() {
	ImageValues toRet;
	bool clearImgs = var->getSettings().getClearVitalityImgs();;
	var->getVitalitty().getImageHealth(toRet.health, clearImgs);
	var->getVitalitty().getImageMana(toRet.mana, clearImgs);
	var->getVitalitty().getImageMS(toRet.manaShield, clearImgs);
	var->getVitalitty().getImageCombined(toRet.combined, clearImgs);
	return toRet;
}
bool ManaHealthStateAnalyzer::restMethodeCanBeUsed(const RestorationMethode& restMethode) {
	const qint64 now = QDateTime::currentMSecsSinceEpoch();
	Timers& timers = var->getTimers();
	switch (restMethode.getType())
	{
	case RestorationMethode::Type::POTION:
		if (now < timers.getTimeLastItemUsageGeneral() + (1000 * restMethode.getCdGroup()))
			return false;
		if (now < timers.getTimeLastItemUsage(restMethode.getName()) + (1000 * restMethode.getCd()))
			return false;
		//later should be added checking if char has proper pot!
		return true;
	case RestorationMethode::Type::SPELL:
		if (var->getVitalitty().getCurrentRawManaVal() < restMethode.getMana())
			return false;
		if (now < timers.getTimeLastSpellUsageHealing() + (1000 * restMethode.getCdGroup()))
			return false;
		if (now < timers.getTimeLastSpellUsed(restMethode.getName()) + (1000 * restMethode.getCd()))
			return false;
		return true;
		break;
	default: 
		return false;
		break;
	}
}
