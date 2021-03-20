#include "VariablesClass.h"

QString VariablesClass::outPutFolderPath()
{
	return 	QString("C:\\Users\\ADMIN\\Desktop\\output");
}

VariablesClass::VariablesClass(){
	var_healthPieceImg = new QImage();
	var_manaPieceImg = new QImage();
	var_manaShieldPieceImg = new QImage();
}

VariablesClass::~VariablesClass(){
	delete var_healthPieceImg;
	delete var_manaPieceImg;
	delete var_manaShieldPieceImg;
}


