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
class Cryptography{
public:
	Cryptography();
	bool encryptUsingUserPublicKey(QByteArray in, QByteArray& out);
	bool encryptUsingUserPublicKey(QByteArray& in_out);
	bool encryptUsingClientAppKey(QByteArray in, QByteArray& out);
	bool encryptUsingClientAppKey(QByteArray& in_out);
	bool getUserIdFromFile(int& userID);
private:
	QString pathToPublicKey;//todo tmp
	
	bool encryptKey_Priv(std::string privateKey, QString in_dataToEncrypt, std::string& out_textEncrypted);
	bool encryptKey_Public(std::string pubKey, QString dataToEncrypt, std::string& encryptedText);
	bool decryptKey_Priv(std::string priKey, QString& encrpytedData, std::string encryptedText);
	bool decryptKey_Public(std::string publicKey, QString& out_decryptedData, std::string in_encryptedData);
	bool generateKeyPairToFile(uint keyLengthInBits, QString pathToOutPutFolder);
	
	bool getKeyFromFile(QString pathToFileWithPrivateKey, std::string& key, int* user_ID);
	bool readFile(QString pathToFileWithPrivateKey, QString& fileData);
	bool testPairOfKeys(QString pathToFolderWithBothKeys);
	bool testPairOfKeys(std::string publicKey, std::string privateKey);

	const QByteArray clientPublicKey = "2d2d2d2d2d424547494e205055424c4943204b45592d2d2d2d2d0a4d494942496a414e42676b71686b6947397730424151454641414f43415138414d49494243674b434151454172656a79484b2f5755336d596a45356b664a33580a6366356c656f413775624353715961723935662f6d4d4b3968316e427754556d2f6a506e4a3246354973785a545437596866435a33476750334a3870655547770a4a7a4f575634376b5039557a35697a6a4b65523762475a4f7535363161576e5965684966325a45306f504d637834324f69415334526a567a424d7371646a6b660a2b2b2f765862414a31657656433374634672746544636b303173566d356d6a6d7041555679416e506e6141625566384876396d5533656b3779316e487465362b0a667876646a324b484a316e7a44496179695556474b4f436b4841307a47536b336e5065456a31597449565976526973644e694c4466484a4345384632384e63700a344c396b364f6b575549493850656e466f65614343554436504b6867476d536a726e4d5478554f467963343352526e306e4d71767764592f756c46392b4d72420a54774944415141420a2d2d2d2d2d454e44205055424c4943204b45592d2d2d2d2d";
};
