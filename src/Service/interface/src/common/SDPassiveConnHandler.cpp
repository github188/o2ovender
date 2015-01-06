#include "SDPassiveConnHandler.h"

#include "SDSocketUtility.h"
#include "SDStringUtility.h"

using namespace std;

IMPL_LOGGER(SDPassiveConnHandler, logger);

SDPassiveConnHandler::SDPassiveConnHandler()
{
    m_total_sockets = 0;
    m_checksum = 0;
}

SDPassiveConnHandler::~SDPassiveConnHandler()
{}

bool SDPassiveConnHandler::init(int maxevents, int queue_size, int timeout)
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
    
bool SDPassiveConnHandler::post(SDSharedSocket& socket)
{
    if (!m_socket_queue->push_nonblock(socket)) {
        LOG4CPLUS_WARN(logger, "push() fail: queue size=" << m_socket_queue->getCapacity());
        return false;
    }
    
    static char c;
    ::send(m_socket_notify_fd, &c, 1, MSG_NOSIGNAL);
    return true;
}

bool SDPassiveConnHandler::post(SDSharedBuffer& buff)
{
    if (!m_buffer_queue->push_nonblock(buff)) {
        LOG4CPLUS_WARN(logger, "push() fail: queue size=" << m_buffer_queue->getCapacity());
        return false;
    }
    
    static char c;
    ::send(m_buffer_notify_fd, &c, 1, MSG_NOSIGNAL);
    return true;
}
    
bool SDPassiveConnHandler::is_socket_notify_fd(int fd)
{
    if (fd == m_socket_fd) {
        static char buf[256];
        ::recv(fd, buf, sizeof(buf), 0);
        return true;
    }
    return false;
}

bool SDPassiveConnHandler::is_buffer_notify_fd(int fd)
{
    if (fd == m_buffer_fd) {
        static char buf[256];
        ::recv(fd, buf, sizeof(buf), 0);
        return true;
    }
    return false;
}
    
void SDPassiveConnHandler::add_read_event(SDSharedSocket& socket)
{
    if (SDEpollUtility::add_read_event(m_efd, socket->m_fd) == 0) {
        ++m_total_sockets;
        socket->m_state = IO_STATE_IN_EPOLL;
        m_socket_list[socket->m_fd] = socket;
        LOG4CPLUS_DEBUG(logger, "fd:" << socket->m_fd << " enter READ mode");
    }
}

void SDPassiveConnHandler::add_write_event(SDSharedSocket& socket)
{
    if (SDEpollUtility::add_write_event(m_efd, socket->m_fd) == 0) {
        ++m_total_sockets;
        socket->m_state = IO_STATE_IN_EPOLL;
        m_socket_list[socket->m_fd] = socket;
        LOG4CPLUS_DEBUG(logger, "fd:" << socket->m_fd << " enter WRITE mode");
    }
}

void SDPassiveConnHandler::mod_read_event(SDSharedSocket& socket)
{
    if (SDEpollUtility::mod_read_event(m_efd, socket->m_fd) == 0) {
        socket->m_state = IO_STATE_IN_EPOLL;
        LOG4CPLUS_DEBUG(logger, "fd:" << socket->m_fd << " enter READ mode");
    }
}

void SDPassiveConnHandler::mod_write_event(SDSharedSocket& socket)
{
    if (SDEpollUtility::mod_write_event(m_efd, socket->m_fd) == 0) {
        socket->m_state = IO_STATE_IN_EPOLL;
        LOG4CPLUS_DEBUG(logger, "fd:" << socket->m_fd << " enter WRITE mode");
    }
}

int SDPassiveConnHandler::del_event(int fd)
{
    static SDSharedSocket null_socket;
    LOG4CPLUS_DEBUG(logger, "fd:" << fd << " exit event mode");
    --m_total_sockets;
    m_socket_list[fd]->m_state = IO_STATE_INIT;
    m_socket_list[fd] = null_socket;
    return SDEpollUtility::del_event(m_efd, fd);
}

void SDPassiveConnHandler::process_socket_notify()
{
    SDSharedSocket socket;
    while (m_socket_queue->pop_nonblock(socket)) {
        if (socket.get() == NULL) {
            LOG4CPLUS_DEBUG(logger, "pop() NULL");
            continue;
        }

        LOG4CPLUS_DEBUG(logger, "add fd:" << socket->m_fd);
        socket->m_conn_handler = this;
        socket->m_checksum = ++m_checksum;
        add_read_event(socket);
    }
}

void SDPassiveConnHandler::process_buffer_notify()
{
    SDSharedBuffer buff;
    while (m_buffer_queue->pop_nonblock(buff)) {
        if (buff.get() == NULL) {
            LOG4CPLUS_DEBUG(logger, "pop() NULL");
            continue;
        }

        LOG4CPLUS_DEBUG(logger, "add key:" << buff->m_ipv4);
        string::size_type pos = buff->m_ipv4.find(':');
        if (pos != string::npos) {
            uint32_t index = 0;
            uint64_t checksum = 0;
            SDStringUtility::passive_conn_key_decode(buff->m_ipv4, &index, &checksum);
            if (index < (uint32_t)m_maxevents) {
                SDSharedSocket& socket = m_socket_list[index];
                if (socket.get()) {
                    if (socket->m_checksum == checksum) {
                        if (socket->m_send_buf_queue == NULL) {
                            socket->m_send_buf_queue = new SDBufferQueue(10000);
                        }
                        if (socket->m_send_buf_queue->push_nonblock(buff)) {
                            process_send(socket);
                        }
                        else {
                            LOG4CPLUS_WARN(logger, buff->m_ipv4 << " push() fail: queue size=" << socket->m_send_buf_queue->getCapacity());
                        }
                    }
                    else {
                        LOG4CPLUS_WARN(logger, "no key " << buff->m_ipv4 << "(" << index << ":" << socket->m_checksum << ")");
                    }
                }
                else {
                    LOG4CPLUS_WARN(logger, "no key " << buff->m_ipv4);
                }
            }
            else {
                LOG4CPLUS_WARN(logger, "invalid key " << buff->m_ipv4);
            }
        }
        else {
            LOG4CPLUS_WARN(logger, "invalid key " << buff->m_ipv4);
        }
    }
}

void SDPassiveConnHandler::process_timeout()
{
    time_t timenow = time(NULL);
    static SDSharedSocket null_socket;
    int count = 0;
    for (int i = 0; i < m_maxevents; ++i) {
        SDSharedSocket socket = m_socket_list[i];

        if (socket.get()) {
            if (socket->is_timeout(timenow, m_timeout)) {
                LOG4CPLUS_DEBUG(logger, "timeout fd:" << socket->m_fd << " timeout:" << m_timeout);
                del_event(socket->m_fd);
            }
            if (++count >= m_total_sockets) {
                break;
            }
        }
    }
}

void SDPassiveConnHandler::process_recv(SDSharedSocket& socket)
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
            del_event(fd);
        }
    }
    else if (recv_bytes < 0) {
        del_event(fd);
    }
    else {
        //add_read_event(socket);
    }
}

void SDPassiveConnHandler::process_send(SDSharedSocket& socket)
{
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
            del_event(fd);
        }
    }
    else if (send_bytes < 0) {
        del_event(fd);
    }
    else {
        //add_write_event(socket);
    }
}
    
void SDPassiveConnHandler::doIt()
{
    LOG4CPLUS_DEBUG(logger, "start thread");
    int timeout = 1 * 1000;
    m_checksum = ((uint64_t)pthread_self() << 32) + (uint32_t)rand();
    LOG4CPLUS_DEBUG(logger, "checksum= " << m_checksum);
    
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
                else {
                    LOG4CPLUS_WARN(logger, "fd:" << fd << " err event " << ev.events);
                    del_event(fd);
                }
            }
        }

        process_timeout();
    }
    LOG4CPLUS_DEBUG(logger, "exit thread");
}
