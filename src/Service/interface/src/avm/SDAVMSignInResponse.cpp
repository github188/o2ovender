#include "SDAVMSignInResponse.h"

using namespace std;

IMPL_LOGGER(SDAVMSignInResponse, logger);

SDAVMSignInResponse::SDAVMSignInResponse()
{}

SDAVMSignInResponse::~SDAVMSignInResponse()
{}

bool SDAVMSignInResponse::encode_body(char* buf, int* size)
{
    *size = BODY_SIZE;
    return true;
}

bool SDAVMSignInResponse::decode_body(const char* buf, int size)
{
    return true;
}

void SDAVMSignInResponse::set_response_code(const char* ptr)
{
    memcpy(m_body+0,ptr,2);
}
std::string SDAVMSignInResponse::get_response_code()
{
    return std::string(m_body+0,2);
}
void SDAVMSignInResponse::get_response_code(char** ptr, int* size)
{
    *ptr = m_body + 0;
    *size = 2;
}

void SDAVMSignInResponse::set_timestamp(const char* ptr)
{
    memcpy(m_body+2,ptr,14);
}
std::string SDAVMSignInResponse::get_timestamp()
{
    return std::string(m_body+2,14);
}
void SDAVMSignInResponse::get_timestamp(char** ptr, int* size)
{
    *ptr = m_body + 2;
    *size = 14;
}
