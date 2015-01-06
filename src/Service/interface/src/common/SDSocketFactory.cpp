#include "SDSocketFactory.h"

using namespace std;

IMPL_LOGGER(SDSocketFactory, logger);

std::map<std::string, SDSocket*> SDSocketFactory::m_objects;

void SDSocketFactory::register_socket(const std::string& key, SDSocket* socket)
{
    m_objects[key] = socket;
}
 
SDSocket* SDSocketFactory::create_socket(const std::string& key)
{
    SDSocket* new_socket = NULL;
    std::map<std::string, SDSocket*>::iterator it = m_objects.find(key);
    if (it != m_objects.end()) {
        new_socket = it->second->clone();
    }
    else {
        LOG4CPLUS_WARN(logger, "Not Support " << key);
    }

    return new_socket;
}
