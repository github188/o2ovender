#ifndef SD_AVM_REQUEST_H
#define SD_AVM_REQUEST_H

#include <string>
#include <stdint.h>

class SDAVMRequest
{
public:
    SDAVMRequest();
    virtual ~SDAVMRequest();

    bool encode(char* buf, int* size);
    bool decode(const char* buf, int size);

    bool encode_head(char* buf, int* size);
    bool decode_head(const char* buf, int size);

    virtual bool encode_body(char* buf, int* size);
    virtual bool decode_body(const char* buf, int size);

public:
    static const int HEAD_SIZE = 37;
    char m_head[HEAD_SIZE];
    uint32_t m_length;
};

#endif
