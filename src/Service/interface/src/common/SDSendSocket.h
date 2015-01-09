#ifndef SD_SEND_SOCKET_H
#define SD_SEND_SOCKET_H

#include <common/SDLogger.h>
#include <common/SDSocket.h>

class SDSendSocket : public SDSocket
{
public:
    SDSendSocket();
    virtual ~SDSendSocket();

    virtual int on_recv(SDSharedSocket& socket);
    virtual int on_send(SDSharedSocket& socket);
    
    virtual SDSocket* clone();

protected:
    DECL_LOGGER(logger);
};

#endif
