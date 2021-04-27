#include "JsonParser.h"

JsonParser::JsonParser()
{
}

JsonParser::~JsonParser()
{
}

bool JsonParser::openJsonFile(QJsonObject* jsonDoc, QString pathToFile){
    QString val;
    QFile file;
    file.setFileName(pathToFile);
    bool sucess = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!sucess)
        return false;//diag //err
    val = file.readAll();
    if (val.size() == 0)
        return false;//diag //err
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    *jsonDoc = doc.object();
    return true;
}

bool JsonParser::readSpellsJson(QList<Utilities::Spell>* spells){
    typedef Utilities::Spell Spell;
    QJsonObject obj;
    bool res = openJsonFile(&obj, spellFilePath);
    if (!res)
        return false;//diag //err

    QJsonArray arr = obj["spells"].toArray();
    int size = arr.count();
    if (size == 0)
        return false;//diag err
    QList<Spell> spellsToRet;
    for (size_t i = 0; i < size; i++){
        QJsonValue val = arr.at(i);
        Spell objToAdd;
        objToAdd.name = val["name"].toString();
        objToAdd.incantations = val["incantantion"].toString();
        objToAdd.EK = val["EK"].toBool();
        objToAdd.RP = val["RP"].toBool();
        objToAdd.MS = val["MS"].toBool();
        objToAdd.ED = val["ED"].toBool();
        objToAdd.mana = val["mana"].toInt();
        objToAdd.cd = val["CD"].toInt();
        objToAdd.cdGroup = val["CDGROUP"].toInt();
        objToAdd.soulPoints = val["SoulPoints"].toInt();
        QString typeAsStr = val["TYPE"].toString();
        Spell::TYPE_OF_SPELL type;
        if (typeAsStr == "HEALING")
            type = Spell::HEALING;
        else if (typeAsStr == "SUPPORT")
            type = Spell::SUPPORT;
        else if (typeAsStr == "ATTACK")
            type = Spell::ATTACK;
        else
            return false; // diag //err
        objToAdd.typeOfSpell = type;
        spellsToRet.push_back(objToAdd);
    }
    *spells = spellsToRet;
    return true;
}

bool JsonParser::filtrSpells(QList<Utilities::Spell>* spells, Profile::PROFESSION* prof, Utilities::Spell::TYPE_OF_SPELL* type){
    if (prof == NULL && type == NULL)
        return false;
    typedef Utilities::Spell Spell;

    //filtr by type
    if (type != NULL) {
        QList<Utilities::Spell> spellsToRet;
        for each (Spell var in *spells) {
            bool isProperType = var.typeOfSpell == *type;
            if (isProperType)
                spellsToRet.push_back(var);
        }
        *spells = spellsToRet;
    }
    if (prof != NULL) {
        QList<Utilities::Spell> spellsToRet;
        for each (Spell var in *spells) {
            Profile::PROFESSION profToCompare;
            if (var.ED)
                profToCompare = Profile::ED;
            else if (var.EK)
                profToCompare = Profile::EK;
            else if (var.MS)
                profToCompare = Profile::MS;
            else if (var.RP)
                profToCompare = Profile::RP;
            else
                return false; //diag //err

            bool isProperProf = profToCompare == *prof;
            if (isProperProf)
                spellsToRet.push_back(var);
        }
        *spells = spellsToRet;
    }
    return true;
}

bool JsonParser::readItemJson(QList<Utilities::Item>* items){
    typedef Utilities::Item Item;
    QJsonObject obj;
    bool res = openJsonFile(&obj, itemsFilePath);
    if (!res)
        return false;//diag //err

    QJsonArray arr;
    QStringList listOfCategoryNames = { "armors","amulets","boots","creature","helmets",
        "legs","other","potions","rings","runes","shields","valuables","ammo","axes","swords",
        "clubs","distance","rod","wands"
    };
    for each (QString var in listOfCategoryNames) {
        QJsonArray toAdd = obj[var].toArray();
        for each (QJsonValue val in toAdd){
            arr.push_back(val);
        }
    }

    int size = arr.count();
    if (size == 0)
        return false;//diag err
    QList<Item> itemToRet;
    for (size_t i = 0; i < size; i++) {
        QJsonValue val = arr.at(i);
        Item objToAdd;
        objToAdd.name = val["name"].toString();
        objToAdd.price = val["price"].toInt();
        objToAdd.weight = val["weight"].toInt();
        if (val["sell_blue_dijn"].toBool())
            objToAdd.seller = Item::SELLER::BLUE_DJIN;
        else if (val["sell_green_dijn"].toBool())
            objToAdd.seller = Item::SELLER::GREEN_DJIN;
        else if (val["sell_yasir"].toBool())
            objToAdd.seller = Item::SELLER::YASIR;
        else if (val["sell_zao"].toBool())
            objToAdd.seller = Item::SELLER::ZAO;
        else if (val["sell_other"].toBool())
            objToAdd.seller = Item::SELLER::OTHER_SELLER;
        else if (val["sell_rashid"].toBool())
            objToAdd.seller = Item::SELLER::RASHID;
        else
            return false;//diag //err

        QString typeOfItem = val["type"].toString();
        
        if (typeOfItem == QString("ARMOR"))
            objToAdd.type = Item::TYPE_OF_ITEM::ARMOR;
        else if (typeOfItem == QString("AMULET"))
            objToAdd.type = Item::TYPE_OF_ITEM::AMULETS;
        else if (typeOfItem == QString("BOOTS"))
            objToAdd.type = Item::TYPE_OF_ITEM::BOOTS;
        else if (typeOfItem == QString("CREATURE"))
            objToAdd.type = Item::TYPE_OF_ITEM::CREATURE;
        else if (typeOfItem == QString("HELMET"))
            objToAdd.type = Item::TYPE_OF_ITEM::HELMETS;
        else if (typeOfItem == QString("LEGS"))
            objToAdd.type = Item::TYPE_OF_ITEM::LEGS;
        else if (typeOfItem == QString("OTHER"))
            objToAdd.type = Item::TYPE_OF_ITEM::OTHER;
        else if (typeOfItem == QString("POTION"))
            objToAdd.type = Item::TYPE_OF_ITEM::POTIONS;
        else if (typeOfItem == QString("RING"))
            objToAdd.type = Item::TYPE_OF_ITEM::RINGS;
        else if (typeOfItem == QString("RUNE"))
            objToAdd.type = Item::TYPE_OF_ITEM::RUNES;
        else if (typeOfItem == QString("SHIELD"))
            objToAdd.type = Item::TYPE_OF_ITEM::SHIELDS;
        else if (typeOfItem == QString("AMMO"))
            objToAdd.type = Item::TYPE_OF_ITEM::AMMO;
        else if (typeOfItem == QString("AXE"))
            objToAdd.type = Item::TYPE_OF_ITEM::AXES;
        else if (typeOfItem == QString("SWORD"))
            objToAdd.type = Item::TYPE_OF_ITEM::SWORDS;
        else if (typeOfItem == QString("CLUB"))
            objToAdd.type = Item::TYPE_OF_ITEM::CLUBS;
        else if (typeOfItem == QString("DISTANCE"))
            objToAdd.type = Item::TYPE_OF_ITEM::DISTANCES;
        else if (typeOfItem == QString("ROD"))
            objToAdd.type = Item::TYPE_OF_ITEM::ROD;
        else if (typeOfItem == QString("WAND"))
            objToAdd.type = Item::TYPE_OF_ITEM::WANDS;
        else
            return false;

        itemToRet.push_back(objToAdd);
    }
    *items = itemToRet;
    return true;
}
