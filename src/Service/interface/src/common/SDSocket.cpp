#include "SDSocket.h"
#include "SDActiveConnHandler.h"
#include "SDSocketUtility.h"

using namespace std;

IMPL_LOGGER(SDSocket, logger);

SDSocket::SDSocket()
{
    m_state = IO_STATE_RECV_MORE;
    m_fd = -1;
    m_send_buf_queue = NULL;
    m_conn_handler = NULL;

    m_recv_buf = SDSharedBuffer(new SDBuffer());
    m_send_buf = SDSharedBuffer(new SDBuffer());

    m_unixtime = time(NULL);
    m_seq = 0;
}

SDSocket::~SDSocket()
{
    close();

    if (m_send_buf_queue) {
        delete m_send_buf_queue;
    }
}

void SDSocket::close()
{
    if (m_fd != -1) {
        ::close(m_fd);
        LOG4CPLUS_DEBUG(logger, "close fd:" << m_fd);
        m_fd = -1;
    }
}

void SDSocket::init_conn_info(int fd, const std::string& local_addr, const std::string& remote_addr)
{
    m_fd = fd;
    m_local_addr = local_addr;
    m_remote_addr = remote_addr;
}
    
void SDSocket::wait_send()
{
    m_state = IO_STATE_SEND_MORE;
}

int SDSocket::get_state()
{
    return m_state;
}

void SDSocket::reset_state()
{
    m_state = IO_STATE_INIT;
}

void SDSocket::wait_recv()
{
    m_state = IO_STATE_RECV_MORE;
}
    
bool SDSocket::is_timeout(time_t timenow, int timeout)
{
    //LOG4CPLUS_DEBUG(logger, "m_unixtime=" << m_unixtime << ", timeout=" << timeout << ", timenow=" << timenow);
    return (m_unixtime + timeout) <= timenow;
}

SDSocket* SDSocket::clone()
{
    return new SDSocket();
}

int SDSocket::on_recv(SDSharedSocket& socket)
{
    m_recv_buf->lseek(0);
    return IO_STATE_RECV_MORE;
}

int SDSocket::on_send(SDSharedSocket& socket)
{
    m_recv_buf->free_buf();
    return IO_STATE_SEND_MORE;
}

int SDSocket::on_request(SDSharedSocket& socket, void* param)
{
    return 0;
}

int SDSocket::recv_data()
{
    m_unixtime = time(NULL);
    
    if (m_recv_buf->data() == NULL || m_recv_buf->size() == 0 || m_recv_buf->length() >= m_recv_buf->size()) {
        return -1;
    }
    
    int total_recv_bytes = 0;
    while (m_recv_buf->length() < m_recv_buf->size()) {
        int recv_bytes = ::recv(m_fd, m_recv_buf->data()+m_recv_buf->length(), m_recv_buf->offset(), 0);
        if (recv_bytes == 0) {
            LOG4CPLUS_WARN(logger, m_remote_addr << "=>" << m_local_addr
                    << " fd:" << m_fd << " recv() fail: Connection closed by foreign host");
            return -1;
        }
        else if (recv_bytes < 0) {
            if (errno == EINTR) {
                continue;
            }
            else if (errno == EWOULDBLOCK || errno == EAGAIN || errno == ETIMEDOUT) {
                return total_recv_bytes;
            }
            LOG4CPLUS_WARN(logger, m_remote_addr << "=>" << m_local_addr
                    << " fd:" << m_fd << " recv() fail: " << strerror(errno));
            return -1;
        }
        LOG4CPLUS_DEBUG(logger, "fd:" << m_fd << " recv " << recv_bytes << "/" << m_recv_buf->offset() << " bytes");

        m_recv_buf->lseek(recv_bytes);
        total_recv_bytes += recv_bytes;     
    }
    return total_recv_bytes;
}

int SDSocket::send_data()
{
    m_unixtime = time(NULL);

    if (m_send_buf.get() == NULL || m_send_buf->size() == 0) {
        if (m_send_buf_queue) {
            if (m_send_buf_queue->getSize()>0 && m_send_buf_queue->pop_nonblock(m_send_buf)) {
            }
            else {
                return 0;
            }
        }
        else {
            return 0;
        }
    }
    
    if (m_send_buf.get() == NULL || m_send_buf->size() == 0) {
        return -1;
    }
    
    int total_send_bytes = 0;
    while (m_send_buf->length() < m_send_buf->size()) {
        int send_bytes = ::send(m_fd, m_send_buf->data()+m_send_buf->length(), m_send_buf->offset(), MSG_NOSIGNAL);
        if (send_bytes <= 0) {
            if (errno == EINTR) {
                continue;
            }
            else if (errno == EWOULDBLOCK || errno == EAGAIN || errno == ETIMEDOUT) {
                return total_send_bytes;
            }
            LOG4CPLUS_WARN(logger, m_remote_addr << "=>" << m_local_addr << " => "
                    << "fd:" << m_fd << " send(" << m_send_buf->offset() << ") fail: " << strerror(errno));
            return -1;
        }

        LOG4CPLUS_DEBUG(logger, "fd:" << m_fd << " send " << send_bytes << "/" << m_send_buf->offset() << " bytes");
        m_send_buf->lseek(send_bytes);
        total_send_bytes += send_bytes;     
    }
    return total_send_bytes;
}

int SDSocket::connect_to()
{
    string::size_type pos = m_remote_addr.find(':');
    if (pos == string::npos) {
        LOG4CPLUS_WARN(logger, "invalid addr " << m_remote_addr);
        return -1;
    }

    bool inprogress = false;
    m_fd = SDSocketUtility::connect(m_remote_addr.substr(0, pos), atoi(m_remote_addr.substr(pos+1).c_str()), &inprogress);
    if (m_fd != -1) {
        if (inprogress) {
            return -2;
        }
        else {
            return 0;
        }
    }

    return -1;
}
