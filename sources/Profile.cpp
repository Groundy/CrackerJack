#include "Profile.h"

Profile::Profile(QObject *parent) : QObject(parent)
{
}

Profile::~Profile()
{
}

QString Profile::VectorToString(QVector<int> vect) {
	//ERR //DIAG
	int size = vect.size();
	if (size == 0)
		return "0";
	
	QString toRet = "" + size + QString("#");
	for (int i = 0; i < size; i++)
		toRet.append(vect[i] + "-");
	
	return toRet;
}

QVector<int> Profile::StringToVector(QString str) {
	QVector<int> vectWithThreshold;
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
