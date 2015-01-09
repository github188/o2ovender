#include "SDAes.h"

bool AES::encrypt(const unsigned char *in, int inlen, unsigned char *out, int *outlen)
{
    if (!in || inlen < 0 || !out || !outlen)
    {
        return false;
    }

    *outlen = 0;

    while (inlen >= AES_BLOCK_SIZE)
    {
        encrypt(in, out);
        in += AES_BLOCK_SIZE;
        out += AES_BLOCK_SIZE;
        inlen -= AES_BLOCK_SIZE;
        *outlen += AES_BLOCK_SIZE;
    }

    if (inlen >= 0)
    {
        unsigned char tmp[AES_BLOCK_SIZE] = {0};
        memcpy(tmp, in, inlen);
        memset(tmp + inlen, AES_BLOCK_SIZE - inlen, AES_BLOCK_SIZE - inlen);
        encrypt(tmp, out);
        *outlen += AES_BLOCK_SIZE;
    }

    return true;
}

bool AES::decrypt(const unsigned char *in, int inlen, unsigned char *out, int *outlen)
{
    if (!in || inlen <= 0 || inlen % AES_BLOCK_SIZE != 0 || !out || !outlen)
    {
        return false;
    }

    *outlen = 0;

    while (inlen > 0)
    {
        decrypt(in, out);
        in += AES_BLOCK_SIZE;
        out += AES_BLOCK_SIZE;
        inlen -= AES_BLOCK_SIZE;
        *outlen += AES_BLOCK_SIZE;
    }

    --out;
    if (*out <= 0 || *out > AES_BLOCK_SIZE)
    {
        return false;
    }

    *outlen -= *out;

    return true;
}

