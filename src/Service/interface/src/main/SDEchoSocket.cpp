#include "SDEchoSocket.h"

using namespace std;

IMPL_LOGGER(SDEchoSocket, logger);

SDEchoSocket::SDEchoSocket()
{
    m_recv_buf = SDSharedBuffer(new SDBuffer(4096));
}

SDEchoSocket::~SDEchoSocket()
{}

SDSocket* SDEchoSocket::clone()
{
    SDEchoSocket* new_socket = new SDEchoSocket();
    new_socket->m_conn_handler = m_conn_handler;
    new_socket->m_work_handler = m_work_handler;

    return new_socket;
}
    
bool SDEchoSocket::init(boost::shared_ptr<SDEchoHandler>& work_handler)
{
    m_work_handler = work_handler;

    return true;
}

int SDEchoSocket::on_recv(SDSharedSocket& socket)
{
    if (m_recv_buf->length() > 0) {
        LOG4CPLUS_DEBUG(logger, "recv bytes " << SDStringUtility::str2hex(m_recv_buf->data(), m_recv_buf->length()));
  
        SDSharedSocket newsock(clone());
        SDSharedBuffer buff(new SDBuffer(m_recv_buf->data(),m_recv_buf->length()));
        buff->m_ipv4 = SDStringUtility::passive_conn_key_encode(socket->m_fd,socket->m_checksum);
        newsock->m_recv_buf = buff;
        
        m_work_handler->post(newsock);
        m_recv_buf->lseek(0);
        return IO_STATE_RECV_MORE;
    }
    else {
        return -1;
    }

    return IO_STATE_RECV_MORE;
}

int SDEchoSocket::on_send(SDSharedSocket& socket)
{
    if (m_send_buf->length() >= m_send_buf->size()){
        m_recv_buf->lseek(0);
        return IO_STATE_RECV_MORE;
    }
    else {
        return IO_STATE_SEND_MORE;
    }

    return 0;
}

int SDEchoSocket::on_work(SDSharedSocket& socket)
{
    m_send_buf->alloc_buf(m_recv_buf->length());
    memcpy(m_send_buf->data(), m_recv_buf->data(), m_recv_buf->length());
    wait_send();
    m_conn_handler->post(socket);

    return 0;
}
