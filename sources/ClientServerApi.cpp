#include "ClientServerApi.h"
//this class is shared between Client and Server;


QMap<REASON_TO_CONNECT_TO_SERVER, QByteArray> ClientServerApi::reasonMap = {
	{REASON_TO_CONNECT_TO_SERVER::ASK_FOR_NEWEST_VERSION, "ASK_FOR_NEWEST_VERSION"},
	{REASON_TO_CONNECT_TO_SERVER::ASK_FOR_VALIDITY_OF_KEY, "ASK_FOR_VALIDITY_OF_KEY"},
	{REASON_TO_CONNECT_TO_SERVER::ASK_FOR_KEY_EXP_DATE, "ASK_FOR_KEY_EXP_DATE"},
	{REASON_TO_CONNECT_TO_SERVER::REGISTER_KEY, "REGISTER_KEY"},
	{REASON_TO_CONNECT_TO_SERVER::ASK_FOR_TIME, "ASK_FOR_TIME"},
	{REASON_TO_CONNECT_TO_SERVER::UNKNOWN, "UNKOWN_REASON"}
};

QMap<FIELDS_OF_MSGS, QByteArray> ClientServerApi::fieldMap = {
	{FIELDS_OF_MSGS::IP_ADRESS, "IP_ADRESS"},
	{FIELDS_OF_MSGS::MAC_ADRESS, "MAC_ADRESS"},
	{FIELDS_OF_MSGS::REASON, "REASON"},
	{FIELDS_OF_MSGS::USER_ID, "USER_ID"},
	{FIELDS_OF_MSGS::VERSION_STR, "VERSION_STR"},
	{FIELDS_OF_MSGS::RESPONSE_TYPE, "RESPONSE_TYPE"},
	{FIELDS_OF_MSGS::CURRENT_EPOCH_UTC_TIME_STR, "CURRENT_EPOCH_UTC_TIME_STR"},
	{FIELDS_OF_MSGS::UNKOWN, "UNKOWN_FIELD"}
};


QMap<SERVER_RESPONSES, QByteArray> ClientServerApi::responsMap = {
	{SERVER_RESPONSES::UNKNOWN, "UNKNOWN_RESPONS"},
	{SERVER_RESPONSES::RET_NEWEST_VERSION_STR, "NEWEST_VERSION_STR"},
	{SERVER_RESPONSES::KEY_IS_VALID, "KEY_IS_VALID"},
	{SERVER_RESPONSES::KEY_IS_VALID_BUT_NOT_REGISTER, "KEY_IS_VALID_BUT_NOT_REGISTER"},
	{SERVER_RESPONSES::KEY_EXPERIENCE_TIME, "KEY_EXPERIENCE_TIME"},
	{SERVER_RESPONSES::RESULT_OF_REGISTARTION, "RESULT_OF_REGISTARTION"},
	{SERVER_RESPONSES::USER_KEY_DOESNT_MATCH, "USER_KEY_DOESNT_MATCH"},
	{SERVER_RESPONSES::KEY_FILE_WRONG_STR, "KEY_FILE_WRONG_STR"},
	{SERVER_RESPONSES::RET_CURRENT_EPOCH_UTC_TIME, "RET_CURRENT_EPOCH_UTC_TIME"}
};

QList<MarkUp> ClientServerApi::splitMsgToMarkUps(QByteArray& decryptedMsg) {
	/*	expected Structure	<FIELD1===VALUE1>\n<FIELD2===VALUE2>\n<FIELD3===VALUE3>\n...	*/
	QList<MarkUp> toRet;
	QByteArrayList allParts = decryptedMsg.split('\n');
	for each (QString var in allParts) {
		bool hasOpening = var.contains("<");
		bool hasEnding = var.contains(">");
		bool hasEqSign = var.contains("===");
		bool vailStructure = hasEnding && hasOpening && hasEqSign;
		if (!vailStructure)	continue;
		
		QStringList partsOfMarkUp = var.split("===",Qt::SkipEmptyParts);//suspicious //todo
		if (partsOfMarkUp.size() != 2)	continue;

		QString leftValue = partsOfMarkUp[0];
		if (leftValue.isEmpty()) continue;
		vailStructure = leftValue.contains('<');
		if (!vailStructure) continue;
		QByteArray fieldVal = leftValue.remove(0, 1).toUtf8();

		QString rightValue = partsOfMarkUp[1];
		if (rightValue.isEmpty()) continue;
		vailStructure = rightValue.contains('>');
		if (!vailStructure) continue;
		QByteArray fieldValue = rightValue.remove(rightValue.size() - 1, 1).toUtf8();//suspicious //todo

		FIELDS_OF_MSGS field = fieldMap.key(fieldVal, FIELDS_OF_MSGS::UNKOWN);

		MarkUp toAdd;
		toAdd.field = field;
		toAdd.value = fieldValue;
		toRet.append(toAdd);
	}
	return toRet;
}

QByteArray ClientServerApi::listMarkUpsToMsg(QList<MarkUp> list) {
	QByteArray toRet;
	for each (MarkUp var in list)
		toRet.append(getMarkUp(var.field, var.value));
	return toRet;
}
QByteArray ClientServerApi::getMarkUp(FIELDS_OF_MSGS markUpField, QByteArray markUpValue) {
	QByteArray nameOfField = ClientServerApi::fieldMap[markUpField];
	QByteArray toRet = QByteArray("<") + nameOfField + QByteArray("===") + markUpValue + QByteArray(">") + QByteArray("\n");
	return toRet;
}