#include "SDEchoHandler.h"

#include <client/dbclient.h>
#include "SDRedisClient.h"
#include <common/SDConnHandler.h>

using namespace std;

IMPL_LOGGER(SDEchoHandler, logger);

SDEchoHandler::SDEchoHandler(int thread_size) : SDThreadPool(thread_size)
{}

SDEchoHandler::~SDEchoHandler()
{}

bool SDEchoHandler::init(SDConfiguration& config)
{
    int queue_size = config.getInt("work.queue.size", 100);
    m_queue = boost::shared_ptr<SDSocketQueue>(new SDSocketQueue(queue_size));

    m_mongo_host = config.getString("mongodb.host", "127.0.0.1");
    m_mongo_port = config.getInt("mongodb.port", 27017);

    m_redis_host = config.getString("redis.host", "127.0.0.1");
    m_redis_port = config.getInt("redis.port", 6379);

    return true;
}

bool SDEchoHandler::post(SDSharedSocket& socket)
{
    if (!m_queue->push_nonblock(socket)) {
        LOG4CPLUS_WARN(logger, "push() fail: queue size=" << m_queue->getCapacity());
        return false;
    }

    return true;
}

void SDEchoHandler::doIt()
{
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
        
        LOG4CPLUS_DEBUG(logger, "pop() succ " << socket->m_fd);
        socket->m_conn_handler->post(socket->m_recv_buf);
    }
}
