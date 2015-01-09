#include "SDAVMSignInRequest.h"

#include "SDAVMSignInResponse.h"

using namespace std;

IMPL_LOGGER(SDAVMSignInRequest, logger);

SDAVMSignInRequest::SDAVMSignInRequest()
{}

SDAVMSignInRequest::~SDAVMSignInRequest()
{}

SDAVMRequest* SDAVMSignInRequest::clone()
{
    return new SDAVMSignInRequest();
}

int SDAVMSignInRequest::on_request(std::map<int, void*>& param)
{
    SDAVMSignInResponse response;
    SDSharedBuffer buff(new SDBuffer(response.HEAD_SIZE+response.BODY_SIZE));
    int len = buff->size();
    response.encode(buff->data(),&len);
    send_response(buff);
    return 0;
}

bool SDAVMSignInRequest::encode_body(char* buf, int* size)
{
    *size = 51;
    return true;
}

bool SDAVMSignInRequest::decode_body(const char* buf, int size)
{
    return false;
}

void SDAVMSignInRequest::set_driver_version(const char* ptr)
{
    memcpy(m_body+0,ptr,14);
}
std::string SDAVMSignInRequest::get_driver_version()
{
    return std::string(m_body+0,14);
}
void SDAVMSignInRequest::get_driver_version(char** ptr, int* size)
{
    *ptr = m_body + 0;
    *size = 14;
}

void SDAVMSignInRequest::set_soft_version(const char* ptr)
{
    memcpy(m_body+14,ptr,14);
}
std::string SDAVMSignInRequest::get_soft_version()
{
    return std::string(m_body+14,14);
}
void SDAVMSignInRequest::get_soft_version(char** ptr, int* size)
{
    *ptr = m_body + 14;
    *size = 14;
}

void SDAVMSignInRequest::set_board_version(const char* ptr)
{
    memcpy(m_body+28,ptr,4);
}
std::string SDAVMSignInRequest::get_board_version()
{
    return std::string(m_body+28,4);
}
void SDAVMSignInRequest::get_board_version(char** ptr, int* size)
{
    *ptr = m_body + 28;
    *size = 4;
}

void SDAVMSignInRequest::set_avm_number(const char* ptr)
{
    memcpy(m_body+32,ptr,8);
}
std::string SDAVMSignInRequest::get_avm_number()
{
    return std::string(m_body+32,8);
}
void SDAVMSignInRequest::get_avm_number(char** ptr, int* size)
{
    *ptr = m_body + 32;
    *size = 8;
}

void SDAVMSignInRequest::set_machine_type(const char* ptr)
{
    memcpy(m_body+40,ptr,2);
}
std::string SDAVMSignInRequest::get_machine_type()
{
    return std::string(m_body+40,2);
}
void SDAVMSignInRequest::get_machine_type(char** ptr, int* size)
{
    *ptr = m_body + 40;
    *size = 2;
}

void SDAVMSignInRequest::set_drink_number(const char* ptr)
{
    memcpy(m_body+42,ptr,1);
}
std::string SDAVMSignInRequest::get_drink_number()
{
    return std::string(m_body+42,1);
}
void SDAVMSignInRequest::get_drink_number(char** ptr, int* size)
{
    *ptr = m_body + 42;
    *size = 1;
}

void SDAVMSignInRequest::set_food_number(const char* ptr)
{
    memcpy(m_body+43,ptr,1);
}
std::string SDAVMSignInRequest::get_food_number()
{
    return std::string(m_body+43,1);
}
void SDAVMSignInRequest::get_food_number(char** ptr, int* size)
{
    *ptr = m_body + 43;
    *size = 1;
}

void SDAVMSignInRequest::set_noodle_number(const char* ptr)
{
    memcpy(m_body+44,ptr,1);
}
std::string SDAVMSignInRequest::get_noodle_number()
{
    return std::string(m_body+44,1);
}
void SDAVMSignInRequest::get_noodle_number(char** ptr, int* size)
{
    *ptr = m_body + 44;
    *size = 1;
}

void SDAVMSignInRequest::set_drink_set(const char* ptr)
{
    memcpy(m_body+45,ptr,2);
}
std::string SDAVMSignInRequest::get_drink_set()
{
    return std::string(m_body+45,2);
}
void SDAVMSignInRequest::get_drink_set(char** ptr, int* size)
{
    *ptr = m_body + 45;
    *size = 2;
}

void SDAVMSignInRequest::set_food_set(const char* ptr)
{
    memcpy(m_body+47,ptr,2);
}
std::string SDAVMSignInRequest::get_food_set()
{
    return std::string(m_body+47,2);
}
void SDAVMSignInRequest::get_food_set(char** ptr, int* size)
{
    *ptr = m_body + 47;
    *size = 2;
}

void SDAVMSignInRequest::set_noodle_set(const char* ptr)
{
    memcpy(m_body+49,ptr,2);
}
std::string SDAVMSignInRequest::get_noodle_set()
{
    return std::string(m_body+49,2);
}
void SDAVMSignInRequest::get_noodle_set(char** ptr, int* size)
{
    *ptr = m_body + 49;
    *size = 2;
}

