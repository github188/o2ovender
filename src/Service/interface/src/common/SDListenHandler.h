#ifndef SD_LISTEN_HANDLER_H
#define SD_LISTEN_HANDLER_H

#include "SDThreadPool.h"
#include "SDQueue.h"
#include "SDSocket.h"
#include "SDCircleIndex.h"

#include <string>
#include <vector>
#include <map>

#include <boost/shared_ptr.hpp>

class SDPassiveConnHandler;
class SDListenHandler : public SDThreadPool
{
public:
    SDListenHandler();
    virtual ~SDListenHandler();
    
    bool init(const std::vector<std::string>& ipv4_list);
    void add_conn_handler(boost::shared_ptr<SDPassiveConnHandler> conn_handler);

    virtual void doIt();

protected:
    DECL_LOGGER(logger);

private:
    std::vector<std::string> m_ipv4_list;

    int m_efd;
    int m_maxevents;
    struct epoll_event* m_events;
   
    std::map<int, SDSharedSocket> m_listen_socket_list;

    SDCircleIndex m_conn_index;
    std::vector<boost::shared_ptr<SDPassiveConnHandler> > m_conn_handler;
};

#endif
