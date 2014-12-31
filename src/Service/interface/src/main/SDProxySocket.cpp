#include "SDProxySocket.h"

using namespace std;

IMPL_LOGGER(SDProxySocket, logger);

SDProxySocket::SDProxySocket()
{
    m_recv_buf->alloc_buf(4096);
}

SDProxySocket::~SDProxySocket()
{}

SDSocket* SDProxySocket::clone()
{
    SDProxySocket* new_socket = new SDProxySocket();
    new_socket->m_ipv4 = m_ipv4;

    return new_socket;
}

bool SDProxySocket::init(const std::string& ipv4)
{
    m_ipv4 = ipv4;

    return true;
}
    
int SDProxySocket::on_recv(SDSharedSocket& socket)
{
    if (m_recv_buf->length() > 0) {
        LOG4CPLUS_DEBUG(logger, "recv bytes " << SDStringUtility::str2hex(m_recv_buf->data(), m_recv_buf->length()));
    
        SDSharedBuffer buff(new SDBuffer(m_recv_buf->data(), m_recv_buf->length()));
        SDNetFramework::post(m_ipv4, buff);
        m_recv_buf->lseek(0);
    }
    return IO_STATE_RECV_MORE;
}

int SDProxySocket::on_send(SDSharedSocket& socket)
{
    return -1;
}
