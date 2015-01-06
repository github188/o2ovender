#include "SDEpollUtility.h"

#include <errno.h>
#include <string.h>

using namespace std;

IMPL_LOGGER(SDEpollUtility,logger);

int SDEpollUtility::create_epoll(int size)
{
    int fd = epoll_create(size);
    if (fd == -1) {
        LOG4CPLUS_WARN(logger, "epoll_create() fail " << errno << ":" << strerror(errno));
        return -1;
    }
    return fd;
}
    
int SDEpollUtility::add_read_event(int efd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;   
    //ev.events = EPOLLIN | EPOLLET;
    ev.events = EPOLLIN;

    if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        LOG4CPLUS_WARN(logger, "epoll_ctl(ADD-READ) fail " << errno << ":" << strerror(errno));
        return -1;
    }

    return 0;
}

int SDEpollUtility::add_write_event(int efd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;   
    //ev.events = EPOLLOUT | EPOLLET;
    ev.events = EPOLLOUT;

    if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        LOG4CPLUS_WARN(logger, "epoll_ctl(ADD-WRITE) fail " << errno << ":" << strerror(errno));
        return -1;
    }

    return 0;
}

int SDEpollUtility::mod_read_event(int efd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;   
    //ev.events = EPOLLIN | EPOLLET;
    ev.events = EPOLLIN;

    if (epoll_ctl(efd, EPOLL_CTL_MOD, fd, &ev) == -1) {
        LOG4CPLUS_WARN(logger, "epoll_ctl(MOD-READ) fail " << errno << ":" << strerror(errno));
        return -1;
    }

    return 0;
}

int SDEpollUtility::mod_write_event(int efd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;   
    //ev.events = EPOLLOUT | EPOLLET;
    ev.events = EPOLLOUT;

    if (epoll_ctl(efd, EPOLL_CTL_MOD, fd, &ev) == -1) {
        LOG4CPLUS_WARN(logger, "epoll_ctl(MOD-WRITE) fail " << errno << ":" << strerror(errno));
        return -1;
    }

    return 0;
}
int SDEpollUtility::del_event(int efd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;

    if (epoll_ctl(efd, EPOLL_CTL_DEL, fd, &ev) == -1) {
        LOG4CPLUS_WARN(logger, "epoll_ctl() fail " << errno << ":" << strerror(errno));
        return -1;
    }

    return 0;
}

int SDEpollUtility::wait(int efd, struct epoll_event* events, int maxevents, int timeout)
{
    return epoll_wait(efd, events, maxevents, timeout);
}
    
bool SDEpollUtility::is_read_event(struct epoll_event& ev)
{
    return (ev.events & EPOLLIN) == EPOLLIN;
}

bool SDEpollUtility::is_write_event(struct epoll_event& ev)
{
    return (ev.events & EPOLLOUT) == EPOLLOUT;
}
