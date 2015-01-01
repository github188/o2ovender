#include "SDMobileSocket.h"

#include "SDAccountInfo.h"

using namespace std;

IMPL_LOGGER(SDMobileSocket, logger);

SDMobileSocket::SDMobileSocket()
{
    m_recv_buf->alloc_buf(MAX_HTTP_HEAD_SIZE);
}

SDMobileSocket::~SDMobileSocket()
{}

SDSocket* SDMobileSocket::clone()
{
    SDMobileSocket* new_socket = new SDMobileSocket();
    new_socket->m_mongodb_handler = m_mongodb_handler;

    return new_socket;
}
    
bool SDMobileSocket::init(boost::shared_ptr<SDMongoDBHandler>& mongodb_handler)
{
    m_mongodb_handler = mongodb_handler;

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
			LOG4CPLUS_DEBUG(logger, "reset recv buffer to " << (head_length+body_length) << " bytes");
			SDSharedBuffer buff(new SDBuffer());
			int length = m_recv_buf->length();
			buff->alloc_buf(head_length + body_length);
			memcpy(buff->data(), m_recv_buf->data(), m_recv_buf->length());
			m_recv_buf = buff;
			m_recv_buf->lseek(length);
		}

		if (head_length + body_length > (uint32_t)m_recv_buf->length()) {
			return IO_STATE_RECV_MORE;
		}
			
		LOG4CPLUS_DEBUG(logger, "recv HTTP BODY done");
		//LOG4CPLUS_DEBUG(logger, "recv HTTP BODY: " << string(body_ptr, body_length));

        bool parse_result = m_request.ParseFromArray(body_ptr, body_length);
        if (!parse_result) {
		    LOG4CPLUS_DEBUG(logger, "ParseFromArray " << "fail");
            return -1;
        }
        
        LOG4CPLUS_DEBUG(logger, "ParseFromArray " << "succ" << "\n" << m_request.DebugString());
        if (!m_mongodb_handler->post(socket)) {
            return -1;
        }

        return 0;
    }
    else {
		return -1;
    }

    return 0;
}

int SDMobileSocket::on_send(SDSharedSocket& socket)
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

int SDMobileSocket::on_request(SDSharedSocket& socket, void* param)
{
    mongo::DBClientConnection* mongodb = (mongo::DBClientConnection*)param;
    int32_t type = m_request.type();
    if (type == o2ovender::request_TYPE_LOGIN) {
        return on_login_req(socket, mongodb);
    }
    if (type == o2ovender::request_TYPE_REGISTER) {
        return on_register_req(socket, mongodb);
    }
    else {
        LOG4CPLUS_WARN(logger, "unsupport type " << type);
        return -1;
    }

    return 0;
}

int SDMobileSocket::on_login_req(SDSharedSocket& socket, mongo::DBClientConnection* mongodb)
{
    const o2ovender::login_req& login_req = m_request.login_req();
    const std::string& uid = login_req.uid();
    const std::string& password = login_req.pass_word();

    m_response.set_type(o2ovender::response_TYPE_LOGIN);
    o2ovender::login_resp* login_resp = m_response.mutable_login_resp();

    SDAccountInfo account_info;
    account_info.m_uid = uid;
    do {
        if  (uid.empty()) {
            login_resp->set_result(-1);
            login_resp->set_msg("no uid");
            login_resp->set_errcode(o2ovender::login_resp_ERRCODE_ACCOUNT_EMPTY);
            break;
        }
        
        if  (password.empty()) {
            login_resp->set_result(-1);
            login_resp->set_msg("no password");
            login_resp->set_errcode(o2ovender::login_resp_ERRCODE_PASSWD_EMPTY);
            break;
        }
       
        int res = SDMongoDBAccountInfo::query(mongodb, account_info);
        if (res == -1) {
            login_resp->set_result(-1);
            login_resp->set_msg("server is busy");
            login_resp->set_errcode(o2ovender::login_resp_ERRCODE_SERVER_BUSY);
            break;
        }

        if (res == 0) {
            login_resp->set_result(-1);
            login_resp->set_msg("account not exists");
            login_resp->set_errcode(o2ovender::login_resp_ERRCODE_ACCOUNT_NOT_EXISTS);
            break;
        }
        
        if (password != account_info.m_passwd) {
            login_resp->set_result(-1);
            login_resp->set_msg("password is wrong");
            login_resp->set_errcode(o2ovender::login_resp_ERRCODE_PASSWD_WRONG);
            break;
        }
            
        login_resp->set_result(0);
    } while(false);
        
    LOG4CPLUS_DEBUG(logger, "login_resp\n" << m_response.DebugString());
    string data;
    bool result = m_response.SerializeToString(&data);
    if (!result) {
        LOG4CPLUS_WARN(logger, "SerializeToString() fail");
        return -1;
    }
    
    m_send_buf = SDSharedBuffer(new SDBuffer());
    m_send_buf->alloc_buf(128+data.length());
    int size = sprintf(m_send_buf->data(), "HTTP/1.0 200 OK\r\nContent-Length: %d\r\n\r\n", (int)data.length());
    memcpy(m_send_buf->data()+size, data.c_str(), data.length());
    m_send_buf->m_size = size + data.length();

    wait_send();
    m_conn_handler->post(socket);
    return 0;
}

int SDMobileSocket::on_register_req(SDSharedSocket& socket, mongo::DBClientConnection* mongodb)
{
    const o2ovender::register_req& register_req = m_request.register_req();
    const std::string& uid = register_req.uid();
    const std::string& password = register_req.pass_word();
    const std::string& identifying_code = register_req.identifying_code();

    m_response.set_type(o2ovender::response_TYPE_REGISTER);
    o2ovender::register_resp* register_resp = m_response.mutable_register_resp();

    SDAccountInfo account_info;
    account_info.m_uid = uid;
    do {
        if (uid.empty()) {
            register_resp->set_result(-1);
            register_resp->set_msg("no uid");
            register_resp->set_errcode(o2ovender::register_resp_ERRCODE_ACCOUNT_EMPTY);
            break;
        }
        
        if (password.empty()) {
            register_resp->set_result(-1);
            register_resp->set_msg("no password");
            register_resp->set_errcode(o2ovender::register_resp_ERRCODE_PASSWD_EMPTY);
            break;
        }
        
        if (identifying_code.empty()) {
            register_resp->set_result(-1);
            register_resp->set_msg("no identifying code");
            register_resp->set_errcode(o2ovender::register_resp_ERRCODE_IDCODE_EMPTY);
            break;
        }

        int res = SDMongoDBAccountInfo::query(mongodb, account_info);
        if (res == -1) {
            register_resp->set_result(-1);
            register_resp->set_msg("server is busy");
            register_resp->set_errcode(o2ovender::register_resp_ERRCODE_SERVER_BUSY);
            break;
        }

        if (res == 1) {
            register_resp->set_result(-1);
            register_resp->set_msg("account exists");
            register_resp->set_errcode(o2ovender::register_resp_ERRCODE_ACCOUNT_EXISTS);
            break;
        }
       
        account_info.m_passwd = password;
        res = SDMongoDBAccountInfo::insert(mongodb, account_info);
        if (res != 1) {
            register_resp->set_result(-1);
            register_resp->set_msg("server is busy");
            register_resp->set_errcode(o2ovender::register_resp_ERRCODE_SERVER_BUSY);
            break;
        }
        
        register_resp->set_result(0);
    } while(false);
        
    LOG4CPLUS_DEBUG(logger, "register_resp\n" << m_response.DebugString());
    string data;
    bool result = m_response.SerializeToString(&data);
    if (!result) {
        LOG4CPLUS_WARN(logger, "SerializeToString() fail");
        return -1;
    }
    
    m_send_buf = SDSharedBuffer(new SDBuffer());
    m_send_buf->alloc_buf(128+data.length());
    int size = sprintf(m_send_buf->data(), "HTTP/1.0 200 OK\r\nContent-Length: %d\r\n\r\n", (int)data.length());
    memcpy(m_send_buf->data()+size, data.c_str(), data.length());
    m_send_buf->m_size = size + data.length();

    wait_send();
    m_conn_handler->post(socket);
    return 0;
}
