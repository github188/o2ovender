#ifndef SD_CONN_HANDLER_H
#define SD_CONN_HANDLER_H

#include "SDThreadPool.h"
#include "SDSocket.h"
#include "SDEpollUtility.h"

class SDConnHandler : public SDThreadPool
{
public:
    SDConnHandler();
    virtual ~SDConnHandler();

    virtual bool post(SDSharedBuffer& socket)=0;
    virtual bool post(SDSharedSocket& socket)=0;
};

#endif
