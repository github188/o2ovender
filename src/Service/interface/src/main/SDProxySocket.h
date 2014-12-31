#ifndef SD_PONG_SOCKET_H
#define SD_PONG_SOCKET_H

#include <common/SDNetFramework.h>

class SDProxySocket : public SDSocket
{
public:
    SDProxySocket();
    virtual ~SDProxySocket();

    bool init(const std::string& ipv4);

    virtual int on_recv(SDSharedSocket& socket);
    virtual int on_send(SDSharedSocket& socket);
    
    virtual SDSocket* clone();

protected:
    DECL_LOGGER(logger);

    std::string m_ipv4;
};

#endif
