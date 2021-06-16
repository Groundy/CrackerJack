//this class is shared between Client and Server;
#include <string>
enum class REASON_TO_CONNECT_TO_SERVER {
	ASK_FOR_NEWEST_VERSION,
	ASK_FOR_VALIDITY_OF_KEY,
	ASK_FOR_KEY_EXP_DATE,
	REGISTER_KEY,
	ASK_FOR_TIME
};
enum class FIELDS_OF_MSGS {
	IP_ADRESS,
	MAC_ADRESS,
	REASON,
	USER_ID
};
enum class SERVER_RESPONSES {
	UNDEFINED_ERROR,
	NEWEST_VERSION_STR,
	KEY_IS_VALID,
	KEY_NOT_VALID,
	KEY_EXPERIENCE_TIME,
	RESULT_OF_REGISTARTION,
	USER_KEY_DOESNT_MATCH,
	KEY_FILE_WRONG_STR
};

inline std::string getConnectionReasonStr(REASON_TO_CONNECT_TO_SERVER reason){
	std::string toRet;
	switch (reason)
	{
	case REASON_TO_CONNECT_TO_SERVER::ASK_FOR_NEWEST_VERSION:
		{toRet = "ASK_FOR_NEWEST_VERSION";  break; }
	case REASON_TO_CONNECT_TO_SERVER::ASK_FOR_VALIDITY_OF_KEY:
		{toRet = "ASK_FOR_VALIDITY_OF_KEY";  break; }
	case REASON_TO_CONNECT_TO_SERVER::ASK_FOR_KEY_EXP_DATE:
		{toRet = "ASK_FOR_KEY_EXP_DATE";  break; }
	case REASON_TO_CONNECT_TO_SERVER::REGISTER_KEY:
		{toRet = "REGISTER_KEY";  break; }
	case REASON_TO_CONNECT_TO_SERVER::ASK_FOR_TIME:
		{toRet = "ASK_FOR_TIME";  break; }
	default: 
		{toRet = "";  break; };
	}
	return toRet;
}

inline std::string getFieldStr(FIELDS_OF_MSGS field) {
	std::string toRet;
	switch (field)
	{
	case FIELDS_OF_MSGS::IP_ADRESS:
		{toRet = "IP_ADRESS";   break; }
	case FIELDS_OF_MSGS::MAC_ADRESS:
		{toRet = "MAC_ADRESS";   break; }
	case FIELDS_OF_MSGS::REASON:
		{toRet = "REASON";   break; }
	case FIELDS_OF_MSGS::USER_ID:
		{toRet = "USER_ID";   break; }
	default:
		{toRet = "UNKOWN";   break;}
	}
	return toRet;
}