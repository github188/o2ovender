#include "SDAVMRequestFactory.h"

using namespace std;

IMPL_LOGGER(SDAVMRequestFactory, logger);

std::map<std::string, SDAVMRequest*> SDAVMRequestFactory::m_objects;

void SDAVMRequestFactory::register_object(const std::string& key, SDAVMRequest* object)
{
    m_objects[key] = object;
}
 
SDAVMRequest* SDAVMRequestFactory::create_object(const std::string& key)
{
    SDAVMRequest* newobject = NULL;
    std::map<std::string, SDAVMRequest*>::iterator it = m_objects.find(key);
    if (it != m_objects.end()) {
        newobject = it->second->clone();
    }
    else {
        LOG4CPLUS_WARN(logger, "Not Support " << key);
    }

    return newobject;
}
