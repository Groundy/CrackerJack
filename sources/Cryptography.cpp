#include "Cryptography.h"
#include <QtCore/qdir.h>

Cryptography::Cryptography(){
	generateKeyPairToFile(512*4, "C:\\Users\\ADMIN\\Desktop\\output");
	QString tmpPrivate = "C:\\Users\\ADMIN\\Desktop\\output\\private.txt";
	QString tmpPublic = "C:\\Users\\ADMIN\\Desktop\\output\\public.txt";
	std::string keyPrivate = getKeyFromFile(tmpPrivate, false);
	std::string keyPublic = getKeyFromFile(tmpPublic, false);

	std::string encryptedText;
	QString toEncrypt = "s";
	QString toBeDecrypted;
	bool ok2 = encryptKey_Public(keyPublic, toEncrypt, encryptedText);
	bool ok1 = decryptKey_Priv(keyPrivate, toBeDecrypted, encryptedText);
	int b = 2;
};


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

std::string Cryptography::getKeyFromFile(QString pathToFileWithPrivateKey, bool cutHeadersOut){
	QDir dir(pathToFileWithPrivateKey);

	bool fileExist = dir.exists();
	if (!fileExist)
		;//todo

	bool fileCanBeRead = dir.isReadable();
	if (!fileCanBeRead)
		;//todo

	QFile file(pathToFileWithPrivateKey);
	bool filedOpened = file.open(QIODevice::OpenModeFlag::ReadOnly);
	if (!filedOpened)
		;//todo

	QByteArray data = file.readAll();
	bool isEmpty = data.size() == 0;
	if (isEmpty)
		;//todo

	QString fileContent = QString(data);

	if(!cutHeadersOut)
		return fileContent.toStdString();

	const QString HEADER_MARK = QString("-----");
	QStringList partOfKeyFile = fileContent.split(HEADER_MARK, Qt::SkipEmptyParts);

	//header, key, foot and sometimes empty part at the end of file
	bool vaildKeyFileStruct = partOfKeyFile.size() == 3 || partOfKeyFile.size() == 4;
	if (!vaildKeyFileStruct)
		;//todo

	std::string key = partOfKeyFile[1].toStdString();
	return key;
}

bool Cryptography::TEST_maxLengthOfEncrypting(std::string privateKey, int& maxNumberOfchars, int lenghthOfKey){
	generateKeyPairToFile(lenghthOfKey, "C:\\Users\\ADMIN\\Desktop\\output");
	QString tmp = "C:\\Users\\ADMIN\\Desktop\\output\\private.txt";
	std::string key = getKeyFromFile(tmp, true);

	for (int i = 4; i < 2048; i += 10) {
		qDebug() << QString::number(i);
		QString fileName = "encrypted_" + QString::number(i) + ".txt";
		QFile filePath("C:\\Users\\ADMIN\\Desktop\\output\\" + fileName);
		QString wordToEncrypt = "";
		wordToEncrypt.fill('H', i);
		std::string encrpyedText;
		bool sucess = encryptKey_Priv(key, wordToEncrypt, encrpyedText);
		if (!sucess) {
			maxNumberOfchars = i;
			break;
		}
	}
	return true;
}
