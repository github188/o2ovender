#include "SDAVMRequest.h"

#include <string.h>

using namespace std;

std::string SDAVMRequest::SIGN_IN = "51";

SDAVMRequest::SDAVMRequest()
{}

SDAVMRequest::~SDAVMRequest()
{}

SDAVMRequest* SDAVMRequest::clone()
{
    return new SDAVMRequest();
}

int SDAVMRequest::on_request(std::map<int, void*>& param)
{
    return -1;
}

int SDAVMRequest::send_response(SDSharedBuffer& buff)
{
    buff->m_ipv4 = m_key;
    m_conn_handler->post(buff);
    return 0;
}

bool SDAVMRequest::encode(char* buf, int* size)
{
    int head_size = *size;
    if (!encode_head(buf, &head_size)) {
        return false;
    }
    
    int body_size = *size - head_size;
    if (!encode_body(buf+head_size, &body_size)) {
        return false;
    }
    
    *((uint32_t*)buf) = head_size + body_size - 4;
    *size = head_size + body_size;
   
    return true;
}

bool SDAVMRequest::decode(const char* buf, int size)
{
    int head_size = size;
    if (!decode_head(buf,head_size)) {
        return false;
    }

    int body_size = size - HEAD_SIZE;
    if (!decode_body(buf+HEAD_SIZE,body_size)) {
        return false;
    }
    
    return true;
}

bool SDAVMRequest::encode_head(char* buf, int* size)
{
    if (*size < HEAD_SIZE) {
        return false;
    }
    memcpy(buf,m_head,HEAD_SIZE);
    *size = HEAD_SIZE;
    return true;
}

bool SDAVMRequest::decode_head(const char* buf, int size)
{
    if (size < HEAD_SIZE) {
        return false;
    }

    memcpy(m_head,buf,HEAD_SIZE);
    return true;
}

bool SDAVMRequest::encode_body(char* buf, int* size)
{
    return false;
}

bool SDAVMRequest::decode_body(const char* buf, int size)
{
    return false;
}
    
void SDAVMRequest::set_timestamp(const char* ptr)
{
    memcpy(m_head+4,ptr,14);
}
std::string SDAVMRequest::get_timestamp()
{
    return std::string(m_head+4,14);
}
void SDAVMRequest::set_terminal_number(const char* ptr)
{
    memcpy(m_head+18,ptr,8);
}
std::string SDAVMRequest::get_terminal_number()
{
    return std::string(m_head+18,8);
}
void SDAVMRequest::set_version(const char* ptr)
{
    memcpy(m_head+26,ptr,2);
}
std::string SDAVMRequest::get_version()
{
    return std::string(m_head+26,2);
}
void SDAVMRequest::set_reserve(const char* ptr)
{
    memcpy(m_head+28,ptr,6);
}
std::string SDAVMRequest::get_reserve()
{
    return std::string(m_head+28,6);
}
void SDAVMRequest::set_data_direction(const char* ptr)
{
    memcpy(m_head+34,ptr,1);
}
std::string SDAVMRequest::get_data_direction()
{
    return std::string(m_head+34,1);
}
void SDAVMRequest::set_control_code(const char* ptr)
{
    memcpy(m_head+35,ptr,2);
}
std::string SDAVMRequest::get_control_code()
{
    return std::string(m_head+35,2);
}
