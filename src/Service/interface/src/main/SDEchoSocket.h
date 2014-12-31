#ifndef SD_ECHO_SOCKET_H
#define SD_ECHO_SOCKET_H

#include <common/SDNetFramework.h>

#include "SDWorkHandler.h"

class SDEchoSocket : public SDSocket
{
public:
    SDEchoSocket();
    virtual ~SDEchoSocket();

    bool init(boost::shared_ptr<SDWorkHandler>& work_handler);

    virtual int on_recv(SDSharedSocket& socket);
    virtual int on_send(SDSharedSocket& socket);
    virtual int on_work(SDSharedSocket& socket);
    
    virtual SDSocket* clone();

protected:
    DECL_LOGGER(logger);

    boost::shared_ptr<SDWorkHandler> m_work_handler;
};

#endif
