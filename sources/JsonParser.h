#pragma once
#include <QObject>
#include <qjsondocument.h>
#include "qjsonobject.h"
#include "qjsonvalue.h"
#include "qjsonarray.h"
#include "qfile.h"
#include "Utilities.h"
#include "Profile.h"
class JsonParser : public QObject
{
	Q_OBJECT

public:
	JsonParser();
	~JsonParser();
	const QString spellFilePath = "C:\\Users\\ADMIN\\source\\repos\\CrackerJackClient\\spells.json";
	const QString itemsFilePath = "C:\\Users\\ADMIN\\source\\repos\\CrackerJackClient\\items.json";
	bool openJsonFile(QJsonObject* jsonDoc, QString pathToFile);
	bool readSpellsJson(QList<Utilities::Spell>* spells);
	bool filtrSpells(QList<Utilities::Spell>* spells, Profile::PROFESSION* prof, Utilities::Spell::TYPE_OF_SPELL* type);
	bool readItemJson(QList<Utilities::Item>* items);

};