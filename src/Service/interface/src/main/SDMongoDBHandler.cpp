#include "SDMongoDBHandler.h"

#include <client/dbclient.h>

using namespace std;

IMPL_LOGGER(SDMongoDBHandler, logger);

SDMongoDBHandler::SDMongoDBHandler(int thread_size) : SDThreadPool(thread_size)
{}

SDMongoDBHandler::~SDMongoDBHandler()
{}

bool SDMongoDBHandler::init(SDConfiguration& config)
{
    int queue_size = config.getInt("mongodb.queue.size", 100);
    m_queue = boost::shared_ptr<SDSocketQueue>(new SDSocketQueue(queue_size));

    m_host = config.getString("mongodb.host", "127.0.0.1");
    m_port = config.getInt("mongodb.port", 27017);

    return true;
}

bool SDMongoDBHandler::post(SDSharedSocket& socket)
{
    if (!m_queue->push_nonblock(socket)) {
        LOG4CPLUS_WARN(logger, "push() fail: queue size=" << m_queue->getCapacity());
        return false;
    }

    return true;
}
void SDMongoDBHandler::doIt()
{
    mongo::DBClientConnection mongodb(true);
    string errmsg;
    try {
        mongodb.connect(mongo::HostAndPort(m_host, m_port), errmsg);
        LOG4CPLUS_DEBUG (logger, "connect to MongoDB " << m_host << ":" << m_port << " succ: " << errmsg);
    }
    catch (mongo::DBException& e) {
        LOG4CPLUS_ERROR (logger, "connect to MongoDB " << m_host << ":" << m_port << " fail: " << e.toString());
    }
    catch (std::exception& e) {
        LOG4CPLUS_ERROR (logger, "connect to MongoDB " << m_host << ":" << m_port << " fail: " << e.what());
    }
    catch (...) {
        LOG4CPLUS_ERROR (logger, "connect to MongoDB " << m_host << ":" << m_port << " fail: " << "Unknown Exception");
    }

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
        socket->on_request(socket, (void*)&mongodb);
    }
}
