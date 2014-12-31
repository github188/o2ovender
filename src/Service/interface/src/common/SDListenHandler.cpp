#include "SDListenHandler.h"
#include "SDSocketUtility.h"
#include "SDEpollUtility.h"
#include "SDPassiveConnHandler.h"
#include "SDSocketFactory.h"

#include <stdlib.h>

using namespace std;

IMPL_LOGGER(SDListenHandler, logger);

SDListenHandler::SDListenHandler() : SDThreadPool(1)
{
}

bool SDListenHandler::init(const std::vector<std::string>& ipv4_list)
{
    m_ipv4_list = ipv4_list;
   
    m_maxevents = m_ipv4_list.size();
    m_events = new struct epoll_event[m_maxevents];
    m_efd = SDEpollUtility::create_epoll(m_maxevents);
    if (m_efd == -1) {
        return false;
    }
    
    for (std::vector<std::string>::iterator it = m_ipv4_list.begin(); it != m_ipv4_list.end(); ++it) {
        const string& ipv4 = *it;
        string::size_type pos = ipv4.find(':');
        if (pos != string::npos) {
            string ip = ipv4.substr(0, pos);
            int port = atoi(ipv4.substr(pos+1).c_str());

            int fd = SDSocketUtility::listen(ip, port);
            if (fd == -1) {
                exit(1);
            }

            if (SDEpollUtility::add_read_event(m_efd, fd) == -1) {
                exit(1);
            }

            SDSharedSocket socket(new SDSocket());
            socket->m_local_addr = ipv4;
            m_listen_socket_list[fd] = socket;
            
            LOG4CPLUS_DEBUG(logger, "listen " << ip << ":" << port);
        }
        else {
            LOG4CPLUS_WARN(logger, "invalid ipv4 " << ipv4);
        }
    }

    return true;
}
    
void SDListenHandler::add_conn_handler(boost::shared_ptr<SDPassiveConnHandler> conn_handler)
{
    m_conn_handler.push_back(conn_handler);
}

SDListenHandler::~SDListenHandler()
{}

void SDListenHandler::doIt()
{
    LOG4CPLUS_DEBUG(logger, "start thread");
    m_conn_index.init(m_conn_handler.size());
    int timeout = -1;
    for (;;) {
        int num = SDEpollUtility::wait(m_efd, m_events, m_maxevents, timeout);
        for (int i = 0; i < num; ++i) {
            int fd = m_events[i].data.fd;
            SDSharedSocket& listen_socket = m_listen_socket_list[fd];
            std::string& local_addr = listen_socket->m_local_addr;
   
            for (;;) {
                struct sockaddr_in addr;
                int cfd = SDSocketUtility::accept(fd, &addr);
                if (cfd == -1) {
                    break;
                }

                std::string remote_addr = SDSocketUtility::to_string(addr);
                LOG4CPLUS_DEBUG(logger, "[" << local_addr << "] accept " << remote_addr << " (fd=" << cfd << ")");
                if (SDSocketUtility::set_linger(cfd) == -1) {
                    ::close(cfd);
                    continue;
                }
                if (SDSocketUtility::set_nonblock(cfd) == -1) {
                    ::close(cfd);
                    continue;
                }

                SDSharedSocket socket(SDSocketFactory::create_socket(local_addr));
                if (socket.get()) {
                    uint32_t index = m_conn_index.get_index();
                    socket->init_conn_info(cfd, local_addr, remote_addr);
                    if (!m_conn_handler[index]->post(socket)) {
                        continue;
                    }
                }
                else {
                    ::close(cfd);
                }
            }
        }
    }
    LOG4CPLUS_DEBUG(logger, "exit thread");
}
