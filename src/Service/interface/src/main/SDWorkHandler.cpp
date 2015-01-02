#include "SDWorkHandler.h"

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
        socket->reset_state();
        socket->on_request(socket);
    }
}
