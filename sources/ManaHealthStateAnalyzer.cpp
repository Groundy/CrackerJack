#include "ManaHealthStateAnalyzer.h"

ManaHealthStateAnalyzer::ManaHealthStateAnalyzer(QObject *parent, VariablesClass* varClass)
	: QThread(parent){
	healthImg = new QImage();
	manaImg = new QImage();
	stateImg = new QImage();
	var = varClass;
}

ManaHealthStateAnalyzer::~ManaHealthStateAnalyzer(){
	delete healthImg;
	delete manaImg;
	delete stateImg;
}

void ManaHealthStateAnalyzer::run(){
	;// mainLoop();
}

void ManaHealthStateAnalyzer::mainLoop(){
	while (true){
		bool isReady = var->var_imgHealthManaStates_readyToRead;
		if (!isReady) {
			msleep(miliSecBetweenCheckingForNewValuesImg);
			continue;
		}
		*healthImg = *var->var_imgHealth;
		//*manaImg = *var->var_imgMana;
		//*stateImg = *var->var_imgStates;

	}

}

float ManaHealthStateAnalyzer::getPercentagesOfHealth(){
	float toRet = 0.0f;
	return 0.0f;
}
