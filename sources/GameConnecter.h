#pragma once
#include <QObject>
//#include "tlhelp32.h"
#include <comdef.h> 

#include "Logger.h"
#include "VariablesClass.h"
#include "Key.h"
class GameConnecter  : public QObject
{
	Q_OBJECT
public:
	GameConnecter(QObject* parent, std::shared_ptr<VariablesClass> var);
	~GameConnecter();
	void clickLeft(QPoint pt);
	void clickRight(QPoint pt);
	void sendStringToGame(QString str);
	bool sendKeyStrokeToProcess(Key key);
private:
	std::shared_ptr<VariablesClass> var;
};
