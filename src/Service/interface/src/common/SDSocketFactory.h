#ifndef SD_SOCKET_FACTORY_H
#define SD_SOCKET_FACTORY_H

#include <string>
#include <map>
#include "SDSocket.h"
#include "SDLogger.h"

class SDSocketFactory
{
public:
    static void register_socket(const std::string& key, SDSocket* socket);
    static SDSocket* create_socket(const std::string& key);

private:
    DECL_LOGGER(logger);

    static std::map<std::string, SDSocket*> m_sockets;
};

#endif
