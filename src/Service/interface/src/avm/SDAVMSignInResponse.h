#ifndef SD_AVM_SIGN_IN_RESPONSE_H
#define SD_AVM_SIGN_IN_RESPONSE_H

#include "SDAVMRequest.h"

class SDAVMSignInResponse : public SDAVMRequest
{
public:
    SDAVMSignInResponse();
    virtual ~SDAVMSignInResponse();

    virtual bool encode_body(char* buf, int* size);
    virtual bool decode_body(const char* buf, int size);

    void set_response_code(const char* ptr);
    std::string get_response_code();
    void get_response_code(char** ptr, int* size);
    
    void set_timestamp(const char* ptr);
    std::string get_timestamp();
    void get_timestamp(char** ptr, int* size);

public:
    DECL_LOGGER(logger);
    static const int BODY_SIZE = 16;
    char m_body[BODY_SIZE];
};

#endif
