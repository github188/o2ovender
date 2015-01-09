#ifndef SD_PASSIVE_CONN_HANDLER_H
#define SD_PASSIVE_CONN_HANDLER_H

#include "SDConnHandler.h"

#include <stdlib.h>
#include <boost/shared_ptr.hpp>

class SDPassiveConnHandler : public SDConnHandler
{
public:
    SDPassiveConnHandler();
    virtual ~SDPassiveConnHandler();

    bool init(int maxevents, int queue_size, int timeout);
    
    virtual bool post(SDSharedBuffer& socket);
    virtual bool post(SDSharedSocket& socket);

    virtual void doIt();

    void add_read_event(SDSharedSocket& socket);
    void add_write_event(SDSharedSocket& socket);
    void mod_read_event(SDSharedSocket& socket);
    void mod_write_event(SDSharedSocket& socket);
    int del_event(int fd);

private:
    bool is_socket_notify_fd(int fd);
    bool is_buffer_notify_fd(int fd);
    void process_socket_notify();
    void process_buffer_notify();

    void process_timeout();
    void process_recv(SDSharedSocket& socket);
    void process_send(SDSharedSocket& socket);

protected:
    DECL_LOGGER(logger);

    int m_queue_size;
    boost::shared_ptr<SDSocketQueue> m_socket_queue;
    boost::shared_ptr<SDBufferQueue> m_buffer_queue;

    int m_efd;
    int m_maxevents;
    struct epoll_event* m_events;

    int m_socket_fd;
    int m_socket_notify_fd;
    int m_buffer_fd;
    int m_buffer_notify_fd;

    int m_timeout;
    int m_total_sockets;
    uint64_t m_checksum;
    SDSharedSocket* m_socket_list;
};

#endif
