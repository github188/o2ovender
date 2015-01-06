#include "SDMobileRequest.h"

using namespace std;

IMPL_LOGGER(SDMobileRequest, logger);

SDMobileRequest::SDMobileRequest()
{
    m_conn_handler = NULL;
    m_checksum = 0;
}

SDMobileRequest::~SDMobileRequest()
{}

SDMobileRequest* SDMobileRequest::clone()
{
    return new SDMobileRequest();
}
    
int SDMobileRequest::on_request(std::map<int, void*>& param)
{
    return -1;
}

int SDMobileRequest::send_response()
{
    string data;
    bool result = m_response.SerializeToString(&data);
    if (!result) {
        LOG4CPLUS_WARN(logger, "SerializeToString() fail");
        return -1;
    }
    
    SDSharedBuffer send_buf(new SDBuffer(256+data.length()));
    int size = sprintf(send_buf->data(), "HTTP/1.1 200 OK\r\nContent-Length: %d\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n", (int)data.length());
    memcpy(send_buf->data()+size, data.c_str(), data.length());
    send_buf->m_size = size + data.length();
    send_buf->m_ipv4 = m_key;

    m_conn_handler->post(send_buf);
    return 0;
}
