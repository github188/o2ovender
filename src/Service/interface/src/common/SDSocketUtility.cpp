#include "SDSocketUtility.h"

#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

IMPL_LOGGER(SDSocketUtility,logger);

int SDSocketUtility::create_socket()
{
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1) {
        LOG4CPLUS_WARN(logger, "socket() fail " << errno << ":" << strerror(errno));
        return -1;
    }
    return fd;
}

int SDSocketUtility::set_nonblock(int fd)
{
    int flag = -1;
    if ( (flag=fcntl(fd, F_GETFL)) == -1 || fcntl(fd, F_SETFL, flag | O_NONBLOCK) == -1) {
        LOG4CPLUS_WARN(logger, "fcntl() fail " << errno << ":" << strerror(errno));
        return -1;
    }
    return 0;
}

int SDSocketUtility::set_reuse(int fd)
{
    int reuse = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        LOG4CPLUS_WARN(logger, "setsockopt() fail " << errno << ":" << strerror(errno));
        return -1;
    }
    return 0;
}

int SDSocketUtility::set_linger(int fd)
{
    struct linger ling = {1, 0};
    if (setsockopt(fd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)) == -1) {
        LOG4CPLUS_WARN(logger, "setsockopt() fail " << errno << ":" << strerror(errno));
        return -1;
    }
    return 0;
}

int SDSocketUtility::bind(int fd, struct sockaddr_in& addr)
{
    if (::bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        LOG4CPLUS_WARN(logger, "bind() fail " << errno << ":" << strerror(errno));
        return -1;
    }
    return 0;
}

int SDSocketUtility::listen(int fd)
{
    if (::listen(fd, SOMAXCONN) == -1) {
        LOG4CPLUS_WARN(logger, "listen() fail " << errno << ":" << strerror(errno));
        return -1;
    }
    return 0;
}

void SDSocketUtility::make_addr(struct sockaddr_in& addr, const std::string& ip, int port){
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);
}
    
int SDSocketUtility::listen(const std::string& ip, int port)
{
    bool is_ok = false;
    
    struct sockaddr_in addr;
    make_addr(addr, ip, port);
   
    int fd = create_socket();
    do {
        if (fd == -1) {
            break;
        }

        if (set_nonblock(fd) == -1) {
            break;
        }

        if (set_reuse(fd) == -1) {
            break;
        }

        if (bind(fd, addr) == -1) {
            break;
        }

        if (listen(fd) == -1) {
            break;
        }

        is_ok = true;
    } while (false);

    if (!is_ok) {
        if (fd != -1) {
            ::close(fd);
            fd = -1;
        }
    }
   
    return fd;
}

int SDSocketUtility::connect(int fd, struct sockaddr_in& addr, bool* inprogress)
{
    *inprogress = false;
    if (::connect(fd, (const struct sockaddr*)&addr, sizeof(addr)) == -1) {
        if(errno != EINPROGRESS) {
            LOG4CPLUS_WARN(logger, "connect() fail " << errno << ":" << strerror(errno));
            return -1;
        }
        *inprogress = true;
    }
    return 0;
}

int SDSocketUtility::connect(const std::string& ip, int port, bool* inprogress)
{
    *inprogress = false;
    bool is_ok = false;
    
    struct sockaddr_in addr;
    make_addr(addr, ip, port);
   
    int fd = create_socket();
    do {
        if (fd == -1) {
            break;
        }

        if (set_nonblock(fd) == -1) {
            break;
        }

        if (connect(fd, addr, inprogress) == -1) {
            break;
        }

        is_ok = true;
    } while (false);

    if (!is_ok) {
        if (fd != -1) {
            ::close(fd);
            fd = -1;
        }
    }
   
    return fd;
}

int SDSocketUtility::connect(const std::string& ip, int port)
{
    bool is_ok = false;
    
    struct sockaddr_in addr;
    make_addr(addr, ip, port);
   
    int fd = create_socket();
    do {
        if (fd == -1) {
            break;
        }

		bool inprogress = false;
        if (connect(fd, addr, &inprogress) == -1) {
            break;
        }

        is_ok = true;
    } while (false);

    if (!is_ok) {
        if (fd != -1) {
            ::close(fd);
            fd = -1;
        }
    }
   
    return fd;
}

int SDSocketUtility::accept(int fd, struct sockaddr_in* addr)
{
    socklen_t sock_len = sizeof(struct sockaddr_in);
    int cfd = ::accept(fd, (struct sockaddr*)addr, &sock_len);
    if (cfd == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            LOG4CPLUS_WARN(logger, "accept() fail " << errno << ":" << strerror(errno));
        }
        return -1;
    }
    return cfd;
}

std::string SDSocketUtility::to_string(struct sockaddr_in& addr)
{
    char buf[256] = {'\0'};
    inet_ntop(AF_INET, (const void *)&addr.sin_addr.s_addr, buf, sizeof(buf));
    sprintf(buf+strlen(buf),":%d", ntohs(addr.sin_port));
    return string(buf);
}
    
int SDSocketUtility::recv(int fd, char* buffer, int size, int* offset)
{
    *offset = 0;
    int left_bytes = size;
    while (left_bytes > 0) {
        int recv_bytes = ::recv(fd, buffer + *offset, left_bytes, 0);
        if (recv_bytes == 0) {
            LOG4CPLUS_WARN(logger, "recv() fail " << errno << ":" << strerror(errno));
            return -1;
        }
        else if (recv_bytes < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == ETIMEDOUT) {
                return 0;
            }
            else {
                LOG4CPLUS_WARN(logger, "recv() fail " << errno << ":" << strerror(errno));
                return -1;
            }
        }
        else {
            *offset += recv_bytes;
            left_bytes -= recv_bytes;
        }
    }

    return *offset;
}

int SDSocketUtility::create_pipe(int* fd)
{
    if (pipe(fd) == -1) {
        LOG4CPLUS_WARN(logger, "pipe() fail " << errno << ":" << strerror(errno));
        return -1;
    }

    if (set_nonblock(fd[0]) == -1) {
        return -1;
    }

    if (set_nonblock(fd[1]) == -1) {
        return -1;
    }

    return 0;
}

int SDSocketUtility::create_socketpair(int* fd)
{
    if (socketpair(AF_UNIX, SOCK_STREAM,0,fd) == -1) {
        LOG4CPLUS_WARN(logger, "socketpair() fail " << errno << ":" << strerror(errno));
        return -1;
    }

    if (set_nonblock(fd[0]) == -1) {
        return -1;
    }

    if (set_nonblock(fd[1]) == -1) {
        return -1;
    }

    return 0;
}
