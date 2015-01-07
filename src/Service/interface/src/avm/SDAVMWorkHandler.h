#ifndef SD_AVM_WORK_HANDLER_H
#define SD_AVM_WORK_HANDLER_H

#include <common/SDThreadPool.h>
#include <common/SDSocket.h>
#include <common/SDConfiguration.h>

#include <avm/SDAVMRequest.h>

class SDAVMWorkHandler : public SDThreadPool
{
public:
    static int MONGODB_CLI;
    static int REDIS_CLI;
    
    SDAVMWorkHandler(int thread_size);
    virtual ~SDAVMWorkHandler();

    bool init(SDConfiguration& config);
    bool post(SDSharedAVMRequest& mobile_request);

    virtual void doIt();

protected:
    DECL_LOGGER(logger);
    boost::shared_ptr<SDAVMRequestQueue> m_queue;
    
    std::string m_mongo_host;
    int m_mongo_port;
    std::string m_redis_host;
    int m_redis_port;
};

#endif
