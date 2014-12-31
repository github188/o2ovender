#include "SDSocketFactory.h"

using namespace std;

IMPL_LOGGER(SDSocketFactory, logger);

std::map<std::string, SDSocket*> SDSocketFactory::m_sockets;

void SDSocketFactory::register_socket(const std::string& key, SDSocket* socket)
{
    m_sockets[key] = socket;
}
 
SDSocket* SDSocketFactory::create_socket(const std::string& key)
{
    SDSocket* new_socket = NULL;
    std::map<std::string, SDSocket*>::iterator it = m_sockets.find(key);
    if (it != m_sockets.end()) {
        new_socket = it->second->clone();
    }
    else {
        LOG4CPLUS_WARN(logger, "no socket for " << key);
    }

    return new_socket;
}
