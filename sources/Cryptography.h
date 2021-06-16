#pragma once
#include <QObject>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/rsaerr.h>
#include <openssl/err.h>
#include <openssl/bn.h>
#include <openssl/bio.h>
#include <openssl/x509.h>
#define ASSERT assert
#include <cassert>
#include <memory>
#include "qdebug.h"
#include "qcryptographichash.h"
#include "qfileinfo.h"
class Cryptography : public QObject{
	Q_OBJECT
public:
	Cryptography();
	bool encryptUsingUserPublicKey(QByteArray in, QByteArray& out);
	bool encryptUsingUserPublicKey(QByteArray& in_out);
	bool getUserIdFromFile(int& userID);
private:
	QString pathToPublicKey;
	bool encryptKey_Priv(std::string privateKey, QString in_dataToEncrypt, std::string& out_textEncrypted);
	bool encryptKey_Public(std::string pubKey, QString dataToEncrypt, std::string& encryptedText);
	bool decryptKey_Priv(std::string priKey, QString& encrpytedData, std::string encryptedText);
	bool decryptKey_Public(std::string publicKey, QString& out_decryptedData, std::string in_encryptedData);
	bool generateKeyPairToFile(uint keyLengthInBits, QString pathToOutPutFolder);
	
	bool getKeyFromFile(QString pathToFileWithPrivateKey, std::string& key, int* user_ID);
	bool readFile(QString pathToFileWithPrivateKey, QString& fileData);
	bool testPairOfKeys(QString pathToFolderWithBothKeys);
	bool testPairOfKeys(std::string publicKey, std::string privateKey);
};
