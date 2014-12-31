/********************************************************************
        created:                2005/11/28
        created:                2005-11-28 15:45
        filename:               \chub\impl\src\common\SDAes.h
        file path:              \chub\impl\src\common
        file base:              SDAes
        file ext:               h
        author:                 hongjianfeng
        description:            AES class
*********************************************************************/

#ifndef SANDAI_C_AES_H_200511281545
#define SANDAI_C_AES_H_200511281545

#include <openssl/aes.h>
#include <cstring>

class AES
{
public:
    int setEncryptKey(const unsigned char *userKey, int bits = 128)
    {
        return AES_set_encrypt_key(userKey, bits, &aesKey_);
    }

    int setDecryptKey(const unsigned char *userKey, int bits = 128)
    {
        return AES_set_decrypt_key(userKey, bits, &aesKey_);
    }

    void encrypt(const unsigned char *in, unsigned char *out)
    {
        AES_encrypt(in, out, &aesKey_);
    }

    void decrypt(const unsigned char *in, unsigned char *out)
    {
        AES_decrypt(in, out, &aesKey_);
    }

    bool encrypt(const unsigned char *in, int inlen, unsigned char *out, int *outlen);
    bool decrypt(const unsigned char *in, int inlen, unsigned char *out, int *outlen);

private:
    AES_KEY aesKey_;
};

#endif

