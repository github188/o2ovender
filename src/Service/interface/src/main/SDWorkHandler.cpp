#include "SDWorkHandler.h"

#include <client/dbclient.h>
#include <common/SDRedisServer.h>
#include <common/SDMongoUtility.h>
#include "SDMobileSocket.h"

using namespace std;

IMPL_LOGGER(SDWorkHandler, logger);

SDWorkHandler::SDWorkHandler(int thread_size) : SDThreadPool(thread_size)
{}

SDWorkHandler::~SDWorkHandler()
{}

bool SDWorkHandler::init(SDConfiguration& config)
{
    int queue_size = config.getInt("work.queue.size", 100);
    m_queue = boost::shared_ptr<SDSocketQueue>(new SDSocketQueue(queue_size));

    m_mongo_host = config.getString("mongodb.host", "127.0.0.1");
    m_mongo_port = config.getInt("mongodb.port", 27017);

    m_redis_host = config.getString("redis.host", "127.0.0.1");
    m_redis_port = config.getInt("redis.port", 6379);

    return true;
}

bool SDWorkHandler::post(SDSharedSocket& socket)
{
    if (!m_queue->push_nonblock(socket)) {
        LOG4CPLUS_WARN(logger, "push() fail: queue size=" << m_queue->getCapacity());
        return false;
    }

    return true;
}
void SDWorkHandler::doIt()
{
    mongo::DBClientConnection mongodb(true);
    SDMongoUtility::connect(&mongodb, m_mongo_host, m_mongo_port);
    
    SDRedisServer redis;
    redis.connect(m_redis_host, m_redis_port);

    for (;;) {
        SDSharedSocket socket;
        if (!m_queue->pop(socket)) {
            LOG4CPLUS_DEBUG(logger, "pop() fail");
            continue;
        }
        if (socket.get() == NULL) {
            LOG4CPLUS_DEBUG(logger, "pop() NULL");
            continue;
        }
        
        LOG4CPLUS_DEBUG(logger, "pop() " << socket->m_fd);
        map<int,void*> param;
        param[SDMobileSocket::MONGODB_CLI] = (void*)&mongodb;
        param[SDMobileSocket::REDIS_CLI] = (void*)&redis;
        socket->on_request(socket, param);
    }
}
