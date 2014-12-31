#ifndef SD_SOCKET_UTILITY_H
#define SD_SOCKET_UTILITY_H

#include "SDLogger.h"
#include <string>
#include <netinet/in.h>

class SDSocketUtility
{
public:
    static int create_socket();
    static void make_addr(struct sockaddr_in& addr, const std::string& ip, int port);
    static int set_nonblock(int fd);
    static int set_reuse(int fd);
    static int set_linger(int fd);
    static int bind(int fd, struct sockaddr_in& addr);
    static int listen(int fd);
    static int listen(const std::string& ip, int port);
    static int connect(int fd, struct sockaddr_in& addr, bool* inprogress);
    static int connect(const std::string& ip, int port, bool* inprogress);
    static int connect(const std::string& ip, int port);
    static int accept(int fd, struct sockaddr_in* addr);

    static std::string to_string(struct sockaddr_in& addr);

    static int recv(int fd, char* buffer, int size, int* offset);

    static int create_pipe(int* fd);
    static int create_socketpair(int* fd);

protected:
    DECL_LOGGER(logger);
};

#endif
