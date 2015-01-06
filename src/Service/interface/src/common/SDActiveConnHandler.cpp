#include "SDActiveConnHandler.h"

#include "SDSocketUtility.h"
#include "SDSocketFactory.h"
#include "SDSendSocket.h"

using namespace std;

IMPL_LOGGER(SDActiveConnHandler, logger);

SDActiveConnHandler::SDActiveConnHandler()
{
    m_total_sockets = 0;
}

SDActiveConnHandler::~SDActiveConnHandler()
{}

bool SDActiveConnHandler::init(int maxevents, int queue_size, int timeout)
{
    m_maxevents = maxevents;
    m_timeout = timeout;

    m_socket_list = new SDSharedSocket[m_maxevents];
    m_socket_queue = boost::shared_ptr<SDSocketQueue>(new SDSocketQueue(queue_size));
    m_buffer_queue = boost::shared_ptr<SDBufferQueue>(new SDBufferQueue(queue_size));
    m_events = new struct epoll_event[m_maxevents];
    m_efd = SDEpollUtility::create_epoll(m_maxevents);
    if (m_efd == -1) {
        return false;
    }
    
    int fd[2];
    fd[0] = fd[1] = -1;
    if (SDSocketUtility::create_socketpair(fd) == -1) {
        return false;
    }

    m_socket_fd = fd[0];
    m_socket_notify_fd = fd[1];
            
    if (SDEpollUtility::add_read_event(m_efd, m_socket_fd) == -1) {
        return false;
    }
   
    fd[0] = fd[1] = -1;
    if (SDSocketUtility::create_socketpair(fd) == -1) {
        return false;
    }

    m_buffer_fd = fd[0];
    m_buffer_notify_fd = fd[1];
            
    if (SDEpollUtility::add_read_event(m_efd, m_buffer_fd) == -1) {
        return false;
    }

    return true;
}
    
bool SDActiveConnHandler::post(SDSharedSocket& socket)
{
    if (!m_socket_queue->push_nonblock(socket)) {
        LOG4CPLUS_WARN(logger, "push() fail: queue size=" << m_socket_queue->getCapacity());
        return false;
    }
    
    static char c;
    ::send(m_socket_notify_fd, &c, 1, MSG_NOSIGNAL);
    return true;
}

bool SDActiveConnHandler::post(SDSharedBuffer& buff)
{
    if (!m_buffer_queue->push_nonblock(buff)) {
        LOG4CPLUS_WARN(logger, "push() fail: queue size=" << m_buffer_queue->getCapacity());
        return false;
    }
    
    static char c;
    ::send(m_buffer_notify_fd, &c, 1, MSG_NOSIGNAL);
    return true;
}
    
bool SDActiveConnHandler::is_socket_notify_fd(int fd)
{
    if (fd == m_socket_fd) {
        static char buf[256];
        ::recv(fd, buf, sizeof(buf), 0);
        return true;
    }
    return false;
}

bool SDActiveConnHandler::is_buffer_notify_fd(int fd)
{
    if (fd == m_buffer_fd) {
        static char buf[256];
        ::recv(fd, buf, sizeof(buf), 0);
        return true;
    }
    return false;
}
    
void SDActiveConnHandler::add_read_event(SDSharedSocket& socket)
{
    if (SDEpollUtility::add_read_event(m_efd, socket->m_fd) == 0) {
        ++m_total_sockets;
        socket->m_state = IO_STATE_IN_EPOLL;
        m_socket_list[socket->m_fd] = socket;
        LOG4CPLUS_DEBUG(logger, "fd:" << socket->m_fd << " enter READ mode");
    }
}

void SDActiveConnHandler::add_write_event(SDSharedSocket& socket)
{
    if (SDEpollUtility::add_write_event(m_efd, socket->m_fd) == 0) {
        ++m_total_sockets;
        socket->m_state = IO_STATE_IN_EPOLL;
        m_socket_list[socket->m_fd] = socket;
        LOG4CPLUS_DEBUG(logger, "fd:" << socket->m_fd << " enter WRITE mode");
    }
}

void SDActiveConnHandler::mod_read_event(SDSharedSocket& socket)
{
    if (SDEpollUtility::mod_read_event(m_efd, socket->m_fd) == 0) {
        socket->m_state = IO_STATE_IN_EPOLL;
        LOG4CPLUS_DEBUG(logger, "fd:" << socket->m_fd << " enter READ mode");
    }
}

void SDActiveConnHandler::mod_write_event(SDSharedSocket& socket)
{
    if (SDEpollUtility::mod_write_event(m_efd, socket->m_fd) == 0) {
        socket->m_state = IO_STATE_IN_EPOLL;
        LOG4CPLUS_DEBUG(logger, "fd:" << socket->m_fd << " enter WRITE mode");
    }
}
int SDActiveConnHandler::del_event(int fd)
{
    static SDSharedSocket null_socket;
    LOG4CPLUS_DEBUG(logger, "fd:" << fd << " exit event mode");
    --m_total_sockets;
    m_socket_list[fd]->m_state = IO_STATE_INIT;
    m_socket_list[fd] = null_socket;
    return SDEpollUtility::del_event(m_efd, fd);
}

void SDActiveConnHandler::process_socket_notify()
{
    SDSharedSocket socket;
    while (m_socket_queue->pop_nonblock(socket)) {
        if (socket.get() == NULL) {
            LOG4CPLUS_DEBUG(logger, "pop() NULL");
            continue;
        }

        LOG4CPLUS_DEBUG(logger, "add fd:" << socket->m_fd);
        socket->m_conn_handler = this;
        if (socket->get_state() == IO_STATE_RECV_MORE) {
            //add_read_event(socket);
            process_recv(socket);
        }
        else {
            process_send(socket);
        }
    }
}

void SDActiveConnHandler::process_buffer_notify()
{
    SDSharedBuffer buff;
    while (m_buffer_queue->pop_nonblock(buff)) {
        if (buff.get() == NULL) {
            LOG4CPLUS_DEBUG(logger, "pop() NULL");
            continue;
        }

        LOG4CPLUS_DEBUG(logger, "add ipv4:" << buff->m_ipv4);
        std::map<std::string, SDSharedSocket>::iterator it = m_active_sockets.begin();
        if (it != m_active_sockets.end()) {
            if ((it->second)->m_send_buf_queue->push_nonblock(buff)) {
                if (it->second->m_fd != -1 && it->second->m_state == IO_STATE_IN_EPOLL) {
                    SDEpollUtility::del_event(m_efd, it->second->m_fd);
                }
                process_send(it->second);
            }
            else {
                LOG4CPLUS_WARN(logger, buff->m_ipv4 << " push() fail: queue size=" << ((it->second)->m_send_buf_queue)->getCapacity());
            }
        }
        else {
            //SDSharedSocket socket(SDSocketFactory::create_socket(buff->m_ipv4));
            SDSharedSocket socket(new SDSendSocket());
            if (socket.get()) {
                m_active_sockets[buff->m_ipv4] = socket;
                socket->m_send_buf_queue = new SDBufferQueue(100000);
                socket->wait_send();
                socket->m_remote_addr = buff->m_ipv4;
                if (socket->m_send_buf_queue->push_nonblock(buff)) {
                    process_send(socket);
                }
            }
        }
    }
}

void SDActiveConnHandler::process_timeout()
{
    time_t timenow = time(NULL);
    static SDSharedSocket null_socket;
    int count = 0;
    for (int i = 0; i < m_maxevents; ++i) {
        SDSharedSocket socket = m_socket_list[i];

        if (socket.get()) {
            std::map<std::string, SDSharedSocket>::iterator it = m_active_sockets.find(socket->m_remote_addr);
            if (it == m_active_sockets.end()) {
                if (socket->is_timeout(timenow, m_timeout)) {
                    LOG4CPLUS_DEBUG(logger, "timeout fd:" << socket->m_fd << " timeout:" << m_timeout);
                    del_event(socket->m_fd);
                }
            }
            if (++count >= m_total_sockets) {
                break;
            }
        }
    }
}

void SDActiveConnHandler::process_recv(SDSharedSocket& socket)
{
    int fd = socket->m_fd;
    int recv_bytes = socket->recv_data();
    LOG4CPLUS_DEBUG(logger, "recv_data(fd:" << fd << ") return " << recv_bytes);
    if (recv_bytes > 0) {
        int state = socket->on_recv(socket);
        LOG4CPLUS_DEBUG(logger, "on_recv(fd:" << fd << ") return " << state);
        if (state == IO_STATE_RECV_MORE) {
            process_recv(socket);
        }
        else if (state == IO_STATE_SEND_MORE) {
            mod_write_event(socket);
        }
        else {
            SDEpollUtility::del_event(m_efd,fd);
            socket->close();
        }
    }
    else if (recv_bytes < 0) {
        SDEpollUtility::del_event(m_efd,fd);
        socket->close();
    }
    else {
        //add_read_event(socket);
    }
}

void SDActiveConnHandler::process_send(SDSharedSocket& socket)
{
    if (!socket->is_connected()) {
        int res = socket->connect_to();
        LOG4CPLUS_DEBUG(logger, "connect_to(" << socket->m_remote_addr << ") return " << res);
        if (res < -1) {
            add_write_event(socket);
        }
        if (res < 0) {
            return ;
        }
        else {
        }
    }

    int fd = socket->m_fd;
    int send_bytes = socket->send_data();
    LOG4CPLUS_DEBUG(logger, "send_data(fd:" << fd << ") return " << send_bytes);
    if (send_bytes > 0) {
        int state = socket->on_send(socket);
        LOG4CPLUS_DEBUG(logger, "on_send(fd:" << fd << ") return " << state);
        if (state == IO_STATE_RECV_MORE) {
            mod_read_event(socket);
        }
        else if (state == IO_STATE_SEND_MORE) {
            process_send(socket);
        }
        else {
            SDEpollUtility::del_event(m_efd,fd);
            socket->close();
        }
    }
    else if (send_bytes < 0) {
        SDEpollUtility::del_event(m_efd,fd);
        socket->close();
    }
    else {
        //add_write_event(socket);
    }
}
    
void SDActiveConnHandler::doIt()
{
    LOG4CPLUS_DEBUG(logger, "start thread");
    int timeout = 1 * 1000;
    
    static SDSharedSocket null_socket;
    for (;;) {
        int num = SDEpollUtility::wait(m_efd, m_events, m_maxevents, timeout);
        for (int i = 0; i < num; ++i) {
            struct epoll_event ev = m_events[i];
            int fd = ev.data.fd;
            LOG4CPLUS_DEBUG(logger, "fd:" << fd << " is available");
            
            if (is_socket_notify_fd(fd)) {
                process_socket_notify();
            }
            else if (is_buffer_notify_fd(fd)) {
                process_buffer_notify();
            }
            else {
                SDSharedSocket socket = m_socket_list[fd];
           
                if (SDEpollUtility::is_read_event(ev)) {
                    process_recv(socket);
                }
                else if (SDEpollUtility::is_write_event(ev)) {
                    process_send(socket);
                }
                else
                {
                    LOG4CPLUS_WARN(logger, "fd:" << fd << " err event " << ev.events);
                    SDEpollUtility::del_event(m_efd,fd);
                    socket->close();
                }
            }
        }

        process_timeout();
    }
    LOG4CPLUS_DEBUG(logger, "exit thread");
}
