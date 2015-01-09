#include "SDMobileSocket.h"

#include "SDMobileRequestFactory.h"

using namespace std;

IMPL_LOGGER(SDMobileSocket, logger);

SDMobileSocket::SDMobileSocket()
{
    m_recv_buf = SDSharedBuffer(new SDBuffer(MAX_HTTP_HEAD_SIZE));
}

SDMobileSocket::~SDMobileSocket()
{}

SDSocket* SDMobileSocket::clone()
{
    SDMobileSocket* new_socket = new SDMobileSocket();
    new_socket->m_conn_handler = m_conn_handler;
    new_socket->m_work_handler = m_work_handler;

    return new_socket;
}
    
bool SDMobileSocket::init(boost::shared_ptr<SDWorkHandler>& work_handler)
{
    m_work_handler = work_handler;

    return true;
}

int SDMobileSocket::on_recv(SDSharedSocket& socket)
{
    if (m_recv_buf->length() > 0) {
        //LOG4CPLUS_DEBUG(logger, "recv bytes " << SDStringUtility::str2hex(m_recv_buf->data(), m_recv_buf->length()));
		char c = m_recv_buf->m_data[m_recv_buf->length()-1];
		m_recv_buf->m_data[m_recv_buf->length()-1] = '\0';
		char* body_ptr = strstr(m_recv_buf->data(), "\r\n\r\n");
        if (!body_ptr) {
			//LOG4CPLUS_DEBUG(logger, "HTTP HEAD:\n" << string(m_recv_buf->data(), m_recv_buf->length()));
			if (m_recv_buf->length() >= m_recv_buf->size()) {
				LOG4CPLUS_DEBUG(logger, "no HTTP HEAD end flag");
				return -1;
			}
			else {
				m_recv_buf->m_data[m_recv_buf->length()-1] = c;
				return IO_STATE_RECV_MORE;
			}
		}
			
		LOG4CPLUS_DEBUG(logger, "found HTTP HEAD end flag(\\r\\n\\r\\n)");
		body_ptr += 4;
		uint32_t head_length = body_ptr - m_recv_buf->data();
		char* length_ptr = strstr(m_recv_buf->data(), "Content-Length:");
	    if (!length_ptr) {
			LOG4CPLUS_DEBUG(logger, "no HTTP HEAD field : Content-Length");
			return -1;
		}
				
		LOG4CPLUS_DEBUG(logger, "found HTTP HEAD field : Content-Length");
		uint32_t body_length = (uint32_t)atoi(length_ptr+15);
		LOG4CPLUS_DEBUG(logger, "Content-Length: " << body_length);
		if (body_length == 0 || body_length >= MAX_HTTP_BODY_SIZE) {
			LOG4CPLUS_DEBUG(logger, "invalid Content-Length: " << body_length);
			return -1;
		}
		
		m_recv_buf->m_data[m_recv_buf->length()-1] = c;
		if (head_length + body_length > MAX_HTTP_HEAD_SIZE) {
			LOG4CPLUS_DEBUG(logger, "resize recv buffer to " << (head_length+body_length) << " bytes");
			int offset = m_recv_buf->length();
			SDSharedBuffer buff(new SDBuffer(head_length + body_length));
			memcpy(buff->data(), m_recv_buf->data(), offset);
			m_recv_buf = buff;
			m_recv_buf->lseek(offset);
		}

		if (head_length + body_length > (uint32_t)m_recv_buf->length()) {
			return IO_STATE_RECV_MORE;
		}
			
		LOG4CPLUS_DEBUG(logger, "recv HTTP BODY done");
		//LOG4CPLUS_DEBUG(logger, "recv HTTP BODY: " << string(body_ptr, body_length));
       
        o2ovender::request mobile_request;
        bool parse_result = mobile_request.ParseFromArray(body_ptr, body_length);
        if (!parse_result) {
		    LOG4CPLUS_DEBUG(logger, "ParseFromArray " << "fail");
            return -1;
        }
        LOG4CPLUS_DEBUG(logger, "ParseFromArray " << "succ" << "\n" << mobile_request.DebugString());
        if (!mobile_request.has_type()) {
		    LOG4CPLUS_DEBUG(logger, "ParseFromArray fail: No Type");
            return -1;
        }
        int32_t type = mobile_request.type();
        
        SDSharedMobileRequest request(SDMobileRequestFactory::create_object(type));
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
        }
        
        m_recv_buf->lseek(0);
        return IO_STATE_RECV_MORE;
    }
    else {
		return -1;
    }

    return IO_STATE_RECV_MORE;
}
