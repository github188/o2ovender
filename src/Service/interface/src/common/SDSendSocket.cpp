#include "SDSendSocket.h"
#include <common/SDSocketUtility.h>
#include <common/SDStringUtility.h>
#include <common/SDNetFramework.h>

using namespace std;

IMPL_LOGGER(SDSendSocket, logger);

SDSendSocket::SDSendSocket()
{
}

SDSendSocket::~SDSendSocket()
{}

SDSocket* SDSendSocket::clone()
{
    SDSendSocket* new_socket = new SDSendSocket();

    return new_socket;
}
    
int SDSendSocket::on_recv(SDSharedSocket& socket)
{
    close();
    return -1;
}

int SDSendSocket::on_send(SDSharedSocket& socket)
{
    if (m_send_buf->length() >= m_send_buf->size()){
        m_send_buf->free_buf();
        if (m_send_buf_queue) {
            if (m_send_buf_queue->getSize()>0 && m_send_buf_queue->pop_nonblock(m_send_buf)) {
            }
            else {
                return IO_STATE_RECV_MORE;
            }
        }
        else {
            return IO_STATE_RECV_MORE;
        }
    }
    else {
        return IO_STATE_SEND_MORE;
    }

    return IO_STATE_SEND_MORE;
}
