#include "AutoHunting.h"

AutoHunting::AutoHunting(QObject *parent, std::shared_ptr<VariablesClass> var, std::shared_ptr<GameConnecter> gameConnector, Route route)
	: QThread(parent), var(var), route(route), gameConnector(gameConnector){
}

AutoHunting::~AutoHunting()
{}
