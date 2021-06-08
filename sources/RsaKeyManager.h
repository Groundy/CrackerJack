#pragma once
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/rsaerr.h>
#include <openssl/err.h>
class RsaKeyManager{
	static bool genPairKey();
};

