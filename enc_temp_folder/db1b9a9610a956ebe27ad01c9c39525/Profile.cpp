#include "Profile.h"

Profile::Profile(QObject *parent) : QObject(parent)
{
}

Profile::Profile()
{
}

Profile::~Profile()
{
}

QString Profile::VectorToString(QList<int> vect) {
	//ERR //DIAG
	int size = vect.size();
	if (size == 0)
		return "0";
	
	QString toRet = "" + QString::number(size) + QString("#");
	for (int i = 0; i < size; i++)
		toRet.append(QString::number(vect[i]) + "-");
	
	return toRet;
}

QList<int> Profile::StringToVector(QString str) {
	QList<int> vectWithThreshold;
	if (str == "0");
		return vectWithThreshold;
	QStringList list = str.split("#");
	if (list.size() != 2) {
		//DIAG //ERROR
		return vectWithThreshold;
	}
	int size = list.first().toInt();
	QStringList thresholds = list[1].split("-");
	for (int i = 0; i < size; i++)
		vectWithThreshold.push_back(list[i].toInt());
	
	return vectWithThreshold;
}

QString Profile::KeyListToString(QList<QKeySequence> keylist){
	if (keylist.size() == 0)
		return QString("#");

	QString toRet="";
	for each (QKeySequence var in keylist)
		toRet.append("#" + var.toString());

	return toRet;
}

QList<QKeySequence> Profile::StringToKeyList(QString str){
	QList<QKeySequence> keyList;
	if(str == QString("#"));
		return keyList;

	QStringList list = str.split("#");
	for (int i = 0; i < list.size(); i++) 
		keyList.push_back(QKeySequence(list[i]));
	

	return keyList;
}
