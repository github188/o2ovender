#include "SDAVMSocket.h"

#include "SDAVMRequest.h"
#include "SDAVMRequestFactory.h"

using namespace std;

IMPL_LOGGER(SDAVMSocket, logger);

SDAVMSocket::SDAVMSocket()
{
    m_recv_buf = SDSharedBuffer(new SDBuffer(HEAD_SIZE));
}

SDAVMSocket::~SDAVMSocket()
{}

SDSocket* SDAVMSocket::clone()
{
    SDAVMSocket* new_socket = new SDAVMSocket();
    new_socket->m_conn_handler = m_conn_handler;
    new_socket->m_work_handler = m_work_handler;

    return new_socket;
}
    
bool SDAVMSocket::init(boost::shared_ptr<SDAVMWorkHandler>& work_handler)
{
    m_work_handler = work_handler;

    return true;
}

int SDAVMSocket::on_recv(SDSharedSocket& socket)
{
    if (m_recv_buf->length() > 0) {
        LOG4CPLUS_DEBUG(logger, "recv bytes " << SDStringUtility::str2hex(m_recv_buf->data(), m_recv_buf->length()));
        if (m_recv_buf->length() >= (int)HEAD_SIZE) {
            SDAVMRequest head;
            if (!head.decode_head(m_recv_buf->data(), m_recv_buf->length())) {
                LOG4CPLUS_DEBUG(logger, "decode head fail");
                return -1;
            }

            uint32_t length = 4 + *((uint32_t*)head.m_head);
            if (length <= HEAD_SIZE || length > MAX_BODY_SIZE) {
                LOG4CPLUS_DEBUG(logger, "invalid length " << length);
                return -1;
            }
		
            if (length > (uint32_t)m_recv_buf->size()) {
                LOG4CPLUS_DEBUG(logger, "resize recv buffer to " << length << " bytes");
                int offset = m_recv_buf->length();
                SDSharedBuffer buff(new SDBuffer(length));
                memcpy(buff->data(), m_recv_buf->data(), offset);
                m_recv_buf = buff;
                m_recv_buf->lseek(offset);
            }

            if ((uint32_t)m_recv_buf->length() >= length) {
                string control_code = head.get_control_code();
                LOG4CPLUS_DEBUG(logger, "succ recv a request " << control_code);
                SDSharedAVMRequest request(SDAVMRequestFactory::create_object(control_code));
                if (request.get()) {
                    request->m_conn_handler = m_conn_handler;
                    request->m_checksum = socket->m_checksum;
                    request->m_key = SDStringUtility::passive_conn_key_encode((uint32_t)socket->m_fd, socket->m_checksum);
                    if (!m_work_handler->post(request)) {
                        return -1;
                    }
                }
                else {
                }
                
                m_recv_buf->lseek(0);
                return IO_STATE_RECV_MORE;
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
		return -1;
    }

    return IO_STATE_RECV_MORE;
}
