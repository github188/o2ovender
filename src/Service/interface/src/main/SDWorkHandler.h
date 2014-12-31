#ifndef SD_WORK_HANDLER_H
#define SD_WORK_HANDLER_H

#include <common/SDThreadPool.h>
#include <common/SDSocket.h>
#include <common/SDConfiguration.h>

class SDWorkHandler : public SDThreadPool
{
public:
    SDWorkHandler(int thread_size);
    virtual ~SDWorkHandler();

    bool init(SDConfiguration& config);
    bool post(SDSharedSocket& socket);

    virtual void doIt();

protected:
    DECL_LOGGER(logger);
    boost::shared_ptr<SDSocketQueue> m_queue;
};

#endif
