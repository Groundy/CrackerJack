#include "Cryptography.h"
#include <QtCore/qdir.h>

Cryptography::Cryptography(){
	pathToPublicKey = QDir::current().absoluteFilePath("public.txt");
	QByteArray s = "test";
	encryptUsingClientAppKey(s);
};


bool Cryptography::encryptUsingUserPublicKey(QByteArray in, QByteArray& out){
	std::string userPublicKey;
	bool keyAndIdFound = getKeyFromFile(pathToPublicKey, userPublicKey, NULL);
	if (!keyAndIdFound)
		return false;//todo

	std::string encryptedStr;
	bool encryptedOK = encryptKey_Public(userPublicKey, QString(in), encryptedStr);
	if(!encryptedOK)
		return false;//todo

	out = QByteArray::fromStdString(encryptedStr);
	return true;
}

bool Cryptography::encryptUsingUserPublicKey(QByteArray& in_out){
	return encryptUsingUserPublicKey(in_out, in_out);
}

bool Cryptography::encryptUsingClientAppKey(QByteArray in, QByteArray& out){
	std::string userPublicKey = QByteArray::fromHex(clientPublicKey).toStdString();
	std::string encryptedStr;
	bool encryptedOK = encryptKey_Public(userPublicKey, QString(in), encryptedStr);
	if (!encryptedOK)
		return false;//todo

	out = QByteArray::fromStdString(encryptedStr);
	return true;
}

bool Cryptography::encryptUsingClientAppKey(QByteArray& in_out){
	return encryptUsingClientAppKey(in_out, in_out);
}

bool Cryptography::getUserIdFromFile(int& userID){
	std::string noImportantKey;
	int userIdTmp;
	bool IdFound = getKeyFromFile(pathToPublicKey, noImportantKey, &userIdTmp);
	if (!IdFound)
		return false;//todo
	userID = userIdTmp;
	return true;
}

bool Cryptography::encryptKey_Priv(std::string privateKey, QString in_dataToEncrypt, std::string& out_textEncrypted){
	unsigned char* privateKeyData = (unsigned char*)privateKey.c_str();
	BIO* keybio = BIO_new_mem_buf(privateKeyData, -1);
	RSA* rsa = RSA_new();
	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	if (!rsa){
		qDebug() << QString("Error in encryptKey_Priv, wrong key passed");//todo
		BIO_free_all(keybio);
		return false;
	}

	// Get the maximum length of the data block that RSA can process at a time
	int key_len = RSA_size(rsa);
	int block_len = key_len - 11; // Because the filling method is RSA_PKCS1_PADDING, so you need to subtract 11 from the key_len

	// Apply for memory: store encrypted ciphertext data
	char* sub_text = new char[key_len + 1];
	memset(sub_text, 0, key_len + 1);
	int pos = 0;

	// Encrypt the data in segments (the return value is the length of the encrypted data)
	std::string clear_text = in_dataToEncrypt.toStdString();
	while (pos < clear_text.length()) {
		memset(sub_text, 0, key_len + 1);
		std::string sub_str = clear_text.substr(pos, block_len);
		const unsigned char* start = (const unsigned char*)sub_str.c_str();
		unsigned char* end = (unsigned char*)sub_text;
		int ret = RSA_private_encrypt(sub_str.length(), start, end, rsa, RSA_PKCS1_PADDING);
		if (ret >= 0) {
			std::string toAdd = std::string(sub_text, ret);
			out_textEncrypted.append(toAdd);
			pos += block_len;
		}
		else {
			qDebug() << QString("encryptKey_Priv") + QString("    Error in encrypting"); //todo 
			delete sub_text;
			BIO_free_all(keybio);
			RSA_free(rsa);
			return false;
		}
	}

	// release memory  
	delete sub_text;
	BIO_free_all(keybio);
	RSA_free(rsa);

	return true;
}
bool Cryptography::decryptKey_Public(std::string publicKey , QString& out_decryptedData, std::string in_encryptedData){
	out_decryptedData.clear();
	std::string decrypt_text;
	unsigned char* pubKeyData = (unsigned char*)publicKey.c_str();
	BIO* keybio = BIO_new_mem_buf(pubKeyData, -1);
	RSA* rsa = RSA_new();

	//rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);
	rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
	if (!rsa){
		BIO_free_all(keybio);
		return false;
	}

	// Get the maximum length of RSA single processing
	int len = RSA_size(rsa);
	char* sub_text = new char[len + 1];
	memset(sub_text, 0, len + 1);
	int pos = 0;

	// Decrypt the ciphertext in segments
	//encryptedText = QByteArray(encryptedText.c_str()).toBase64().toStdString();
	while (pos < in_encryptedData.length()) {
		std::string sub_str = in_encryptedData.substr(pos, len);
		memset(sub_text, 0, len + 1);
		const unsigned char* start = (const unsigned char*)sub_str.c_str();
		unsigned char* end = (unsigned char*)sub_text;
		int ret = RSA_public_decrypt(sub_str.length(), start, end, rsa, RSA_PKCS1_PADDING);
		if (ret >= 0) {
			std::string partOfDncryptedText = std::string(sub_text, ret);
			QString toAdd = QString::fromStdString(partOfDncryptedText);
			out_decryptedData.append(toAdd);
			printf("pos:%d, sub: %s\n", pos, sub_text);
			pos += len;
		}
		else {
			qDebug() << "Encrypting error"; //todo
			delete sub_text;
			BIO_free_all(keybio);
			RSA_free(rsa);
			return false;
		}
	}

	// release memory  
	delete sub_text;
	BIO_free_all(keybio);
	RSA_free(rsa);
	return true;
}
bool Cryptography::encryptKey_Public(std::string publicKey, QString in_dataToEncrypt, std::string& out_encryptedText){
	unsigned char* pubKeyData = (unsigned char*)publicKey.c_str();
	BIO* keybio = BIO_new_mem_buf(pubKeyData, -1);
	RSA* rsa = RSA_new();

	//rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);
	rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);

	// Get the maximum length of the data block that RSA can process at a time
	int key_len = RSA_size(rsa);
	int block_len = key_len - 11; // Because the filling method is RSA_PKCS1_PADDING, so you need to subtract 11 from the key_len

	// Apply for memory: store encrypted ciphertext data
	char* sub_text = new char[key_len + 1];
	memset(sub_text, 0, key_len + 1);
	int pos = 0;
	// Encrypt the data in segments (the return value is the length of the encrypted data)
	std::string clear_text = in_dataToEncrypt.toStdString();
	while (pos < clear_text.length()) {
		std::string sub_str = clear_text.substr(pos, block_len);
		memset(sub_text, 0, key_len + 1);
		const unsigned char* start = (const unsigned char*)sub_str.c_str();
		unsigned char* end = (unsigned char*)sub_text;
		int ret = RSA_public_encrypt(sub_str.length(), start, end, rsa, RSA_PKCS1_PADDING);
		if (ret >= 0) {
			std::string partOfEncryptedText = std::string(sub_text, ret);
			out_encryptedText.append(partOfEncryptedText);
			pos += block_len;
		}
		else {
			// release memory  
			qDebug() << "Error in encryptKey_Public";
			BIO_free_all(keybio);
			RSA_free(rsa);
			delete[] sub_text;
			return false;
		}
	}

	// release memory  
	BIO_free_all(keybio);
	RSA_free(rsa);
	delete[] sub_text;

	return true;
}
bool Cryptography::decryptKey_Priv(std::string privateKey, QString& out_decrpytedData, std::string in_encryptedText) {
	RSA* rsa = RSA_new();
	unsigned char* privateKeyData = (unsigned char*)privateKey.c_str();
	BIO* keybio = BIO_new_mem_buf(privateKeyData, -1);

	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	if (rsa == nullptr) {
		unsigned long err = ERR_get_error(); //Get the error number
		char err_msg[1024] = { 0 };
		ERR_error_string(err, err_msg); // Format: error:errId: library: function: reason
		QString errNumberAsStr = QString::number(err);
		QString errMsg = QString::fromStdString(err_msg);
		qDebug() << "error decryptKey_Priv   wrong key input";//to do
		qDebug() << errNumberAsStr + "   " + errMsg;//to do
		return false;
	}

	// Get the maximum length of RSA single processing
	int key_len = RSA_size(rsa);
	char* sub_text = new char[key_len + 1];
	memset(sub_text, 0, key_len + 1);
	int pos = 0;

	// Decrypt the ciphertext in segments
	while (pos < in_encryptedText.length()) {
		std::string sub_str = in_encryptedText.substr(pos, key_len);
		memset(sub_text, 0, key_len + 1);
		const unsigned char* start = (const unsigned char*)sub_str.c_str();
		unsigned char* end = (unsigned char*)sub_text;
		int ret = RSA_private_decrypt(sub_str.length(), start, end, rsa, RSA_PKCS1_PADDING);
		if (ret >= 0) {
			std::string partOfText = (std::string(sub_text, ret));
			QString toAdd = QString::fromStdString(partOfText);
			out_decrpytedData.append(toAdd);
			pos += key_len;
		}
		else {
			// release memory  
			delete[] sub_text;
			BIO_free_all(keybio);
			RSA_free(rsa);
			return false;
			qDebug() << "error in decryptKey_Priv";
		}
	}
	// release memory  
	delete[] sub_text;
	BIO_free_all(keybio);
	RSA_free(rsa);
	return true;

}


bool Cryptography::generateKeyPairToFile(uint keyLengthInBits, QString pathToOutPutFolder) {
	bool lengthOk = keyLengthInBits == 512 || keyLengthInBits == 1024 || keyLengthInBits == 2048 || keyLengthInBits == 4096;
	if(!lengthOk)
		keyLengthInBits = 2048;

	std::string tmpStrPrivate = pathToOutPutFolder.toStdString() + "\\private.txt";
	const char* privateKeyFile = tmpStrPrivate.c_str();

	std::string tmpStrPublic = pathToOutPutFolder.toStdString() + "\\public.txt";
	const char* publicKeyFile = tmpStrPublic.c_str();

	using std::unique_ptr;
	using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;
	using RSA_ptr = std::unique_ptr<RSA, decltype(&::RSA_free)>;
	using EVP_KEY_ptr = std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>;
	using BIO_FILE_ptr = std::unique_ptr<BIO, decltype(&::BIO_free)>;

	RSA_ptr rsa(RSA_new(), ::RSA_free);
	BN_ptr bn(BN_new(), ::BN_free);

	BIO_FILE_ptr publicKey(BIO_new_file(publicKeyFile, "wb"), ::BIO_free);
	BIO_FILE_ptr PrivateKey(BIO_new_file(privateKeyFile, "wb"), ::BIO_free);

	int rc = BN_set_word(bn.get(), RSA_F4);
	ASSERT(rc == 1);

	// Generate key
	rc = RSA_generate_key_ex(rsa.get(), keyLengthInBits, bn.get(), NULL);
	ASSERT(rc == 1);

	// Convert RSA to PKEY
	EVP_KEY_ptr pkey(EVP_PKEY_new(), ::EVP_PKEY_free);
	rc = EVP_PKEY_set1_RSA(pkey.get(), rsa.get());
	ASSERT(rc == 1);

	// Write public key in Traditional PEM
	rc = PEM_write_bio_PUBKEY(publicKey.get(), pkey.get());
	ASSERT(rc == 1);

	// Write private key in PKCS PEM.
	rc = PEM_write_bio_PrivateKey(PrivateKey.get(), pkey.get(), NULL, NULL, 0, NULL, NULL);
	ASSERT(rc == 1);

	return true;
}
bool Cryptography::getKeyFromFile(QString pathToFileWithPrivateKey, std::string& key, int* user_ID) {
	QString data;
	bool ok = readFile(pathToFileWithPrivateKey, data);
	if (!ok)
		return false;


	QStringList partsOfFile = data.split(QString("#"), Qt::SplitBehaviorFlags::SkipEmptyParts);
	int size = partsOfFile.size();
	if (size == 1) {
		//parts[0]		-----BEGIN PUBLIC KEY-----_key_-----END PUBLIC KEY-----
		key = partsOfFile[0].toStdString();
		return true;
	}
	else if (size == 2) {
		//parts[0]		USER::_number_
		//parts[1]		-----BEGIN PUBLIC KEY-----_key_-----END PUBLIC KEY-----
		QStringList partsOfUserId = partsOfFile[0].split(QString("::"), Qt::SplitBehaviorFlags::SkipEmptyParts);
		key = partsOfFile[1].remove(0, 1).toStdString();
		bool vaildFileStructure = partsOfUserId.size() == 2;
		if (vaildFileStructure&& user_ID != NULL) {
			//partsOfUserId[0] USER
			//partsOfUserId[1] _number_
			*user_ID = partsOfUserId[1].toInt(&vaildFileStructure);
		}
		return vaildFileStructure;
	}
	else {
		return false;//todo, poinformowac, wrong file structure
	}


	return true;
}

bool Cryptography::readFile(QString pathToFileWithPrivateKey, QString& fileData){
	//file struct -----BEGIN PUBLIC KEY-----_key_-----END PUBLIC KEY-----;
	QFileInfo dir(pathToFileWithPrivateKey);

	bool fileExist = dir.exists();
	if (!fileExist)
		;//return false;//todo, poinformowac

	bool fileCanBeRead = dir.isReadable();
	if (!fileCanBeRead)
		return false;//todo, poinformowac

	QFile file(pathToFileWithPrivateKey);
	bool filedOpened = file.open(QIODevice::OpenModeFlag::ReadOnly);
	if (!filedOpened)
		return false;//todo, poinformowac

	QByteArray data = file.readAll();
	bool isEmpty = data.size() == 0;
	if (isEmpty)
		return false;//todo, poinformowac

	fileData = QString(data);
	return true;
}
bool Cryptography::testPairOfKeys(QString pathToFolderWithBothKeys){
	std::string publicKey, privateKey;
	QString privPath = QDir(pathToFolderWithBothKeys).absoluteFilePath("private.txt");
	QString publicPath = QDir(pathToFolderWithBothKeys).absoluteFilePath("public.txt");
	bool filesExist = QFileInfo(privPath).exists() && QFileInfo(publicPath).exists();
	if (!filesExist) return false;
	bool ok1 = getKeyFromFile(publicPath, publicKey, NULL);
	bool ok2 = getKeyFromFile(privPath, privateKey, NULL);
	bool toRet = (ok1 && ok2) ? testPairOfKeys(publicKey, privateKey) : false;
	return toRet;
}
bool Cryptography::testPairOfKeys(std::string publicKey, std::string privateKey){
	QString toEncrypt = "Test sentence !@#123";
	std::string encrypted_Pri, encrypted_Pub;
	QString decrypted_Pub, decrypted_Priv;
	bool ok1 = encryptKey_Priv(privateKey, toEncrypt, encrypted_Pri);
	bool ok2 = encryptKey_Public(publicKey, toEncrypt, encrypted_Pub);
	bool ok3 = decryptKey_Priv(privateKey, decrypted_Priv, encrypted_Pub);
	bool ok4 = decryptKey_Public(publicKey, decrypted_Pub, encrypted_Pri);
	bool ok5 = decrypted_Pub == toEncrypt;
	bool ok6 = decrypted_Priv == toEncrypt;
	bool toRet = ok1 && ok2 && ok3 && ok4 && ok5 && ok6;
	return toRet;
}

