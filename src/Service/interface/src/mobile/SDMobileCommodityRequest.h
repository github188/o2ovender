#ifndef SD_MOBILE_COMMODITY_REQUEST_H
#define SD_MOBILE_COMMODITY_REQUEST_H

#include "SDMobileRequest.h"

class SDMobileCommodityRequest : public SDMobileRequest
{
public:
    SDMobileCommodityRequest();
    virtual ~SDMobileCommodityRequest();

    virtual SDMobileRequest* clone();
    virtual int on_request(std::map<int, void*>& param);

protected:
    DECL_LOGGER(logger);
};

#endif
