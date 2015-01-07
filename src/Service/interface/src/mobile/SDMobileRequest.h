#ifndef SD_MOBILE_REQUEST_H
#define SD_MOBILE_REQUEST_H

#include <string>
#include <map>
#include <common/SDLogger.h>
#include <common/SDNetFramework.h>
#include <protocol/mobile2service.pb.h>
#include <client/dbclient.h>

class SDMobileRequest;
typedef boost::shared_ptr<SDMobileRequest> SDSharedMobileRequest;
typedef SDQueue<SDSharedMobileRequest> SDMobileRequestQueue;

class SDMobileRequest
{
public:
    SDMobileRequest();
    virtual ~SDMobileRequest();

    virtual SDMobileRequest* clone();
    virtual int on_request(std::map<int, void*>& param);


    DECL_LOGGER(logger);
    o2ovender::request m_request;
    o2ovender::response m_response;

    SDConnHandler* m_conn_handler;
    std::string m_key;
    uint64_t m_checksum;
    int send_response();
};

#endif
