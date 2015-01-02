#ifndef SD_SOCKET_H
#define SD_SOCKET_H

#include <string>
#include <string.h>
#include <netinet/in.h>
#include <boost/shared_ptr.hpp>

#include "SDQueue.h"
#include "SDLogger.h"
#include "SDBuffer.h"

enum IO_STATE
{
    IO_STATE_INIT = 0,
    IO_STATE_RECV_MORE,
    IO_STATE_SEND_MORE,
    IO_STATE_NOTHING_TO_DO,
    IO_STATE_IN_EPOLL,
    IO_STATE_FAIL
};

class SDSocket;
typedef boost::shared_ptr<SDSocket> SDSharedSocket;
typedef SDQueue<SDSharedSocket> SDSocketQueue;

class SDEventHandler;
class SDConnHandler;

class SDSocket
{
public:
    SDSocket();
    ~SDSocket();
    void init_conn_info(int fd, const std::string& local_addr, const std::string& remote_addr);
    bool is_timeout(time_t timenow, int timeout);

    virtual SDSocket* clone();

    void wait_recv();
    void wait_send();
    int get_state();
    void reset_state();

    virtual int on_recv(SDSharedSocket& socket);
    virtual int on_send(SDSharedSocket& socket);

    virtual int on_request(SDSharedSocket& socket, void* param=NULL);

    bool is_connected() const {return m_fd != -1;}
    int connect_to();

    void close();

protected:
    int recv_data();
    int send_data();

public:
    DECL_LOGGER(logger);
    
    int m_fd;
    std::string m_local_addr;
    std::string m_remote_addr;

    IO_STATE m_state;
    SDSharedBuffer m_recv_buf;
    SDSharedBuffer m_send_buf;
    SDBufferQueue* m_send_buf_queue;

    time_t m_unixtime;
    uint64_t m_seq;
    SDConnHandler* m_conn_handler;
    friend class SDEventHandler;
    friend class SDPassiveConnHandler;
    friend class SDActiveConnHandler;
};

#endif
