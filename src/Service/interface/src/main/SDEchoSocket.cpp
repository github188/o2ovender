#include "SDEchoSocket.h"

using namespace std;

IMPL_LOGGER(SDEchoSocket, logger);

SDEchoSocket::SDEchoSocket()
{
    m_recv_buf->alloc_buf(4096);
    m_send_buf->alloc_buf(4096);
}

SDEchoSocket::~SDEchoSocket()
{}

SDSocket* SDEchoSocket::clone()
{
    SDEchoSocket* new_socket = new SDEchoSocket();
    new_socket->m_work_handler = m_work_handler;

    return new_socket;
}
    
bool SDEchoSocket::init(boost::shared_ptr<SDWorkHandler>& work_handler)
{
    m_work_handler = work_handler;

    return true;
}

int SDEchoSocket::on_recv(SDSharedSocket& socket)
{
    if (m_recv_buf->length() > 0) {
        LOG4CPLUS_DEBUG(logger, "recv bytes " << SDStringUtility::str2hex(m_recv_buf->data(), m_recv_buf->length()));
        m_work_handler->post(socket);
        return 0;
    }
    else {
    }

    return 0;
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
