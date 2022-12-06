#include "AutoHunting.h"

AutoHunting::AutoHunting(QObject *parent, std::shared_ptr<VariablesClass> var, std::shared_ptr<GameConnecter> gameConnector, Route route, Profile* profile)
	: QThread(parent), var(var), route(route), gameConnector(gameConnector), profile(profile){
	auto data = profile->getAutoHuntData();
	attackMethodes = data.getAttacks();
	minEnemiesToStop = data.getMinMonToStop();
	minEnemiesToContinue = data.getMinMonToContinue();
}

AutoHunting::~AutoHunting()
{}
