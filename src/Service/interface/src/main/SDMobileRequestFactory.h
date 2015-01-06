#ifndef SD_MOBILE_REQUEST_FACTORY_H
#define SD_MOBILE_REQUEST_FACTORY_H

#include <string>
#include <map>
#include <stdint.h>
#include "SDMobileRequest.h"
#include <common/SDLogger.h>

class SDMobileRequestFactory
{
public:
    static void register_object(int32_t key, SDMobileRequest* object);
    static SDMobileRequest* create_object(int32_t key);

private:
    DECL_LOGGER(logger);

    static std::map<int32_t, SDMobileRequest*> m_objects;
};

#endif
