#ifndef SD_MOBILE_SOCKET_H
#define SD_MOBILE_SOCKET_H

#include <common/SDNetFramework.h>
#include <protocol/mobile2service.pb.h>
#include <client/dbclient.h>

#include "SDMongoDBHandler.h"

class SDMobileSocket : public SDSocket
{
public:
    SDMobileSocket();
    virtual ~SDMobileSocket();

    bool init(boost::shared_ptr<SDMongoDBHandler>& mongodb_handler);

    virtual int on_recv(SDSharedSocket& socket);
    virtual int on_send(SDSharedSocket& socket);

    virtual int on_request(SDSharedSocket& socket, void* param);
    int on_login_req(SDSharedSocket& socket, mongo::DBClientConnection* mongodb);
    int on_register_req(SDSharedSocket& socket, mongo::DBClientConnection* mongodb);
    
    virtual SDSocket* clone();

protected:
    DECL_LOGGER(logger);
    o2ovender::request m_request;
    o2ovender::response m_response;

	static const uint32_t MAX_HTTP_HEAD_SIZE = 4096;
	static const uint32_t MAX_HTTP_BODY_SIZE = 1024*1024;
    boost::shared_ptr<SDMongoDBHandler> m_mongodb_handler;
};

#endif
