//this class is shared between Client and Server;
#pragma once
#include <QObject>
#include <qdebug.h>
enum class REASON_TO_CONNECT_TO_SERVER {
	ASK_FOR_NEWEST_VERSION,//Retrun Str with newest version to client to compare with it's local version
	ASK_FOR_VALIDITY_OF_KEY,//Check if combination of userId public key have coresponding private key at usage
	ASK_FOR_KEY_EXP_DATE,// Return date with expiration date of that key
	REGISTER_KEY,//Set expiration date of this Key
	ASK_FOR_TIME,//Return current time

	END_ENUM_REASON//to Count enum 
};
enum class FIELDS_OF_MSGS {
	IP_ADRESS,
	MAC_ADRESS,
	REASON,
	USER_ID,
	RESPONSE_TYPE,
	VERSION_STR,
	UNKOWN,

	END_ENUM_FIELD//to Count enum 
};
enum class SERVER_RESPONSES {
	UNDEFINED_ERROR,
	NEWEST_VERSION_STR,//Server return newest version Str
	KEY_IS_VALID,// userID and public client has current active private key on server
	KEY_NOT_VALID,// userID and public client key, has no exisiting private key on server 
	KEY_IS_VALID_BUT_NOT_REGISTER,
	KEY_EXPERIENCE_TIME,
	RESULT_OF_REGISTARTION,
	USER_KEY_DOESNT_MATCH,
	KEY_FILE_WRONG_STR,






	END_ENUM_RESPONSE//toCount enum 
};

struct MarkUp {
	FIELDS_OF_MSGS field;
	QByteArray value;
};

class ClientServerApi : public QObject
{
	Q_OBJECT
public:
	static QMap<REASON_TO_CONNECT_TO_SERVER, QByteArray> reasonMap;
	static QMap<FIELDS_OF_MSGS, QByteArray> fieldMap;
	static QMap<SERVER_RESPONSES, QByteArray> responsMap;

	static QList<MarkUp> splitMsgToMarkUps(QByteArray& encryptedMsg);
	static QByteArray listMarkUpsToMsg(QList<MarkUp> list);
	static QByteArray getMarkUp(FIELDS_OF_MSGS markUpField, QByteArray markUpValue);

};
