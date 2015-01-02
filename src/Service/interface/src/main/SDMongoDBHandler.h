#ifndef SD_MONGODB_HANDLER_H
#define SD_MONGODB_HANDLER_H

#include <common/SDThreadPool.h>
#include <common/SDSocket.h>
#include <common/SDConfiguration.h>

class SDMongoDBHandler : public SDThreadPool
{
public:
    SDMongoDBHandler(int thread_size);
    virtual ~SDMongoDBHandler();

    bool init(SDConfiguration& config);
    bool post(SDSharedSocket& socket);

    virtual void doIt();

protected:
    DECL_LOGGER(logger);
    boost::shared_ptr<SDSocketQueue> m_queue;
    std::string m_host;
    int m_port;
};

#endif
