#include "SDAVMWorkHandler.h"

#include <client/dbclient.h>
#include <main/SDRedisClient.h>
#include <main/SDMongoClient.h>

using namespace std;

IMPL_LOGGER(SDAVMWorkHandler, logger);

int SDAVMWorkHandler::MONGODB_CLI = 0;
int SDAVMWorkHandler::REDIS_CLI = 1;

SDAVMWorkHandler::SDAVMWorkHandler(int thread_size) : SDThreadPool(thread_size)
{}

SDAVMWorkHandler::~SDAVMWorkHandler()
{}

bool SDAVMWorkHandler::init(SDConfiguration& config)
{
    int queue_size = config.getInt("work.queue.size", 100);
    m_queue = boost::shared_ptr<SDAVMRequestQueue>(new SDAVMRequestQueue(queue_size));

    m_mongo_host = config.getString("mongodb.host", "127.0.0.1");
    m_mongo_port = config.getInt("mongodb.port", 27017);

    m_redis_host = config.getString("redis.host", "127.0.0.1");
    m_redis_port = config.getInt("redis.port", 6379);

    return true;
}

bool SDAVMWorkHandler::post(SDSharedAVMRequest& mobile_request)
{
    if (!m_queue->push_nonblock(mobile_request)) {
        LOG4CPLUS_WARN(logger, "push() fail: queue size=" << m_queue->getCapacity());
        return false;
    }

    return true;
}

void SDAVMWorkHandler::doIt()
{
    SDMongoClient mongodb;
    mongodb.connect(m_mongo_host, m_mongo_port);
    
    SDRedisClient redis;
    redis.connect(m_redis_host, m_redis_port);

    for (;;) {
        SDSharedAVMRequest request;
        if (!m_queue->pop(request)) {
            LOG4CPLUS_DEBUG(logger, "pop() fail");
            continue;
        }
        if (request.get() == NULL) {
            LOG4CPLUS_DEBUG(logger, "pop() NULL");
            continue;
        }
        
        LOG4CPLUS_DEBUG(logger, "pop() succ");
        map<int,void*> param;
        param[MONGODB_CLI] = (void*)&mongodb;
        param[REDIS_CLI] = (void*)&redis;
        request->on_request(param);
    }
}
