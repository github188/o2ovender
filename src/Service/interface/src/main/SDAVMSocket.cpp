#include "SDAVMSocket.h"

#include "SDAVMRequest.h"
//#include "SDAVMRequestFactory.h"

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
    
bool SDAVMSocket::init(boost::shared_ptr<SDWorkHandler>& work_handler)
{
    m_work_handler = work_handler;

    return true;
}

int SDAVMSocket::on_recv(SDSharedSocket& socket)
{
    if (m_recv_buf->length() > 0) {
        LOG4CPLUS_DEBUG(logger, "recv bytes " << SDStringUtility::str2hex(m_recv_buf->data(), m_recv_buf->length()));
        if (m_recv_buf->length() >= (int)HEAD_SIZE) {
            SDAVMRequest request;
            if (!request.decode_head(m_recv_buf->data(), m_recv_buf->length())) {
                LOG4CPLUS_DEBUG(logger, "decode head fail");
                return -1;
            }

            if (request.m_length <= HEAD_SIZE || request.m_length > MAX_BODY_SIZE) {
                LOG4CPLUS_DEBUG(logger, "invalid length " << request.m_length);
                return -1;
            }
		
            if ((4+request.m_length) > (uint32_t)m_recv_buf->size()) {
                LOG4CPLUS_DEBUG(logger, "reset recv buffer to " << (4+request.m_length) << " bytes");
                SDSharedBuffer buff(new SDBuffer());
                int length = m_recv_buf->length();
                buff->alloc_buf(4 + request.m_length);
                memcpy(buff->data(), m_recv_buf->data(), m_recv_buf->length());
                m_recv_buf = buff;
                m_recv_buf->lseek(length);
            }

        }
		/*if (head_length + body_length > MAX_HTTP_HEAD_SIZE) {
			LOG4CPLUS_DEBUG(logger, "reset recv buffer to " << (head_length+body_length) << " bytes");
			SDSharedBuffer buff(new SDBuffer());
			int length = m_recv_buf->length();
			buff->alloc_buf(head_length + body_length);
			memcpy(buff->data(), m_recv_buf->data(), m_recv_buf->length());
			m_recv_buf = buff;
			m_recv_buf->lseek(length);
		}

        SDSharedAVMRequest request(SDAVMRequestFactory::create_object(type));
        if (request.get()) {
            request->m_request = mobile_request;
            request->m_conn_handler = m_conn_handler;
            request->m_checksum = socket->m_checksum;
            request->m_key = SDStringUtility::passive_conn_key_encode((uint32_t)socket->m_fd, socket->m_checksum);
            if (!m_work_handler->post(request)) {
                return -1;
            }
        }
        else {
        }*/
        
        m_recv_buf->lseek(0);
        return IO_STATE_RECV_MORE;
    }
    else {
		return -1;
    }

    return IO_STATE_RECV_MORE;
}
