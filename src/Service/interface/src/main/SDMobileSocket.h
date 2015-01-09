#ifndef SD_MOBILE_SOCKET_H
#define SD_MOBILE_SOCKET_H

#include <common/SDNetFramework.h>

#include "SDWorkHandler.h"

class SDMobileSocket : public SDSocket
{
public:
    SDMobileSocket();
    virtual ~SDMobileSocket();

    bool init(boost::shared_ptr<SDWorkHandler>& work_handler);

    virtual int on_recv(SDSharedSocket& socket);

    virtual SDSocket* clone();

protected:
    DECL_LOGGER(logger);

	static const uint32_t MAX_HTTP_HEAD_SIZE = 4096;
	static const uint32_t MAX_HTTP_BODY_SIZE = 1024*1024;

    boost::shared_ptr<SDWorkHandler> m_work_handler;
};

#endif
