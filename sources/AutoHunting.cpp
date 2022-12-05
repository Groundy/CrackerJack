#include "AutoHunting.h"

AutoHunting::AutoHunting(QObject *parent, std::shared_ptr<VariablesClass> var, std::shared_ptr<GameConnecter> gameConnector, Route route, QVector<AttackMethode> attackMethodes, Profile* profile)
	: QThread(parent), var(var), route(route), gameConnector(gameConnector), attackMethodes(attackMethodes), profile(profile){
}

AutoHunting::~AutoHunting()
{}
