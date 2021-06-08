#include "RsaKeyManager.h"

bool RsaKeyManager::genPairKey(){
	    size_t privateKeyLength;
	    size_t publicKeyLength;
	    char* privateKey;
	    char* publicKey;
	
        RSA *r = NULL;
        BIO* bp_public = NULL;
        BIO* bp_private = NULL;
        BIGNUM *bne = NULL;
        int bits = 2048;
        unsigned long e = RSA_F4;

        EVP_PKEY *evp_pbkey = NULL;
        EVP_PKEY *evp_pkey = NULL;

        BIO *pbkeybio = NULL;
        BIO *pkeybio = NULL;

        // 1. generate rsa key
        bne = BN_new();
        bool sucess = BN_set_word(bne, e) == 1;
        if (!sucess)
            goto free_all;

        r = RSA_new();
        bool sucess2 = RSA_generate_key_ex(r, bits, bne, NULL) == 1;
        if (!sucess2)
            goto free_all;

        // 2. save public key
        //bp_public = BIO_new_file("public.pem", "w+");
        bp_public = BIO_new(BIO_s_mem());
        bool sucess3 = PEM_write_bio_RSAPublicKey(bp_public, r) == 1;
        if (!sucess3)
            goto free_all;

        // 3. save private key
        //bp_private = BIO_new_file("private.pem", "w+");
        bp_private = BIO_new(BIO_s_mem());
        bool scuess4 = PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL) == 1;
        if (!scuess4)
            ;//unkown but possibly goto

        //4. Get the keys are PEM formatted strings
        privateKeyLength = BIO_pending(bp_private);
        publicKeyLength = BIO_pending(bp_public);

        privateKey = (char*) malloc(privateKeyLength + 1);
        publicKey = (char*) malloc(publicKeyLength + 1);

        BIO_read(bp_private, privateKey, privateKeyLength);
        BIO_read(bp_public, publicKey, publicKeyLength);

        privateKey[privateKeyLength] = '\0';
        publicKey[publicKeyLength] = '\0';

        printf("\n%s\n%s\n", privateKey, publicKey);

        //verify if you are able to re-construct the keys
        pbkeybio = BIO_new_mem_buf((void*) publicKey, -1);
        if (pbkeybio == NULL) {
            return -1;
        }
        evp_pbkey = PEM_read_bio_PUBKEY(pbkeybio, &evp_pbkey, NULL, NULL);
        if (evp_pbkey == NULL) {
            char buffer[120];
            ERR_error_string(ERR_get_error(), buffer);
            printf("Error reading public key:%s\n", buffer);
        }

        pkeybio = BIO_new_mem_buf((void*) privateKey, -1);
        if (pkeybio == NULL) {
            return -1;
        }
        evp_pkey = PEM_read_bio_PrivateKey(pkeybio, &evp_pkey, NULL, NULL);
        if (evp_pbkey == NULL) {
            char buffer[120];
            ERR_error_string(ERR_get_error(), buffer);
            printf("Error reading private key:%s\n", buffer);
        }

        BIO_free(pbkeybio);
        BIO_free(pkeybio);

        // 4. free
        free_all:

        BIO_free_all(bp_public);
        BIO_free_all(bp_private);
        RSA_free(r);
        BN_free(bne);

        return scuess4;
}
