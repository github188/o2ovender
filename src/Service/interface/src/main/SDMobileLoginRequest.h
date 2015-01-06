#ifndef SD_MOBILE_LOGIN_REQUEST_H
#define SD_MOBILE_LOGIN_REQUEST_H

#include "SDMobileRequest.h"

class SDMobileLoginRequest : public SDMobileRequest
{
public:
    SDMobileLoginRequest();
    virtual ~SDMobileLoginRequest();

    virtual SDMobileRequest* clone();
    virtual int on_request(std::map<int, void*>& param);

protected:
    DECL_LOGGER(logger);
};

#endif
