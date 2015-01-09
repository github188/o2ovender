#ifndef SD_MOBILE_CODE_REQUEST_H
#define SD_MOBILE_CODE_REQUEST_H

#include "SDMobileRequest.h"

class SDMobileCodeRequest : public SDMobileRequest
{
public:
    SDMobileCodeRequest();
    virtual ~SDMobileCodeRequest();

    virtual SDMobileRequest* clone();
    virtual int on_request(std::map<int, void*>& param);

protected:
    DECL_LOGGER(logger);
};

#endif
