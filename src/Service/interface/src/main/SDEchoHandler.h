#ifndef SD_ECHO_HANDLER_H
#define SD_ECHO_HANDLER_H

#include <common/SDThreadPool.h>
#include <common/SDSocket.h>
#include <common/SDConfiguration.h>

class SDEchoHandler : public SDThreadPool
{
public:
    SDEchoHandler(int thread_size);
    virtual ~SDEchoHandler();

    bool init(SDConfiguration& config);
    bool post(SDSharedSocket& socket);

    virtual void doIt();

protected:
    DECL_LOGGER(logger);
    boost::shared_ptr<SDSocketQueue> m_queue;
    
    std::string m_mongo_host;
    int m_mongo_port;
    std::string m_redis_host;
    int m_redis_port;
};

#endif
