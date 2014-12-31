#ifndef SD_EPOLL_UTILITY_H
#define SD_EPOLL_UTILITY_H

#include "SDLogger.h"

#include <sys/epoll.h>

class SDEpollUtility
{
public:
    static int create_epoll(int size);
    static int add_read_event(int efd, int fd);
    static int add_write_event(int efd, int fd);
    static int del_event(int efd, int fd);
    static int wait(int efd, struct epoll_event* events, int maxevents, int timeout);

    static bool is_read_event(struct epoll_event& ev);
    static bool is_write_event(struct epoll_event& ev);

protected:
    DECL_LOGGER(logger);

};

#endif
