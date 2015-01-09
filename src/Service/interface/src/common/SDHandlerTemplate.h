#ifndef SD_HANDLER_TEMPLATE_H
#define SD_HANDLER_TEMPLATE_H

#include "SDThreadPool.h"

class SDHandlerName : public SDThreadPool
{
public:
    SDHandlerName();
    virtual ~SDHandlerName();

    virtual void doIt();

protected:
    DECL_LOGGER(logger);
};

#endif
