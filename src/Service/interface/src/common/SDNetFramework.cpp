#include "SDNetFramework.h"

#include "SDStringUtility.h"
#include "SDSocketFactory.h"

using namespace std;

std::vector<std::string> SDNetFramework::m_listen_ipv4;
boost::shared_ptr<SDListenHandler> SDNetFramework::m_listen_handler;

std::vector<boost::shared_ptr<SDPassiveConnHandler> > SDNetFramework::m_passive_conn_handler;
SDLock SDNetFramework::m_lock;
int SDNetFramework::m_active_conn_dispatcher_index;
std::vector<boost::shared_ptr<SDActiveConnHandler> > SDNetFramework::m_active_conn_handler;
    
void SDNetFramework::doConfigure(const std::string& filename)
{
    SDConfiguration config;
    if (!config.load(filename.c_str()))
    {
        exit(1);
    }
    
    SDStringUtility::split(config.getString("bind"), ",", m_listen_ipv4);
    m_listen_handler = boost::shared_ptr<SDListenHandler>(new SDListenHandler());
    if (!m_listen_handler->init(m_listen_ipv4)) {
        exit(1);
    }

    int handler_size = config.getInt("passive.conn.handler.size", 1);
    for (int i=0; i<handler_size; i++) { 
        boost::shared_ptr<SDPassiveConnHandler> conn_handler(new SDPassiveConnHandler());
        int max_events = config.getInt("passive.conn.handler.max.events", 100);
        int queue_size = config.getInt("passive.conn.handler.queue.size", 100);
        int timeout = config.getInt("passive.conn.handler.timeout", 60);
        if (!conn_handler->init(max_events, queue_size, timeout)) {
            exit(1);
        }
        m_listen_handler->add_conn_handler(conn_handler);
        m_passive_conn_handler.push_back(conn_handler);
    }

    m_active_conn_dispatcher_index = 0;
    handler_size = config.getInt("active.conn.handler.size", 1);
    for (int i=0; i<handler_size; i++) { 
        boost::shared_ptr<SDActiveConnHandler> conn_handler(new SDActiveConnHandler());
        int max_events = config.getInt("active.conn.handler.max.events", 100);
        int queue_size = config.getInt("active.conn.handler.queue.size", 100);
        int timeout = config.getInt("active.conn.handler.timeout", 3600);
        if (!conn_handler->init(max_events, queue_size, timeout)) {
            exit(1);
        }
        m_active_conn_handler.push_back(conn_handler);
   }
}
    
void SDNetFramework::startThreads()
{
    for (std::vector<boost::shared_ptr<SDPassiveConnHandler> >::iterator it = m_passive_conn_handler.begin(); it != m_passive_conn_handler.end(); ++it) {
        (*it)->startThreads();
    }
    for (std::vector<boost::shared_ptr<SDActiveConnHandler> >::iterator it = m_active_conn_handler.begin(); it != m_active_conn_handler.end(); ++it) {
        (*it)->startThreads();
    }
    m_listen_handler->startThreads();
}

void SDNetFramework::waitThreadsTermination()
{
    for (std::vector<boost::shared_ptr<SDPassiveConnHandler> >::iterator it = m_passive_conn_handler.begin(); it != m_passive_conn_handler.end(); ++it) {
        (*it)->waitThreadsTermination();
    }
    for (std::vector<boost::shared_ptr<SDActiveConnHandler> >::iterator it = m_active_conn_handler.begin(); it != m_active_conn_handler.end(); ++it) {
        (*it)->waitThreadsTermination();
    }
    m_listen_handler->waitThreadsTermination();
}
    
bool SDNetFramework::post(const std::string& ipv4, SDSharedBuffer& buff)
{
    SDAutoLock auto_lock(m_lock);

    int index = m_active_conn_dispatcher_index++;
    if ((unsigned)m_active_conn_dispatcher_index >= m_active_conn_handler.size()) {
        m_active_conn_dispatcher_index = 0;
    }
    buff->m_ipv4 = ipv4;
    return m_active_conn_handler[index]->post(buff);
}
