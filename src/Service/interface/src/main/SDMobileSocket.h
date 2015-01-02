#ifndef SD_MOBILE_SOCKET_H
#define SD_MOBILE_SOCKET_H

#include <common/SDNetFramework.h>
#include <protocol/mobile2service.pb.h>
#include <client/dbclient.h>

#include "SDWorkHandler.h"

class SDMobileSocket : public SDSocket
{
public:
    static int MONGODB_CLI;
    static int REDIS_CLI;
    
    SDMobileSocket();
    virtual ~SDMobileSocket();

    bool init(boost::shared_ptr<SDWorkHandler>& work_handler);

    virtual int on_recv(SDSharedSocket& socket);
    virtual int on_send(SDSharedSocket& socket);

    virtual int on_request(SDSharedSocket& socket, std::map<int, void*>& param);
    int on_login_req(SDSharedSocket& socket, std::map<int, void*>& param);
    int on_register_req(SDSharedSocket& socket, std::map<int, void*>& param);
    int on_identifying_code(SDSharedSocket& socket, std::map<int, void*>& param);
    
    virtual SDSocket* clone();

protected:
    DECL_LOGGER(logger);
    o2ovender::request m_request;
    o2ovender::response m_response;

    int send_response(SDSharedSocket& socket);

	static const uint32_t MAX_HTTP_HEAD_SIZE = 4096;
	static const uint32_t MAX_HTTP_BODY_SIZE = 1024*1024;

    boost::shared_ptr<SDWorkHandler> m_work_handler;
};

#endif
