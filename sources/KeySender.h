#pragma once
#include <qthread.h>
#include "Profile.h"
#include "Key.h"
#include "qdatetime.h"
#include "qkeysequence.h"
#include "Utilities.h"
#include "VariablesClass.h"
class KeySender : public QThread
{
	Q_OBJECT
public:
	enum ERROR_CODE {
		OK = 0,
		UNDEFINED_ERROR = 2
	};
	KeySender(QObject* parent, Profile* profile, VariablesClass* variableClass);
	~KeySender();
	void run();
private:
	VariablesClass* var;
	void mainLoop();
};