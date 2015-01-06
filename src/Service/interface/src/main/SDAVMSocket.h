#ifndef SD_AVM_SOCKET_H
#define SD_AVM_SOCKET_H

#include <common/SDNetFramework.h>

#include "SDWorkHandler.h"

class SDAVMSocket : public SDSocket
{
public:
    SDAVMSocket();
    virtual ~SDAVMSocket();

    bool init(boost::shared_ptr<SDWorkHandler>& work_handler);

    virtual int on_recv(SDSharedSocket& socket);

    virtual SDSocket* clone();

protected:
    DECL_LOGGER(logger);

	static const uint32_t HEAD_SIZE = 37;
	static const uint32_t MAX_BODY_SIZE = 1024*1024;

    boost::shared_ptr<SDWorkHandler> m_work_handler;
};

#endif
