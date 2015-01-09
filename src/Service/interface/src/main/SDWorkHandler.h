#ifndef SD_WORK_HANDLER_H
#define SD_WORK_HANDLER_H

#include <common/SDThreadPool.h>
#include <common/SDSocket.h>
#include <common/SDConfiguration.h>

#include <mobile/SDMobileRequest.h>

class SDWorkHandler : public SDThreadPool
{
public:
    static int MONGODB_CLI;
    static int REDIS_CLI;
    
    SDWorkHandler(int thread_size);
    virtual ~SDWorkHandler();

    bool init(SDConfiguration& config);
    bool post(SDSharedMobileRequest& mobile_request);

    virtual void doIt();

protected:
    DECL_LOGGER(logger);
    boost::shared_ptr<SDMobileRequestQueue> m_queue;
    
    std::string m_mongo_host;
    int m_mongo_port;
    std::string m_redis_host;
    int m_redis_port;
};

#endif
