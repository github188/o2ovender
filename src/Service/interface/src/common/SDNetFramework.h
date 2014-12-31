#ifndef SD_NET_FRAMEWORK_H
#define SD_NET_FRAMEWORK_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include <common/SDConfiguration.h>
#include <common/SDListenHandler.h>
#include <common/SDPassiveConnHandler.h>
#include <common/SDActiveConnHandler.h>
#include <common/SDThreadLock.h>
#include <common/SDStringUtility.h>

class SDNetFramework
{
public:
    static void doConfigure(const std::string& filename);

    static void startThreads();
    static void waitThreadsTermination();

    static bool post(const std::string& ipv4, SDSharedBuffer& buff);

public:
    static std::vector<std::string> m_listen_ipv4;
    static boost::shared_ptr<SDListenHandler> m_listen_handler;
    static std::vector<boost::shared_ptr<SDPassiveConnHandler> > m_passive_conn_handler;

    static SDLock m_lock;
    static int m_active_conn_dispatcher_index;
    static std::vector<boost::shared_ptr<SDActiveConnHandler> > m_active_conn_handler;
};

#endif
