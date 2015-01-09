#ifndef SD_AVM_REQUEST_FACTORY_H
#define SD_AVM_REQUEST_FACTORY_H

#include <string>
#include <map>
#include <stdint.h>
#include "SDAVMRequest.h"
#include <common/SDLogger.h>

class SDAVMRequestFactory
{
public:
    static void register_object(const std::string& key, SDAVMRequest* object);
    static SDAVMRequest* create_object(const std::string& key);

private:
    DECL_LOGGER(logger);

    static std::map<std::string, SDAVMRequest*> m_objects;
};

#endif
