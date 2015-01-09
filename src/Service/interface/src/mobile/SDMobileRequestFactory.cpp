#include "SDMobileRequestFactory.h"

using namespace std;

IMPL_LOGGER(SDMobileRequestFactory, logger);

std::map<int32_t, SDMobileRequest*> SDMobileRequestFactory::m_objects;

void SDMobileRequestFactory::register_object(int32_t key, SDMobileRequest* object)
{
    m_objects[key] = object;
}
 
SDMobileRequest* SDMobileRequestFactory::create_object(int32_t key)
{
    SDMobileRequest* newobject = NULL;
    std::map<int32_t, SDMobileRequest*>::iterator it = m_objects.find(key);
    if (it != m_objects.end()) {
        newobject = it->second->clone();
    }
    else {
        LOG4CPLUS_WARN(logger, "Not Support " << key);
    }

    return newobject;
}
