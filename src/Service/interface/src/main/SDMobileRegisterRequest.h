#ifndef SD_MOBILE_REGISTER_REQUEST_H
#define SD_MOBILE_REGISTER_REQUEST_H

#include "SDMobileRequest.h"

class SDMobileRegisterRequest : public SDMobileRequest
{
public:
    SDMobileRegisterRequest();
    virtual ~SDMobileRegisterRequest();

    virtual SDMobileRequest* clone();
    virtual int on_request(std::map<int, void*>& param);

protected:
    DECL_LOGGER(logger);
};

#endif
